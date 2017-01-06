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
* Project File: MainFormHandler.h
* Project Description: Header file for the main form handler
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include "Headers/MainFormHandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#ifdef Q_OS_WIN
#include <opencv2/opencv.hpp>
#endif
#include <DataObject.h>
#include <Headers/speechdecoder.h>
#include <Headers/wxforecastitem.h>

// Constructor for the form handler
MainFormHandler::MainFormHandler(QObject *parent) : QObject(parent)
{
    // Create all the threads and their connections
    // Setup raspiGPIO if on unix
#ifdef Q_OS_UNIX
    this->raspiThread.setupWiringPi();
#endif

    // Setup Variables
    this->vanityAsleep = true;
    this->motionDetected = false;
    this->loggingIn = false;

    this->clockThread = new ClockThread();
    this->dialThread = new DIALThread();
    this->trafficThread = new TrafficThread();
    this->wxRecieverThread = new WeatherRetrieverThread();

    qDebug() << "Setting up threads.";
    this->setupThreads();

    qDebug() << "Setting up speech recorder and text to speech.";
    this->speechRecorder = new SpeechAudioRecorder();
    this->textToSpeech = new TextToSpeech();
}

MainFormHandler::MainFormHandler(QQmlContext *mainContext, QObject *parent )
    : QObject(parent)
{
    this->myMainContext = mainContext;
    // Create all the threads and their connections
    // Setup raspiGPIO if on unix
#ifdef Q_OS_UNIX
    this->raspiThread.setupWiringPi();
#endif

    // Setup Variables
    this->vanityAsleep = true;
    this->motionDetected = false;
    this->loggingIn = false;

    this->clockThread = new ClockThread();
    this->dialThread = new DIALThread();
    this->trafficThread = new TrafficThread();
    this->wxRecieverThread = new WeatherRetrieverThread();

    qDebug() << "Setting up threads.";
    this->setupThreads();

    qDebug() << "Setting up speech recorder and text to speech.";
    this->speechRecorder = new SpeechAudioRecorder();
    this->textToSpeech = new TextToSpeech();
}

// Destructor
MainFormHandler::~MainFormHandler()
{
    clockThread->quit();
    clockThread->wait();
    wxRecieverThread->quit();
    wxRecieverThread->wait();
    trafficThread->quit();
    trafficThread->wait();
    dialThread->quit();
    dialThread->wait();
    mainStatesThread.quit();
    mainStatesThread.wait();
    raspiThread.quit();
    raspiThread.wait();
    quoteThread.quit();
    quoteThread.wait();
}

// Sets the root context from the ui form
void MainFormHandler::setMainContext(QQmlContext* myMainContext)
{
    this->myMainContext = myMainContext;

    // Change this for testing based on OS
#ifdef Q_OS_UNIX
    this->motionDetected = false;
    this->vanityAsleep = true;
#elif defined(Q_OS_WIN32)
    this->motionDetected = true;
    this->vanityAsleep = true;
#endif


}

void MainFormHandler::test()
{

}


