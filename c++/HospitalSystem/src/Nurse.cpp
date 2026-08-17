#include "Nurse.h"

Nurse::Nurse(const char* name, int employeeId, int yearsOfExperience)
    : MedicalStaff(name, employeeId), yearsOfExperience(yearsOfExperience)
{
}

bool Nurse::setYearsOfExperience(int yearsOfExperience)
{
    this->yearsOfExperience = yearsOfExperience;
    return true;
}

void Nurse::toOs(std::ostream& os) const
{
    os << "Nurse - ";
    MedicalStaff::toOs(os);
    os << ", Years of Experience: " << yearsOfExperience;
}