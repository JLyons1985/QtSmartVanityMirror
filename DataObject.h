#ifndef DATAOBJECT_H
#define DATAOBJECT_H


#include <QObject>




class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY endChanged)
    Q_PROPERTY(QString end READ end WRITE setEnd NOTIFY endChanged)

public:
    DataObject(QObject *parent=0);
    DataObject(const QString &name, const QString &start, const QString &location, const QString &end, QObject *parent=0);

    QString name() const;
    void setName(const QString &name);

    QString start() const;
    void setStart(const QString &start);

    QString location() const;
    void setLocation(const QString &location);

    QString end() const;
    void setEnd(const QString &end);

signals:
    void nameChanged();
    void startChanged();
    void locationChanged();
    void endChanged();

private:
    QString m_name;
    QString m_start;
    QString m_location;
    QString m_end;

};



#endif // DATAOBJECT_H
