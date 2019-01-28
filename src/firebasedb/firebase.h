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

/*!
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
class Q_FIREBASEDB_EXPORT Firebase : public QObject
{
    Q_OBJECT
public:
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
    explicit Firebase(const QString& hostName = ""
                      , const QString& dbPath = ""
                      , QObject *parent = 0);

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
    void setValue(QJsonDocument jsonDoc
                  , const QString& verb = "PATCH"
                  , const QString &queryString = "");

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
    void getValue(const QString& queryString = "");

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
    void listenEvents(const QString& queryString = "");

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
    QString getPath(const QString &queryString="");



signals:
    /*!
     * \brief eventResponseReady Sent after a
     * <a href="http://doc.qt.io/qt-5/qnetworkaccessmanager.html#finished">
     * QNetworkAccessManager::finished()</a> signal is revived.
     *
     * Handle this * signal if you want to inspect the reply.
     *
     * \param replyData holds the reply's data. Result of calling reply->readAll() where
     * reply is the parameter passed to QNetworkAccessManager::finished().
     */
    void eventResponseReady(QByteArray replyData);

    /*!
     * \brief eventDataChanged Sent after a
     * <a href="http://doc.qt.io/qt-5/qiodevice.html#readyRead">
     * QNetworkReply::readyRead</a> is received. It checks
     * for a "put" event with non empty data before forwarding the signal.
     *
     * \param changedData holds the update detected to the REST endpoint.
     * The first signal received is the initial contents of the endpoint.
     */
    void eventDataChanged(QString changedData);

private slots:
    void replyFinished(QNetworkReply*);
    void eventFinished();
    void eventReadyRead();
private:
    void init();    


    QByteArray signMessage(QByteArray toSign, QByteArray privateKey);
    QString host;
    QString firebaseToken="";
    QNetworkAccessManager *manager;
    QString buildPath(const QString &queryString = "");
    void open(const QUrl &url);
    QString forceEndChar(const QString& string, char endCh);
    QString forceStartChar(const QString& string, char startCh);
    QByteArray trimValue(const QByteArray &line) const;
};

#endif // FIREBASE_H
