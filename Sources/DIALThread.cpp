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
* Project File: DIALThread.h
* Project Description: Listens for information from the DIAL server.
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include "Headers/DIALThread.h"
#include <QTimer>
#include <QStringRef>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QFile>
#include <Headers/texttospeech.h>

DIALThread::DIALThread()
{

}

void DIALThread::run()
{
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(500000);
    timer.start();   // puts one event in the threads event queue
    exec();
    timer.stop();
}

void DIALThread::setupDIALThread()
{
    qDebug() << "In DIAL setup";
    this->userLoggedIn = true;
    // Setup the variables based on the config file
    QString val;
    QFile file(":/mirrorConfig.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    // Make the json object
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject configJson = d.object();
    QJsonObject serverConfig = configJson.value(QString("mirrorServers")).toObject();

    // Now populate the fields
    this->masterServerPort = serverConfig.value("masterServerPort").toInt();
    this->mirrorPort = serverConfig.value("mirrorPort").toInt();
    this->masterServerIP = serverConfig["masterServerIP"].toString();

    serverConfig = configJson.value(QString("faceRecognition")).toObject();
    this->recognitionTriesTotal = serverConfig.value("recognitionTries").toInt();

    qDebug() << "Binding UDP Mirror Socket to: " << QString::number(this->mirrorPort);

    // Now bind the mirror to the mirror port
    this->mirrorSocket.bind(this->mirrorPort);

    // Connect the ready ready to process datagram
    connect(&mirrorSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

}

void DIALThread::processPendingDatagrams()
{
    QByteArray datagram;

    // Grab everything from the socket
    do {        
        datagram.resize(this->mirrorSocket.pendingDatagramSize());

        this->mirrorSocket.readDatagram(datagram.data(), datagram.size());
        this->mirrorSocket.readAll();

    } while (this->mirrorSocket.hasPendingDatagrams());

    // Convert datagram to string
    QString strReply = QString(datagram);

    // Convert string to json
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    // Now do some logic to determine what needs to be done
    if (jsonObject["Program"].toString() == "YouTube")
    {
        if (jsonObject["Command"].toString() == "Launch")
            emit launchYoutube(jsonObject["URL"].toString());
        else if (jsonObject["Command"].toString() == "Hide" || jsonObject["Command"].toString() == "Stop")
            emit closeYoutube();
    }
    else if(jsonObject["Program"].toString() == "Calendar")
    {
        // Send a qstring version of the items array but summarized to what we want
        if (jsonObject["Command"].toString() == "UpdateList")
        {
            QJsonArray tmpArray = jsonObject["Items"].toArray();
            QList<QObject*> dataList;
            int calendarItems = tmpArray.size();
            int i = 0;
            foreach (const QJsonValue & value, tmpArray)
            {
                // Create new json obj
                QJsonObject returnObj;
                QJsonObject obj = value.toObject();

                QDateTime startTime = QDateTime::fromString(obj["start"].toObject()["dateTime"].toString(), Qt::ISODate);
                QDateTime endTime = QDateTime::fromString(obj["end"].toObject()["dateTime"].toString(), Qt::ISODate);
                QDateTime nowTime = QDateTime::currentDateTime();

                //if (startTime.date().day() == nowTime.date().day())
                //{
                    CalendarItem* tmpObj = new CalendarItem();
                    tmpObj->setName(obj["summary"].toString());
                    tmpObj->setLocation(obj["location"].toString());
                    tmpObj->setStart(startTime.toString("HH:mm"));
                    tmpObj->setStart(startTime.toString("HH:mm"));
                    tmpObj->setEnd(endTime.toString("HH:mm"));

                    int day = startTime.date().day();
                    if (day == 1 || day == 21 || day == 31)
                        tmpObj->setDate(startTime.toString("dd") + " st");
                    else if (day == 2 || day == 22)
                        tmpObj->setDate(startTime.toString("dd") + " nd");
                    else if (day == 3 || day == 23)
                        tmpObj->setDate(startTime.toString("dd") + " rd");
                    else
                        tmpObj->setDate(startTime.toString("dd") + " th");

                    double opacity = (calendarItems - i) / (double)calendarItems;

                    tmpObj->setOpacity(opacity);

                    dataList.append(tmpObj);

                    i += 1;
                //}
            }

            // Emit the data object
            emit updateCalendarList(dataList);
        }
    }
    else if(jsonObject["Program"].toString() == "Speech")
    {
        emit speechDecoded(jsonObject["SpeechReturn"].toString());
        qDebug() << jsonObject["SpeechReturn"].toString();
    }
    else if(jsonObject["Program"].toString() == "FaceRecognition")
    {
       if (jsonObject["Msg"].toString() == "NoFace") // Reposition face
       {
           //TextToSpeech* tts = new TextToSpeech();
           //tts->convertTextToSpeech("It's hard to recognize you if I can't see you.");
            emit recognitionMessage("Can't find a face, reposition yourself better...");
       }

       if (jsonObject["Msg"].toString() != "NoFace" && jsonObject["Msg"].toString() != "Neg")
       {
           // Face found
           emit recognitionSuccess(jsonObject["Msg"].toString());
       }
       else
       {
           // Face not found if we can continue searching then try again
           if (this->currentRecognitionTries < this->recognitionTriesTotal)
           {
               //if ((this->recognitionTriesTotal - this->currentRecognitionTries) > 1)
                    //emit recognitionMessage("No match found trying " + QString::number(this->recognitionTriesTotal - this->currentRecognitionTries) + " more times.");
               //else
                   //emit recognitionMessage("No match found trying " + QString::number(this->recognitionTriesTotal - this->currentRecognitionTries) + " more time.");

               this->recognizeFace();
           }
           else
           {
               TextToSpeech* tts = new TextToSpeech();
               tts->convertTextToSpeech("Sranger danger.");
               emit recognitionMessage("No match found, please step away from the mirror and try again shortly...");
               emit recognitionSuccess("false");
           }
       }

    }
    else if(jsonObject["Program"].toString() == "Quotes")
    {
        // Make the json into a string
        qDebug() << jsonObject;

        QJsonDocument doc(jsonObject["Msg"].toObject());
       emit updateQuote(QString(doc.toJson(QJsonDocument::Compact)));
    }

}

void DIALThread::timerHit()
{


}

// Request the calendar items from the server
void DIALThread::getCalendarItems(int numberOfItemsToGet)
{
    qDebug() << "Trying to get calendar items";
    if (this->userLoggedIn)
    {
        // Create the json object for the message
        QJsonObject jsonObject;
        jsonObject.insert("From", "Mirror");
        jsonObject.insert("To", "Calendar");
        jsonObject.insert("Command", "UpdateList");
        jsonObject.insert("ItemsToGet", numberOfItemsToGet);

        // Make the json into a string
        QJsonDocument doc(jsonObject);

        qDebug() << "Asking for calendar items.";

        // Send the message
        this->sendUDPMsgToServer(QString(doc.toJson(QJsonDocument::Compact)));
    }
}

// Sends the message to the server
void DIALThread::sendUDPMsgToServer(QString messageToSend)
{
    QByteArray datagram;
    datagram = messageToSend.toUtf8();

    // Create the socket
    QUdpSocket* tmpSocket = new QUdpSocket(this);

    // Send the msg
    tmpSocket->writeDatagram(datagram, datagram.size(), QHostAddress(this->masterServerIP), this->masterServerPort);
}

// Tell the server to send the audio file up for processing
void DIALThread::getSpeechDecoded()
{
    if (this->userLoggedIn)
    {
        // Create the json object for the message
        QJsonObject jsonObject;
        jsonObject.insert("From", "Mirror");
        jsonObject.insert("To", "Speech");
        jsonObject.insert("Command", "Decode");

        // Make the json into a string
        QJsonDocument doc(jsonObject);

        qDebug() << "Asking to decode speech.";

        // Send the message
        this->sendUDPMsgToServer(QString(doc.toJson(QJsonDocument::Compact)));
    }
}

// Calls to train face recognition
void DIALThread::trainFaceRecognition()
{
    // Create the json object for the message
    QJsonObject jsonObject;
    jsonObject.insert("From", "Mirror");
    jsonObject.insert("To", "FaceRecognition");
    jsonObject.insert("Command", "Train");

    // Make the json into a string
    QJsonDocument doc(jsonObject);

    // Send the message
    this->sendUDPMsgToServer(QString(doc.toJson(QJsonDocument::Compact)));
}

// Starts the recognize face
void DIALThread::recognizeFaceStart()
{
    this->currentRecognitionTries = 0;
    this->recognizeFace();
}

// Makes a call to recognize a face
void DIALThread::recognizeFace()
{
    // Create the json object for the message
    QJsonObject jsonObject;
    jsonObject.insert("From", "Mirror");
    jsonObject.insert("To", "FaceRecognition");
    jsonObject.insert("Command", "RecognizeFace");

    // Make the json into a string
    QJsonDocument doc(jsonObject);

    // Send the message
    this->sendUDPMsgToServer(QString(doc.toJson(QJsonDocument::Compact)));

    this->currentRecognitionTries += 1;
}

void DIALThread::setUserLoggedIn(bool userLoggedIn)
{
    this->userLoggedIn = userLoggedIn;
    if (userLoggedIn)
        this->timerHit();
}
