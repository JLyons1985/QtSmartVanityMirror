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
* Project File: WxForecastItem.h
* Project Description: Item for the wx forecast list view.
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#ifndef WXFORECASTITEM_H
#define WXFORECASTITEM_H

#include <QObject>

class WxForecastItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString day READ day WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(QString imgLoc READ imgLoc WRITE setImgLoc NOTIFY imgLocChanged)
    Q_PROPERTY(QString highTemp READ highTemp WRITE setHighTemp NOTIFY highTempChanged)
    Q_PROPERTY(QString lowTemp READ lowTemp WRITE setLowTemp NOTIFY lowTempChanged)
    Q_PROPERTY(double myopacity READ myopacity WRITE setOpacity NOTIFY opacityChanged)

public:
    WxForecastItem(QObject *parent=0);
    WxForecastItem(const QString &day, const QString &imgLoc, const QString &highTemp, const QString &lowTemp, QObject *parent=0);

    QString day() const;
    void setDay(const QString &day);

    QString imgLoc() const;
    void setImgLoc(const QString &imgLoc);

    QString highTemp() const;
    void setHighTemp(const QString &highTemp);

    QString lowTemp() const;
    void setLowTemp(const QString &lowTemp);

    double myopacity() const;
    void setOpacity(const double &opacity);

signals:
    void dayChanged();
    void imgLocChanged();
    void highTempChanged();
    void lowTempChanged();
    void opacityChanged();

private:
    QString m_day;
    QString m_imgLoc;
    QString m_highTemp;
    QString m_lowTemp;
    double m_opacity;

};

#endif // WXFORECASTITEM_H
