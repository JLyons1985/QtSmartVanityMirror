#include <QDebug>
#include "DataObject.h"

DataObject::DataObject(QObject *parent)
    : QObject(parent)
{
}

DataObject::DataObject(const QString &name, const QString &start, const QString &location, const QString &end, QObject *parent)
    : QObject(parent), m_name(name), m_start(start), m_location(location), m_end(end)
{
}

QString DataObject::name() const
{
    return m_name;
}

void DataObject::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        emit nameChanged();
    }
}

QString DataObject::start() const
{
    return m_start;
}

void DataObject::setStart(const QString &start)
{
    if (start != m_start) {
        m_start = start;
        emit startChanged();
    }
}

QString DataObject::location() const
{
    return m_location;
}

void DataObject::setLocation(const QString &location)
{
    if (location != m_location) {
        m_location = location;
        emit locationChanged();
    }
}

QString DataObject::end() const
{
    return m_end;
}

void DataObject::setEnd(const QString &end)
{
    if (end != m_end) {
        m_end = end;
        emit endChanged();
    }
}
