#include "Department.h"
#include "MedicalStaff.h"
#include <cstring>

Department::Department(const char* name)
    : name(new char[strlen(name) + 1]), staff(new MedicalStaff*[2]), logicalSize(0), physicalSize(2)
{
    strcpy(this->name, name);
}

Department::~Department()
{
    for (int i = 0; i < logicalSize; ++i)
    {
        delete staff[i];
    }
    delete[] staff;
    delete[] name;
}

bool Department::addStaff(MedicalStaff* member)
{
    if (logicalSize == physicalSize)
    {
        physicalSize *= 2;
        MedicalStaff** newStaff = new MedicalStaff*[physicalSize];
        for (int i = 0; i < logicalSize; ++i)
        {
            newStaff[i] = staff[i];
        }
        delete[] staff;
        staff = newStaff;
    }
    staff[logicalSize] = member;
    logicalSize++;
    return true;
}

std::ostream& operator<<(std::ostream& os, const Department& dept)
{
    os << "Department: " << dept.name << std::endl;
    for (int i = 0; i < dept.logicalSize; ++i)
    {
        os << *dept.staff[i] << std::endl;
    }
    return os;
}