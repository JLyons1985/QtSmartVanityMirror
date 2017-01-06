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

#include "Headers/MainWindowStatesThread.h"
#include <QTimer>
#include <QDateTime>

// Main run override for the thread
void MainWindowStatesThread::run()
{
    //this->timerHit();
	QTimer timer;
	connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
    timer.setInterval(1500000);
	timer.start();   // puts one event in the threads event queue
	exec();
	timer.stop();
}

// When the timer is reached this does logic to determine on showing or hiding panels
void MainWindowStatesThread::timerHit()
{
		emit checkToHideMainPanels();	
}
