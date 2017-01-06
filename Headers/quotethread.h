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
* Project File: QuoteThread.h
* Project Description: Adds quotes to the mirror
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <QThread>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#ifndef QUOTETHREAD_H
#define QUOTETHREAD_H

class QuoteThread : public QThread
{
    Q_OBJECT

signals:
    void updateQuote(QString quote, QString author);

private:
    void run();
    bool userLoggedIn;
    QNetworkAccessManager* nam;

public:
    QuoteThread();
    void setUserLoggedIn(bool userLoggedIn);

private slots:
    void timerHit();
    void getQuoteRequestFinished(QNetworkReply* reply);
};

#endif // QUOTETHREAD_H
