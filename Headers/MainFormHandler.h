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

#ifndef MAINFORMHANDLER_H
#define MAINFORMHANDLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Headers/clockthread.h>
#include <Headers/DIALThread.h>
#include <Headers/WeatherRetrieverThread.h>
#include <Headers/TrafficThread.h>
#include <Headers/MainWindowStatesThread.h>
#include <Headers/RaspiGPIOThread.h>
#include <Headers/CalendarModel.h>
#include <Headers/speechaudiorecorder.h>
#include <Headers/quotethread.h>
#include <Headers/texttospeech.h>



class MainFormHandler : public QObject
{
    Q_OBJECT

    // Connects the ui labels to the functions to update

    // Date Time
    Q_PROPERTY(QString dateText READ getDateText NOTIFY changeOfDateText)
    Q_PROPERTY(QString timeText READ getTimeText NOTIFY changeOfTimeText)
    Q_PROPERTY(QString secondsText READ getSecondsText NOTIFY changeOfSecondsText)
    Q_PROPERTY(QString amPmText READ getAmPmText NOTIFY changeOfAmPmText)

    // Current Weather
    Q_PROPERTY(QString currentWxTempText READ getCurrentWxTempText NOTIFY changeOfCurrentWx)
    Q_PROPERTY(QString currentWxText READ getCurrentWxText NOTIFY changeOfCurrentWx)
    Q_PROPERTY(QString currentWindText READ getCurrentWindText NOTIFY changeOfCurrentWx)
    Q_PROPERTY(QString currentWxImgLoc READ getCurrentWxImgLoc NOTIFY changeOfCurrentWx)

    // Traffic
    Q_PROPERTY(QString trafficDestinationText READ getTrafficDestinationText NOTIFY changeOfTraffic)
    Q_PROPERTY(QString trafficDepartureTimeText READ getTrafficDepartureTimeText NOTIFY changeOfTraffic)
    Q_PROPERTY(QString trafficArrivalTimeText READ getTrafficArrivalTimeText NOTIFY changeOfTraffic)

    // Browser
    Q_PROPERTY(QString browserURL READ getBrowserURL NOTIFY changeOfBrowserURL)

    // Main Window
    Q_PROPERTY(QString notificationText READ getNotificationText NOTIFY changeOfNotificationText)

    // Quote of the day
    Q_PROPERTY(QString quoteText READ getQuoteText NOTIFY changeOfQuote)
    Q_PROPERTY(QString authorText READ getAuthorText NOTIFY changeOfQuote)


public:
    // Constructors / Destructors
    explicit MainFormHandler(QObject *parent = 0);
    MainFormHandler(QQmlContext *mainContext, QObject *parent = 0);
    ~MainFormHandler();

    // Functions
    void setMainContext(QQmlContext* myMainContext);

    // Date Time
    QString getDateText() const { return dateText; }
    QString getTimeText() const { return timeText; }
    QString getSecondsText() const { return secondsText; }
    QString getAmPmText() const { return amPmText; }

    // Weather
    QString getCurrentWxTempText() const { return currentWxTempText; }
    QString getCurrentWxText() const { return currentWxText; }
    QString getCurrentWindText() const { return currentWindText; }
    QString getCurrentWxImgLoc() const { return currentWxImgLoc; }

    // Quote of the day
    QString getQuoteText() const { return quoteText; }
    QString getAuthorText() const { return authorText; }

    // Traffic
    QString getTrafficDestinationText() const { return trafficDestinationText; }
    QString getTrafficDepartureTimeText() const { return trafficDepartureTimeText; }
    QString getTrafficArrivalTimeText() const { return trafficArrivalTimeText; }

    // Browser
    QString getBrowserURL() const { return browserURL; }

    // Main Window
    QString getNotificationText() const { return notificationText; }

    bool isMotionDetected();
    bool isVanityAsleep();
    bool isUserLoggedIn();
    QString getWhichUserIsLoggedIn();
    void setMotionDetected(bool isMotionDetected);
    void setVanityAsleep(bool isVanityAsleep);
    void setUserLoggedIn(bool isUserLoggedIn);
    void setWhichUserIsLoggedIn(QString whichUserLoggedIn);

    // Show/Hide main panels
    void showMainPanels();
    void hideMainPanels();
    void fadeInPanels();
    void fadeOutPanels();

    // Show/Hide Notification Label
    void showNotificationLabel(QString notification);

    void test();

private:
    // Variables
    CalendarModel *myCalendarModel;
    QQmlContext *myMainContext;

    // Date Times
    QString dateText;
    QString timeText;
    QString secondsText;
    QString amPmText;
    ClockThread* clockThread;

    // Current Weather
    QString currentWxTempText;
    QString currentWxText;
    QString currentWindText;
    QString currentWxImgLoc;
    WeatherRetrieverThread* wxRecieverThread;

    // Traffic
    QString trafficDestinationText;
    QString trafficDepartureTimeText;
    QString trafficArrivalTimeText;
    TrafficThread* trafficThread;

    // Quote of the day
    QString quoteText;
    QString authorText;
    QuoteThread quoteThread;

    // Browser
    QString browserURL;

    // Dial interaction
    DIALThread* dialThread;

    // Main Window States
    bool motionDetected;
    bool vanityAsleep;
    bool userLoggedIn;
    bool loggingIn;
    QString whichUserLoggedIn;
    MainWindowStatesThread mainStatesThread;

    // Raspberry Pi GPIO
    RaspiGPIOThread raspiThread;


    // Main Windows
    QString notificationText;

    // Speech
    SpeechAudioRecorder* speechRecorder;
    TextToSpeech* textToSpeech;

    static const int fadeSeconds = 3000;

    // Private functions
    void setupThreads();
    void startLoginProcess();
    void setupUI();

signals:
    // Date Time
    void changeOfDateText();
    void changeOfTimeText();
    void changeOfSecondsText();
    void changeOfAmPmText();

    // Weather
    void changeOfCurrentWx();
    void hideForecast();
    void showForecast();

    // Traffic
    void changeOfTraffic();

    // Quote
    void changeOfQuote();

    // Browser
    void changeOfBrowserURL();
    void showBrowser();
    void hideBrowser();

    // Main Window
    void changeOfNotificationText();
    void showPanels();
    void hidePanels();
    void showNotificationLabel();
    void hideNotificationLabel();
    void showThenHideNotificationLabel();
    void wakeUpMirror();
    void sleepMirror();

    // Calendar
    void showCalendar();
    void hideCalendar();

public slots:

private slots:
    // Slot functions used only by this object
    // Date Time
    void setDateText(QString dateText);
    void setTimeText(QString timeText);
    void setSecondsText(QString secondsText);
    void setAmPmText(QString amPmText);

    // Weather
    void recieveCurrentWeatherJson(QString currentWx);
    void recieveForecastWeatherJson(QString fcstWx);

    // Traffic
    void recieveTrafficJson(QString trafficJson);

    // Browser
    void launchYoutube(QString url);
    void closeYoutube();
    void hideYoutubeBrowser();

    // Raspberyy Pi GPIO Pins
    void pinValueChanged(int pin, QString value);

    // Main Window
    void checkToHideMainPanels();

    // Master Server interactions
    void updateCalendarList(QList<QObject*> dataList);

    // Speech
    void handleDecodedSpeech(QString decodedSpeech);

    // Recognition Msg
    void showRecognitionMsg(QString msg);
    void handleRecognitionSuccess(QString msg);

    // Quote
    void handleUpdateQuote(QString quote, QString author);

};

#endif // MAINFORMHANDLER_H
