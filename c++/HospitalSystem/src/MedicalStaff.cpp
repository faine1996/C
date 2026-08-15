#include "MedicalStaff.h"
#include <cstring>

MedicalStaff::MedicalStaff(const char* name, int employeeId, Department* department)
    : name(new char[strlen(name) + 1]), employeeId(employeeId), department(department)
{
    strcpy(this->name, name);
}

MedicalStaff::MedicalStaff(const MedicalStaff& other)
    : name(new char[strlen(other.name) + 1]), employeeId(other.employeeId), department(other.department)
{
    strcpy(name, other.name);
}

MedicalStaff& MedicalStaff::operator=(const MedicalStaff& other)
{
    if (this != &other)
    {
        delete[] name;
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        employeeId = other.employeeId;
        department = other.department;
    }
    return *this;
}

bool MedicalStaff::setName(const char* name)
{
    delete[] this->name;
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
    return true;
}

bool MedicalStaff::setEmployeeId(int employeeId)
{
    this->employeeId = employeeId;
    return true;
}

bool MedicalStaff::setDepartment(Department* department)
{
    this->department = department;
    return true;
}

std::ostream& operator<<(std::ostream& os, const MedicalStaff& staff)
{
    staff.toOs(os);
    return os;
}