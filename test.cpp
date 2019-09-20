#include "test.h"
#include <QDebug>

//Test::Test()
//    : measure([](){return Results();})
//    , evaluate([](Results){return false;})
//    , m_state(Test::INCOMPLETE)
//    , m_initialized(false)
//{

//}

//Test::Test(QString name)
//    : measure([](){return Results();})
//    , evaluate([](Results){return false;})
//    , m_state(Test::INCOMPLETE)
//    , m_name(name)
//    , m_initialized(false)
//{

//}

//Test::~Test()
//{
//    m_results.clear();
//}

//void Test::add(Test *test)
//{
//    m_childNodes.push_back(test);
//}

//void Test::add(QString test)
//{
//    m_childNodes.push_back(new Test(test));
//}

//void Test::find(QString test)
//{

//}

//Test *Test::at(QString section)
//{

//}

///* Terminal nodes should carry task
// * status, while inner nodes with
// * branches  carry reference wrappers? */
//Test::Progress Test::progress()
//{
//    Test::Progress container;
//    for (const auto& branch: m_childNodes) {
//        container.insert(container.end(), branch->progress().begin(), branch->progress().end());
//    }

//    if (this->isTerminal()) {
//        container.push_back(&m_state);
//    }
//    return container;
//}

//const Test::Results &Test::results() const
//{
//    return m_results;
//}



//Test::Results Test::tick()
//{
//    if (this->isTerminal()) {
//        return process();
//    }

//    auto nextBranch = std::find_if(m_childNodes.begin(), m_childNodes.end(),
//    [&](Test *task){
//        return !task->isDone();
//    });

//    if (nextBranch != m_childNodes.end()) {
//        auto results = (*nextBranch)->tick();
//        if (!results.empty()) {
//            m_results.insert(results.begin(), results.end());
//            if (this->isDone()) {
//                return m_results;
//            }
//        }
//    }
//    return Results();
//}

//bool Test::isDone()
//{
//    auto pending = std::find_if(progress().begin(), progress().end(),
//    [=](const TaskState *state){
//        return (*state == Test::INCOMPLETE);
//    });

//    return (pending == progress().end());
//}

//void Test::addResult(const QString &name, const QVariant &score)
//{
//    m_results[name] = QVariant::fromValue(score);
//}

//bool Test::isTerminal() const
//{
//    return m_childNodes.empty();
//}

//Test::Results Test::process()
//{
//    auto results = measure();
//    if (!results.empty()) {
//        //m_progress.front() = evaluate(results) ? Test::PASSED : Test::FAILED;
//    }
//    return results;
//}

//void Test::setCondition(std::function<bool()> cond)
//{
//    m_conditional = cond;
//}

//void Test::setMeasuring(std::function<Test::Results ()> meas)
//{
//    measure = meas;
//}

//void Test::setCriterion(std::function<bool (Test::Results)> comp)
//{
//    evaluate = comp;
//}
