#ifndef TRAFFICTHREAD_H
#define TRAFFICTHREAD_H

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
* Project File: WeatherRetrieverThread.h
* Project Description: Retrieves weather conditions
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <QThread>
#include <QJsonObject>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

class TrafficThread : public QThread
{
    Q_OBJECT

signals :
    void sendTrafficJson(QString trafficJson);

private:
    // Variables
    QString googleDistanceAPIKey;
    const QString googleDistanceMainURL = "https://maps.googleapis.com/maps/api/distancematrix/json?origins=";
    QString origins;
    QString destinations;
    bool userLoggedIn;

    // Functions
    void run();
    QNetworkAccessManager* nam;

private slots :
    void timerHit();
    void getTrafficRequestFinished(QNetworkReply* reply);

public:
    TrafficThread();
    void setupTrafficThread();
    void setUserLoggedIn(bool userLoggedIn);

};

#endif // TRAFFICTHREAD_H
