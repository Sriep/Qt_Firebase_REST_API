# Qt Firebase REST API

This provides a wrapper around the Firebase REST API for use with Qt Projects. An alternative explanation of this project can be found at [the blog post](http://piersshepperson.co.uk/programming/2017/06/26/firebase-database-rest-api-qt/).  

Details of the Firebase REST API can be found at [here](https://firebase.google.com/docs/reference/rest/database/).

This porject is related to [chlasd's Firebase API](https://github.com/clkasd/qt-firebaseapi) but updated, slimmed and modernised.

To use, copy the firebase.h and firebase.cpp files into your own projects. Either copy the files form GitHub indivdually, or downlaod the whole project with 

`git clone https://github.com/Sriep/Qt_Firebase_REST_API.git`

## Overview

The API supports three types of commands. Read, write and listen. The steps to perform a request are as follows.

1. Create a Firebase object. The constructor takes your Firebase URL and REST endpoint inside your database as parameters.
2. Call the appropriate Firebase function. setValue, getValue or listenEvents. All three take the query string as an optional parameter.
3. Create a slot to connect to either of the Firebase signals, eventDataChange and eventResponseReady. Read up here for a refresher on signals and slots.
4. If necessary handle the response in your slot(s).

## Anatomy of API request.

Each request can be split up into five main blocks of text plus separators.

- Data. Used for save requests. This has to be in JSON format.
- Firebase URL. The URL of your database, typically “App-Name.firebaseio.com”.
- REST Endpoint. The location in your database we are interacting with.
- Verb. The action to carry out. GET, PUT, PATCH, POST or DELETE.
- Query. Modifications to your request; appears at the end of the URL.
- Separators. “https://”,”/”,”.json” and “?”. splits the other items.

### GET Request

There is no Data for GET requests and the Verb is already known. So a typical GET request might look like the following.

`https://[PROJECT_ID].firebaseio-demo.com/users/jack/name.json?print=pretty`

We make this request through the API follows. First, craete our Firebase object that points to `users/jack/name` item in our database. Then we use `getValue` to gert the content. The `print=pretty` query returns data in human readable format.

```
Firebase *fb=new Firebase("[PROJECT_ID].firebaseio-demo.com"
                                , "users/jack/name");
fb->getValue("print=pretty");
```

### Save Request

A save request might look like the following in curl format. Note that in this example there is no query.
```
curl -X PUT -d ‘{
“alanisawesome”: {
“name”: “Alan Turing”,
“birthday”: “June 23, 1912”
}
}’ ‘https://docs-examples.firebaseio.com/rest/saving-data/fireblog/users.json‘
```

To make this call we could proceed as follows. First, we create our JSON data, I personally think its less error prone to use the Qt JSON API functions rather than raw strings for JSON content. Next, we create our Firebase object that points to `rest/saving-data/fireblog/users` item in our database. We can then call `setValue` passing the data in a `QJsonDocument` and entering the Verb “PUT”. There is no query this time so we skip the optional parameter.

```
QJsonObject jsonDetails;
jsonObj["name"] = "Alan Turing";
jsonObj["birthday"] = "June 23, 1912";
QJsonObject jsonAlanTuring;
jsonAlanTuring["alanisawesome"] = jsonDetails;
QJsonDocument uploadDoc(jsonAlanTuring);
Firebase *fb = new Firebase(“docs-examples.firebaseio.com“, “rest/saving-data/fireblog/users“);
fb->setValue(uploadDoc, “PUT“);
```

### Handle response

Once we have made a request we will normally wish to see the response. Even if it is just to know the request has finished successfully. To do this we handle the `responesReady` signal from the Firebase object. So if we define an `onResponseReasy` function in our class that is using the Firebase object, we can add the following line:
```
connect(fb,SIGNAL(eventResponseReady(QByteArray)) 
             ,this,SLOT(onResponseReady(QByteArray)));
```
And create the onResponseReady function to handle it. You can see examples in firebaseexamples.cpp.
```
MyFirebaseUsingClass::onResponseReady(QByteArray replyData)
{
  // Handel response
}
```
Now whenever the Firebase object emits a responseReasy signal, the onResponseReady function will be called with the `QByteArray` parameter will hold any information returned. You can put any code to handle the data and finish the request in here. The `Firebase::onResponseReasy` signal actually chains the `QtNetworkReply::finished` signal which is emitted when the reply has finished processing. The `QByteArry` parameter is the return value from the inherited function `QtNetworkReply::readAll` which reads all the remaining data.

### Stream changes to database

The Firebase REST API allows the streaming of changes to a single location in your Realtime Database, using the EventSource / Server-Sent Events protocol. Detailed at https://firebase.google.com/docs/reference/rest/database/.

What that means is that you can listen to a location in your database and every time there is a change at that location you are informed. Each time the data at the point you are watching in your Firebase database is changed, the Firebase object sends an `eventDataChanged` signal. The first time the signal is called it returns the initial contents of your database endpoint, further singls indicate subsequent changes. The following code snippet will arrange for the `onDataChanged` function to handle that signal.
```
Firebase *firebaseGet=new Firebase(fbUrl, "lll/.json");
firebaseGet->listenEvents();;
connect(firebaseGet,SIGNAL(eventDataChanged(QString))
          , this,SLOT(onDataChanged(QString)));
```
You can then create the onDataChanged function to place your code to handle the database changes. Look at firebaseexamples.cpp for an example of this.
```
MyFirebaseUsingClass::onDataChanged(QString databaseUpdate)
{
  // Handel response
}
```





