#include "pairingfunctional_test.h"

int PairingFunctionalTest::s_timeout = 30000;
PairingFunctionalTest::PairingFunctionalTest(WingChargeBlock &unit)
    : TestingTask("Pairing test")
    , m_unit(unit)
    , m_powerReset(false)
    , m_pairingStarted(false)
{
    setCondition([this]{
        if (!m_unit.isEnabled()) {
            m_unit.enable(true);
            return false;
        }

        if (!m_powerReset && m_unit.isInducing()) {
            m_unit.setInduction(false);
            return false;
        } else {
            m_powerReset = true;
        }
        if (!m_unit.isInducing() && !m_unit.isPaired()) {
            m_unit.setInduction(true);
            return false;
        }
        if (!m_pairingStarted) {
            m_pairingWatch.start();
            m_pairingStarted = true;
        }
        if (!m_unit.isPaired()) {
            m_unit.pair();
            return false;
        }

        return true;
    });

    setMeasuring([&]{
        Results results;
        results.insert("PairingElapsed", m_pairingWatch.elapsed());
        return results;
    });

    setTimeout(s_timeout);
}

int PairingFunctionalTest::getPairTimeout()
{
    return s_timeout;
}

void PairingFunctionalTest::setPairTimeout(int timeout)
{
    s_timeout = timeout;
}
