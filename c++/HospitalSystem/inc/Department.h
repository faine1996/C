#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <iostream>

class MedicalStaff; // forward declaration — avoids circular include with MedicalStaff.h

class Department
{
private:
    char* name;
    MedicalStaff** staff;
    int logicalSize;
    int physicalSize;

public:
    explicit Department(const char* name);
    Department(const Department& other) = delete;    // deep-copying an array of polymorphic base pointers would require a virtual clone() method, which nothing in this system needs
    Department& operator=(const Department& other) = delete;
    ~Department();

    const char* getName() const
    {
        return name;
    }

    int getStaffCount() const
    {
        return logicalSize;
    }

    MedicalStaff* getStaffAt(int index) const
    {
        return staff[index];
    }

    bool addStaff(MedicalStaff* member);

    friend std::ostream& operator<<(std::ostream& os, const Department& dept);
};

#endif
