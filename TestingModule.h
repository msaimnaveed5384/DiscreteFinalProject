#ifndef TESTING_MODULE_H
#define TESTING_MODULE_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

using namespace std;

class University;
class Scheduler;

class TestingModule {
public:
    // Runs a suite of tests that validate:
    // - Courses and prerequisites (Modules 1, 3)
    // - Student enrollments and conflicts (Modules 1, 2, 5, 6, 7, 9)
    // - Global consistency and performance (Modules 8, 9, 10)
    static void runUnitTests(const University& uni,
        const Scheduler& scheduler);
};

#endif