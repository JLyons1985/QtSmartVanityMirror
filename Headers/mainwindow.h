#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include "clockthread.h"
#include "RaspiGPIOThread.h"
#include "MainWindowStatesThread.h"
#include "DIALThread.h"
#include "WeatherRetrieverThread.h"
#include "TrafficThread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QObject
{
    Q_OBJECT
    
public:
	
	// Constructor destructors
    MainWindow();
    ~MainWindow();
	
	// Getters
	bool isMotionDetected();
	bool isVanityAsleep();
	
	// Setters
	void setMotionDetected(bool isMotionDetected);
	void setVanityAsleep(bool isVanityAsleep);
	
	// Show/Hide main panels
	void showMainPanels();
	void hideMainPanels();
	void fadeInPanels();
	void fadeOutPanels();
	
	// Show/Hide Notification Label
	void showNotificationLabel(QString notification);
	void hideNotificationLabel();

protected slots :
	
	// Used to pass the pin value changes
	void pinValueChanged(int pin, QString value);
    void checkToHideMainPanels();
    void launchYoutube(QString url);
    void closeYoutube();
    void hideYoutubeBrowser();
    void recieveCurrentWeatherJson(QString currentWx);
    void recieveTrafficJson(QString trafficJson);

private:
	
	// Private variables
	ClockThread clockThread;
	RaspiGPIOThread raspiThread;
	MainWindowStatesThread mainStatesThread;
    DIALThread dialThread;
	bool motionDetected;
    bool vanityAsleep;
    WeatherRetrieverThread wxRecieverThread;
    TrafficThread trafficThread;
	
    static const int fadeSeconds = 3000;
    static const int port = 7465;
	
	// Private Functions
	void setupThreads();
	void delay(int millisecondsToWait);

    // Setup any other ui elements
    void setupMyUI();

	
};

#endif // MAINWINDOW_H
