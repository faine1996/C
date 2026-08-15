#ifndef NURSE_H
#define NURSE_H

#include "MedicalStaff.h"

class Nurse : public MedicalStaff
{
private:
    int yearsOfExperience;

public:
    Nurse(const char* name, int employeeId, Department* department, int yearsOfExperience);

    int getYearsOfExperience() const
    {
        return yearsOfExperience;
    }

    bool setYearsOfExperience(int yearsOfExperience);

protected:
    virtual void toOs(std::ostream& os) const override;
};

#endif