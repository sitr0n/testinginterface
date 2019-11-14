#pragma once

#include <QWidget>
#include <testingfactory.h>
#include "testingworker.h"
#include <qstatebutton.h>
#include <QComboBox>
#include <qdynamicbar.h>

/* TODO:
 * Indicate which unit has been tested ( if possible )? Otherwise make testing framework more verbose
 * Worker gets too coupled by having to add PALM columns. (Could inject a worker via base pointer interface like the factory...)
 */

class QTesting : public QWidget
{
    Q_OBJECT
public:
    explicit QTesting(TestingFactory &factory, QWidget *parent = nullptr);

signals:
    void output(const QString &message);
    void error(const QString &message);

protected:
    void setupLayout();
    void stop();

private:
    TestingFactory &m_factory;
    TestingWorker m_worker;

    QStateButton m_button;
    QComboBox m_selector;
    QDynamicBar m_progress;
};
