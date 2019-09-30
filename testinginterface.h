#pragma once
#include <QVariantMap>
#include <QMap>
#include <vector>


class TestingInterface
{
public:
    virtual void tick() = 0;
    virtual bool isFinished() const = 0;

    enum State {
        INCOMPLETE,
        PASSED,
        FAILED,
        DISQUALIFIED,
        TIMEOUT,
    };
    typedef QMap<QString, State*> States;
    virtual States state() = 0;
    typedef QVariantMap Results;
    virtual Results results() const = 0;

    virtual int lift() const = 0;
    virtual int weight() const = 0;

};

