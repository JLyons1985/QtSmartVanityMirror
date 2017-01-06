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
* Project File: QuoteThread.cpp
* Project Description: Adds quotes to the mirror
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include "Headers/quotethread.h"
#include <QTimer>
#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QJsonArray>

QuoteThread::QuoteThread()
{

}

void QuoteThread::run()
{
    this->userLoggedIn = true;
    this->timerHit();
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(300000);
    timer.start();   // puts one event in the threads event queue
    exec();
    timer.stop();
}

void QuoteThread::timerHit()
{
    if (this->userLoggedIn)
    {
        nam = new QNetworkAccessManager(this);
        QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(getQuoteRequestFinished(QNetworkReply*)));

        QUrl serviceUrl = QUrl("http://api.forismatic.com/api/1.0/");
        QUrlQuery postData;
        postData.addQueryItem("method", "getQuote");
        postData.addQueryItem("format", "json");
        postData.addQueryItem("lang", "en");

        QNetworkRequest request(serviceUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader,
            "application/x-www-form-urlencoded");
        nam->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    }

}

void QuoteThread::setUserLoggedIn(bool userLoggedIn)
{
    this->userLoggedIn = userLoggedIn;
    if (userLoggedIn)
        this->timerHit();
}

void QuoteThread::getQuoteRequestFinished(QNetworkReply* reply)
{
    // Handle the json reply fir the current weather
    if(reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();
        qDebug() << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();

        qDebug() << jsonObject;

        emit updateQuote(jsonObject.value("quoteText").toString(), jsonObject.value("quoteAuthor").toString());

    } else {
        QString strReply = (QString)reply->readAll();
    }
}
