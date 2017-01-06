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
* Project File: TextToSpeech.cpp
* Project Description: Handles text to speech and playing the audio
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <Headers/texttospeech.h>
#include <QUrl>
#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include <QAudioOutput>

TextToSpeech::TextToSpeech()
{

}

TextToSpeech::~TextToSpeech()
{

}

void TextToSpeech::convertTextToSpeech(QString text)
{
    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(getTTSRequestFinished(QNetworkReply*)));

    QUrl url = QUrl("http://api.voicerss.org/?key=4c205293427c493ea9a518658a47bae4&hl=en-gb&c=MP3&f=48khz_16bit_stereo&src=" + text );

    QNetworkReply* reply = nam->get(QNetworkRequest(url));
}

void TextToSpeech::getTTSRequestFinished(QNetworkReply* reply)
{
    // Get the audio data and write to file, then play that file
    QFile file("/home/pi/PythonMasterServer/ttsAudio.mp3");
    file.open((QIODevice::WriteOnly));
    QDataStream out(&file);

    if(reply->error() == QNetworkReply::NoError)
    {
        out << reply->readAll();
    }

    file.close();

    //file.open(QIODevice::ReadOnly);

    //QAudioFormat format;
    //format.setSampleRate(16000);
    //format.setSampleSize(8);
    //format.setChannelCount(2);
    //format.setCodec("audio/pcm");

    //QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

   // qDebug() << info.supportedCodecs();

    //output = new QAudioOutput(format, this);
    //output->start(&file);
    player = new QMediaPlayer();

    player->setMedia(QUrl::fromLocalFile("/home/pi/PythonMasterServer/ttsAudio.mp3"));
    player->setVolume(100);
    player->play();

}
