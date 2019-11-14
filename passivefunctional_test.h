#pragma once
#include "testingtask.h"
#include "wingchargeblock.h"
#include <QTime>
#include <vector>

class PassiveFunctionalTest : public TestingTask
{
public:
    PassiveFunctionalTest(WingChargeBlock &unit);

    struct Margins {
        float inputCurrent = 8;
        float inductiveCurrent = 8;
    };
    struct Parameters {
        int measuringDuration = 10000;
        Margins margin;
    };
    static Parameters getParameters();
    static void setParameters(Parameters param);

private:
    WingChargeBlock &m_unit;
    bool m_measuringStarted;

    std::vector<float> m_iSupplySamples;
    std::vector<float> m_iSupplyWingSamples;
    QTime m_measuringWatch;

    static Parameters s_param;

    const int STABILIZING_INERTIA = 5000; // iSupply seems to stabilize after ~5 seconds [= 5000 ms]
    const int TIMEOUT_MARGIN = 2000;
};
