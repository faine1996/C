#include "Hospital.h"
#include "Department.h"
#include "Doctor.h"
#include "Nurse.h"
#include <iostream>
#include <cstring>

void initDepartments(Hospital& hospital);
void initStaff(Hospital& hospital);
Department* findDepartment(Hospital& hospital, const char* name);

int main()
{
    Hospital hospital;

    initDepartments(hospital);
    initStaff(hospital);

    std::cout << hospital;

    return 0;
}

void initDepartments(Hospital& hospital)
{
    hospital.addDepartment(new Department("Surgery"));
    hospital.addDepartment(new Department("Internal Medicine"));
    hospital.addDepartment(new Department("Pediatrics"));
}

void initStaff(Hospital& hospital)
{
    Department* surgery = findDepartment(hospital, "Surgery");
    Department* internalMedicine = findDepartment(hospital, "Internal Medicine");
    Department* pediatrics = findDepartment(hospital, "Pediatrics");

    hospital.addStaff("Surgery", new Doctor("Alice Cohen", 101, surgery, "Orthopedics"));
    hospital.addStaff("Surgery", new Doctor("Ben Levi", 102, surgery, "Neurosurgery"));
    hospital.addStaff("Surgery", new Nurse("Carol Mizrahi", 103, surgery, 5));

    hospital.addStaff("Internal Medicine", new Doctor("David Katz", 104, internalMedicine, "Cardiology"));
    hospital.addStaff("Internal Medicine", new Nurse("Elior Peretz", 105, internalMedicine, 8));

    hospital.addStaff("Pediatrics", new Nurse("Faye Azoulay", 106, pediatrics, 3));
}

Department* findDepartment(Hospital& hospital, const char* name)
{
    for (int i = 0; i < hospital.getDepartmentCount(); i++)
    {
        if (strcmp(hospital.getDepartmentAt(i)->getName(), name) == 0)
        {
            return hospital.getDepartmentAt(i);
        }
    }
    return nullptr;
}