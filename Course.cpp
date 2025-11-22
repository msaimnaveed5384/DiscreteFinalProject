#include "Course.h"
using namespace std;

// Default Constructor
Course::Course() 
{
    code = "";
    title = "";
    creditHours = 0;
}

// Parameterized Constructor
Course::Course(string code, string title, int creditHours) 
{
    this->code = code;
    this->title = title;
    this->creditHours = creditHours;
}

// Getter Functions
string Course::getCode() const 
{
    return code;
}

string Course::getTitle() const 
{
    return title;
}

int Course::getCreditHours() const 
{
    return creditHours;
}

// Setter Functions
void Course::setTitle(string newTitle) 
{
    title = newTitle;
}

void Course::setCreditHours(int newCH) 
{
    creditHours = newCH;
}
