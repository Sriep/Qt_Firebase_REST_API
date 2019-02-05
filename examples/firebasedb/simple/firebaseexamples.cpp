#include "firebaseexamples.h"

#include <QUrl>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include <QtFirebaseDb/firebase.h>

FirebaseExamples::FirebaseExamples(int action
                             , const QString &firebaseURL
                             , const QString &authToken)
    : fbUrl(firebaseURL), authToken(authToken)
{
    switch (action)
    {
    case 1:
        patch();
        break;
    case 2:
        get();
        break;
    case 3:
        useToken();
        break;
    case 4:
        listen();
        break;
    };

}

void FirebaseExamples::patch()
{
    QJsonObject jsonObj;
    jsonObj["user"] = "hii";
    jsonObj["password"] = "password";
    jsonObj["email"] = "email@hi.co.uk";
    QJsonDocument uploadDoc(jsonObj);
    QString path = "lll/users/fred/";
    Firebase *firebaseSet = new Firebase(fbUrl, path);
    firebaseSet->setValue(uploadDoc, Firebase::PATCH);

    connect(firebaseSet, &Firebase::eventResponseReady,
            this, &FirebaseExamples::onResponseReady);
}


void FirebaseExamples::get()
{
    Firebase *firebaseGet = new Firebase(fbUrl, "lll/users/.json");
    qDebug() << "URL:" << firebaseGet->getPath();
    firebaseGet->getValue();

    connect(firebaseGet, &Firebase::eventResponseReady,
            this, &FirebaseExamples::onResponseReady);
}

void FirebaseExamples::useToken()
{
    QJsonObject jsonRule;
    jsonRule[".read"] = true;
    QJsonObject jsonRules;
    jsonRules["rules"] = jsonRule;
    QByteArray ba;
    QJsonDocument uploadDoc(jsonRules);
    ba = uploadDoc.toJson(QJsonDocument::Compact);
    Firebase *firebaseSet = new Firebase(fbUrl, ".settings/rules");
    qDebug() << "URL:" << firebaseSet->getPath(authToken);
    firebaseSet->setValue(uploadDoc, Firebase::PUT, authToken);

    connect(firebaseSet, &Firebase::eventResponseReady,
            this, &FirebaseExamples::onResponseReady);
}

void FirebaseExamples::listen()
{
    Firebase *firebaseGet = new Firebase(fbUrl, "lll/.json");
    firebaseGet->listenEvents();
    connect(firebaseGet, &Firebase::eventResponseReady,
            this, &FirebaseExamples::onResponseReady);
    connect(firebaseGet, &Firebase::eventDataChanged,
            this, &FirebaseExamples::onDataChanged);
}

void FirebaseExamples::onResponseReady(QNetworkReply::NetworkError error, QByteArray data)
{
    if (error != QNetworkReply::NoError) {
        qDebug() << "the firebase request failed!";
        return;
    }
    qDebug() << "onResponseReady";
    qDebug() << data;
}

void FirebaseExamples::onDataChanged(QString data)
{
    qDebug() << "onDataChanged";
    qDebug() << data;
}
















