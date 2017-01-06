#include "Headers/mainwindow.h"
#include <QPropertyAnimation>
#include <QColor>
#include <QTime>
#include <QThread>

// Constructors
MainWindow::MainWindow()
{
    // Setup the UI
    this->setupMyUI();

	// Setup raspiGPIO
	this->raspiThread.setupWiringPi();
	
    // Setup the thread
    this->setupThreads();
	
}

// Destructor
MainWindow::~MainWindow()
{
    // Destory
	clockThread.quit();
	clockThread.wait();
	raspiThread.quit();
	raspiThread.wait();
	mainStatesThread.quit();
	mainStatesThread.wait();
    dialThread.quit();
    dialThread.wait();
    wxRecieverThread.quit();
    wxRecieverThread.wait();
    trafficThread.quit();
    trafficThread.wait();
	
}

// Setup any other ui elements
void MainWindow::setupMyUI()
{
    // Setup the graphics effect basically making everything transparent
//    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
//    eff->setOpacity(0);
//    ui->notificationLabel->setGraphicsEffect(eff);

//    QGraphicsOpacityEffect *effTwo = new QGraphicsOpacityEffect(this);
//    effTwo->setOpacity(0);
//    ui->dateTimeFrame->setGraphicsEffect(effTwo);

//    QGraphicsOpacityEffect *effThree = new QGraphicsOpacityEffect(this);
//    effThree->setOpacity(0);
//    ui->wxCurrentFrame->setGraphicsEffect(effThree);

//    QGraphicsOpacityEffect *effFour = new QGraphicsOpacityEffect(this);
//    effFour->setOpacity(0);
//    ui->trafficFrame->setGraphicsEffect(effFour);

    // Set the variables for motion detection
    this->motionDetected = false;
    this->vanityAsleep = true;

    // Setup the web view variables and start the browser as hidden
    //this->myPage = new QWebEnginePage();
    //ui->browserWidget->setPage(this->myPage);
    //ui->browserWidget->setVisible(false);
    //->browserWidget->
}

// Sets up all the threads for the display
void MainWindow::setupThreads()
{
    // Setup the Discovery and Launch Listerner
    this->dialThread.setupDIALThread(port);

    // Setup weather reciever
    this->wxRecieverThread.setupWeatherRetriever();

    // Setup traffic thread
    this->trafficThread.setupTrafficThread();

	// Instantiate clock timer
	QObject::connect(&clockThread, SIGNAL(sendTime(QString)), this->ui->timeLabel, SLOT(setText(QString)), Qt::QueuedConnection);
	QObject::connect(&clockThread, SIGNAL(sendDate(QString)), this->ui->dateLabel, SLOT(setText(QString)), Qt::QueuedConnection);
	QObject::connect(&clockThread, SIGNAL(sendSeconds(QString)), this->ui->timeSecondsLabel, SLOT(setText(QString)), Qt::QueuedConnection);
	QObject::connect(&clockThread, SIGNAL(sendAmPm(QString)), this->ui->timeAmPmLabel, SLOT(setText(QString)), Qt::QueuedConnection);
	
	// Instantiate Raspi timer
	QObject::connect(&raspiThread, SIGNAL(pinValueChanged(int, QString)), this, SLOT(pinValueChanged(int, QString)), Qt::QueuedConnection);
	
	// Instantiate Main States
	QObject::connect(&mainStatesThread, SIGNAL(checkToHideMainPanels()), this, SLOT(checkToHideMainPanels()), Qt::QueuedConnection);

    // Instantiate DIAL Thread
    QObject::connect(&dialThread, SIGNAL(launchYoutube(QString)), this, SLOT(launchYoutube(QString)), Qt::QueuedConnection);
    QObject::connect(&dialThread, SIGNAL(closeYoutube()), this, SLOT(closeYoutube()), Qt::QueuedConnection);

    // Instantiate Wx Reciever
    QObject::connect(&wxRecieverThread, SIGNAL(sendCurrentWeatherJson(QString)), this, SLOT(recieveCurrentWeatherJson(QString)), Qt::QueuedConnection);
	
    // Instantiate Traffic Thread
    QObject::connect(&trafficThread, SIGNAL(sendTrafficJson(QString)), this, SLOT(recieveTrafficJson(QString)), Qt::QueuedConnection);

	// Start the threads
	clockThread.start();
	raspiThread.start();
	mainStatesThread.start();
    dialThread.start();
    wxRecieverThread.start();
    trafficThread.start();
}

