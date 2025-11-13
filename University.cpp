#include"University.h"
#include<iostream>
using namespace std;

//constructors
University::University() 
{
    //nothing
}

//student management
void University::addStudent(const Student& s) 
{
    students.push_back(s);
}

//list students
void University::listStudents() const 
{
    if (students.empty()) 
    {
        cout << "No students found!"<<endl;
        return;
    }

    cout << "Students:"<<endl;
    for (const Student& s : students) 
    {
        cout << "ID: " << s.getId()
             << "\nName: " << s.getName()
             << "\nProgram: " << s.getProgram()
             << "\nSemester: " << s.getSemester()<< endl;
    }
}

//course management
void University::addCourse(const Course& c) 
{
    courses.push_back(c);
}

//list courses
void University::listCourses() const 
{
    if (courses.empty()) 
    {
        cout << "No Courses found!"<<endl;
        return;
    }

    cout << "Courses:"<<endl;
    for (const Course& c : courses) 
    {
        cout << "Code: " << c.getCode()
             << "\nTitle: " << c.getTitle()
             << "\nCredit Hours: " << c.getCreditHours()<< endl;
    }
}

//faculty management
void University::addFaculty(const Faculty& f)
{
    faculty.push_back(f);
}

//list faculty
void University::listFaculty() const 
{
    if (faculty.empty()) {
        cout << "No Faculty found!\n";
        return;
    }

    cout << "Faculty Members:"<<endl;
    for (const Faculty& f : faculty)
    {
        cout << "ID: " << f.getFacultyID()
             << "\nName: " << f.getName()
             << "\nDepartment: " << f.getDepartment()<< endl;
    }
}


//room management
void University::addRoom(const Room& r) 
{
    rooms.push_back(r);
}

//list rooms
void University::listRooms() const
{
    if (rooms.empty()) {
        cout << "No rooms found!" << endl;
        return;
    }

    cout << "Rooms:"<<endl;
    for (const Room& r : rooms)
    {
        cout << "Room: " << r.getRoomNumber()
            << "\nCapacity: " << r.getCapacity()
            << "\nType: " << r.getType()<<endl;
    }
}