// Setup the threads
void MainFormHandler::setupThreads()
{
    // Setup the Discovery and Launch Listerner
    qDebug () << "Starting DIAL Thread";
    this->dialThread->setupDIALThread();

    // Setup weather reciever
    qDebug () << "Starting WX Thread";
    this->wxRecieverThread->setupWeatherRetriever();

    // Setup traffic thread
    qDebug () << "Starting Traffic Thread";
    this->trafficThread->setupTrafficThread();

    // Instantiate clock timer
    QObject::connect(clockThread, SIGNAL(sendTime(QString)), this, SLOT(setTimeText(QString)), Qt::QueuedConnection);
    QObject::connect(clockThread, SIGNAL(sendDate(QString)), this, SLOT(setDateText(QString)), Qt::QueuedConnection);
    QObject::connect(clockThread, SIGNAL(sendSeconds(QString)), this, SLOT(setSecondsText(QString)), Qt::QueuedConnection);
    QObject::connect(clockThread, SIGNAL(sendAmPm(QString)), this, SLOT(setAmPmText(QString)), Qt::QueuedConnection);

    // Instantiate Wx Reciever
    QObject::connect(wxRecieverThread, SIGNAL(sendCurrentWeatherJson(QString)), this, SLOT(recieveCurrentWeatherJson(QString)), Qt::QueuedConnection);
    QObject::connect(wxRecieverThread, SIGNAL(sendForecastWeatherJson(QString)), this, SLOT(recieveForecastWeatherJson(QString)), Qt::QueuedConnection);

    // Instantiate Traffic Thread
    QObject::connect(trafficThread, SIGNAL(sendTrafficJson(QString)), this, SLOT(recieveTrafficJson(QString)), Qt::QueuedConnection);

    // Instantiate DIAL Thread
    QObject::connect(dialThread, SIGNAL(launchYoutube(QString)), this, SLOT(launchYoutube(QString)), Qt::QueuedConnection);
    QObject::connect(dialThread, SIGNAL(closeYoutube()), this, SLOT(closeYoutube()), Qt::QueuedConnection);
    QObject::connect(dialThread, SIGNAL(updateCalendarList(QList<QObject*>)), this, SLOT(updateCalendarList(QList<QObject*>)), Qt::QueuedConnection);
    QObject::connect(dialThread, SIGNAL(speechDecoded(QString)), this, SLOT(handleDecodedSpeech(QString)), Qt::QueuedConnection);
    QObject::connect(dialThread, SIGNAL(recognitionMessage(QString)), this, SLOT(showRecognitionMsg(QString)), Qt::QueuedConnection);
    QObject::connect(dialThread, SIGNAL(recognitionSuccess(QString)), this, SLOT(handleRecognitionSuccess(QString)), Qt::QueuedConnection);

    // Quote thread
    QObject::connect(&quoteThread, SIGNAL(updateQuote(QString, QString)), this, SLOT(handleUpdateQuote(QString, QString)), Qt::QueuedConnection);

    // Instantiate Raspi timer
    QObject::connect(&raspiThread, SIGNAL(pinValueChanged(int, QString)), this, SLOT(pinValueChanged(int, QString)), Qt::QueuedConnection);

    // Instantiate Main States
    QObject::connect(&mainStatesThread, SIGNAL(checkToHideMainPanels()), this, SLOT(checkToHideMainPanels()), Qt::QueuedConnection);

    // Start the threads
    qDebug () << "Starting clock, raspi, and main states Thread";
    clockThread->start();
    wxRecieverThread->start();
    trafficThread->start();
    dialThread->start();
    raspiThread.start();
    mainStatesThread.start();
    quoteThread.start();

    // Get the calendar items
    qDebug () << "Asking for starting calendar items";
    this->dialThread->getCalendarItems(9);
}

// Slot functions used only by this object
// Date Time
// Set the date
void MainFormHandler::setDateText(QString dateText)
{
    this->dateText = dateText;
    emit changeOfDateText();
}

// Set the time
void MainFormHandler::setTimeText(QString timeText)
{
    this->timeText = timeText;
    emit changeOfTimeText();
}

// Set the seconds
void MainFormHandler::setSecondsText(QString secondsText)
{
    this->secondsText = secondsText;
    emit changeOfSecondsText();
}

// Set the am pm
void MainFormHandler::setAmPmText(QString amPmText)
{
    this->amPmText = amPmText;
    emit changeOfAmPmText();
}


// WEATHER

