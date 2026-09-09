cmake_minimum_required(VERSION 3.16)
project(ground_station CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)
include_directories(${CMAKE_SOURCE_DIR}/../shared/tlv/inc)

set(SHARED_TLV_SOURCES
    ${CMAKE_SOURCE_DIR}/../shared/tlv/src/value_blocks.cpp
    ${CMAKE_SOURCE_DIR}/../shared/tlv/src/tcp_framer.cpp
)

add_executable(ground_station
    main.cpp
    src/tcpclient.cpp
    src/groundstation.cpp
    src/menu.cpp
    ${SHARED_TLV_SOURCES}
)