// This is ran when the RaspiThread detects a change on the GPIO Pins
void MainWindow::pinValueChanged(int pin, QString value)
{
	
	// Switch on the pin value changed
	switch (pin)
	{
	// Motion detector
	case 2:
		if (value == "HIGH") // Motion has been detected
		{
			this->motionDetected = true;
			this->showMainPanels();
		}
		else // No more motion detected
		{
			this->motionDetected = false;
		}
		break;
		
	}
}

// Return if motion is detected
void MainWindow::checkToHideMainPanels()
{
	// Logic to see if we show show or hide main panels
	if (!this->isMotionDetected())
		this->hideMainPanels();
	
}

// GETTERS

// Return if motion is detected
bool MainWindow::isMotionDetected()
{
	return this->motionDetected;
}

// Return if is asleep
bool MainWindow::isVanityAsleep()
{
	return this->vanityAsleep;
}

// SETTERS

// Return if motion is detected
void MainWindow::setMotionDetected(bool isMotionDetected)
{
	this->motionDetected = isMotionDetected;
}

// Return if is asleep
void MainWindow::setVanityAsleep(bool isVanityAsleep)
{
	this->vanityAsleep = isVanityAsleep;
}

// Show the main panels
void MainWindow::showMainPanels()
{
	// Show the main panels
	// Check if vanity was asleep first
	if (this->isVanityAsleep())
	{
		// Set vanity awake
		this->setVanityAsleep(false);
		
		// Show the notification
		this->showNotificationLabel("Hello Beautiful!");
		
        // Delay
		this->delay(this->fadeSeconds);
		
		// Now fade in the main panels
		this->fadeInPanels();		
		
		// Now hide the notification
		this->hideNotificationLabel();		
	}		
}

// Hide the main panels
void MainWindow::hideMainPanels()
{
	// Check if vanity was asleep first
	if (!this->isVanityAsleep())
	{
		// Show the notification		
		this->setVanityAsleep(true);
		
		this->showNotificationLabel("Goodbye...");
		
        // Delay
		this->delay(this->fadeSeconds);
		
		// Now fade in the main panels
		this->fadeOutPanels();
		
		// Now hide the notification
		this->hideNotificationLabel();	
		
	}
}

// Fade in the main panels
void MainWindow::fadeInPanels()
{
    // Setup the graphics effects for the wx frame
	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
	ui->wxCurrentFrame->setGraphicsEffect(eff);
	QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
	a->setDuration(this->fadeSeconds);
	a->setStartValue(0);
	a->setEndValue(1);
	a->setEasingCurve(QEasingCurve::Linear);
	
    // Setup the graphics effects for the date time frame
	QGraphicsOpacityEffect *effTwo = new QGraphicsOpacityEffect(this);
	ui->dateTimeFrame->setGraphicsEffect(effTwo);
	QPropertyAnimation *b = new QPropertyAnimation(effTwo, "opacity");
	b->setDuration(this->fadeSeconds);
	b->setStartValue(0);
	b->setEndValue(1);
	b->setEasingCurve(QEasingCurve::Linear);

    // Setup the graphics effects for the traffic frame
    QGraphicsOpacityEffect *effThree = new QGraphicsOpacityEffect(this);
    ui->trafficFrame->setGraphicsEffect(effThree);
    QPropertyAnimation *c = new QPropertyAnimation(effThree, "opacity");
    c->setDuration(this->fadeSeconds);
    c->setStartValue(0);
    c->setEndValue(1);
    c->setEasingCurve(QEasingCurve::Linear);
	
    // Start the animations
	b->start(QPropertyAnimation::DeleteWhenStopped);
	a->start(QPropertyAnimation::DeleteWhenStopped);
    c->start(QPropertyAnimation::DeleteWhenStopped);
}

// Fade out the main panels
void MainWindow::fadeOutPanels()
{
    // Setup the graphics effects for the wx frame
	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
	ui->wxCurrentFrame->setGraphicsEffect(eff);
	QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
	a->setDuration(this->fadeSeconds);
	a->setStartValue(1);
	a->setEndValue(0);
	a->setEasingCurve(QEasingCurve::Linear);
	
    // Setup the graphics effects for the date time frame
	QGraphicsOpacityEffect *effTwo = new QGraphicsOpacityEffect(this);
	ui->dateTimeFrame->setGraphicsEffect(effTwo);
	QPropertyAnimation *b = new QPropertyAnimation(effTwo, "opacity");
	b->setDuration(this->fadeSeconds);
	b->setStartValue(1);
	b->setEndValue(0);
	b->setEasingCurve(QEasingCurve::Linear);

    // Setup the graphics effects for the traffic frame
    QGraphicsOpacityEffect *effThree = new QGraphicsOpacityEffect(this);
    ui->trafficFrame->setGraphicsEffect(effThree);
    QPropertyAnimation *c = new QPropertyAnimation(effThree, "opacity");
    c->setDuration(this->fadeSeconds);
    c->setStartValue(1);
    c->setEndValue(0);
    c->setEasingCurve(QEasingCurve::Linear);
	
    // Start the animations
	b->start(QPropertyAnimation::DeleteWhenStopped);
	a->start(QPropertyAnimation::DeleteWhenStopped);
    c->start(QPropertyAnimation::DeleteWhenStopped);
}
	