// Handle retrieving the weather json
void MainFormHandler::recieveCurrentWeatherJson(QString currentWx)
{
    QJsonDocument jsonStr = QJsonDocument::fromJson(currentWx.toUtf8());
    QJsonObject currentWxObj = jsonStr.object();

    // Populate the weather fields
    QString tmpTempString = QString::number(currentWxObj.value("temp_f").toDouble());

    // Set Text to variables
    this->currentWxText = "Weather: " + currentWxObj.value("weather").toString();
    this->currentWindText = "Winds: " + currentWxObj.value("wind_string").toString();
    this->currentWxTempText = tmpTempString + "°" + "F";
    QDateTime currentTime = QDateTime::currentDateTime();
    if (currentTime.time().hour() >= 19 || currentTime.time().hour() < 5)
        this->currentWxImgLoc = "images/wxIcons/nt_" + currentWxObj.value("icon").toString() + ".png";
    else
        this->currentWxImgLoc = "images/wxIcons/" + currentWxObj.value("icon").toString() + ".png";

    // Emit change
    emit changeOfCurrentWx();
}

// Handle retrieving the forecast weather json
void MainFormHandler::recieveForecastWeatherJson(QString fcstWx)
{
    QJsonDocument jsonStr = QJsonDocument::fromJson(fcstWx.toUtf8());
    QJsonObject fcstWxObj = jsonStr.object()["simpleforecast"].toObject();

    //qDebug() << fcstWxObj;

    QJsonArray tmpArray = fcstWxObj["forecastday"].toArray();
    QList<QObject*> fcstDataList;
    int fcstItems = tmpArray.size();
    int i = 0;
    foreach (const QJsonValue & value, tmpArray)
    {
        // Create new json obj
        QJsonObject obj = value.toObject();

        WxForecastItem* tmpObj = new WxForecastItem();
        tmpObj->setDay(obj["date"].toObject().value("weekday").toString());
        QDateTime currentTime = QDateTime::currentDateTime();
        if (currentTime.time().hour() >= 19 || currentTime.time().hour() < 5)
            tmpObj->setImgLoc("images/wxIcons/nt_" + obj.value("icon").toString() + ".png");
        else
            tmpObj->setImgLoc("images/wxIcons/" + obj.value("icon").toString() + ".png");
        tmpObj->setHighTemp(obj["high"].toObject().value("fahrenheit").toString() + "°" + "F");
        tmpObj->setLowTemp(obj["low"].toObject().value("fahrenheit").toString() + "°" + "F");

        double opacity = (fcstItems - i) / (double)fcstItems;

        tmpObj->setOpacity(opacity);

        fcstDataList.append(tmpObj);

        i += 1;
    }

    this->myMainContext->setContextProperty("wxForecastModel", QVariant::fromValue(fcstDataList));
}

// TRAFFIC

// Handle retrieving the traffic json
void MainFormHandler::recieveTrafficJson(QString trafficJson)
{
    QJsonDocument jsonStr = QJsonDocument::fromJson(trafficJson.toUtf8());
    QJsonObject trafficJsonObj = jsonStr.object();

    // Populate the traffic fields
    this->trafficArrivalTimeText = "Arrival Time: " + trafficJsonObj["arrival_time"].toString();
    this->trafficDepartureTimeText = "Departure Time : " + trafficJsonObj["departure_time"].toString();
    this->trafficDestinationText = "Destination: " + trafficJsonObj["destination"].toString();

    // Emit change
    emit changeOfTraffic();
}

// BROWSER

// Launch the youtube browser and emit change of url
void MainFormHandler::launchYoutube(QString url)
{
    this->browserURL = url;
    emit changeOfBrowserURL();
    emit showBrowser();
}

// Close youtube browser
void MainFormHandler::closeYoutube()
{
    emit hideBrowser();
}

// Hide youtube browser
void MainFormHandler::hideYoutubeBrowser()
{
    emit hideBrowser();
}

// MAIN WINDOW

// Handles new calendar data
void MainFormHandler::updateCalendarList(QList<QObject*> dataList)
{
    this->myMainContext->setContextProperty("calendarModel", QVariant::fromValue(dataList));
}

// Setters
void MainFormHandler::setMotionDetected(bool isMotionDetected)
{
    this->motionDetected = isMotionDetected;
}

