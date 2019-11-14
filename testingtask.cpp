#include "testingtask.h"
#include <QDebug>

TestingTask::TestingTask(const QString &name)
    : m_name(name)
    , m_state(TestingInterface::INCOMPLETE)
    , m_conditionsHaveBeenMet(false)
    , m_watchdogEnabled(false)
{
    m_conditional = []{
        return true;
    };
    m_measure = []{
        return TestingInterface::Results();
    };
    m_assert = [](TestingInterface::Results){
        return true;
    };
    m_teardown = [](){};
    setTimeout(DEFAULT_TIMEOUT);
}

void TestingTask::setCondition(std::function<bool ()> cond)
{
    m_conditional = cond;
}

void TestingTask::setMeasuring(std::function<TestingInterface::Results ()> meas)
{
    m_measure = meas;
}

void TestingTask::setAssertion(std::function<bool (TestingInterface::Results)> assert)
{
    m_assert = assert;
}

void TestingTask::setTearDown(std::function<void ()> teardown)
{
    m_teardown = teardown;
}

void TestingTask::setTimeout(int timeout)
{
    m_timeout = timeout;
}

void TestingTask::tick()
{
    if (m_conditional() == true) {
        m_conditionsHaveBeenMet = true;
        m_results = m_measure();
        if (!m_results.empty()) {
            m_state = m_assert(m_results) ?
                TestingInterface::PASSED:
                TestingInterface::FAILED;
        }
    } else if (m_conditionsHaveBeenMet) {
        m_state = TestingInterface::DISQUALIFIED;
    } else if (m_watchdogEnabled && (m_watchdog.elapsed() >= m_timeout)) {
        m_state = TestingInterface::TIMEOUT;
    } else {
        enableWatchdog();
    }
    if (isFinished()) {
        m_teardown();
    }
}

bool TestingTask::isFinished() const
{
    return (m_state != TestingInterface::INCOMPLETE);
}

TestingInterface::States TestingTask::state()
{
    States container;
    container.insert(m_name, &m_state);
    return container;
}

TestingInterface::Results TestingTask::results() const
{
    return m_results;
}

int TestingTask::lift() const
{
    return isFinished() ? m_timeout : 0;
}

int TestingTask::weight() const
{
    return m_timeout;
}

void TestingTask::reset()
{
    m_state = TestingInterface::INCOMPLETE;
    m_conditionsHaveBeenMet = false;
    m_watchdogEnabled = false;
}

void TestingTask::enableWatchdog()
{
    if (!m_watchdogEnabled) {
        m_watchdog.start();
        m_watchdogEnabled = true;
    }
}
