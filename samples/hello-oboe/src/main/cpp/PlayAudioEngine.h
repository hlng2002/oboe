/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OBOE_HELLOOBOE_PLAYAUDIOENGINE_H
#define OBOE_HELLOOBOE_PLAYAUDIOENGINE_H

#include <oboe/Oboe.h>
#include <shared/AudioEngine.h>

#include "SoundGenerator.h"
#include "LatencyTuningCallback.h"

constexpr int32_t kBufferSizeAutomatic = 0;

// This sample inherits the AudioEngine in the shared folder, with a custom audio source and callback
class PlayAudioEngine : public AudioEngine<SoundGenerator, LatencyTuningCallback> {

public:
    PlayAudioEngine();


    // These methods reset the underlying stream with new properties

    /**
     * Set the audio device which should be used for playback. Can be set to oboe::kUnspecified if
     * you want to use the default playback device (which is usually the built-in speaker if
     * no other audio devices, such as headphones, are attached).
     *
     * @param deviceId the audio device id, can be obtained through an {@link AudioDeviceInfo} object
     * using Java/JNI.
    */
    void setDeviceId(int32_t deviceId);

    void setChannelCount(int channelCount);

    void setAudioApi(oboe::AudioApi audioApi);

    void setBufferSizeInBursts(int32_t numBursts);

    /**
     * Calculate the current latency between writing a frame to the output stream and
     * the same frame being presented to the audio hardware.
     *
     * Here's how the calculation works:
     *
     * 1) Get the time a particular frame was presented to the audio hardware
     * @see AudioStream::getTimestamp
     * 2) From this extrapolate the time which the *next* audio frame written to the stream
     * will be presented
     * 3) Assume that the next audio frame is written at the current time
     * 4) currentLatency = nextFramePresentationTime - nextFrameWriteTime
     *
     * @return  Output Latency in Milliseconds
     */
    double getCurrentOutputLatencyMillis();

    bool isLatencyDetectionSupported();

private:
    void updateLatencyDetection();

    bool mIsLatencyDetectionSupported = false;
    // This will be used to automatically tune the buffer size of the stream, obtaining optimal latency
    std::unique_ptr<oboe::LatencyTuner> mLatencyTuner;
};

#endif //OBOE_HELLOOBOE_PLAYAUDIOENGINE_H
