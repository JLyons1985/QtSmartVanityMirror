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

#include <Headers/speechdecoder.h>

SpeechDecoder::SpeechDecoder()
{

}

SpeechDecoder::~SpeechDecoder()
{

}

QString SpeechDecoder::decodeSpeech(const QString speech)
{

    qDebug() << "Decoding: " << speech;

    // User wants to hide something
    if (speech.contains("hide", Qt::CaseInsensitive) ||
            speech.contains("dismiss", Qt::CaseInsensitive))
    {
        if (speech.contains("calendar", Qt::CaseInsensitive)) // hide the calendar
            return "hideCalendar";
        if (speech.contains("forecast", Qt::CaseInsensitive)) // hide the calendar
            return "hideForecast";
    }
    // User wants to show something
    else if (speech.contains("show", Qt::CaseInsensitive) ||
             speech.contains("display", Qt::CaseInsensitive))
    {
        if (speech.contains("calendar", Qt::CaseInsensitive)) // show the calendar
            return "showCalendar";
        if (speech.contains("forecast", Qt::CaseInsensitive)) // show the calendar
            return "showForecast";
    }
    // Test
    else if (speech.contains("sleep", Qt::CaseInsensitive)) // Put mirror to sleep
    {
         return "sleep";
    }
    // No match
    else
    {
        return "Undef";
    }
    return "Undef";
}
