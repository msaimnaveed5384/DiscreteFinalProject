#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include<vector>
#include"Student.h"
#include"Course.h"
#include"Faculty.h"
#include"Room.h"
using namespace std;

class University {

private:
    vector<Student> students;
    vector<Course> courses;
    vector<Faculty> faculty;
    vector<Room> rooms;
public:
	//Constructors
    University();

	//Student Management
    void addStudent(const Student& s);
    void listStudents() const;

	//Course Management
    void addCourse(const Course& c);
    void listCourses() const;

	//Faculty Management
    void addFaculty(const Faculty& f);
    void listFaculty() const;

	//Room Management
    void addRoom(const Room& r);
    void listRooms() const;
};
#endif
