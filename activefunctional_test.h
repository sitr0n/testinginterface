#pragma once
#include "testingtask.h"
#include "wingchargeblock.h"
#include <QTime>
#include <vector>

class ActiveFunctionalTest : public TestingTask
{
public:
    ActiveFunctionalTest(WingChargeBlock &unit);

    struct Margins {
        float inputCurrent = 180.0;
        float inductiveCurrent = 180.0;
        float wingLQI = 60.0;
        float slotLQI = 60.0;
        float wingLoss = 1.0;
        float slotLoss = 1.0;
        float chargeCurrent = 48.0;
    };
    struct Parameters {
        int measuringDuration = 15000;
        Margins margin;
    };
    static Parameters getParameters();
    static void setParameters(Parameters param);

private:
    WingChargeBlock &m_unit;
    bool m_pairDelayStarted;
    bool m_measuringStarted;

    QTime m_pairDelayWatch;
    QTime m_measuringWatch;

    std::vector<float> m_iSupplySamples;
    std::vector<float> m_iSupplyWingSamples;
    std::vector<float> m_wingLossSamples;
    std::vector<float> m_wingLQISamples;
    std::vector<float> m_slotLQISamples;

    static Parameters s_param;

    const int PAIRING_DELAY = 2000;
    const int STABILIZING_INERTIA = 22500; // wingLoss seems to stabilize after ~22.5 seconds [= 22500 ms]
    const int TIMEOUT_MARGIN = 5000;
};
