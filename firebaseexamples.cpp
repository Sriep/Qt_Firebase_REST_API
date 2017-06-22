#include "firebaseexamples.h"
#include <QUrl>
#include <firebase.h>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonDocument>

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
    jsonObj["user"] = "hi";
    jsonObj["password"] = "password";
    jsonObj["email"] = "email@hi.co.uk";
    QJsonDocument uploadDoc(jsonObj);
    QString path="lll/users/fred/";
    Firebase *firebaseSet = new Firebase(fbUrl, path);
    firebaseSet->setValue(uploadDoc, "PATCH");

    connect(firebaseSet,SIGNAL(replyFinished(QNetworkReply))
            ,this,SLOT(onReplyFinished(QNetworkReply)));

}

void FirebaseExamples::get()
{
    Firebase *firebaseGet=new Firebase(fbUrl, "lll/users/.json");
    qDebug()<<"URL:" << firebaseGet->getPath();
    firebaseGet->getValue();

    connect(firebaseGet,SIGNAL(eventResponseReady(QByteArray))
            ,this,SLOT(onResponseReady(QByteArray)));

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
    qDebug()<<"URL:" << firebaseSet->getPath(authToken);
    firebaseSet->setValue(uploadDoc, "PUT", authToken);

    connect(firebaseSet,SIGNAL(eventResponseReady(QNetworkReply))
            ,this,SLOT(onResponseReady(QNetworkReply)));

}

void FirebaseExamples::listen()
{
    Firebase *firebaseGet=new Firebase(fbUrl, "lll/.json");
    firebaseGet->listenEvents();
    connect(firebaseGet,SIGNAL(eventResponseReady(QByteArray))
            ,this,SLOT(onResponseReady(QByteArray)));
    connect(firebaseGet,SIGNAL(eventDataChanged(QString))
            ,this,SLOT(onDataChanged(QString)));
}

void FirebaseExamples::onResponseReady(QByteArray data)
{
    qDebug()<<"onResponseReady";
    qDebug()<<data;
}

void FirebaseExamples::onDataChanged(QString data)
{
    qDebug()<<"onDataChanged";
    qDebug()<<data;
}
















