#pragma once
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "testinginterface.h"
#include "palm.h"

class TestingWorker : public QThread
{
    Q_OBJECT
public:
    explicit TestingWorker(QObject *parent = nullptr);
    ~TestingWorker();
    void start(TestingInterface *test);
    void stop();

signals:
    void progress(int percentage, const QString &currentTest);
    void finished(bool approved);
    void output(const QString &message);
    void error(const QString &message);

protected:
    virtual void run();
    void emitHeader(TestingInterface *test);
    int percentCompleted(TestingInterface *test) const;
    TestingInterface::Results evaluate(TestingInterface *test);
    void log(TestingInterface::Results result);
    TestingInterface* awaitNext();
    void unravelThread();

private:
    bool m_quit;
    QMutex m_mutex;
    QWaitCondition m_cond;
    PALM m_log;
    TestingInterface *m_test;
};
