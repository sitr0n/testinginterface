#include "activefunctional_test.h"

ActiveFunctionalTest::Parameters ActiveFunctionalTest::s_param;

ActiveFunctionalTest::ActiveFunctionalTest(WingChargeBlock &unit)
    : TestingTask("Active test")
    , m_unit(unit)
    , m_pairDelayStarted(false)
    , m_measuringStarted(false)
{
    setCondition([&, this]{
        if (!m_unit.isEnabled()) {
            m_unit.enable(true);
            return false;
        }

        if (!m_unit.isInducing()) {
            m_unit.setInduction(true);
            return false;
        }

        if (!m_pairDelayStarted) {
            m_pairDelayWatch.start();
            m_pairDelayStarted = true;
        }

        if (m_pairDelayWatch.elapsed() < PAIRING_DELAY) {
            return false;
        }

        if (!m_unit.isPaired()) {
            m_unit.pair();
            return false;
        }

        if (!m_measuringStarted) {
            m_measuringWatch.start();
            m_measuringStarted = true;
        }
        return true;
    });

    setMeasuring([&]{
        if (m_measuringWatch.elapsed() >= STABILIZING_INERTIA) {
            m_iSupplySamples.push_back(m_unit.data().iSupply);
            m_iSupplyWingSamples.push_back(m_unit.data().iSupplyWing);
            m_wingLQISamples.push_back(m_unit.data().wing.LQI);
            m_slotLQISamples.push_back(m_unit.data().LQI);
            m_wingLossSamples.push_back(m_unit.data().wing.loss);
        }

        Results results;
        if (m_measuringWatch.elapsed() >= s_param.measuringDuration) {
            results.insert("iSupply_charge",
                std::accumulate(m_iSupplySamples.begin(), m_iSupplySamples.end(), 0.0) / m_iSupplySamples.size()
            );
            results.insert("iSupplyWing_charge",
                std::accumulate(m_iSupplyWingSamples.begin(), m_iSupplyWingSamples.end(), 0.0) / m_iSupplyWingSamples.size()
            );
            results.insert("WingLQI",
                std::accumulate(m_wingLQISamples.begin(), m_wingLQISamples.end(), 0.0) / m_wingLQISamples.size()
            );
            results.insert("SlotLQI",
                std::accumulate(m_slotLQISamples.begin(), m_slotLQISamples.end(), 0.0) / m_slotLQISamples.size()
            );
            results.insert("WingLoss",
                std::accumulate(m_wingLossSamples.begin(), m_wingLossSamples.end(), 0.0) / m_wingLossSamples.size()
            );
            results.insert("SlotLoss", m_unit.data().loss);
            results.insert("WingBatCurrent", m_unit.data().wing.batCurrent);
            results.insert("WingSerial", m_unit.data().wing.serial);
            results.insert("InertiaDelay_charge", STABILIZING_INERTIA);
            results.insert("MeasurigDuration_charge", s_param.measuringDuration);
            results.insert("WingBatCapacity", m_unit.data().wing.batCapacity);
            results.insert("WingBatVolt", m_unit.data().wing.batVolt);
            results.insert("WingHumidity", m_unit.data().wing.humidity);
            results.insert("WingTemperature", m_unit.data().wing.temperature);
            results.insert("SlotTemperature", m_unit.data().temperature);
        }
        return results;
    });

    setAssertion([&](Results results){
        bool approved = true;
        if (results.value("iSupply_charge") > s_param.margin.inputCurrent) {
            approved = false;
        }
        if (results.value("iSupplyWing_charge") > s_param.margin.inductiveCurrent) {
            approved = false;
        }
        if (results.value("WingLQI") < s_param.margin.wingLQI) {
            approved = false;
        }
        if (results.value("SlotLQI") < s_param.margin.slotLQI) {
            approved = false;
        }
        if (results.value("WingLoss") > s_param.margin.wingLoss) {
            approved = false;
        }
        if (results.value("SlotLoss") > s_param.margin.slotLoss) {
            approved = false;
        }
        if (results.value("WingChargeCurrent") < s_param.margin.chargeCurrent) {
            approved = false;
        }
        if (results.value("SlotTemperature").toFloat() < 15.0 || 50.0 < results.value("SlotTemperature").toFloat()) { // TODO: Extract the hardcoded margins
            approved = false;
        }
        return approved;
    });

    setTearDown([&]{
        m_iSupplySamples.clear();
        m_iSupplyWingSamples.clear();
        m_wingLQISamples.clear();
        m_slotLQISamples.clear();
        m_wingLossSamples.clear();
    });

    setTimeout(s_param.measuringDuration + TIMEOUT_MARGIN);
}

ActiveFunctionalTest::Parameters ActiveFunctionalTest::getParameters()
{
    return s_param;
}

void ActiveFunctionalTest::setParameters(ActiveFunctionalTest::Parameters param)
{
    s_param = param;
}
