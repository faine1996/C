cmake_minimum_required(VERSION 3.16)
project(central_computer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

add_executable(central_computer
    main.cpp
    src/message.cpp
    src/submarine.cpp
    src/researchsubmarine.cpp
    src/serialcomm.cpp
    src/centralcomputer.cpp
    src/combatsubmarine.cpp
    src/menu.cpp
)

add_executable(run_tests
    ${CMAKE_SOURCE_DIR}/tests/test_runner.cpp
    ${CMAKE_SOURCE_DIR}/src/message.cpp
    ${CMAKE_SOURCE_DIR}/src/submarine.cpp
    ${CMAKE_SOURCE_DIR}/src/researchsubmarine.cpp
    ${CMAKE_SOURCE_DIR}/src/serialcomm.cpp
    ${CMAKE_SOURCE_DIR}/src/centralcomputer.cpp
    ${CMAKE_SOURCE_DIR}/src/combatsubmarine.cpp
    ${CMAKE_SOURCE_DIR}/src/menu.cpp
)