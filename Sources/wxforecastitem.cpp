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
* Project File: WxForecastItem.cpp
* Project Description: Item for the wx forecast list view.
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include <Headers/wxforecastitem.h>

WxForecastItem::WxForecastItem(QObject *parent)
    : QObject(parent)
{
}

WxForecastItem::WxForecastItem(const QString &day, const QString &imgLoc, const QString &highTemp, const QString &lowTemp, QObject *parent)
    : QObject(parent), m_day(day), m_imgLoc(imgLoc), m_highTemp(highTemp), m_lowTemp(lowTemp)
{
}

QString WxForecastItem::day() const
{
    return m_day;
}

void WxForecastItem::setDay(const QString &day)
{
    if (day != m_day) {
        m_day = day;
        emit dayChanged();
    }
}

QString WxForecastItem::imgLoc() const
{
    return m_imgLoc;
}

void WxForecastItem::setImgLoc(const QString &imgLoc)
{
    if (imgLoc != m_imgLoc) {
        m_imgLoc = imgLoc;
        emit imgLocChanged();
    }
}

QString WxForecastItem::highTemp() const
{
    return m_highTemp;
}

void WxForecastItem::setHighTemp(const QString &highTemp)
{
    if (highTemp != m_highTemp) {
        m_highTemp = highTemp;
        emit highTempChanged();
    }
}

QString WxForecastItem::lowTemp() const
{
    return m_lowTemp;
}

void WxForecastItem::setLowTemp(const QString &lowTemp)
{
    if (lowTemp != m_lowTemp) {
        m_lowTemp = lowTemp;
        emit lowTempChanged();
    }
}

double WxForecastItem::myopacity() const
{
    return m_opacity;
}

void WxForecastItem::setOpacity(const double &opacity)
{
    if (opacity != m_opacity) {
        m_opacity = opacity;
        emit opacityChanged();
    }
}
