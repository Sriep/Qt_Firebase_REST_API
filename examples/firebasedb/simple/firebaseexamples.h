#ifndef FIREBASEEXAMPLES_H
#define FIREBASEEXAMPLES_H
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QString>

/*!
 * \brief The ActionHandler Examples of using Firebase.
 *
 * You will probably want to edit this class and run under debug
 * to see what happens.
 *
 * \see Firebase
 */
class FirebaseExamples:public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief ActionHandler
     * \param action Id indicating which example you want to run
     * \param firebaseURL URL of your Firebase Database
     * \param authToken Your Secret.
     */
    FirebaseExamples(int action
                  , const QString& firebaseURL = ""
                  , const QString& authToken = "");

    /*!
     * \brief patch Example of updating your database with PATCH request.
     */
    void patch();
    /*!
     * \brief get Example of retrieving part of your database with GET request.
     */
    void get();
    /*!
     * \brief useToken Example of updating your rules.
     *
     * Based on example in
     * <a href="https://firebase.google.com/docs/database/rest/app-management">
     * Firebase REST API Documentation </a>.
     */
    void useToken();
    /*!
     * \brief listen Example of listing for database updates.
     */
    void listen();

public slots:
    /*!
     * \brief onResponseReady Connect to Firebase replayFinished signal.
     *
     * \see Firebase::eventResponseReady(QByteArray replyData)
     */
    void onResponseReady(QByteArray);

    /*!
     * \brief onDataChanged Connect to Firebase eventDataChanged signal
     *
     * \see Firebase::eventDataChanged(QString changedData)
     */
    void onDataChanged(QString);

private:
    QString fbUrl;
    QString authToken;
};

#endif // FIREBASEEXAMPLES_H
