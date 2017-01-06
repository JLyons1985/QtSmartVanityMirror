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
* Project File: main.ccp
* Project Description: Main entry into the Smart Mirror Program
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <Headers/MainFormHandler.h>
#include <Headers/calendaritem.h>
#include <QtQuick/QQuickView>
#include <QtWebEngine>

// Main entry
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Register DataObject MetaType
    //qRegisterMetaType<QList<DataObject*>>("DataObject");

    // Initialize the web engine
    QtWebEngine::initialize();

    QList<QObject*> dataList;
        dataList.append(new CalendarItem("","","",""));

    // Create the qml engine
    QQuickView view;
    QQmlContext *context = view.rootContext();

    // Create the main form handler
    QScopedPointer<MainFormHandler> mainFormHandler(new MainFormHandler(context));

    qDebug() << "Main Form created trying to show ui.";

    // Connect the handler to the QML file
    context->setContextProperty("mainFormHandler", mainFormHandler.data());
    context->setContextProperty("calendarModel", QVariant::fromValue(dataList));

    // Load the qml file for the ui
    view.setSource(QUrl(QStringLiteral("qrc:/qml/MainForm.qml")));

    view.show();

    // Start the app
    return app.exec();
}
