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

#include "Headers/TrafficThread.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QtNetwork>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>

TrafficThread::TrafficThread()
{

}

void TrafficThread::run()
{
    this->timerHit();
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(180000);
    timer.start();   // puts one event in the threads event queue
    exec();
    timer.stop();
}

void TrafficThread::timerHit()
{
    if (userLoggedIn)
    {
        // Get the current weather from Wunderground
        nam = new QNetworkAccessManager(this);
        QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(getTrafficRequestFinished(QNetworkReply*)));

        // Get the date time
        QDateTime departureTime = QDateTime::currentDateTime();
        departureTime.addSecs(300); // 5 Mins from now
        QString departString = QString::number(departureTime.toTime_t());

        qDebug() << "Asking for traffic data.";

        //Create get current wx url
        QUrl url(googleDistanceMainURL + this->origins + "&destinations=" + this->destinations + "&key=" + this->googleDistanceAPIKey +
              "&departure_time=" + departString);
        QNetworkReply* reply = nam->get(QNetworkRequest(url));
    }
}

void TrafficThread::getTrafficRequestFinished(QNetworkReply* reply)
{
    // Handle the json reply fir the current weather
    if(reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();
        //qDebug() << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        //qDebug() << jsonObject;
        QJsonArray rows = jsonObject.value(QString("rows")).toArray();
        QJsonArray elements = rows[0].toObject()["elements"].toArray();
        QJsonObject durationInTraffic = elements[0].toObject()["duration_in_traffic"].toObject();

        // Set the departure time for 5 mins from now
        QDateTime departureTime = QDateTime::currentDateTime();
        departureTime = departureTime.addSecs(300); // 5 Mins from now
        durationInTraffic.insert("departure_time", departureTime.toString("hh:mm"));

        // Set the arrival time to the time retrieved from google
        departureTime = departureTime.addSecs(durationInTraffic.value("value").toDouble());
        durationInTraffic.insert("arrival_time", departureTime.toString("hh:mm"));

        // Set the destination location
        if (destinations == "7205+North+137th+Avenue+Luke+Air+Force+Base+AZ") // Work
            durationInTraffic.insert("destination", "Work");
        else
            durationInTraffic.insert("destination", "Unknown Fix Me");

        // Send the updated json
        QJsonDocument doc(durationInTraffic);
        emit sendTrafficJson(QString(doc.toJson(QJsonDocument::Compact)));

    } else {
        QString strReply = (QString)reply->readAll();
    }
}

void TrafficThread::setupTrafficThread()
{
    this->userLoggedIn = true;
    QString val;
    QFile file(":/mirrorConfig.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject configJson = d.object();
    QJsonObject googleApiConfig = configJson.value(QString("googleApi")).toObject();

    // Now populate the fields
    this->googleDistanceAPIKey = googleApiConfig["googDistanceMatrixApi"].toString();
    this->origins = googleApiConfig["origins"].toString();
    this->destinations = googleApiConfig["destinations"].toString();
}

void TrafficThread::setUserLoggedIn(bool userLoggedIn)
{
    this->userLoggedIn = userLoggedIn;
    if (userLoggedIn)
        this->timerHit();
}
