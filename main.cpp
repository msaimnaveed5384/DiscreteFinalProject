#include<iostream>
#include<string>
#include"University.h"
using namespace std;

int main() 
{
    University uni;
    int choice;
    do 
    {
        cout <<endl<<"===== UNIVERSITY MANAGEMENT MENU =====\n";
        cout << "1) Add Student\n";
        cout << "2) List Students\n";
        cout << "3) Add Course\n";
        cout << "4) List Courses\n";
        cout << "5) Add Faculty\n";
        cout << "6) List Faculty\n";
        cout << "7) Add Room\n";
        cout << "8) List Rooms\n";
        cout << "0) Exit\n";
        cout << "Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            string id, name, program;
            int semester;
            cout << "Enter student ID: ";
            cin >> id;
            cout << "Enter student name: ";
            cin >> name;
            cout << "Enter program: ";
            cin >> program;
            cout << "Enter semester number: ";
            cin >> semester;

            Student s(id, name, program, semester);
            uni.addStudent(s);
            
            cout << "student added successfully\n";
            break;
        }
        

        case 2:
        {
            uni.listStudents();
            break;
        }

        case 3:
        {
            string code, title;
            int ch;

            cout << "Enter course code: ";
            cin >> code;

            cout << "Enter course title: ";
            cin >> title;

            cout << "Enter credit hours(CH): ";
            cin >> ch;

            Course c(code, title, ch);
            uni.addCourse(c);

            cout << "Course added successfully\n";
            break;
        }

        case 4:
        {
            uni.listCourses();
            break;

        }
        case 5:
        {
            string id, name, dept;

            cout << "Enter faculty ID: ";
            cin >> id;

            cout << "Enter faculty name: ";
            cin >> name;

            cout << "Enter department: ";
            cin >> dept;

            Faculty f(id, name, dept);
            uni.addFaculty(f);

            cout << "Faculty added successfully\n";
            break;
        }

        case 6:
        {
            uni.listFaculty();
            break;

        }
        case 7:
        {
            string roomNo, type;
            int cap;

            cout << "Enter room number: ";
            cin >> roomNo;

            cout << "Enter capacity: ";
            cin >> cap;

            cout << "Enter type (Lecture/Lab): ";
            cin >> type;

            Room r(roomNo, cap, type);
            uni.addRoom(r);

            cout << "Room added successfully\n";
            break;
        }

        case 8:
        {
            uni.listRooms();
            break;
        }
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice\n";
        }
    } while (choice != 0);

    return 0;
}
