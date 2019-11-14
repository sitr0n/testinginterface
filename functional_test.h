#pragma once
#include "testingcomposite.h"
#include "passivefunctional_test.h"
#include "activefunctional_test.h"
#include "wingchargeblock.h"

class FunctionalTest : public TestingComposite
{
public:
    FunctionalTest(WingChargeBlock &unit);

    struct Parameters
    {
        float wingComInterval;
        PassiveFunctionalTest::Parameters passive;
        int pairingTimeout;
        ActiveFunctionalTest::Parameters active;
    };
    static Parameters getParameters();
    static void setParameters(Parameters param);
    static QString getVersion();

private:
    static QString s_version;
    static float s_wingComInterval;
};
