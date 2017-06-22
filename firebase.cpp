#include "firebase.h"
#include <string.h>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QCryptographicHash>
#include <QtDebug>
#include <QBuffer>

Firebase::Firebase(const QString &hostName
                   , const QString &dbPath
                   , QObject *parent)
   : QObject(parent)
{
    host = forceEndChar(hostName.trimmed(), '/');
    host = host.append(dbPath.trimmed());
    init();
}

void Firebase::init()
{
    manager=new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*))
            ,this,SLOT(replyFinished(QNetworkReply*)));
}

void Firebase::listenEvents(const QString& queryString)
{
    open(buildPath(queryString));
}

void Firebase::open(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setRawHeader("Accept", "text/event-stream");
    QNetworkReply *_reply = manager->get(request);
    connect(_reply, &QNetworkReply::readyRead, this, &Firebase::eventReadyRead);
    connect(_reply, &QNetworkReply::finished, this, &Firebase::eventFinished);
}

void Firebase::eventFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply)
    {
        QUrl redirectUrl = reply->attribute(
                    QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if (!redirectUrl.isEmpty())
        {
            reply->deleteLater();
            open(redirectUrl);
            return;
        }
        reply->deleteLater();
    }
}

void Firebase::eventReadyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if(reply)
    {
        QByteArray line=reply->readLine();
        if(!line.isEmpty())
        {
            QByteArray eventName=trimValue(line);
            line=reply->readAll();
            if(eventName=="put")
            {
                QString dataSnapshot(line);
                emit eventDataChanged(dataSnapshot);
            }
        }
    }
    reply->readAll();
}


void Firebase::setValue(QJsonDocument jsonDoc
                        , const QString &verb
                        , const QString& queryString)
{
    QString path = buildPath(queryString);
    QNetworkRequest request(path);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    QByteArray jsonBA = jsonDoc.toJson(QJsonDocument::Compact);

    QBuffer *buffer=new QBuffer();
    buffer->open((QBuffer::ReadWrite));
    buffer->write(jsonBA);
    buffer->seek(0);

    QByteArray verbBA = verb.toUtf8();
    manager->sendCustomRequest(request, verbBA ,buffer);
    buffer->close();
}

void Firebase::getValue(const QString& queryString)
{
    QNetworkRequest request(buildPath(queryString));
    manager->get(request);
}

void Firebase::replyFinished(QNetworkReply *reply)
{
    emit eventResponseReady(reply->readAll());
}

QString Firebase::getPath(const QString &queryString)
{
    return buildPath(queryString);
}

QString Firebase::buildPath(const QString &queryString)
{
    QString destination=host;

    const int dotJsonLength = 5;
    if (destination.length() <= dotJsonLength
            || destination.right(dotJsonLength) != ".json")
        destination.append(".json");

    if (queryString.length() > 0)
            destination.append(forceStartChar(queryString,'?'));

    return destination;
}

QByteArray Firebase::trimValue(const QByteArray &line) const
{
    QByteArray value;
    int index = line.indexOf(':');
    if (index > 0)
        value = line.right(line.size() - index  - 1);
    return value.trimmed();
}

QString Firebase::forceEndChar(const QString &string, char endCh)
{
    if (string[string.length()-1] != endCh)
        return QString(string).append(endCh);
    return string;
}

QString Firebase::forceStartChar(const QString &string, char startCh)
{
    if (string.length() > 0 && string[0] != startCh)
        return QString(string).prepend(startCh);
    return string;
}

