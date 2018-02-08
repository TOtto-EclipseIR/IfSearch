#include <QtCore/QString>
#include <QtTest/QtTest>

#include <eirCore/eirCore.h>
#include <eirCore/VariableSet.h>
#include <eirCore/VariableSettings.h>
#include <eirCore/VariableSettingsList.h>

class eirCoreTest : public QObject
{
    Q_OBJECT

public:
    eirCoreTest(void);

private Q_SLOTS:
    void testCase1(void);
    void testVariableSet(void);
    void testVariableSettings(void);
    void testVariableSettingsList(void);
};

eirCoreTest::eirCoreTest(void)
{
}

void eirCoreTest::testCase1(void)
{
    QVERIFY2(true, "Failure");
}

void eirCoreTest::testVariableSettingsList(void)
{
    VariableSettings stgs1(QSettings::IniFormat, "../testData/VariableSettingsTest.ini");
    stgs1.read();
    stgs1.blog();
    QCOMPARE(stgs1.value("Input/SampleMsec").toInt(), 1000);
    VariableSettings stgs2(QSettings::IniFormat, "../testData/VariableSettingsTest2.ini");
    stgs2.read();
    stgs2.blog();
    QCOMPARE(stgs2.value("Input/SampleMsec").toInt(), 500);

    VariableSettingsList vsl;
    vsl.add(&stgs1);
    vsl.add(&stgs2);
    vsl.read();
    VariableSet input(vsl.exportSection("Input"));
    input.blog();
    QVariant var(input.value("SampleMsec"));
    QCOMPARE(var.toInt(), 500);
}

void eirCoreTest::testVariableSettings(void)
{
    VariableSettings stgs(QSettings::IniFormat, "../testData/VariableSettingsTest.ini");
    stgs.read();
    stgs.blog();
    QCOMPARE(stgs.value("Input/SampleMsec").toInt(), 1000);
}

void eirCoreTest::testVariableSet(void)
{
    VariableSet nullVbls;
    QCOMPARE(nullVbls.isEmpty(), true);

    VariableSet v1("vbls1");
    v1.set("Data");
    QCOMPARE(v1.isEmpty(), false);
    QCOMPARE(v1.name(), QString("vbls1"));
    v1.clear();
    QCOMPARE(v1.isEmpty(), true);
    QCOMPARE(v1.name(), QString("vbls1"));

    VariableSet vki(12345, "vkiId");
    QCOMPARE(vki.key(), 12345ULL);
    QCOMPARE(vki.id(), QString("vkiId"));

    vki.set("Export/Number", 4567);
    VariableSet v2 = vki.exportSection("Export");
    QCOMPARE(v2.value("Number").toInt(), 4567);
}

QTEST_APPLESS_MAIN(eirCoreTest);

#include "eirCoreTest.moc"
