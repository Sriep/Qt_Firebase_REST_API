#ifndef FIREBASE_H
#define FIREBASE_H

#include <QObject>

#include "firebaseinterface.h"

class FirebasePrivate;
class Q_FIREBASEDB_EXPORT Firebase : public FirebaseInterface
{
    Q_OBJECT

public:
    explicit Firebase(const QString& hostName = QStringLiteral(""), const QString& dbPath = QStringLiteral(""),
                      QObject *parent = nullptr);

    void setValue(QJsonDocument jsonDoc, const QString& verb = QStringLiteral("PATCH"),
                  const QString &queryString = QStringLiteral("")) override;
    void getValue(const QString& queryString = QStringLiteral("")) override;
    void listenEvents(const QString& queryString = QStringLiteral("")) override;

    QString getPath(const QString &queryString = QStringLiteral("")) override;

private:
    Q_DECLARE_PRIVATE(Firebase)
};

#endif // FIREBASE_H
