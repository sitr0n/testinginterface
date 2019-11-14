#pragma once

#include "testingfactory.h"
#include "wingchargeblockmanager.h"
#include <QMap>

class WingChargeBlockTestFactory : public TestingFactory
{
public:
    enum class TestType {
        NONE,
        FunctionalTest,
        PairingTest
    };

    WingChargeBlockTestFactory(WingChargeBlockManager &units);
    void selectUnit(int id);
    Test createTest(const QString &description);
    virtual QStringList availableTests() const;

private:
    WingChargeBlockManager &m_units;
    int m_unitUnderTest;

    QMap<QString, TestType> m_tests;
};
