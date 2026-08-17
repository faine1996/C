#ifndef DOCTOR_H
#define DOCTOR_H

#include "MedicalStaff.h"

class Doctor : public MedicalStaff
{
private:
    char* specialty;

public:
    Doctor(const char* name, int employeeId, const char* specialty);
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
    Doctor(const Doctor& other);              // protected: prevents object slicing through a base-class handle; derived classes still reach it, outside code cannot
    Doctor& operator=(const Doctor& other);    // protected: same reasoning as the copy constructor

    virtual void toOs(std::ostream& os) const override;
};

#endif