# Qt_Firebase_REST_API

This provides a wrapper around the Firebase REST API for use with Qt Projects. A detailed explanation of this project can be found at [the blog post](http://piersshepperson.co.uk/programming/2017/06/26/firebase-database-rest-api-qt/). 

Details of the Firebase REST API can be found at [here](https://firebase.google.com/docs/reference/rest/database/).

This porject is related to [chlasd's Firebase API](https://github.com/clkasd/qt-firebaseapi) but updated, slimmed and modernised.

To use copy the firebase.h and firebase.cpp files into your own projects. Either copy the files form GitHub indivdually, or downlaod the whole project with 

`git clone https://github.com/Sriep/Qt_Firebase_REST_API.git`

##Overview

The API supports three types of commands. Read, write and listen. The steps to perform a request are as follows.

1. Create a Firebase object. The constructor takes your Firebase URL and REST endpoint inside your database as parameters.
2. Call the appropriate Firebase function. setValue, getValue or listenEvents. All three take the query string as an optional parameter.
3. Create a slot to connect to either of the Firebase signals, eventDataChange and eventResponseReady. Read up here for a refresher on signals and slots.
4. If necessary handle the response in your slot(s).