// Show the notification label
void MainWindow::showNotificationLabel(QString notification)
{
	// Set the notification label
	ui->notificationLabel->setText(notification);
	
    // Setup the graphics effects
	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
	ui->notificationLabel->setGraphicsEffect(eff);
	QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
	a->setDuration(this->fadeSeconds);
	a->setStartValue(0);
	a->setEndValue(1);
	a->setEasingCurve(QEasingCurve::Linear);

    // Start the animation
	a->start(QPropertyAnimation::DeleteWhenStopped);
}

// Hidethe notification label
void MainWindow::hideNotificationLabel()
{
    // Setup the graphics effects
	QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
	ui->notificationLabel->setGraphicsEffect(eff);
	QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
	a->setDuration(this->fadeSeconds);
	a->setStartValue(1);
	a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::Linear);

    // Start the animation
	a->start(QPropertyAnimation::DeleteWhenStopped);
}

// Delay
void MainWindow::delay(int millisecondsToWait)
{
	QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
	while (QTime::currentTime() < dieTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

// DIAL Slots

// Launches the youtube browser
void MainWindow::launchYoutube(QString url)
{
    // Set the url to the privided url
    this->myPage->setUrl(QUrl::QUrl(url));

    ui->browserWidget->setVisible(true);

    // Create the graphics effects
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->browserWidget->setGraphicsEffect(eff);
    QPropertyAnimation *b = new QPropertyAnimation(eff, "opacity");
    b->setDuration(this->fadeSeconds);
    b->setStartValue(0);
    b->setEndValue(1);
    b->setEasingCurve(QEasingCurve::Linear);

    // Start the animations
    b->start(QPropertyAnimation::DeleteWhenStopped);
}

// Closes the you tube browser
void MainWindow::closeYoutube()
{
    // Create the graphics effects
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->browserWidget->setGraphicsEffect(eff);
    QPropertyAnimation *b = new QPropertyAnimation(eff, "opacity");
    b->setDuration(this->fadeSeconds);
    b->setStartValue(1);
    b->setEndValue(0);
    b->setEasingCurve(QEasingCurve::Linear);

    // Start the animations
    b->start(QPropertyAnimation::DeleteWhenStopped);

    QObject::connect(b, SIGNAL(finished()), this, SLOT(hideYoutubeBrowser()), Qt::DirectConnection);
}

// Show hide the youtube browser after animation done
void MainWindow::hideYoutubeBrowser()
{
    ui->browserWidget->setVisible(true);
}

// Handle recieve the current weather json
void MainWindow::recieveCurrentWeatherJson(QString currentWx)
{
    QJsonDocument jsonStr = QJsonDocument::fromJson(currentWx.toUtf8());
    QJsonObject currentWxObj = jsonStr.object();
    //qDebug() << currentWxObj;
    // Populate the weather fields
    QString tmpTempString = QString::number(currentWxObj.value("temp_f").toDouble());
    //qDebug() << currentWxObj.value("temp_f");
    ui->wxTempLabel->setText(tmpTempString + "°" + "F");
    ui->wxLabel->setText("Weather: " + currentWxObj.value("weather").toString());
    ui->windLabel->setText("Winds: " + currentWxObj.value("wind_string").toString());

}

// Handle recieve traffic json
void MainWindow::recieveTrafficJson(QString trafficJson)
{
    QJsonDocument jsonStr = QJsonDocument::fromJson(trafficJson.toUtf8());
    QJsonObject trafficJsonObj = jsonStr.object();

    ui->trafficArrivalTimeLabel->setText("Arrival Time: " + trafficJsonObj["arrival_time"].toString());
    ui->trafficDepartureTime->setText("Departure Time : " + trafficJsonObj["departure_time"].toString());
    ui->trafficDestinationLabel->setText("Destination: " + trafficJsonObj["destination"].toString());

}

