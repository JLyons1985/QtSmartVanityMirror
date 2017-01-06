#ifndef WEATHERRETRIEVERTHREAD_H
#define WEATHERRETRIEVERTHREAD_H

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

class WeatherRetrieverThread : public QThread
{
	Q_OBJECT

signals :
    void sendCurrentWeatherJson(QString weatherJson);
    void sendForecastWeatherJson(QString weatherJson);
	
private:
    // Variables
    QString wundergroundAPIKey;
    const QString wundergroundMainURL = "http://api.wunderground.com/api/";
    const QString wundergroundWXIconURL = "http://icons.wxug.com/i/c/j/";
    const  QString RADAR_RADIUS = "50";
    const QString RADAR_WIDTH = "640";
    const  QString RADAR_HEIGHT = "480";
    QString obsLat;
    QString obsLon;
    QString state;
    QString city;
    bool userLoggedIn;

    // Functions
	void run();
	QNetworkAccessManager* nam;
    QNetworkAccessManager* namFcst;
	
private slots :
	void timerHit();
    void getCurrentWXRequestFinished(QNetworkReply* reply);
    void getWxRequestForecastFinished(QNetworkReply* replyFcst);

public:
    WeatherRetrieverThread();
    void setupWeatherRetriever();
    void setUserLoggedIn(bool userLoggedIn);
    void getWXForecast();

};

#endif // WEATHERRETRIEVERTHREAD_H
