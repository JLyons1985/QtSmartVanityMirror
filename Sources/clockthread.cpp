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
* Project File: ClockThread.cpp
* Project Description: Updates the clock as needed
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
*************************************************************************/

#include "Headers/clockthread.h"
#include <QTimer>
#include <QDateTime>

ClockThread::ClockThread()
{

}

void ClockThread::run()
{
    this->userLoggedIn = true;
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(500);
    timer.start();   // puts one event in the threads event queue
    exec();
    timer.stop();
}

void ClockThread::timerHit()
{
    if (this->userLoggedIn)
    {
        // Setup the variables based on time
        QDateTime currentTime = QDateTime::currentDateTime();
        QString newTime= currentTime.toString("hh:mm");
        QString newDate = currentTime.toString("ddd MMM d, yyyy");
        QString newSeconds = currentTime.toString("ss");
        QString newAmPm;
        int hour = currentTime.time().hour();
	
        // Compare the last variables with the new variables
        if(lastTime != newTime ){
            lastTime = newTime;
            emit sendTime(newTime) ;
        }
        if (lastDate != newDate) {
            lastDate = newDate;
            emit sendDate(newDate);
        }
        if (lastSeconds != newSeconds) {
            lastSeconds = newSeconds;
            emit sendSeconds(newSeconds);
        }
        if (hour >= 12)
            newAmPm = "PM";
        else
            newAmPm = "AM";
        if (lastAmPm != newAmPm)
        {
            lastAmPm = newAmPm;
            emit sendAmPm(newAmPm);
        }
    }
}

void ClockThread::setUserLoggedIn(bool userLoggedIn)
{
    this->userLoggedIn = userLoggedIn;
    if (userLoggedIn)
        this->timerHit();
}
