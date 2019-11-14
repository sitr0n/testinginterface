#pragma once
#include "testinginterface.h"
#include <QString>
#include <functional>
#include <QTime>

class TestingTask : public TestingInterface
{
public:
    TestingTask(const QString &name);
    ~TestingTask() = default;

    /* Return true from condition, if ready to measure */
    void setCondition(std::function<bool()> cond);

    /* Return complete results to assert their values */
    void setMeasuring(std::function<Results()> meas);

    /* Compare results to expectations and return approval */
    void setAssertion(std::function<bool(Results)> assert);

    /* Clean up after testing */
    void setTearDown(std::function<void()> teardown);

    /* Set a timeout to assess work load and prevent infinite attempts */
    void setTimeout(int timeout);

    void tick();
    bool isFinished() const;
    States state();
    Results results() const;

    int lift() const;
    int weight() const;

    void reset();

protected:
    void enableWatchdog();

private:
    QString m_name;
    TestingInterface::State m_state;
    bool m_conditionsHaveBeenMet;

    std::function<bool ()> m_conditional;
    std::function<Results ()> m_measure;
    std::function<bool (Results)> m_assert;
    std::function<void()> m_teardown;

    QTime m_watchdog;
    int m_timeout;
    bool m_watchdogEnabled;

    Results m_results;

    const int DEFAULT_TIMEOUT = 30000;
};
