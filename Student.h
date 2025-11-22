#ifndef STUDENT_H
#define STUDENT_H
#include <string>
using namespace std;
class Student {
private:
    string id;
    string name;
    string program;
    int semester;
public:
    //Constructors
    Student();
    Student(string id, string name, string program, int semester);

    //Getter Functions
    string getId() const;
    string getName() const;
    string getProgram() const;
    int getSemester() const;

    //Setter Functions
    void setName(string newName);
    void setProgram(string newProgram);
    void setSemester(int newSemester);
};
#endif