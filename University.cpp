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
void University::enrollStudentInCourse(const string& studentId, const string& courseCode) 
{
    // Optional: later you can check if studentId and courseCode actually exist.
    // For now we just record the relationship.

    courseEnrollments[courseCode].push_back(studentId);

    cout << "Student " << studentId << " enrolled in course " << courseCode << " successfully.\n";
}

vector<string> University::getStudentsInCourse(const string& courseCode) const
{
    auto it = courseEnrollments.find(courseCode);
    if (it != courseEnrollments.end())
    {
        return it->second;
    }
    // no enrollment yet
    return vector<string>();
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
//existence checks
bool University::hasStudent(const string& id) const 
{
    for (const Student& s : students) 
    {
        if (s.getId() == id) 
        {
            return true;
        }
    }
    return false;
}

bool University::hasFaculty(const string& id) const 
{
    for (const Faculty& f : faculty)
    {
        if (f.getFacultyID() == id)
        {
            return true;
        }
    }
    return false;
}

vector<string> University::getAllStudentIDs() const 
{
    vector<string> ids;
    for (const Student& s : students) 
    {
        ids.push_back(s.getId());
    }
    return ids;
}

vector<string> University::getAllStudentIds() const {
    vector<string> ids;
    for (const auto& s : students) {
        ids.push_back(s.getId());   // assumes Student has getId()
    }
    return ids;
}

vector<string> University::getAllCourseCodes() const {
    vector<string> codes;
    for (const auto& c : courses) {
        codes.push_back(c.getCode());   // assumes Course has getCode()
    }
    return codes;
}