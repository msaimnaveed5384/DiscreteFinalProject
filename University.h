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
    map<string, vector<string>> facultyCourses;      // facultyId -> list of courseCodes
    map<string, string> courseRooms;        // courseCode -> roomId
    vector<pair<string, string>> courseConflicts;    // (courseA, courseB) pairs
public:
    //Constructors
    University();

    vector<string> getAllStudentIds() const;
    vector<string> getAllCourseCodes() const;
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
    bool hasCourse(const string& id) const;
    bool hasRoom(const string& id) const;

    vector<string> getAllStudentIDs() const;

    void enrollStudentInCourse(const string& studentId, const string& courseCode);
    vector<string> getStudentsInCourse(const string& courseCode) const;

    // Module 6 relation helpers
    void assignFacultyToCourse(const string& facultyId, const string& courseCode);
    void assignRoomToCourse(const string& courseCode, const string& roomId);
    void addCourseConflict(const string& courseA, const string& courseB);

    vector<pair<string, string>> getFacultyCoursePairs() const;
    vector<pair<string, string>> getCourseRoomPairs() const;
    vector<pair<string, string>> getCourseConflictPairs() const;
    //sdjfkllskf
    vector<string> getAllFacultyIds() const;
};
#endif