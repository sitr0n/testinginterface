#pragma once
#include "testinginterface.h"
#include <QString>
#include <functional>
#include <QTime>

class TestingTask : public TestingInterface
{
public:
    TestingTask(const QString &name);
    /* Measuring will not start until Conditions are met.
     * If conditions are lost before measuring returns
     * the results, the test will fail before evaluation
     * is allowed to run */
    void setCondition(std::function<bool()> cond);
    void setMeasuring(std::function<Results()> meas);
    void setEvaluation(std::function<bool(Results)> eval);
    void setTimeout(int timeout);

    void tick();
    bool isFinished() const;
    States state();
    Results results() const;

    int lift() const;
    int weight() const;

protected:
    void enableWatchdog();

private:
    QString m_name;
    TestingInterface::State m_state;
    bool m_conditionsHaveBeenMet;

    std::function<bool ()> m_conditional;
    std::function<Results ()> m_measure;
    std::function<bool (Results)> m_evaluate;

    QTime m_watchdog;
    int m_timeout;
    bool m_watchdogEnabled;

    Results m_results;

    const int DEFAULT_TIMEOUT = 30000;
};
