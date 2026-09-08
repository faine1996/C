cmake_minimum_required(VERSION 3.16)
project(central_computer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(inc)
include_directories(${CMAKE_SOURCE_DIR}/../shared/tlv/inc)

set(SHARED_TLV_SOURCES
    ${CMAKE_SOURCE_DIR}/../shared/tlv/src/value_blocks.cpp
    ${CMAKE_SOURCE_DIR}/../shared/tlv/src/uart_framer.cpp
    ${CMAKE_SOURCE_DIR}/../shared/tlv/src/tcp_framer.cpp
)

add_executable(central_computer
    main.cpp
    src/message.cpp
    src/submarine.cpp
    src/researchsubmarine.cpp
    src/serialcomm.cpp
    src/socketcomm.cpp
    src/centralcomputer.cpp
    src/combatsubmarine.cpp
    src/menu.cpp
    src/groundstationlink.cpp
    ${SHARED_TLV_SOURCES}
)

target_link_libraries(central_computer PRIVATE Threads::Threads)

add_executable(run_tests
    ${CMAKE_SOURCE_DIR}/tests/test_runner.cpp
    ${CMAKE_SOURCE_DIR}/src/message.cpp
    ${CMAKE_SOURCE_DIR}/src/submarine.cpp
    ${CMAKE_SOURCE_DIR}/src/researchsubmarine.cpp
    ${CMAKE_SOURCE_DIR}/src/serialcomm.cpp
    ${CMAKE_SOURCE_DIR}/src/socketcomm.cpp
    ${CMAKE_SOURCE_DIR}/src/centralcomputer.cpp
    ${CMAKE_SOURCE_DIR}/src/combatsubmarine.cpp
    ${CMAKE_SOURCE_DIR}/src/menu.cpp
    ${CMAKE_SOURCE_DIR}/src/groundstationlink.cpp
    ${SHARED_TLV_SOURCES}
)

target_link_libraries(run_tests PRIVATE Threads::Threads)
