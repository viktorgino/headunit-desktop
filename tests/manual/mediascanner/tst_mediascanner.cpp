#include <QString>
#include <QtTest>
#include <QSignalSpy>
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

    int ret = mediascanner->addLocation("Gino HDD", "280CC3BC0CC382F4", "/media/gino/Gino HDD","/music/RnB");

    QVERIFY2(ret, "Adding location to database and starting scanning");
    QCOMPARE(scanningStarted.count(), 1);
    QCOMPARE(scanningFinished.count(), 1);
}

void TestMediaScanner::testCase2()
{

    int ret = mediascanner->addLocation("Gino HDD", "280CC3BC0CC382F4", "/media/gino/Gino HDD","/music/RnB");

    QVERIFY2(ret == -2, "Re-adding location to database and starting scanning");
}

QTEST_APPLESS_MAIN(TestMediaScanner)

#include "tst_mediascanner.moc"
