#include "testingworker.h"
#include <QMutexLocker>

#include <QDebug>

TestingWorker::TestingWorker(QObject *parent)
    : QThread(parent)
    , m_quit(false)
{
    m_log.setTitle("Activity_Test_118_eBird_Wing");
    m_log.setPath("~/PALM/log");

    m_log.addColumn("TestVersion");
    m_log.addColumn("Approved");
    m_log.addColumn("PairingElapsed");
    m_log.addColumn("PairingStable");
    m_log.addColumn("WingComInterval");
    m_log.addColumn("WingSerial");
    m_log.addColumn("InertiaDelay_noCharge");
    m_log.addColumn("MeasuringDuration_noCharge");
    m_log.addColumn("iSupply_noCharge");
    m_log.addColumn("iSupplyWing_noCharge");
    m_log.addColumn("InertiaDelay_charge");
    m_log.addColumn("MeasurigDuration_charge");
    m_log.addColumn("iSupply_charge");
    m_log.addColumn("iSupplyWing_charge");
    m_log.addColumn("InertiaDelay_noLoad");         // To be implemented
    m_log.addColumn("MeasuringDuration_noLoad");    // To be implemented
    m_log.addColumn("iSupply_noLoad");              // To be implemented
    m_log.addColumn("iSupplyWing_noLoad");          // To be implemented
    m_log.addColumn("WingBatCapacity");
    m_log.addColumn("WingBatCurrent");
    m_log.addColumn("WingBatVolt");
    m_log.addColumn("WingHumidity");
    m_log.addColumn("WingLoss");
    m_log.addColumn("WingLQI");
    m_log.addColumn("WingTemperature");
    m_log.addColumn("SlotLoss");
    m_log.addColumn("SlotLQI");
    m_log.addColumn("SlotTemperature");
}

TestingWorker::~TestingWorker()
{
    stop();
}

void TestingWorker::start(TestingInterface *test)
{
    QMutexLocker locker(&m_mutex);
    m_test = test;
    if (isRunning()) {
        m_cond.wakeOne();
    } else {
        m_quit = false;
        QThread::start();
    }
}

void TestingWorker::stop()
{
    m_quit = true;
    unravelThread();
}

void TestingWorker::run()
{
    m_mutex.lock();
    auto test = m_test;
    auto completed = percentCompleted(test);
    emitHeader(test);
    m_mutex.unlock();

    while (!m_quit) {
        if (!test->isFinished()) {
            test->tick();
            QThread::msleep(500);
            if (percentCompleted(test) != completed) {
                completed = percentCompleted(test);
                emit progress(completed, "Test"); // TODO: Implement QString getCurrentTask() in TestingComposite class, and emit in place of "Test"
            }
        } else {
            auto results = evaluate(test);
            log(results);
            emit finished(results["Approved"].toBool());

            test = awaitNext();
            completed = percentCompleted(test);
            emitHeader(test);
        }
    }
}

void TestingWorker::emitHeader(TestingInterface *test)
{
    auto testCases = test->state().keys().size();
    emit output(QString("[==========] Running %0 test %1").arg(testCases).arg((testCases == 1) ? "case" : "cases"));
}

int TestingWorker::percentCompleted(TestingInterface *test) const
{
    int percent = test->lift() * 100 / test->weight();
    return percent;
}

TestingInterface::Results TestingWorker::evaluate(TestingInterface *test)
{
    bool testApproved = true;
    bool testStable = true;

    for (const auto& state: test->state().keys()) {
        switch (*(test->state().value(state))) {
        case TestingInterface::PASSED :
            emit output(QString("[  PASSED  ] %0").arg(state));
        break;
        case TestingInterface::FAILED :
            testApproved = false;
            emit error(QString("[  FAILED  ] %0").arg(state));
        break;
        case TestingInterface::DISQUALIFIED :
            testApproved = false;
            testStable = false;
            emit error(QString("[ UNSTABLE ] %0").arg(state));
        break;
        case TestingInterface::TIMEOUT :
            testApproved = false;
            emit error(QString("[ TIMEOUT! ] %0").arg(state));
        break;
        case TestingInterface::INCOMPLETE :
            testApproved = false;
            /* Fall through */
        default:
            emit error(QString("FAULTY DATA ERROR: TestingWorker thread received an unexpected test state from %0!").arg(state));
        }
    }
    emit output(""); /* Line separation */

    auto results = test->results();
    results.insert("Approved", testApproved);
    results.insert("PairingStable", testStable);
    return results;
}

void TestingWorker::log(TestingInterface::Results result)
{
    for (const auto& field: result.keys()) {
        m_log.setValue(field, result.value(field));
    }
    m_log.save();
}

TestingInterface *TestingWorker::awaitNext()
{
    QMutexLocker locker(&m_mutex);
    m_cond.wait(&m_mutex);
    return m_test;
}

void TestingWorker::unravelThread()
{
    m_mutex.lock();
    m_cond.wakeOne();
    m_mutex.unlock();
    if (!wait(1)) {
        unravelThread();
    }
}
