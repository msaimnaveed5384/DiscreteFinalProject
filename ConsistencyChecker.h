#ifndef CONSISTENCY_CHECKER_H
#define CONSISTENCY_CHECKER_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>

using namespace std;

class University;
class Scheduler;

class ConsistencyChecker {
public:
    // 1) Check if any student is enrolled in two conflicting courses
    static void checkCourseConflicts(const University& uni);

    // 2) Check missing prerequisites for enrolled students
    static void checkMissingPrerequisites(const University& uni,
        const Scheduler& scheduler);

    // 3) Check if any student is overloaded with too many courses
    static void checkStudentOverload(const University& uni,
        int maxCourses);

    // 4) Run all checks together
    static void runFullConsistencyCheck(const University& uni,
        const Scheduler& scheduler,
        int maxCourses);
};

#endif