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

#ifndef DIALTHREAD_H
#define DIALTHREAD_H

#include <QThread>
#include <QUdpSocket>
#include <Headers/calendaritem.h>

class DIALThread : public QThread
{    
    Q_OBJECT

signals:
    void launchYoutube(QString url);
    void closeYoutube();
    void updateCalendarList(QList<QObject*> dataList);
    void speechDecoded(QString decodedSpeech);
    void recognitionMessage(QString msg);
    void recognitionSuccess(QString msg);
    void updateQuote(QString quote);

private:
    void run();
    QUdpSocket masterServerSocket;
    QUdpSocket mirrorSocket;
    int masterServerPort;
    int mirrorPort;
    QString masterServerIP;
    int recognitionTriesTotal;
    int currentRecognitionTries;
    bool userLoggedIn;

    // Functions
    void sendUDPMsgToServer(QString messageToSend);


private slots:
    void processPendingDatagrams();
    void timerHit();

public:
    DIALThread();
    void setupDIALThread();
    void getCalendarItems(int numberOfItemsToGet);
    void getSpeechDecoded();
    void trainFaceRecognition();
    void recognizeFace();
    void recognizeFaceStart();
    void setUserLoggedIn(bool userLoggedIn);

};

#endif // DIALTHREAD_H
