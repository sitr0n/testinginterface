#include "testingtask.h"
#include <QDebug>

TestingTask::TestingTask(const QString &name)
    : m_name(name)
    , m_state(TestingInterface::INCOMPLETE)
    , m_conditionsHaveBeenMet(false)
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

void TestingTask::tick()
{
    if (m_conditional()) {
        m_conditionsHaveBeenMet = true;
        m_results = m_measure();
        if (!m_results.empty()) {
            m_state = m_evaluate(m_results) ?
                TestingInterface::PASSED:
                TestingInterface::FAILED;
        }
    } else if (m_conditionsHaveBeenMet){
        m_state = TestingInterface::DISQUALIFIED;
    } else if (false) {
        // TODO: Implement watchdog option
        m_state = TestingInterface::TIMEOUT;
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