void MainFormHandler::setVanityAsleep(bool isVanityAsleep)
{
    this->vanityAsleep = isVanityAsleep;
}

void MainFormHandler::setUserLoggedIn(bool isUserLoggedIn)
{
    this->userLoggedIn = isUserLoggedIn;
}

void MainFormHandler::setWhichUserIsLoggedIn(QString whichUserLoggedIn)
{
    this->whichUserLoggedIn = whichUserLoggedIn;
}

// Getters
bool MainFormHandler::isMotionDetected()
{
    return this->motionDetected;
}

bool MainFormHandler::isVanityAsleep()
{
    return this->vanityAsleep;
}

bool MainFormHandler::isUserLoggedIn()
{
    return this->userLoggedIn;
}

// gets which user is logged in
QString MainFormHandler::getWhichUserIsLoggedIn()
{
    return this->whichUserLoggedIn;
}

// Check if we should hide the panels
void MainFormHandler::checkToHideMainPanels()
{
    // Logic to see if we show show or hide main panels
    if (this->isMotionDetected() == false && this->isVanityAsleep() == false)
        this->hideMainPanels();
}

// Starts the login procees with fac recognition
void MainFormHandler::startLoginProcess()
{
    // Check if vanity was asleep first
    if (this->isVanityAsleep() && !this->loggingIn)
    {
        // Show the notification
        this->textToSpeech->convertTextToSpeech("Hello, trying to see if I recognize you.");
        this->showNotificationLabel("Hello, trying to see if I recognize you...");
        this->dialThread->recognizeFaceStart();
        this->loggingIn = true;
    }
}

// Show/Hide main panels
void MainFormHandler::showMainPanels()
{
    // Show the main panels
    // Check if vanity was asleep first
    if (this->isVanityAsleep())
    {
        // Set vanity awake
        this->setVanityAsleep(false);

        // Now wake the mirror
        emit wakeUpMirror();
    }
}

void MainFormHandler::hideMainPanels()
{
    // Check if vanity was asleep first
    if (!this->isVanityAsleep())
    {
        // Set vanity asleep
        this->setVanityAsleep(true);

        // Show the notification
        this->textToSpeech->convertTextToSpeech("Hope to see you again soon.");
        this->notificationText = "Goodbye...";
        emit changeOfNotificationText();

        this->setUserLoggedIn(false);
        this->setWhichUserIsLoggedIn("");
        this->dialThread->setUserLoggedIn(false);
        this->trafficThread->setUserLoggedIn(false);
        this->wxRecieverThread->setUserLoggedIn(false);
        this->clockThread->setUserLoggedIn(false);

        // Now wake the mirror
        emit sleepMirror();
    }
}

void MainFormHandler::fadeInPanels()
{
    emit showPanels();
}

void MainFormHandler::fadeOutPanels()
{
    emit hidePanels();
}

// Show/Hide Notification Label
void MainFormHandler::showNotificationLabel(QString notification)
{
    this->notificationText = notification;
    emit changeOfNotificationText();
    emit showNotificationLabel();
}

// RASPBERRY PI GPIO

// Pin value changed so call the correct functions
void MainFormHandler::pinValueChanged(int pin, QString value)
{
    // Switch on the pin value changed
    switch (pin)
    {
        // Motion detector
        case 2:
            if (value == "HIGH") // Motion has been detected
            {
                this->motionDetected = true;
                this->startLoginProcess();
            }
            else // No more motion detected
            {
                this->motionDetected = false;
            }
            break;
        // Push Button
        case 0:
            if (value == "HIGH") // Button is NOT Pressed
            {
                // Check if we are recording
                if (this->speechRecorder->isRecording()) // We were recording
                {
                    this->speechRecorder->stopRecording();

                    // Now run the speech through the speech decoder
                    this->dialThread->getSpeechDecoded();
                }

            }
            else // Button IS Pressed
            {
                // Check if we were recording already
                if (this->speechRecorder->isRecording())
                {
                    this->speechRecorder->stopRecording();
                    this->speechRecorder->startRecording();
                }
                else
                    this->speechRecorder->startRecording();
            }
            break;
     }
}

