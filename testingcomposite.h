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

    double operator%(const TestingComposite &p) {
        auto completed = std::count_if(state().begin(), state().end(),
        [](State *state){
            return (*state != INCOMPLETE);
        });
        return static_cast<double>(completed*100)/state().size();
    }

private:
    std::vector<TestingInterface*> m_children;
};
