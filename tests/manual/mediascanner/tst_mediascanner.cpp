#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include <QVariant>
#include <QMap>
#include "mediascanner.h"


class TestMediaScanner : public QObject
{
    Q_OBJECT

public:
    TestMediaScanner();
    ~TestMediaScanner();

private Q_SLOTS:
    void testCase1();
    void testCase2();
    void testCase3();
private:
    MediaScanner *mediascanner;
};

TestMediaScanner::TestMediaScanner()
{
    QFile file ("media_database");
    file.remove();
    mediascanner = new MediaScanner();
}
TestMediaScanner::~TestMediaScanner()
{
}

void TestMediaScanner::testCase1()
{

    QSignalSpy scanningStarted(mediascanner, SIGNAL(scanningStarted()));
    QSignalSpy scanningFinished(mediascanner, SIGNAL(scanningFinished()));

    int ret = mediascanner->addLocation("/media/gino/Gino HDD/music/BeerKey");

    QVERIFY2(ret, "Adding location to database and starting scanning");
}

void TestMediaScanner::testCase2()
{

    int ret = mediascanner->addLocation("/media/gino/Gino HDD/music/BeerKey");

    QVERIFY2(ret == -2, "Re-adding location to database and starting scanning");
}

void TestMediaScanner::testCase3()
{

    /*QVariantMap result = mediascanner->getMusicInfo("/media/gino/Gino HDD/music/Drum and Bass/VA-DNB sound vol.21/Audio - Rogue (Original mix).mp3");
    qDebug()<<result["artist"]<<result["title"]<<result["album"];
    QVERIFY2(!result.isEmpty(), "Getting musicinfo is unsucessful");*/
}

QTEST_APPLESS_MAIN(TestMediaScanner)

#include "tst_mediascanner.moc"
