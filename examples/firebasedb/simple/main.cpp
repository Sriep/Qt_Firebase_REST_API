#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <firebaseexamples.h>
#include <QString>

/*!
 * \brief main Text some examples of using Firebase REST API
 *
 * \param argc Number of arguments
 * \param argv 3 user arguments
 *  <ul>
 *      <li>Id of example to run</li>
 *      <li>Firebase Database URL</li>
 *      <li>Your Firebase Secret</li>
 * </ul>
 *
 * \return
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString strId = "1";
    if (argc >= 2)
        strId = argv[1];

    QString fbUrl;
    if (argc >= 3)
        fbUrl = argv[2];

    QString authToken;
    if (argc >= 4)
        authToken = argv[3];

    FirebaseExamples *examples = new FirebaseExamples(strId.toInt()
                                                    , fbUrl
                                                    , authToken);
    return a.exec();
}
