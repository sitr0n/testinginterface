#include "testingwidget.h"
#include <QHBoxLayout>
#include <QTimer>
#include "functional_test.h"
#include <QDebug>

TestingWidget::TestingWidget(WingChargeBlockManager *units, QCheckView *selector, QWidget *parent)
    : QWidget(parent)
    , m_units(units)
    , m_selector(selector)
{
    connect(&m_worker, &TestingWorker::finished, [&](bool approved){
        if (approved) {
            m_selector->getRow(m_currentUnit)->setForeground(Qt::darkGreen);
        } else {
            m_selector->getRow(m_currentUnit)->setForeground(Qt::red);
        }

        QTimer::singleShot(NEW_TEST_DELAY, this, [&]{
            auto started = startTest();
            if (!started) {
                m_startButton.setState("Test");
            }
        });
    });

    /* Temp solution until better progress handling within TestingComposite is solved */
    connect(&m_worker, &TestingWorker::progress, this, [&](int percent, const QString &test){
        Q_UNUSED(test);
        if (percent == 100) {
            m_progressBar.glideTo(100, 1);
        }
    });
    connect(&m_worker, &TestingWorker::output, this, &TestingWidget::output);
    connect(&m_worker, &TestingWorker::error, this, &TestingWidget::error);
    setLayout(new QHBoxLayout(this));
    m_startButton.addState("Test", [&]{
        m_testQueue = getUnits();
        auto started = startTest();
        if (started) {
            m_progressBar.setValue(0);
            auto param = FunctionalTest::getParameters();
            auto duration = param.passive.measuringDuration;
            duration += param.pairingTimeout;
            duration += param.active.measuringDuration;
            m_progressBar.glideTo(99, duration);
        }
        return started;
    });

    m_startButton.addState("Stop", [&]{
        m_worker.stop();
        m_progressBar.pause();
        emit error("[==========] Test stopped by user\n");
        return true;
    });

    layout()->addWidget(&m_startButton);
    layout()->addWidget(&m_progressBar);
}

void TestingWidget::setup(WingChargeBlockManager *units, QCheckView *selector)
{
    m_units = units;
    m_selector = selector;
}

QQueue<WingChargeBlock *> TestingWidget::getUnits()
{
    if (m_selector->isEmpty()) {
        emit error("No units to select from!");
        return QQueue<WingChargeBlock*>();
    }

    QQueue<WingChargeBlock*> queue;
    for (auto& unit: *m_units) {
        for (const auto& select: m_selector->checkedItems()) {
            if (unit.id() == select) {
                queue.push_back(&unit);
                break;
            }
        }
    }

    if (queue.isEmpty()) {
        int index = 0;
        auto target = m_selector->currentIndex().row();
        for (auto& unit: *m_units) {
            if (index >= target) {
                queue.push_back(&unit);
                break;
            }
            ++index;
        }
    }

    return queue;
}

bool TestingWidget::startTest()
{
    if (m_testQueue.empty()) {
        return false;
    }

    auto unit = m_testQueue.first();
    qDebug() << QString("Starting test on [%0]").arg(unit->id());
    m_worker.start(new FunctionalTest(*unit));
    m_currentUnit = unit->id();
    m_testQueue.pop_front();
    return true;
}
