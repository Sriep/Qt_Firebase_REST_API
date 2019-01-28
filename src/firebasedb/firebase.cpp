#include "firebase.h"
#include <string.h>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QCryptographicHash>
#include <QtDebug>
#include <QBuffer>

/*!
 * \class Firebase
 * \inmodule QtFirebaseDb
 * \brief The Firebase class provides access to the Firebase Database REST API
 *
 * Enter the URL of your database endpoint in the constructor. The Firebase
 * object will now interact with that endpoint. You can send read, write and
 * listen requests. Results are accessed by handling the eventResponseReady
 * and eventDataChanged signals.
 *
 * For further infomation check out
 * <a href="http://piersshepperson.co.uk/programming/2017/06/26/firebase-database-rest-api-qt/">
 * this blog post</a>.
 *
 */

/*!
 * \brief Firebase constructor, sets the REST endpoint.
 *
 * Creates a Firebase object that knows your Firebase Database URL, and
 * the path to the part of your database you wish to interact with.
 * The REST endpoint structure is explained int the
 * <a href="https://firebase.google.com/docs/reference/rest/database/">
 * Firebase Database REST API </a>.
 *
 * \param hostName Firebase database URL. This will look something like
 * [PROJECT_ID].firebaseio-demo.com/. A '/' will be added to the end if one
 * is not already present. Your Firebase Database URL can be found on your
 * Firebase console at the top of the 'Database rh-tab/Data top-tab'. This
 * can be empty or null if operating on the whole database.
 *
 * \param dbPath Path in the database to the location of interest. If
 * necessary ".json" will be added at the end.
 *
 * \param parent Assign parent QObject if required.
 */
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

/*!
 * \brief listenEvents Use to stream changes from the REST endpoint.
 *
 * Download changes made to the indicated location in our Firebase database.
 * Details at <a href="https://firebase.google.com/docs/database/rest/retrieve-data">
 * Streaming from the REST API</a>. Handle the Firebase::eventDataChanged
 * signal to be informed at the moment changes occur.
 *
 * /see  Firebase::eventDataChanged signal.
 *
 * \param queryString Query string, a preceding query is added if necessary.
 * Query choices include: access_token, startAt, print, endAt, orderBy.
 */
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
            line = reply->readAll();
            if(eventName == "put")
            {
                QString dataSnapshot = QString::fromLatin1(line);
                emit eventDataChanged(dataSnapshot);
            }
        }
    }
    reply->readAll();
}


/*!
 * \brief setValue Sends a write request to your Firebase database.
 *
 * Assumes the REST endpoint has already been defined in the constructor.
 * setValue's parameters make up the rest of the request. The format of the
 * requests available at
 * <a href="https://firebase.google.com/docs/reference/rest/database/">
 * Firebase Database REST API </a>.
 *
 * If you want to see the response, handle the Firebase::EventResponseReady
 * signal.
 *
 * \see Firebase::EventResponseReady signal.
 *
 * \param jsonDoc Data in the format of a QJsonDocuemnt.
 *
 * \param verb Action to perform. Choices include:: PUT, POST, PATCH and DELETE.
 *
 * \param queryString Query string, a preceding query is added if necessary.
 * Query choices include: access_token, shallow, print, callback, format
 * and download.
 */
void Firebase::setValue(QJsonDocument jsonDoc
                        , const QString &verb
                        , const QString& queryString)
{
    QString path = buildPath(queryString);
    QNetworkRequest request(path);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("application/x-www-form-urlencoded"));
    QByteArray jsonBA = jsonDoc.toJson(QJsonDocument::Compact);

    QBuffer *buffer=new QBuffer();
    buffer->open((QBuffer::ReadWrite));
    buffer->write(jsonBA);
    buffer->seek(0);

    QByteArray verbBA = verb.toUtf8();
    manager->sendCustomRequest(request, verbBA ,buffer);
    buffer->close();
}

/*!
 * \brief getValue Sends a read or "GET" request to your Firebase database.
 *
 * Assumes the REST endpoint has already been defined in the constructor.
 * setValue's parameters make up the rest of the request. The format of the
 * requests available at
 * <a href="https://firebase.google.com/docs/reference/rest/database/">
 * Firebase Database REST API </a>.
 * To see the response, handle the handle the Firebase::EventResponseReady
 * signal; the returned data will be in the QByteArray parameter.
 *
 * \see Firebase::EventResponseReady signal.
 *
 * \param queryString Query string, a preceding query is added if necessary.
 * Query choices include: access_token, shallow, print, callback, format
 * and download.
 */
void Firebase::getValue(const QString& queryString)
{
    QNetworkRequest request(buildPath(queryString));
    manager->get(request);
}

void Firebase::replyFinished(QNetworkReply *reply)
{
    emit eventResponseReady(reply->readAll());
}

/*!
 * \brief getPath Returns the URL that a request would use.
 *
 * This is an information only function. It returns the request that would
 * be sent if the given query. Useful for debugging.
 *
 * \param queryString Query string.
 *
 * \return The request that would be sent if the given query.
 */
QString Firebase::getPath(const QString &queryString)
{
    return buildPath(queryString);
}

QString Firebase::buildPath(const QString &queryString)
{
    QString destination=host;

    const int dotJsonLength = 5;
    if (destination.length() <= dotJsonLength
            || destination.right(dotJsonLength) != QStringLiteral(".json"))
        destination.append(QStringLiteral(".json"));

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

/*!
 * \fn Firebase::eventResponseReady(QByteArray replyData)
 * \brief eventResponseReady Sent after a
 * <a href="http://doc.qt.io/qt-5/qnetworkaccessmanager.html#finished">
 * QNetworkAccessManager::finished()</a> signal is revived.
 *
 * Handle this * signal if you want to inspect the reply.
 *
 * \param replyData holds the reply's data. Result of calling reply->readAll() where
 * reply is the parameter passed to QNetworkAccessManager::finished().
 */

/*!
 * \fn Firebase::eventDataChanged(QString changedData)
 * \brief eventDataChanged Sent after a
 * <a href="http://doc.qt.io/qt-5/qiodevice.html#readyRead">
 * QNetworkReply::readyRead</a> is received. It checks
 * for a "put" event with non empty data before forwarding the signal.
 *
 * \param changedData holds the update detected to the REST endpoint.
 * The first signal received is the initial contents of the endpoint.
 */

