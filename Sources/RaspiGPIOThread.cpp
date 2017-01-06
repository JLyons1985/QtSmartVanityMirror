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
* Project File: RaspberryPiGPIO.cpp
* Project Description: Controls the interaction between the program and the
*						GPIO pins on the Pi.
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
*************************************************************************/

#include "Headers/RaspiGPIOThread.h"
#include <QTimer>
#include <QDebug>

void RaspiGPIOThread::run()
{
    QTimer timer;
	connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);
	timer.setInterval(100);
	timer.start();   // puts one event in the threads event queue
	exec();
	timer.stop();
}

void RaspiGPIOThread::timerHit()
{
	// Look through all the pins for value changes
	QString newValue = getPinValue(motionDetectorPin);

    // Compare motion detector to old value
	if (motionDetectorOldValue != newValue) // Stuff has changed
	{
		motionDetectorOldValue = newValue;
		emit pinValueChanged(motionDetectorPin, motionDetectorOldValue);
	}

    newValue = getPinValue(pushButtonPin);

    // Compare pushbutton to old value
    if (pushButtonOldValue != newValue) // Stuff has changed
    {
        pushButtonOldValue = newValue;
        emit pinValueChanged(pushButtonPin, pushButtonOldValue);
    }
}

// Retrives the pin value as a string high or low
QString RaspiGPIOThread::getPinValue(int pin)
{
#ifdef Q_OS_UNIX
    if (digitalRead(pin) == 1)
        return "HIGH";
    else
        return "LOW";
#elif defined(Q_OS_WIN32)
    return "HIGH";
#endif
}

// Sets up wiring pi, has to be ran as sudo
void RaspiGPIOThread::setupWiringPi()
{
    this->motionDetectorOldValue = "LOW";
#ifdef Q_OS_UNIX
	// Start wiringPi
    wiringPiSetup();
	
	// Set the different pin modes
    pinMode(motionDetectorPin, INPUT);
    pullUpDnControl(motionDetectorPin, PUD_DOWN);
    pinMode(pushButtonPin, INPUT);
    pullUpDnControl(pushButtonPin, PUD_UP);
#endif
	
}
