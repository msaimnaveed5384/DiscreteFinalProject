#include "ConsistencyChecker.h"
#include "University.h"
#include "Scheduler.h"

using namespace std;

// Helper to build a map student -> list of courses enrolled
static void buildStudentCourseMap(const University& uni,
    map<string, vector<string>>& studentCourses) {
    vector<string> allCourses = uni.getAllCourseCodes();

    for (const string& courseCode : allCourses) {
        vector<string> students = uni.getStudentsInCourse(courseCode);
        for (const string& sid : students) {
            studentCourses[sid].push_back(courseCode);
        }
    }
}

// 1) Check course conflicts based on University courseConflicts
// For each pair (C1, C2) in courseConflicts, if a student is enrolled in both,
// we report a violation.
void ConsistencyChecker::checkCourseConflicts(const University& uni) {
    cout << "=== Consistency Check: Course Conflicts ===\n";

    vector<pair<string, string>> conflictPairs = uni.getCourseConflictPairs();
    if (conflictPairs.empty()) {
        cout << "No course conflict pairs defined.\n\n";
        return;
    }

    bool anyViolation = false;

    // build student -> courses map
    map<string, vector<string>> studentCourses;
    buildStudentCourseMap(uni, studentCourses);

    for (const auto& entry : studentCourses) {
        const string& studentId = entry.first;
        const vector<string>& courses = entry.second;

        // check all conflict pairs for this student
        for (const auto& cp : conflictPairs) {
            const string& c1 = cp.first;
            const string& c2 = cp.second;

            bool hasC1 = false;
            bool hasC2 = false;

            for (const string& c : courses) {
                if (c == c1) hasC1 = true;
                if (c == c2) hasC2 = true;
            }

            if (hasC1 && hasC2) {
                cout << "Violation: Student " << studentId
                    << " is enrolled in conflicting courses "
                    << c1 << " and " << c2 << ".\n";
                anyViolation = true;
            }
        }
    }

    if (!anyViolation) {
        cout << "No students found in conflicting course pairs.\n";
    }

    cout << "==========================================\n\n";
}

// 2) Check missing prerequisites.
// For each course C with prerequisites P1, P2, ...,
// for every student enrolled in C, ensure they are also enrolled in all Pi.
void ConsistencyChecker::checkMissingPrerequisites(const University& uni,
    const Scheduler& scheduler) {
    cout << "=== Consistency Check: Missing Prerequisites ===\n";

    vector<pair<string, string>> prereqPairs = scheduler.getAllPrerequisitePairs();
    if (prereqPairs.empty()) {
        cout << "No prerequisites defined in scheduler.\n\n";
        return;
    }

    // Build course -> list of prereqs
    map<string, vector<string>> prereqMap;
    for (const auto& p : prereqPairs) {
        const string& course = p.first;
        const string& prereq = p.second;
        prereqMap[course].push_back(prereq);
    }

    bool anyViolation = false;

    // For each course with prereqs
    for (const auto& entry : prereqMap) {
        const string& course = entry.first;
        const vector<string>& prereqs = entry.second;

        vector<string> studentsInCourse = uni.getStudentsInCourse(course);

        // For each student in this course
        for (const string& sid : studentsInCourse) {

            // For each prerequisite
            for (const string& pre : prereqs) {
                bool hasPre = false;
                vector<string> studentsInPre = uni.getStudentsInCourse(pre);

                for (const string& s2 : studentsInPre) {
                    if (s2 == sid) {
                        hasPre = true;
                        break;
                    }
                }

                if (!hasPre) {
                    cout << "Violation: Student " << sid
                        << " is enrolled in " << course
                        << " without prerequisite " << pre << ".\n";
                    anyViolation = true;
                }
            }
        }
    }

    if (!anyViolation) {
        cout << "No missing prerequisite violations found for enrolled students.\n";
    }

    cout << "===============================================\n\n";
}

// 3) Check student overload.
// Overload is defined as student enrolled in more than maxCourses courses.
void ConsistencyChecker::checkStudentOverload(const University& uni,
    int maxCourses) {
    cout << "=== Consistency Check: Student Overload ===\n";

    map<string, vector<string>> studentCourses;
    buildStudentCourseMap(uni, studentCourses);

    bool anyOverload = false;

    for (const auto& entry : studentCourses) {
        const string& sid = entry.first;
        int count = static_cast<int>(entry.second.size());

        if (count > maxCourses) {
            anyOverload = true;
            cout << "Overload: Student " << sid
                << " is enrolled in " << count
                << " courses (limit is " << maxCourses << ").\n";
        }
    }

    if (!anyOverload) {
        cout << "No students exceed the course load limit of "
            << maxCourses << ".\n";
    }

    cout << "==========================================\n\n";
}

// 4) Run all checks together.
void ConsistencyChecker::runFullConsistencyCheck(const University& uni,
    const Scheduler& scheduler,
    int maxCourses) {
    cout << "\n========== Global Consistency Check ==========\n\n";

    checkCourseConflicts(uni);
    checkMissingPrerequisites(uni, scheduler);
    checkStudentOverload(uni, maxCourses);

    cout << "========== End of Global Consistency ==========\n\n";
}