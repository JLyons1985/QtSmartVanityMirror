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
* Project File: CalendarModel.cpp
* Project Description: Code file for the colendar model. Used to populate
*  QML listview.
* @author Joshua Lyons (josh@lyonsdensoftware.com)
* @version 0.0.1
**************************************************************************/

#include "Headers/CalendarModel.h"

CalendarObject::CalendarObject(const QString &type, const QString &size)
    : m_type(type), m_size(size)
{
}

QString CalendarObject::type() const
{
    return m_type;
}

QString CalendarObject::size() const
{
    return m_size;
}

CalendarModel::CalendarModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CalendarModel::addCalendarObject(const CalendarObject &calendarObject)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_calendarObjects << calendarObject;
    endInsertRows();
}

int CalendarModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_calendarObjects.count();
}

QVariant CalendarModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() >= m_calendarObjects.count())
            return QVariant();

        const CalendarObject &calendarObject = m_calendarObjects[index.row()];
        if (role == TypeRole)
            return calendarObject.type();
        else if (role == SizeRole)
            return calendarObject.size();
        return QVariant();
}

QHash<int, QByteArray> CalendarModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    return roles;
}
