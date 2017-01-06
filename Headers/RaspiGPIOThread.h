#ifndef RASPIGPIOTHREAD_H
#define RASPIGPIOTHREAD_H

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
* Project File: RaspberryPiGPIO.h
* Project Description: Controls the interaction between the program and the
*						GPIO pins on the Pi.
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
*************************************************************************/

// IMPORTS
#include <QThread>

#ifdef Q_OS_UNIX
#include <wiringPi.h>
#endif

class RaspiGPIOThread : public QThread
{
	Q_OBJECT
		
signals :
	
	void pinValueChanged(int pin, QString value);
	
private:
	
	void run();
	QString getPinValue(int pin);
	
	const static int motionDetectorPin = 2;
    const static int pushButtonPin = 0;
	QString motionDetectorOldValue;
    QString pushButtonOldValue;
	
	
private slots :
	
	void timerHit();
	
public:
	
	void setupWiringPi();

};

#endif // RASPIGPIOTHREAD_H
