#pragma once
#include <QWidget>
#include "testingworker.h"
#include "../wingchargeblock.h"
#include "qcheckview.h"
#include "qstatebutton.h"
#include "qdynamicbar.h"
#include <QQueue>

class TestingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestingWidget(WingChargeBlockManager *units, QCheckView *selector, QWidget *parent = nullptr);
    void setup(WingChargeBlockManager *units, QCheckView *selector);

signals:
    void output(const QString &message);
    void error(const QString &message);

protected:
    QQueue<WingChargeBlock*> getUnits();
    bool startTest();

private:
    WingChargeBlockManager *m_units;
    QCheckView *m_selector;

    QStateButton m_startButton;
    QDynamicBar m_progressBar;

    TestingWorker m_worker;
    QQueue<WingChargeBlock*> m_testQueue;  // Find this <something> in mainwindow
    int m_currentUnit;

    const int NEW_TEST_DELAY = 500;
};
