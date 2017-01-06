import QtQuick 2.7
import QtQuick.Window 2.2
import QtWebEngine 1.1

Rectangle {

    Item {
        id: vars
        property int aTime: 3000
    }

    width: 1080
    height: 1920
    color: "#000000"

    FontLoader { id: helveticaNeue; source: "qrc:/font/HelveticaNeue.ttf" }

    Connections {
        target: mainFormHandler

        onShowCalendar: {
            showCalendarAnim.start()
        }

        onHideCalendar: {
            hideCalendarAnim.start()
        }

        onShowPanels: {
            showPanelsAnim.start()
        }

        onHidePanels: {
            hidePanelsAnim.start()
        }

        onShowNotificationLabel: {
            showNotificationLabelAnim.start()
        }

        onHideNotificationLabel: {
            hideNotificationLabelAnim.start()
        }

        onShowBrowser: {
            showBrowserAnim.start()
        }

        onHideBrowser: {
            hideBrowserAnim.start()
        }

        onWakeUpMirror: {
            showNotificationLabelAnim.start()
            wakeMirrorDelay.start()
        }

        onSleepMirror: {
            showNotificationLabelAnim.start()
            sleepMirrorDelay.start()
        }

        onShowThenHideNotificationLabel: {
            notificationLabelSeqAnim.start()
        }

        onShowForecast : {
            showForecastAnim.start()
        }

        onHideForecast: {
            hideForcastAnim.start()
        }

    }

    Timer {
        id: wakeMirrorDelay
        repeat: false
        interval: vars.aTime
        onTriggered: {
            showPanelsAnim.start()
            hideNotificationLabelAnim.start()
        }
    }

    Timer {
        id: sleepMirrorDelay
        repeat: false
        interval: vars.aTime
        onTriggered: {
            hidePanelsAnim.start()
            hideNotificationLabelAnim.start()
        }
    }

    NumberAnimation {
        target: wxForecastFrame
        id: showForecastAnim
        property: "opacity"
        duration: vars.aTime
        easing.type: Easing.Linear
        from: 0
        to: 1
    }

    NumberAnimation {
        target: wxForecastFrame
        id: hideForcastAnim
        property: "opacity"
        duration: vars.aTime
        easing.type: Easing.Linear
        from: 1
        to: 0
    }

    NumberAnimation {
        target: calendarFrame
        id: showCalendarAnim
        property: "opacity"
        duration: vars.aTime
        easing.type: Easing.Linear
        from: 0
        to: 1
    }

    NumberAnimation {
        target: calendarFrame
        id: hideCalendarAnim
        property: "opacity"
        duration: vars.aTime
        easing.type: Easing.Linear
        from: 1
        to: 0
    }

    NumberAnimation {
        target: browserContainer
        id: showBrowserAnim
        property: "opacity"
        duration: vars.aTime
        easing.type: Easing.Linear
        from: 0
        to: 1
    }

    NumberAnimation {
        target: browserContainer
        id: hideBrowserAnim
        property: "opacity"
        duration: vars.aTime
        easing.type: Easing.Linear
        from: 1
        to: 0
    }

    NumberAnimation {
        targets: [dateTimeFrame, trafficFrame, wxCurrentFrame, quoteFrame]
        id: showPanelsAnim
        properties: "opacity"
        easing.type: Easing.Linear
        from: 0
        to: 1
        duration: vars.aTime
    }

    NumberAnimation {
        targets: [dateTimeFrame, trafficFrame, wxCurrentFrame, calendarFrame, wxForecastFrame, quoteFrame]
        id: hidePanelsAnim
        properties: "opacity"
        easing.type: Easing.Linear
        from: 1
        to: 0
        duration: vars.aTime
    }

    NumberAnimation {
        target: notificationLabel
        id: showNotificationLabelAnim
        properties: "opacity"
        easing.type: Easing.Linear
        from: 0
        to: 1
        duration: vars.aTime
    }

    NumberAnimation {
        target: notificationLabel
        id: hideNotificationLabelAnim
        properties: "opacity"
        easing.type: Easing.Linear
        from: 1
        to: 0
        duration: vars.aTime
    }

    SequentialAnimation {
        id: notificationLabelSeqAnim

        NumberAnimation {
            target: notificationLabel
            properties: "opacity"
            easing.type: Easing.Linear
            from: 0
            to: 1
            duration: vars.aTime
        }


        PauseAnimation {
            duration: 200
        }

        NumberAnimation {
            target: notificationLabel
            properties: "opacity"
            easing.type: Easing.Linear
            from: 1
            to: 0
            duration: vars.aTime
        }

    }

    Item {
        id: browserContainer
        x: 220
        y: 684
        width: 640
        height: 360
        visible: true
        opacity: 0
        WebEngineView {
            anchors.fill: parent
            url: mainFormHandler.browserURL
        }
    }

    Item {
        id: dateTimeFrame
        x: 550
        y: 20
        width: 521
        height: 171
        opacity: 0

        Text {
            id: dateLabel
            x: 40
            y: 0
            width: 481
            height: 41
            color: "#ffffff"
            text: mainFormHandler.dateText
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 30
            font.family: helveticaNeue.name
        }

        Text {
            id: timeLabel
            x: 270
            y: 50
            width: 201
            height: 61
            color: "#ffffff"
            text: mainFormHandler.timeText
            font.pixelSize: 50
            font.family: helveticaNeue.name
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: timeSecondsLabel
            x: 480
            y: 50
            width: 41
            height: 31
            color: "#ffffff"
            text: mainFormHandler.secondsText
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
            font.family: helveticaNeue.name
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: timeAmPmLabel
            x: 480
            y: 80
            width: 41
            height: 31
            color: "#ffffff"
            text: mainFormHandler.amPmText
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
            font.family: helveticaNeue.name
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Item {
        id: wxCurrentFrame
        x: 20
        y: 20
        width: 521
        height: 145
        opacity: 0

        Text {
            id: wxTempLabel
            x: 0
            y: 0
            width: 140
            height: 61
            color: "#ffffff"
            text: mainFormHandler.currentWxTempText
            font { family: helveticaNeue.name; pixelSize: 48 }
        }

        Text {
            id: wxLabel
            x: 0
            y: 60
            width: 481
            height: 41
            color: "#ffffff"
            text: mainFormHandler.currentWxText
            font.pixelSize: 22
            font.family: helveticaNeue.name
        }

        Text {
            id: windLabel
            x: 0
            y: 100
            width: 521
            height: 41
            color: "#ffffff"
            text: mainFormHandler.currentWindText
            font.pixelSize: 22
            font.family: helveticaNeue.name
        }

        Image {
            id: currentWxImage
            x: 151
            y: 3
            width: 80
            height: 55
            source: mainFormHandler.currentWxImgLoc
        }
    }

    Item {
        id: trafficFrame
        x: 20
        y: 1682
        width: 287
        height: 221
        opacity: 0

        Text {
            id: trafficArrivalTimeLabel
            x: 0
            y: 180
            width: 431
            height: 41
            color: "#ffffff"
            text: mainFormHandler.trafficArrivalTimeText
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.family: helveticaNeue.name
            horizontalAlignment: Text.AlignLeft
        }

        Text {
            id: trafficDepartureTimeLabel
            x: 0
            y: 140
            width: 431
            height: 41
            color: "#ffffff"
            text: mainFormHandler.trafficDepartureTimeText
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.family: helveticaNeue.name
            horizontalAlignment: Text.AlignLeft
        }

        Text {
            id: trafficDestinationLabel
            x: 0
            y: 100
            width: 431
            height: 41
            color: "#ffffff"
            text: mainFormHandler.trafficDestinationText
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.family: helveticaNeue.name
            horizontalAlignment: Text.AlignLeft
        }
    }

    Text {
        id: notificationLabel
        x: 0
        y: 905
        width: 1080
        height: 133
        color: "#ffffff"
        text: mainFormHandler.notificationText
        wrapMode: Text.WordWrap
        opacity: 0
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 48
        font.family: helveticaNeue.name
    }

    Rectangle {
        id: calendarFrame
        x: 781
        y: 171
        width: 290
        height: 420
        color: "#000000"
        radius: 1
        opacity: 0
        border.color: "#000000"

        ListView {
            id: calendarListView
            x: 8
            y: 43
            width: 274
            height: 400
            model: calendarModel
            delegate: Rectangle {
                id: rectangle1
                x: 0
                y: 0
                width: 274
                height: 50
                color: "#000000"

                Text {
                    x: 0
                    y: 0
                    width: 274
                    height: 22
                    color: "#ffffff"
                    opacity: myopacity
                    text: name
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18
                    font.family: helveticaNeue.name
                }

                Text {
                    x: 0
                    y: 22
                    width: 73
                    height: 20
                    color: "#ffffff"
                    opacity: myopacity
                    text: date
                    font.pixelSize: 16
                    font.family: helveticaNeue.name
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    x: 121
                    y: 22
                    width: 53
                    height: 20
                    color: "#ffffff"
                    opacity: myopacity
                    text: start
                    font.pixelSize: 16
                    font.family: helveticaNeue.name
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    x: 213
                    y: 22
                    width: 53
                    height: 20
                    color: "#ffffff"
                    opacity: myopacity
                    text: end
                    font.pixelSize: 16
                    font.family: helveticaNeue.name
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    x: 175
                    y: 22
                    width: 38
                    height: 20
                    color: "#ffffff"
                    opacity: myopacity
                    text: qsTr("To")
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                    font.family: helveticaNeue.name
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                x: 83
                y: 22
                width: 38
                height: 20
                color: "#ffffff"
                opacity: myopacity
                text: qsTr("From")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 16
                font.family: helveticaNeue.name
                verticalAlignment: Text.AlignVCenter
                }
        }
    }

    Text {
        id: calendarLabel
        x: 8
        y: 8
        width: 274
        height: 29
        color: "#ffffff"
        text: qsTr("Calendar")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        font.family: helveticaNeue.name
    }
}

    Item {
    id: wxForecastFrame
    x: 20
    y: 171
    width: 425
    height: 420
    opacity: 0

        Text {
        id: dayLabel
        x: 8
        y: 8
        width: 120
        height: 29
        color: "#ffffff"
        text: qsTr("Day")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        font.family: helveticaNeue.name
    }

        Text {
        id: lowLabel
        x: 327
        y: 8
        width: 74
        height: 29
        color: "#ffffff"
        text: qsTr("Low")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        font.family: helveticaNeue.name
    }

        Text {
        id: highLabel
        x: 240
        y: 8
        width: 74
        height: 29
        color: "#ffffff"
        text: qsTr("High")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        font.family: helveticaNeue.name
    }

        Text {
        id: skyImage
        x: 138
        y: 8
        width: 80
        height: 29
        color: "#ffffff"
        text: qsTr("Sky")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        font.family: helveticaNeue.name
    }

        ListView {
        id: wxForecastListView
        x: 8
        y: 43
        width: 409
        height: 400
        model: wxForecastModel
        delegate: Rectangle {
            x: 0
            y: 0
            width: 393
            height: 50
            color: "#000000"

            Text {
                x: 0
                y: 0
                width: 120
                height: 50
                color: "#ffffff"
                opacity: myopacity
                text: day
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 18
                font.family: helveticaNeue.name
            }

            Image {
                x: 130
                y: 0
                width: 80
                height: 50
                source: imgLoc
                opacity: myopacity
            }

            Text {
                x: 232
                y: 0
                width: 74
                height: 50
                color: "#ffffff"
                opacity: myopacity
                text: highTemp
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 18
                font.family: helveticaNeue.name
            }

            Text {
                x: 319
                y: 0
                width: 74
                height: 50
                opacity: myopacity
                color: "#ffffff"
                text: lowTemp
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 18
                font.family: helveticaNeue.name
            }
        }
        }

    }

    Item {
        id: quoteFrame
        x: 313
        y: 1682
        width: 759
        height: 230
        opacity: 0

        Text {
            id: quote
            x: 8
            y: 8
            width: 743
            height: 177
            color: "#ffffff"
            text: mainFormHandler.quoteText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 22
            font.family: helveticaNeue.name
        }

        Text {
            id: author
            x: 8
            y: 191
            width: 743
            height: 27
            color: "#ffffff"
            text: mainFormHandler.authorText
            style: Text.Normal
            font.italic: true
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 18
            verticalAlignment: Text.AlignVCenter
            font.family: helveticaNeue.name
        }
    }


}


