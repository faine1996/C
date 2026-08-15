#ifndef MEDICALSTAFF_H
#define MEDICALSTAFF_H

#include <iostream>

class Department; // forward declaration — avoids circular include with Department.h

class MedicalStaff
{
protected:
    char* name;
    int employeeId;
    Department* department; // NON-OWNING observer — must never be deleted here; points to the department this staff member currently works in

public:
    MedicalStaff(const char* name, int employeeId, Department* department);
    MedicalStaff(const MedicalStaff& other);
    MedicalStaff& operator=(const MedicalStaff& other);
    virtual ~MedicalStaff()
    {
        delete[] name;
    }

    const char* getName() const
    {
        return name;
    }

    int getEmployeeId() const
    {
        return employeeId;
    }

    Department* getDepartment() const
    {
        return department;
    }

    bool setName(const char* name);
    bool setEmployeeId(int employeeId);
    bool setDepartment(Department* department);

    friend std::ostream& operator<<(std::ostream& os, const MedicalStaff& staff);

protected:
    virtual void toOs(std::ostream& os) const = 0;
};

#endif
