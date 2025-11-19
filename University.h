#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include<map>
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
    map<string, vector<string>> courseEnrollments; // courseCode -> list of studentIds

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

	//Existence Checks
    bool hasStudent(const string& id) const;
    bool hasFaculty(const string& id) const;

    vector<string> getAllStudentIDs() const;

    void enrollStudentInCourse(const string& studentId, const string& courseCode);
    vector<string> getStudentsInCourse(const string& courseCode) const;


};
#endif
