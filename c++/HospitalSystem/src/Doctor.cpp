#include "Doctor.h"
#include <cstring>

Doctor::Doctor(const char* name, int employeeId, Department* department, const char* specialty)
    : MedicalStaff(name, employeeId, department), specialty(new char[strlen(specialty) + 1])
{
    strcpy(this->specialty, specialty);
}

Doctor::Doctor(const Doctor& other)
    : MedicalStaff(other), specialty(new char[strlen(other.specialty) + 1])
{
    strcpy(specialty, other.specialty);
}

Doctor& Doctor::operator=(const Doctor& other)
{
    if (this != &other)
    {
        MedicalStaff::operator=(other);
        delete[] specialty;
        specialty = new char[strlen(other.specialty) + 1];
        strcpy(specialty, other.specialty);
    }
    return *this;
}

bool Doctor::setSpecialty(const char* specialty)
{
    delete[] this->specialty;
    this->specialty = new char[strlen(specialty) + 1];
    strcpy(this->specialty, specialty);
    return true;
}

void Doctor::toOs(std::ostream& os) const
{
    os << "Doctor - Name: " << getName() << ", Employee ID: " << getEmployeeId() << ", Specialty: " << specialty;
}