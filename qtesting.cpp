#include "qtesting.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

QTesting::QTesting(TestingFactory &factory, QWidget *parent)
    : QWidget(parent)
    , m_factory(factory)
{
    setupLayout();
    connect(&m_worker, &TestingWorker::output, this, &QTesting::output);
    connect(&m_worker, &TestingWorker::error, this, &QTesting::error);
    m_button.addState("Start", [=]{
        auto userChoice = m_selector.itemText(m_selector.currentIndex());
        auto test = m_factory.createTest(userChoice);
        m_worker.start(&(*test));
        m_selector.setEnabled(false);
        return true;
    });
    m_button.addState("Stop", [=]{
        stop();
        return true;
    });
    m_selector.addItems(m_factory.availableTests());
}

void QTesting::setupLayout()
{
    auto inputWidget = new QWidget(this);
    inputWidget->setLayout(new QHBoxLayout());
    inputWidget->layout()->addWidget(&m_button);
    inputWidget->layout()->addWidget(&m_selector);

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(inputWidget);
    layout()->addWidget(&m_progress);
}

void QTesting::stop()
{
    m_worker.stop();
    m_selector.setEnabled(true);
}
