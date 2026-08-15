cmake_minimum_required(VERSION 3.10)
project(HospitalSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HospitalSystem
    src/main.cpp
    src/MedicalStaff.cpp
    src/Doctor.cpp
    src/Nurse.cpp
    src/Department.cpp
    src/Hospital.cpp
)

target_include_directories(HospitalSystem PRIVATE inc)