#include<iostream>
#include<string>
#include"University.h"
#include"Scheduler.h"
#include"GroupManager.h"
#include"PrerequisiteChecker.h"

using namespace std;

int main() 
{
    University uni;
    Scheduler scheduler;
    GroupManager groupManager;
    int choice;
    do 
    {
        cout <<"\n===== UNIVERSITY MANAGEMENT MENU =====\n";
        cout << "1) Add Student\n";
        cout << "2) List Students\n";
        cout << "3) Add Course\n";
        cout << "4) List Courses\n";
        cout << "5) Add Faculty\n";
        cout << "6) List Faculty\n";
        cout << "7) Add Room\n";
        cout << "8) List Rooms\n";
        cout << "9) Add course and prerequisite (Scheduler)\n";
        cout << "10) Generate course schedule\n";
        cout << "11) Create a group\n";
        cout << "12) Add student to a group\n";
        cout << "13) Assign faculty to a group\n";
        cout << "14) List groups\n";
        cout << "15) Generate all student groups of size k (combinations)\n";
        cout << "16) Verify prerequisite chain using induction\n";

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

            // Keep schedulers course list in sync with university
            scheduler.addCourse(code);

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
        case 9:
        {
            string courseCode, prereqCode;

            cout << "Enter course code: ";
            cin >> courseCode;

            cout << "Enter prerequisite course code: ";
            cin >> prereqCode;

            //We assume both courses we already added via option 3
            scheduler.addPrerequisite(courseCode, prereqCode);

            cout << "Prerequisite added successfully.\n";
            break;
        }

        case 10:
        {
            vector<string> schedule = scheduler.generateSchedule();
            scheduler.printSchedule(schedule);
            break;
        }
        case 11:
        {
            int gid;
            cout << "Enter group ID: ";
            cin >> gid;
            groupManager.createGroup(gid);
            break;
		}
        case 12:
        {
            int gid;
            string sid;
            cout << "Enter group ID: ";
            cin >> gid;
            cout << "Enter student ID to add: ";
            cin >> sid;

            if (!uni.hasStudent(sid)) {
                cout << "Student with ID " << sid << " does not exist in the university\n";
                break;
            }

            groupManager.addStudentToGroup(gid, sid);
        }
        break;

        case 13:
        {
            int gid;
            string fid;
            cout << "Enter group ID: ";
            cin >> gid;
            cout << "Enter faculty ID to assign: ";
            cin >> fid;

            if (!uni.hasFaculty(fid)) {
                cout << "Faculty with ID " << fid << " does not exist in the university.\n";
                break;
            }

            groupManager.assignFacultyToGroup(gid, fid);
        }
        break;


        case 14:
        {
            groupManager.listGroups();
            break;
        }
        case 15:
        {
            int k;
            cout << "Enter group size k: ";
            cin >> k;

            vector<string> ids = uni.getAllStudentIDs();
            groupManager.generateStudentCombinations(ids, k);
            break;
        }

        case 16:
        {
            string baseCourse, targetCourse;
            cout << "Enter base course code (e.g., IntroCS): ";
            cin >> baseCourse;
            cout << "Enter target (advanced) course code (e.g., AdvCS): ";
            cin >> targetCourse;

            map<string, vector<string>> prereqMap = scheduler.getPrerequisiteMap();
            if (prereqMap.empty()) 
            {
                cout << "No prerequisite data available. "
                    << "First add course prerequisites using option 9.\n";
                break;
            }

            PrerequisiteChecker checker(prereqMap);
            checker.proveByInduction(baseCourse, targetCourse);
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
