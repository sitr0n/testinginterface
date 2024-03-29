#include "testingcomposite.h"
#include <QDebug>

TestingComposite::~TestingComposite()
{
    // TODO: delete children
}

void TestingComposite::add(TestingInterface *test)
{
    m_children.push_back(test);
}

void TestingComposite::tick()
{
    auto next = std::find_if(m_children.begin(), m_children.end(),
    [&](const TestingInterface *test){
        return !test->isFinished();
    });

    if (next != m_children.end()) {
        (*next)->tick();
    }
}

bool TestingComposite::isFinished() const
{
    auto pending = std::find_if(m_children.begin(), m_children.end(),
    [](const TestingInterface *test){
        return !test->isFinished();
    });

    return (pending == m_children.end());
}

TestingInterface::States TestingComposite::state()
{
    States container;
    for (const auto& test : m_children) {
        container.unite(test->state());
    }
    return container;
}


TestingInterface::Results TestingComposite::results() const
{
    Results container;
    for (const auto &test : m_children) {
        if (test->isFinished()) {
            container.unite(test->results());
        }
    }
    return container;
}

double TestingComposite::progress()
{
    if (state().size() < 1) {
        return 100.0;
    }
    return static_cast<double>(lift()*100)/weight();
}

int TestingComposite::lift() const
{
    int lift = 0;
    for (const auto& test : m_children) {
        lift += test->lift();
    }
    return lift;
}

int TestingComposite::weight() const
{
    int weight = 0;
    for (const auto& test : m_children) {
        weight += test->weight();
    }
    return weight;
}

void TestingComposite::reset()
{
    for (auto& test : m_children) {
        test->reset();
    }
}




