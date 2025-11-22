#include "Faculty.h"
using namespace std;

Faculty::Faculty() 
{
    facultyID = "";
    name = "";
    department = "";
}

Faculty::Faculty(string facultyID, string name, string department) 
{
    this->facultyID = facultyID;
    this->name = name;
    this->department = department;
}

// Getter Functions
string Faculty::getFacultyID() const 
{
    return facultyID;
}

string Faculty::getName() const 
{
    return name;
}

string Faculty::getDepartment() const 
{
    return department;
}

// Setter Functions
void Faculty::setName(string newName) 
{
    name = newName;
}

void Faculty::setDepartment(string newDept) 
{
    department = newDept;
}

