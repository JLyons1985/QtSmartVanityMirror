/*************************************************************************
* Copyright 2016 Joshua Lyons
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Project File: SpeechAudioRecorder.h
* Project Description: Handles the recording of speech audio
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#ifndef SPEECHAUDIORECORDER_H
#define SPEECHAUDIORECORDER_H

#include <QObject>
#include <QAudioRecorder>

class SpeechAudioRecorder : public QObject
{
    Q_OBJECT

public:
    // Functions
    SpeechAudioRecorder();
    ~SpeechAudioRecorder();

    void startRecording();
    void stopRecording();

    bool isRecording();

private:
    // Variables
    QString audioFilePath;
    bool recording;
    QAudioRecorder* audioRecorder;
};

#endif // SPEECHAUDIORECORDER_H
