#ifndef FIREBASE_H
#define FIREBASE_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QtGlobal>
#include <QJsonDocument>

#include "firebase_global.h"

class Q_FIREBASEDB_EXPORT Firebase : public QObject
{
    Q_OBJECT
public:
    explicit Firebase(const QString& hostName = QStringLiteral(""), const QString& dbPath = QStringLiteral(""),
                      QObject *parent = nullptr);

    void setValue(QJsonDocument jsonDoc, const QString& verb = QStringLiteral("PATCH"),
                  const QString &queryString = QStringLiteral(""));
    void getValue(const QString& queryString = QStringLiteral(""));
    void listenEvents(const QString& queryString = QStringLiteral(""));

    QString getPath(const QString &queryString = QStringLiteral(""));



signals:
    void eventResponseReady(QByteArray replyData);
    void eventDataChanged(QString changedData);

private slots:
    void replyFinished(QNetworkReply*);
    void eventFinished();
    void eventReadyRead();

private:
    void init();

    QByteArray signMessage(QByteArray toSign, QByteArray privateKey);
    QString host;
    QString firebaseToken = QStringLiteral("");
    QNetworkAccessManager *manager;
    QString buildPath(const QString &queryString = QStringLiteral(""));
    void open(const QUrl &url);
    QString forceEndChar(const QString& string, char endCh);
    QString forceStartChar(const QString& string, char startCh);
    QByteArray trimValue(const QByteArray &line) const;
};

#endif // FIREBASE_H