// Speech
void MainFormHandler::handleDecodedSpeech(QString decodedSpeech)
{
    // Grab the transcribed speed
    QJsonDocument jsonStr = QJsonDocument::fromJson(decodedSpeech.toUtf8());
    QJsonObject speechJsonObj = jsonStr.object();
    QJsonArray myArray = speechJsonObj["results"].toArray()[0].toObject()["alternatives"].toArray();

    QString speech = myArray[0].toObject()["transcript"].toString();

    // Decode the speech
    SpeechDecoder* myDecoder = new SpeechDecoder();
    QString command = myDecoder->decodeSpeech(speech);

    qDebug() << command;

    // Now do something with the commands
    if (command == "showCalendar")
    {
        this->textToSpeech->convertTextToSpeech("Okay, displaying your calendar.");
        this->notificationText = "Displaying your calendar.";
        emit changeOfNotificationText();
        emit showThenHideNotificationLabel();
        emit showCalendar();
    }
    if (command == "hideCalendar")
    {
        this->textToSpeech->convertTextToSpeech("Okay, hiding your calendar.");
        this->notificationText = "Hiding your calendar.";
        emit changeOfNotificationText();
        emit showThenHideNotificationLabel();
        emit hideCalendar();
    }
    if (command == "hideForecast")
    {
        this->textToSpeech->convertTextToSpeech("Okay, hiding the weather forecast.");
        this->notificationText = "Hiding the weather forecast.";
        emit changeOfNotificationText();
        emit showThenHideNotificationLabel();
        emit hideForecast();
    }
    if (command == "showForecast")
    {
        this->textToSpeech->convertTextToSpeech("Okay, displaying the weather forecast.");
        this->wxRecieverThread->getWXForecast();
        this->notificationText = "Displaying the weather forecast.";
        emit changeOfNotificationText();
        emit showThenHideNotificationLabel();
        emit showForecast();
    }
    if (command == "recognizeFace") this->dialThread->recognizeFace();
    if (command == "sleep")
    {
        this->textToSpeech->convertTextToSpeech("Okay, going to sleep.");
        this->motionDetected = false;
        this->hideMainPanels();
    }
    if (command == "Undef")
    {
        this->textToSpeech->convertTextToSpeech("I didn't quite hear you.");
        this->notificationText = "I did not understand your request. Please try again.";
        emit changeOfNotificationText();
        emit showThenHideNotificationLabel();
    }
}

// Face recognitions
void MainFormHandler::showRecognitionMsg(QString msg)
{
    this->notificationText = msg;
    emit changeOfNotificationText();
}

// Handles the face recognition
void MainFormHandler::handleRecognitionSuccess(QString msg)
{
    if (msg == "false") // No one recognized
    {
        this->setUserLoggedIn(false);
        this->vanityAsleep = true;
        this->setWhichUserIsLoggedIn("");
        emit hideNotificationLabel();
        this->loggingIn = false;
    }
    else
    {        
        this->dialThread->setUserLoggedIn(true);
        this->trafficThread->setUserLoggedIn(true);
        this->wxRecieverThread->setUserLoggedIn(true);
        this->clockThread->setUserLoggedIn(true);
        this->setUserLoggedIn(true);
        this->setWhichUserIsLoggedIn(msg);
        this->textToSpeech->convertTextToSpeech("Welcome back, " + this->getWhichUserIsLoggedIn() + "! You look great!");
        this->notificationText =  "Welcome back, " + this->getWhichUserIsLoggedIn() + "! You look great!";
        emit changeOfNotificationText();
        this->showMainPanels();
        this->loggingIn = false;
    }
}

// Quote of the day
void MainFormHandler::handleUpdateQuote(QString quote, QString author)
{
    this->quoteText = quote;
    this->authorText = "~ " + author;
    emit changeOfQuote();
}

