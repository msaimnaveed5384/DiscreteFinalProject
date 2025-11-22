#ifndef FACULTY_H
#define FACULTY_H
#include <string>
using namespace std;
class Faculty {
private:
    string facultyID;
    string name;
    string department;
public:
    //Constructors
    Faculty();
    Faculty(string facultyID, string name, string department);

    //Getter Functions
    string getFacultyID() const;
    string getName() const;
    string getDepartment() const;

    //Setter Functions
    void setName(string newName);
    void setDepartment(string newDept);
};
#endif
