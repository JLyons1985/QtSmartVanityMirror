#ifndef MAINWINDOWSTATESTHREAD_H
#define MAINWINDOWSTATESTHREAD_H

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
* Project File: MainWindowStatesThread.h
* Project Description: Checks and updates window states as needed
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <QThread>

class MainWindowStatesThread : public QThread
{
	Q_OBJECT

signals :
    void checkToHideMainPanels();
	
public :
	
private:
	
	// Override for run
	void run();
	
private slots :
	
	// Internal time function
	 void timerHit();
	
	
};

#endif // MAINWINDOWSTATESTHREAD_H