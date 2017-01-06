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
* Project File: CalendarItem.h
* Project Description: Item for the calendar list
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include <QObject>

class CalendarItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString end READ end WRITE setEnd NOTIFY endChanged)
    Q_PROPERTY(QString date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(double myopacity READ myopacity WRITE setOpacity NOTIFY opacityChanged)

public:
    CalendarItem(QObject *parent=0);
    CalendarItem(const QString &name, const QString &start, const QString &location, const QString &end, QObject *parent=0);

    QString name() const;
    void setName(const QString &name);

    QString start() const;
    void setStart(const QString &start);

    QString location() const;
    void setLocation(const QString &location);

    QString end() const;
    void setEnd(const QString &end);

    QString date() const;
    void setDate(const QString &date);

    double myopacity() const;
    void setOpacity(const double &opacity);

signals:
    void nameChanged();
    void startChanged();
    void locationChanged();
    void endChanged();
    void dateChanged();
    void opacityChanged();

private:
    QString m_name;
    QString m_start;
    QString m_location;
    QString m_end;
    QString m_date;
    double m_opacity;

};


#endif // CALENDARITEM_H
