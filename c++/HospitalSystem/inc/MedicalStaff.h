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
    MedicalStaff(const char* name, int employeeId);
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

    friend std::ostream& operator<<(std::ostream& os, const MedicalStaff& staff);
    friend class Department; // Department is the owner and the only entity permitted to set the back-pointer via the private setDepartment below, so the pointer cannot go stale or disagree with the array that owns the object

protected:
    MedicalStaff(const MedicalStaff& other);              // protected: prevents object slicing through a base-class handle; derived classes still reach it, outside code cannot
    MedicalStaff& operator=(const MedicalStaff& other);    // protected: same reasoning as the copy constructor

    virtual void toOs(std::ostream& os) const = 0;

private:
    bool setDepartment(Department* department);
};

#endif
