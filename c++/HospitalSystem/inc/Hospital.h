#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <iostream>

class Department;    // forward declaration — real include only needed in Hospital.cpp
class MedicalStaff;  // forward declaration — needed for addStaff's parameter type

class Hospital
{
private:
    Department** departments;
    int logicalSize;
    int physicalSize;

public:
    Hospital();
    Hospital(const Hospital& other) = delete;    // deep-copying an array of Departments (which themselves own polymorphic MedicalStaff pointers) has no use in this system and would require a full recursive clone
    Hospital& operator=(const Hospital& other) = delete;
    ~Hospital();

    int getDepartmentCount() const
    {
        return logicalSize;
    }

    Department* getDepartmentAt(int index) const
    {
        return departments[index];
    }

    bool addDepartment(Department* dept);

    // Takes ownership of member unconditionally: on success it belongs to the matched department;
    // on failure (no department named departmentName) it is deleted before returning false.
    // The caller must never delete or reuse the pointer after this call, regardless of outcome.
    bool addStaff(const char* departmentName, MedicalStaff* member);

    friend std::ostream& operator<<(std::ostream& os, const Hospital& hospital);
};

#endif