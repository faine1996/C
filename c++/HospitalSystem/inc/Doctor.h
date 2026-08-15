#ifndef DOCTOR_H
#define DOCTOR_H

#include "MedicalStaff.h"

class Doctor : public MedicalStaff
{
private:
    char* specialty;

public:
    Doctor(const char* name, int employeeId, Department* department, const char* specialty);
    Doctor(const Doctor& other);
    Doctor& operator=(const Doctor& other);
    virtual ~Doctor()
    {
        delete[] specialty;
    }

    const char* getSpecialty() const
    {
        return specialty;
    }

    bool setSpecialty(const char* specialty);

protected:
    virtual void toOs(std::ostream& os) const override;
};

#endif