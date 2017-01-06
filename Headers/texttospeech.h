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
* Project File: TextToSpeech.h
* Project Description: Handles text to speech and playing the audio
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include <QObject>
#include <QMediaPlayer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QAudioOutput>

class TextToSpeech : public QObject
{
    Q_OBJECT

public:
    // Functions
    TextToSpeech();
    ~TextToSpeech();

    void convertTextToSpeech(QString text);

private:
    // Variables
    bool playing;
    QMediaPlayer* player;    
    QAudioOutput* output;
    QNetworkAccessManager* nam;

private slots:

    void getTTSRequestFinished(QNetworkReply* reply);
};

#endif // TEXTTOSPEECH_H
