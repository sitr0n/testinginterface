cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if (VERSION VERSION_GREATER_EQUAL 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    add_compile_options(-std=c++11)
endif()

find_package(Qt5 REQUIRED Core Widgets)

set(testing_SOURCES
    testinginterface.cpp
    testingtask.cpp
    testingcomposite.cpp
    palm.cpp
    qtestingpanel.cpp
    #testingworker.cpp
)

set(testing_HEADERS
    testinginterface.h
    testingtask.h
    testingcomposite.h
    palm.h
    qtestingpanel.h
    #testingworker.h
)

qt5_wrap_cpp(TESTING_QT_HEADERS_MOC ${testing_HEADERS})
    
add_library (testing ${testing_SOURCES} ${TESTING_QT_HEADERS_MOC})
target_link_libraries(testing
    Qt5::Core
    Qt5::Widgets
)
