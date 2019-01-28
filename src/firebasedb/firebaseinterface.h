#ifndef FIREBASEINTERFACE_H
#define FIREBASEINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>

#include "firebase_global.h"

class Q_FIREBASEDB_EXPORT FirebaseInterface : public QObject
{
    Q_OBJECT

public:
    FirebaseInterface(QObject *parent = nullptr) : QObject(parent) {}

    virtual void setValue(QJsonDocument jsonDoc, const QString& verb = QStringLiteral("PATCH"),
                  const QString &queryString = QStringLiteral("")) = 0;
    virtual void getValue(const QString& queryString = QStringLiteral("")) = 0;
    virtual void listenEvents(const QString& queryString = QStringLiteral("")) = 0;

    virtual QString getPath(const QString &queryString = QStringLiteral("")) = 0;

signals:
    void eventResponseReady(QByteArray replyData);
    void eventDataChanged(QString changedData);
};

#endif // FIREBASEINTERFACE_H
