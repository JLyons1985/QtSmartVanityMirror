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

#include "Headers/WeatherRetrieverThread.h"
#include <QTimer>
#include <QFile>
#include <QJsonDocument>

WeatherRetrieverThread::WeatherRetrieverThread()
{

}

void WeatherRetrieverThread::getCurrentWXRequestFinished(QNetworkReply* reply)
{


    // Handle the json reply fir the current weather
    if(reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();
        //qDebug() << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject currentObservation = jsonObject.value(QString("current_observation")).toObject();

        // set the lat lon then pass the curr
        QJsonObject tmpJson = currentObservation["display_location"].toObject();
        this->obsLat = tmpJson.value(QString("latitude")).toString();
        this->obsLon = tmpJson.value(QString("longitude")).toString();

        QJsonDocument doc(currentObservation);

        emit sendCurrentWeatherJson(QString(doc.toJson(QJsonDocument::Compact)));

    } else {
        qDebug() << "ERROR";
    }

    //delete reply;

}

void WeatherRetrieverThread::getWxRequestForecastFinished(QNetworkReply* replyFcst)
{


    // Handle the json reply fir the current weather
    if(replyFcst->error() == QNetworkReply::NoError) {

        QString strReply = (QString)replyFcst->readAll();

        //qDebug() << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject wxForecast = jsonObject.value(QString("forecast")).toObject();

        //qDebug() << wxForecast;

        QJsonDocument doc(wxForecast);

        emit sendForecastWeatherJson(QString(doc.toJson(QJsonDocument::Compact)));

    } else {
        qDebug() << "ERROR Wx Forecast";
    }

    //delete reply;

}

void WeatherRetrieverThread::run()
{
    this->timerHit();
    //qDebug() << "Timer hit 1";
	QTimer timer;
	connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(3000000);
	timer.start();   // puts one event in the threads event queue
	exec();
	timer.stop();
}

void WeatherRetrieverThread::timerHit()
{
    if(this->userLoggedIn)
    {
        // Get the current weather from Wunderground
        nam = new QNetworkAccessManager(this);
        QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCurrentWXRequestFinished(QNetworkReply*)));

        qDebug() << "Asking for current weather.";

        //Create get current wx url
        QUrl url(wundergroundMainURL + wundergroundAPIKey + "/conditions/q/" + state + "/" + city + ".json");
        QNetworkReply* reply = nam->get(QNetworkRequest(url));
    }

    this->getWXForecast();
}

// Opens the config file and stores the data needed
void WeatherRetrieverThread::setupWeatherRetriever()
{
    this->userLoggedIn = true;
    QString val;
    QFile file(":/mirrorConfig.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    //qWarning() << val;
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject configJson = d.object();
    QJsonObject wundergrounApiConfig = configJson.value(QString("wundergroundApi")).toObject();

    // Now populate the fields
    this->wundergroundAPIKey = wundergrounApiConfig["wundergroundApiKey"].toString();
    this->state = wundergrounApiConfig["wundergroundStateIdentifier"].toString();
    this->city = wundergrounApiConfig["wundergroundCity"].toString();
}

void WeatherRetrieverThread::setUserLoggedIn(bool userLoggedIn)
{
    this->userLoggedIn = userLoggedIn;
    if (userLoggedIn)
        this->timerHit();
}

// Retrieves the wx forecast
void WeatherRetrieverThread::getWXForecast()
{
    if(this->userLoggedIn)
    {
        // Get the current weather from Wunderground
        namFcst = new QNetworkAccessManager(this);
        QObject::connect(namFcst, SIGNAL(finished(QNetworkReply*)), this, SLOT(getWxRequestForecastFinished(QNetworkReply*)));

        qDebug() << "Asking for wx forecast.";

        //Create get current wx url
        QUrl url(wundergroundMainURL + wundergroundAPIKey + "/forecast10day/q/" + state + "/" + city + ".json");
        QNetworkReply* replyFcst = namFcst->get(QNetworkRequest(url));
    }
}
