#pragma once
#include "testingtask.h"
#include "wingchargeblock.h"

#include <QTime>

class PairingFunctionalTest : public TestingTask
{
public:
    PairingFunctionalTest(WingChargeBlock &unit);

    static int getPairTimeout();
    static void setPairTimeout(int timeout);

private:
    WingChargeBlock &m_unit;
    bool m_powerReset;
    bool m_pairingStarted;
    QTime m_pairingWatch;

    static int s_timeout;
};
