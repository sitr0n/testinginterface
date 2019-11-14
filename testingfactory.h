#pragma once

#include "testinginterface.h"
#include <memory>
#include <QStringList>

class TestingFactory
{
public:
    TestingFactory() = default;
    typedef std::unique_ptr<TestingInterface> Test;
    virtual Test createTest(const QString &description) = 0;
    virtual QStringList availableTests() const = 0;

};
