#pragma once
#include <QWidget>
#include <QProgressBar>
#include <QQueue>
#include "testinginterface.h"

class QTestingPanel : public QWidget
{
public:
    explicit QTestingPanel(QWidget *parent = nullptr);
    void addTest(TestingInterface &test);

protected:

private:
    QQueue<TestingInterface*> m_tests;

    QProgressBar m_progress;
};
