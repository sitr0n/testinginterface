#include "wingchargeblocktestfactory.h"
#include "functional_test.h"
#include "pairingfunctional_test.h"

WingChargeBlockTestFactory::WingChargeBlockTestFactory(WingChargeBlockManager &units)
    : m_units(units)
{
    m_tests.insert("Functional test", TestType::FunctionalTest);
    m_tests.insert("Pairing test", TestType::PairingTest);
}

void WingChargeBlockTestFactory::selectUnit(int id)
{
    m_unitUnderTest = id;
}

TestingFactory::Test WingChargeBlockTestFactory::createTest(const QString &description)
{
    auto test = m_tests.value(description, TestType::NONE);
    if (test == TestType::NONE) {
        throw "Invalid test description";
    }
    for (auto& unit : m_units) {
        if (unit.id() == m_unitUnderTest) {
            switch (test) {
            case WingChargeBlockTestFactory::TestType::FunctionalTest :
                return std::make_unique<FunctionalTest>(unit);
            case WingChargeBlockTestFactory::TestType::PairingTest:
                return std::make_unique<PairingFunctionalTest>(unit);
            }
        }
    }
    throw QString("Invalid test unit id(%1)").arg(m_unitUnderTest);
}

QStringList WingChargeBlockTestFactory::availableTests() const
{
    return m_tests.keys();
}
