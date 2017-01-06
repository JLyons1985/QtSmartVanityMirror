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
* Project File: SpeechDecoder.h
* Project Description: Decodes the speech to a usable command
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#ifndef SPEECHDECODER_H
#define SPEECHDECODER_H

#include <QObject>
#include <QDebug>

class SpeechDecoder : public QObject
{
    Q_OBJECT

public:
    SpeechDecoder();
    ~SpeechDecoder();

    QString decodeSpeech(const QString speech);

};

#endif // SPEECHDECODER_H
