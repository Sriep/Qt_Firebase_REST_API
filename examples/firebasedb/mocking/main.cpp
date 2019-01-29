#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <trompeloeil/trompeloeil.hpp>

namespace trompeloeil
{
    template <>
    void reporter<specialized>::send(severity s, const char* file,
        unsigned long line, const char* msg)
    {
        std::ostringstream os;
        if (line) os << file << ':' << line << '\n';
            os << msg;
        auto failure = os.str();
        if (s == severity::fatal) {
            FAIL(failure);
        } else {
            CAPTURE(failure);
            CHECK(failure.empty());
        }
    }
}

/*** code to test (should not be written here :) ) ***/

#include <QtFirebaseDb/firebaseinterface.h>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QVariant>

class ShoppingList
{
public:
    ShoppingList(FirebaseInterface &firebaseConnection)
        : m_connection(firebaseConnection)
    {
        download();
        QObject::connect(&firebaseConnection, &FirebaseInterface::eventResponseReady, [this](QByteArray data) {
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject obj = doc.object();
            for (auto &item : obj["articles"].toArray().toVariantList()) {
                const QString article = item.toString();
                if (!m_articles.contains(article))
                    m_articles.append(article);
            }
        });
    }

    int articleCount() const { return m_articles.count(); }
    QStringList articles() const { return m_articles; }
    void addArticle(const QString &article) { m_articles.append(article); }

    void upload()
    {
        QJsonArray arr = QJsonArray::fromStringList(m_articles);
        QJsonObject obj;
        obj["articles"] = arr;
        QJsonDocument doc(obj);
        m_connection.setValue(doc);
    }
    void download()
    {
        m_connection.getValue();
    }

private:
    FirebaseInterface &m_connection;
    QStringList m_articles;
};

/*** mocking ***/

class FirebaseMock : public FirebaseInterface
{
public:
    MAKE_MOCK3(setValue,        void(QJsonDocument, const QString&, const QString&), override);
    MAKE_MOCK1(getValue,        void(const QString&), override);
    MAKE_MOCK1(listenEvents,    void(const QString&), override);
    MAKE_MOCK1(getPath,         QString(const QString &), override);
};

/*** sample data ***/
const std::string downloadInitialSample = "{ \"articles\": [ \"Apple\", \"Banana\" ] }";
const QString uploadSample1 = QLatin1String("{ \"articles\": [ \"Apple\", \"Banana\", \"Tomatoes\", \"Cheese\" ] }");

/*** tests ***/
using trompeloeil::_;
using trompeloeil::gt;

TEST_CASE("get user data", "[download]") {
    FirebaseMock mock;

    REQUIRE_CALL_V(mock, getValue(""));
    ShoppingList list(mock);

    SECTION("simple download") {
        emit mock.eventResponseReady(QByteArray::fromStdString(downloadInitialSample));
        REQUIRE(list.articleCount() == 2);
    }

    SECTION("download finished after append") {
        list.addArticle("Ananas");
        emit mock.eventResponseReady(QByteArray::fromStdString(downloadInitialSample));
        REQUIRE(list.articleCount() == 3);

        SECTION("don't simply append after initial sync") {
            REQUIRE_CALL_V(mock, getValue(""));
            list.download();
            REQUIRE(list.articleCount() == 3);
        }
    }
}

TEST_CASE("set user data", "[upload]") {
    FirebaseMock mock;

    REQUIRE_CALL_V(mock, getValue(""));
    ShoppingList list(mock);

    emit mock.eventResponseReady(QByteArray::fromStdString(downloadInitialSample));
    REQUIRE(list.articleCount() == 2);

    SECTION("add two articles and sync") {
        list.addArticle("Tomatoes");
        list.addArticle("Cheese");

        REQUIRE_CALL_V(mock, setValue(QJsonDocument::fromJson(uploadSample1.toUtf8()), _, _));
        list.upload();
    }
}
