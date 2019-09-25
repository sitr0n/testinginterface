#include "testingtask.h"
#include <QDebug>

TestingTask::TestingTask(const QString &name)
    : m_name(name)
    , m_state(TestingInterface::INCOMPLETE)
    , m_conditionsHaveBeenMet(false)
    , m_watchdogEnabled(false)
{
    m_conditional = [](){
        return true;
    };
    m_measure = [](){
        return TestingInterface::Results();
    };
    m_evaluate = [](TestingInterface::Results){
        return false;
    };
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

void TestingTask::setEvaluation(std::function<bool (TestingInterface::Results)> eval)
{
    m_evaluate = eval;
}

void TestingTask::setTimeout(int timeout)
{
    m_timeout = timeout;
}

void TestingTask::tick()
{
    if (m_conditional() == true) {
        //qDebug() << "Enter conditional";
        m_conditionsHaveBeenMet = true;
        m_results = m_measure();
        if (!m_results.empty()) {
            m_state = m_evaluate(m_results) ?
                TestingInterface::PASSED:
                TestingInterface::FAILED;
        }
    } else if (m_conditionsHaveBeenMet) {
        //qDebug() << "Enter disqualify";
        m_state = TestingInterface::DISQUALIFIED;
    } else if (m_watchdog.elapsed() >= m_timeout) {
        //qDebug() << "Enter timeout";
        m_state = TestingInterface::TIMEOUT;
    } else {
        //qDebug() << "Enter enable wd";
        //qDebug() << QString::number(m_watchdog.elapsed());
        //qDebug() << QString::number(m_timeout);
        enableWatchdog();
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

void TestingTask::enableWatchdog()
{
    if (!m_watchdogEnabled) {
        m_watchdog.start();
        m_watchdogEnabled = true;
    }
}
