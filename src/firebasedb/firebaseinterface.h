#ifndef FIREBASEINTERFACE_H
#define FIREBASEINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QJsonDocument>

#include "firebase_global.h"

/**
 * TODO:
 * ** add "subpath" parameter
 * ** add overload for setValue with QByteArray() instead of QJsonDocument()
 */

class Q_FIREBASEDB_EXPORT FirebaseInterface : public QObject
{
    Q_OBJECT

public:
    enum HttpVerb {
        PUT,
        PATCH,
        POST,
        DELETE
    };
    Q_ENUM(HttpVerb)

    FirebaseInterface(QObject *parent = nullptr) : QObject(parent) {}

    virtual void setValue(QJsonDocument jsonDoc, HttpVerb verb = PATCH,
                  const QString &queryString = QStringLiteral("")) = 0;
    virtual void getValue(const QString& queryString = QStringLiteral("")) = 0;
    virtual void listenEvents(const QString& queryString = QStringLiteral("")) = 0;

    virtual QString getPath(const QString &queryString = QStringLiteral("")) = 0;

signals:
    void eventResponseReady(QByteArray replyData);
    void eventDataChanged(QString changedData);
};

#endif // FIREBASEINTERFACE_H
