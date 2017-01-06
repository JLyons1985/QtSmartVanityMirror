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
* Project File: SpeechAudioRecorder.cpp
* Project Description: Handles the recording of speech audio
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <Headers/speechaudiorecorder.h>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

SpeechAudioRecorder::SpeechAudioRecorder()
{
    QString val;
    QFile file(":/mirrorConfig.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    //qWarning() << val;
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject configJson = d.object();
    QJsonObject speechConfig = configJson.value(QString("speech")).toObject();

    // Now populate the fields
    this->audioFilePath = speechConfig["pathToFile"].toString();
}

SpeechAudioRecorder::~SpeechAudioRecorder()
{

}

void SpeechAudioRecorder::startRecording()
{
    // Make a new audio recorder
    this->audioRecorder = new QAudioRecorder();
    this->audioRecorder->setAudioInput("alsa:plughw:CARD=Device,DEV=0");

    // Create the settings
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/FLAC"); //flac
    audioSettings.setSampleRate(16000);
    audioSettings.setBitRate(16); // 32
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding);
    //audioRecorder->setVolume(100);

    audioRecorder->setEncodingSettings(audioSettings, QVideoEncoderSettings(), "raw");

    audioRecorder->setOutputLocation(QUrl::fromLocalFile(this->audioFilePath));

    qDebug() << audioRecorder->supportedAudioCodecs();
    //Debug() << audioRecorder->

    this->recording = true;
    audioRecorder->record();
}

void SpeechAudioRecorder::stopRecording()
{
    audioRecorder->stop();
    this->recording = false;
}

bool SpeechAudioRecorder::isRecording()
{
    return this->recording;
}
