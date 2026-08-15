#include "Hospital.h"
#include "Department.h"
#include <cstring>

Hospital::Hospital()
    : departments(new Department*[2]), logicalSize(0), physicalSize(2)
{
}

Hospital::~Hospital()
{
    for (int i = 0; i < logicalSize; ++i)
    {
        delete departments[i];
    }
    delete[] departments;
}

bool Hospital::addDepartment(Department* dept)
{
    if (logicalSize == physicalSize)
    {
        physicalSize *= 2;
        Department** newDepartments = new Department*[physicalSize];
        for (int i = 0; i < logicalSize; ++i)
        {
            newDepartments[i] = departments[i];
        }
        delete[] departments;
        departments = newDepartments;
    }
    departments[logicalSize] = dept;
    logicalSize++;
    return true;
}

bool Hospital::addStaff(const char* departmentName, MedicalStaff* member)
{
    for (int i = 0; i < logicalSize; ++i)
    {
        if (0 == strcmp(departments[i]->getName(), departmentName))
        {
            return departments[i]->addStaff(member);
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Hospital& hospital)
{
    for (int i = 0; i < hospital.logicalSize; ++i)
    {
        os << *hospital.departments[i];
    }
    return os;
}