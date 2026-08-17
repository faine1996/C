#include "Hospital.h"
#include "Department.h"
#include "Doctor.h"
#include "Nurse.h"
#include <iostream>

void initDepartments(Hospital& hospital);
void initStaff(Hospital& hospital);

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
    hospital.addDepartment(new Department("Cardiology"));
    hospital.addDepartment(new Department("Oncology"));
}

void initStaff(Hospital& hospital)
{
    hospital.addStaff("Surgery", new Doctor("Alice Cohen", 101, "Orthopedics"));
    hospital.addStaff("Surgery", new Doctor("Ben Levi", 102, "Neurosurgery"));
    hospital.addStaff("Surgery", new Nurse("Carol Mizrahi", 103, 5));
    hospital.addStaff("Surgery", new Doctor("Dan Peretz", 104, "Cardiothoracic Surgery"));
    hospital.addStaff("Surgery", new Nurse("Eden Azoulay", 105, 7));

    hospital.addStaff("Internal Medicine", new Doctor("Frida Katz", 106, "Cardiology"));
    hospital.addStaff("Internal Medicine", new Nurse("Gil Barak", 107, 8));

    hospital.addStaff("Pediatrics", new Nurse("Hila Sharon", 108, 3));

    hospital.addStaff("Cardiology", new Doctor("Itai Rosen", 109, "Interventional Cardiology"));

    hospital.addStaff("Oncology", new Nurse("Yael Golan", 110, 6));
}