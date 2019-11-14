#include "qtestingpanel.h"
#include <QPushButton>
#include <QHBoxLayout>

QTestingPanel::QTestingPanel(QWidget *parent)
    : QWidget(parent)
{
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(new QPushButton("Test", this));
}

void QTestingPanel::addTest(TestingInterface &test)
{
    m_tests.push_back(&test);
}
