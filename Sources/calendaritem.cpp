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
* Project File: CalendarItem.cpp
* Project Description: Item for the calendar list
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <Headers/calendaritem.h>

CalendarItem::CalendarItem(QObject *parent)
    : QObject(parent)
{
}

CalendarItem::CalendarItem(const QString &name, const QString &start, const QString &location, const QString &end, QObject *parent)
    : QObject(parent), m_name(name), m_start(start), m_location(location), m_end(end)
{
}

QString CalendarItem::name() const
{
    return m_name;
}

void CalendarItem::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        emit nameChanged();
    }
}

QString CalendarItem::start() const
{
    return m_start;
}

void CalendarItem::setStart(const QString &start)
{
    if (start != m_start) {
        m_start = start;
        emit startChanged();
    }
}

QString CalendarItem::location() const
{
    return m_location;
}

void CalendarItem::setLocation(const QString &location)
{
    if (location != m_location) {
        m_location = location;
        emit locationChanged();
    }
}

QString CalendarItem::end() const
{
    return m_end;
}

void CalendarItem::setEnd(const QString &end)
{
    if (end != m_end) {
        m_end = end;
        emit endChanged();
    }
}

QString CalendarItem::date() const
{
    return m_date;
}

void CalendarItem::setDate(const QString &date)
{
    if (date != m_date) {
        m_date = date;
        emit dateChanged();
    }
}

double CalendarItem::myopacity() const
{
    return m_opacity;
}

void CalendarItem::setOpacity(const double &opacity)
{
    if (opacity != m_opacity) {
        m_opacity = opacity;
        emit opacityChanged();
    }
}
