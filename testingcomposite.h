#pragma once
#include "testinginterface.h"

class TestingComposite : public TestingInterface
{
public:
    void add(TestingInterface *test);

    void tick();
    bool isFinished() const;

    States state();
    Results results() const;
    double progress();

    int lift() const;
    int weight() const;

private:
    std::vector<TestingInterface*> m_children;
};
