#include "passivefunctional_test.h"
#include <QDebug>


PassiveFunctionalTest::Parameters PassiveFunctionalTest::s_param;

PassiveFunctionalTest::PassiveFunctionalTest(WingChargeBlock &unit)
    : TestingTask("Passive test")
    , m_unit(unit)
    , m_measuringStarted(false)
{
    setCondition([&]{
        if (!m_unit.isEnabled()) {
            m_unit.enable(true);
            return false;
        }

        if (m_unit.isInducing()) {
            m_unit.setInduction(false);
            return false;
        }

        if (!m_measuringStarted) {
            m_measuringWatch.start();
            m_measuringStarted = true;
        }
        return true;
    });

    setMeasuring([&]{
        if (m_measuringWatch.elapsed() > STABILIZING_INERTIA) {
            m_iSupplySamples.push_back(m_unit.data().iSupply);
            m_iSupplyWingSamples.push_back(m_unit.data().iSupplyWing);
        }

        Results results;
        if (m_measuringWatch.elapsed() >= s_param.measuringDuration) {
            results.insert("iSupply_noCharge",
                std::accumulate(m_iSupplySamples.begin(), m_iSupplySamples.end(), 0.0) / m_iSupplySamples.size()
            );
            results.insert("iSupplyWing_noCharge",
                std::accumulate(m_iSupplyWingSamples.begin(), m_iSupplyWingSamples.end(), 0.0) / m_iSupplyWingSamples.size()
            );
            results.insert("InertiaDelay_noCharge", STABILIZING_INERTIA);
            results.insert("MeasuringDuration_noCharge", s_param.measuringDuration);
        }
        return results;
    });

    setAssertion([&](Results results){
        bool approved = true;
        if (results.value("iSupply_noCharge") > s_param.margin.inputCurrent) {
            approved = false;
        }
        if (results.value("iSupplyWing_noCharge") > s_param.margin.inductiveCurrent) {
            approved = false;
        }
        return approved;
    });

    setTearDown([&]{
        m_iSupplySamples.clear();
        m_iSupplyWingSamples.clear();
    });

    setTimeout(s_param.measuringDuration + TIMEOUT_MARGIN);
}

PassiveFunctionalTest::Parameters PassiveFunctionalTest::getParameters()
{
    return s_param;
}

void PassiveFunctionalTest::setParameters(PassiveFunctionalTest::Parameters param)
{
    s_param = param;
}
