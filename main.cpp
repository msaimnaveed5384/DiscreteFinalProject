#include<iostream>
#include<string>
#include"University.h"
#include"Scheduler.h"
#include"GroupManager.h"
#include"PrerequisiteChecker.h"
#include"SetOperations.h"
#include "Relations.h"
#include "InferenceEngine.h"
using namespace std;


void loadFactsFromUniversity(const University& uni,
    const Scheduler& scheduler,
    InferenceEngine& engine) {
    // 1) Faculty → Course
    vector<pair<string, string>> fcPairs = uni.getFacultyCoursePairs();
    for (const auto& p : fcPairs) {
        engine.addFact(p.first + "teaches" + p.second);
    }

    // 2) Course → Room
    vector<pair<string, string>> crPairs = uni.getCourseRoomPairs();
    for (const auto& p : crPairs) {
        engine.addFact(p.first + "in" + p.second);
    }

    // 3) Course prerequisites as facts: "Course_requires_Prereq"
    vector<pair<string, string>> prereqPairs = scheduler.getAllPrerequisitePairs();
    for (const auto& p : prereqPairs) {
        const string& course = p.first;
        const string& prereq = p.second;
        engine.addFact(course + "requires" + prereq);
    }
}




int main() 
{
    University uni;
    InferenceEngine logicEngine;
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
        cout << "17) Enroll a student in a course\n";
        cout << "18) Analyze students across two courses (set operations)\n";
        cout << "19) Generate all possible student groups (power set) for a course\n";
        cout << "20) Analyze Student-Course relation properties (Relations Module)\n";
        cout << "21) Assign faculty to a course\n";
        cout << "22) Assign room to a course\n";
        cout << "23) Add conflict between two courses\n";
        cout << "24) Analyze Faculty Course relation\n";
        cout << "25) Analyze Course Room relation\n";   // ← missing earlier
        cout << "26) Detect indirect student conflicts (relation composition)\n";
        cout << "27) Add teaching-room policy rule (Logic Engine)\n";
        cout << "28) Run inference engine on current data\n";
        cout << "29) Show all logic facts and rules\n";
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
        case 17:
        {
            string studentId, courseCode;

            cout << "Enter student ID: ";
            cin >> studentId;

            cout << "Enter course code: ";
            cin >> courseCode;

            uni.enrollStudentInCourse(studentId, courseCode);
            break;
        }

        case 18:
        {
            string courseA, courseB;

            cout << "Enter first course code (e.g. CS101): ";
            cin >> courseA;

            cout << "Enter second course code (e.g. MATH101): ";
            cin >> courseB;

            // Get the enrolled student IDs for each course from University
            vector<string> studentsA = uni.getStudentsInCourse(courseA);
            vector<string> studentsB = uni.getStudentsInCourse(courseB);

            cout << "\nStudents in " << courseA << ":\n";
            SetOperations::printSet(studentsA, "S(" + courseA + ")");

            cout << "Students in " << courseB << ":\n";
            SetOperations::printSet(studentsB, "S(" + courseB + ")");

            // Now apply proper set operations
            vector<string> both = SetOperations::setIntersection(studentsA, studentsB);
            vector<string> either = SetOperations::setUnion(studentsA, studentsB);
            vector<string> onlyA = SetOperations::setDifference(studentsA, studentsB);
            vector<string> onlyB = SetOperations::setDifference(studentsB, studentsA);

            cout << "\nSet analysis:\n";
            SetOperations::printSet(both, "Students in both " + courseA + " and " + courseB);
            SetOperations::printSet(either, "Students in at least one of the two courses");
            SetOperations::printSet(onlyA, "Students only in " + courseA);
            SetOperations::printSet(onlyB, "Students only in " + courseB);

            // Dry run mapping: "Find students enrolled in both CS101 and Math101"
            cout << "\nDry run example completed using real data.\n\n";

            break;
        }

        case 19:
        {
            string courseCode;
            cout << "Enter course code to generate all possible student groups: ";
            cin >> courseCode;

            // Get enrolled students for this course
            vector<string> students = uni.getStudentsInCourse(courseCode);

            if (students.empty()) {
                cout << "No students enrolled in course " << courseCode << ".\n\n";
                break;
            }

            cout << "\nStudents enrolled in " << courseCode << ":\n";
            SetOperations::printSet(students, "S(" + courseCode + ")");

            // To avoid crazy output, you can gently warn if too many students
            if (students.size() > 15)
            {
                cout << "\nWarning: More than 15 students. "
                    << "Power set size 2^n will be very large.\n\n";
                break;
            }

            vector<vector<string>> allGroups = SetOperations::powerSet(students);

            cout << "\nAll possible student groups for course " << courseCode << " (power set):\n";
            for (int i = 0; i < allGroups.size();i++) 
            {
                cout << "Group " << i << ": { ";
                for (int j = 0; j < allGroups[i].size(); j++)
                {
                    cout << allGroups[i][j];
                    if (j + 1 < allGroups[i].size()) 
                    {
                        cout << ", ";
                    }
                }
                cout << " }\n";
            }

            cout << "\nTotal number of possible groups (2^n) = " << allGroups.size() << "\n\n";
            break;
        }
        case 20:
        {
            RelationsModule::analyzeStudentCourseRelation(uni);
            break;
        }
        case 21:
        {
            string facultyId, courseCode;
            cout << "Enter faculty ID: ";
            cin >> facultyId;
            cout << "Enter course code: ";
            cin >> courseCode;
            uni.assignFacultyToCourse(facultyId, courseCode);
            break;
        }

        case 22:
        {
            string courseCode, roomId;
            cout << "Enter course code: ";
            cin >> courseCode;
            cout << "Enter room id or name: ";
            cin >> roomId;
            uni.assignRoomToCourse(courseCode, roomId);
            break;
        }

        case 23:
        {
            string courseA, courseB;
            cout << "Enter first course code: ";
            cin >> courseA;
            cout << "Enter second course code: ";
            cin >> courseB;
            uni.addCourseConflict(courseA, courseB);
            break;
        }

        case 24:
        {
            RelationsModule::analyzeFacultyCourseRelation(uni);
            break;
        }

        
        case 25:
        {
            RelationsModule::analyzeCourseRoomRelation(uni);
            break;
        }

        case 26:
        {
            RelationsModule::detectIndirectStudentConflicts(uni);
            break;
        }
        case 27:
        {
            string facultyId, courseCode, roomId;

            cout << "Enter faculty ID (e.g. ProfX): ";
            cin >> facultyId;

            cout << "Enter course code (e.g. CS101): ";
            cin >> courseCode;

            cout << "Enter required room/lab (e.g. LabA): ";
            cin >> roomId;

            vector<string> conds;
            // Condition: this faculty teaches this course
            conds.push_back(facultyId + "teaches" + courseCode);

            // Conclusion: this course must be in this room
            string conclusion = courseCode + "in" + roomId;

            logicEngine.addRule(conds, conclusion);

            cout << "\nRule added:\n";
            cout << "IF " << conds[0] << " THEN " << conclusion << "\n\n";

            break;
        }
        case 28:
        {
            // Load current real-world facts from University
            loadFactsFromUniversity(uni, scheduler, logicEngine);

            cout << "\n[Logic Engine] Facts loaded from University data.\n";
            logicEngine.printFacts();

            logicEngine.printRules();

            cout << "[Logic Engine] Running inference...\n";
            logicEngine.runInference();

            // After inference, check for any rule violations
            logicEngine.detectConflicts();

            break;
        }
        case 29:
        {
            logicEngine.printFacts();
            logicEngine.printRules();
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
