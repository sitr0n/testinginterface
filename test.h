#pragma once

#include <vector>
#include <QMap>
#include <map>
#include <functional>
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <QDebug>

//class Test
//{
//public:
//    //typedef QMap<QString, QVariant> Results;
//    typedef std::map<QString, QVariant> Results;
//    Test();
//    Test(QString name);
//    ~Test();
//    void add(Test *test);
//    void add(QString test);
//    void find(QString test); // Alternatively;
//    Test *at(QString section);
//    void setCondition(std::function<bool()> cond);
//    void setMeasuring(std::function<Results()> meas);
//    void setCriterion(std::function<bool(Results)> comp);

//    enum TaskState {
//        INCOMPLETE,
//        PASSED,
//        FAILED
//    };
//    typedef std::vector<TaskState*> Progress;
//    virtual Progress progress();
//    virtual const Results &results() const;
//    virtual Results tick();
//    bool isDone();

//protected:
//    void addResult(const QString &name, const QVariant &score);
//    bool isTerminal() const;
//    Results process();

//    std::vector<Test*> m_childNodes;
//    Results m_results;

//    std::function<bool()> m_conditional;
//    std::function<Results()> measure;
//    std::function<bool(Results)> evaluate;
//    TaskState m_state;
//    QString m_name;
//    bool m_initialized;
//};

class TestingInterface
{
public:
    enum class State {
        INCOMPLETE,
        PASSED,
        FAILED
    };
    typedef std::vector<State*> States;
    virtual States state() = 0; // Should contain references to reduce computing as this function will be called frequently
    struct Percentage {
        Percentage() {
            m_value = 0;
        }
        double operator%(const Percentage &p) {
            return p.m_value;
        }
        template<typename T>
        void operator=(const T other) {
            m_value = other;
            if (m_value > MAX) {
                m_value = MAX;
            }
            if (m_value < MIN) {
                m_value = MIN;
            }
        }
        double m_value;
        const double MAX = 100.0;
        const double MIN = 0.0;
    };
    virtual Percentage progress() = 0;
    virtual bool isFinished() = 0;
    typedef QVariantMap Results;
    virtual Results results() = 0;
    virtual void tick() = 0;
};

class TestingTask : public TestingInterface
{
public:
    TestingTask(const QString &name) : m_name(name), m_state(State::INCOMPLETE) {}
    States state() {
        States container;
        container.push_back(&m_state);
        return container;
    }
    Percentage progress() {
        return Percentage();
    }
    bool isFinished() {
        return (m_state != State::INCOMPLETE);
    }
    Results results() {
        qDebug() << QString("Have [%0] this many").arg(m_results.size());
        return m_results;
    }
    void tick() {
        if (m_conditional()) {
            m_results = m_measure();
            qDebug() << QString("[%0] !!!!").arg(m_measure().size());
            if (!m_results.empty()) {
                m_state = m_evaluate(m_results) ? State::PASSED : State::FAILED;
            }
        } else {
            // remark conditions lost
        }
    }

    void setCondition(std::function<bool()> cond) {
        m_conditional = cond;
    }
    void setMeasuring(std::function<Results()> meas) {
        m_measure = meas;
    }
    void setEvaluation(std::function<bool(Results)> eval) {
        m_evaluate = eval;
    }

private:
    QString m_name;
    State m_state;
    Results m_results;

    std::function<bool()> m_conditional;
    std::function<Results()> m_measure;
    std::function<bool(Results)> m_evaluate;
};

class TestingComposite : public TestingInterface
{
public:
    TestingComposite() {}
    void add(TestingInterface *test) {
        m_children.push_back(test);
    }
    States state() {
        States container;
        for (auto& test : m_children) {
            container.insert(container.end(), test->state().begin(), test->state().end());
        }
        return container;
    }
    Percentage progress() {
        return Percentage();
    }
    bool isFinished() {
        auto pending = std::find_if(m_children.begin(), m_children.end(),
        [](TestingInterface *test){
            return !(*test).isFinished();
        });

        return (pending == m_children.end());
    }
    Results results() {
        Results container;
        for (auto &test : m_children) {
            if (test->isFinished()) {
                container.unite(test->results());
            }
        }
        return container;
    }
    void tick() {
        auto next = std::find_if(m_children.begin(), m_children.end(),
        [&](TestingInterface *test){
            return !test->isFinished();
        });

        if (next != m_children.end()) {
            (*next)->tick();
        }
    }

private:
    std::vector<TestingInterface*> m_children;
};
