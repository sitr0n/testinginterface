#include "functional_test.h"
#include "pairingfunctional_test.h"

QString FunctionalTest::s_version = "2.01.02";
float FunctionalTest::s_wingComInterval = 0.1;

FunctionalTest::FunctionalTest(WingChargeBlock &unit)
{
    auto overhead = new TestingTask("Setup");
    overhead->setCondition([=, &unit]{
        if (unit.data().wingSampling != s_wingComInterval) {
            unit.setWingSampling(s_wingComInterval);
            return false;
        }
        return true;
    });
    overhead->setMeasuring([=, &unit]{
        Results results;
        results.insert("Product", "eB-WCB_001");
        results.insert("SerialNo", unit.id());
        results.insert("Import configuration", 118);
        results.insert("ProductFamily", "eBird");
        results.insert("FirmwareVersion", unit.data().firmware);
        results.insert("TestVersion", s_version);
        results.insert("WingComInterval", unit.data().wingSampling);
        return results;
    });
    overhead->setTimeout(1000);
    add(overhead);
    add(new PassiveFunctionalTest(unit));
    add(new PairingFunctionalTest(unit));
    add(new ActiveFunctionalTest(unit));

}

FunctionalTest::Parameters FunctionalTest::getParameters()
{
    FunctionalTest::Parameters param;
    param.wingComInterval = s_wingComInterval;
    param.passive = PassiveFunctionalTest::getParameters();
    param.pairingTimeout = PairingFunctionalTest::getPairTimeout();
    param.active = ActiveFunctionalTest::getParameters();
    return param;
}

void FunctionalTest::setParameters(FunctionalTest::Parameters param)
{
    s_wingComInterval = param.wingComInterval;
    PassiveFunctionalTest::setParameters(param.passive);
    PairingFunctionalTest::setPairTimeout(param.pairingTimeout);
    ActiveFunctionalTest::setParameters(param.active);
}

QString FunctionalTest::getVersion()
{
    return s_version;
}
