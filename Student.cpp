#include "Student.h"
using namespace std;

// Default Constructor
Student::Student() 
{
    id = "";
    name = "";
    program = "";
    semester = 1;
}

// Parameterized Constructor
Student::Student(string id, string name, string program, int semester) 
{
    this->id = id;
    this->name = name;
    this->program = program;
    this->semester = semester;
}

// Getter Functions
string Student::getId() const 
{
    return id;
}

string Student::getName() const 
{
    return name;
}

string Student::getProgram() const 
{
    return program;
}

int Student::getSemester() const 
{
    return semester;
}

// Setter Functions
void Student::setName(string newName) 
{
    name = newName;
}

void Student::setProgram(string newProgram) 
{
    program = newProgram;
}

void Student::setSemester(int newSemester) 
{
    semester = newSemester;
}
