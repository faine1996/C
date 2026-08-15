#include "Nurse.h"

Nurse::Nurse(const char* name, int employeeId, Department* department, int yearsOfExperience)
    : MedicalStaff(name, employeeId, department), yearsOfExperience(yearsOfExperience)
{
}

bool Nurse::setYearsOfExperience(int yearsOfExperience)
{
    this->yearsOfExperience = yearsOfExperience;
    return true;
}

void Nurse::toOs(std::ostream& os) const
{
    os << "Nurse - Name: " << getName() << ", Employee ID: " << getEmployeeId() << ", Years of Experience: " << yearsOfExperience;
}
