#include "TestingModule.h"
#include "University.h"
#include "Scheduler.h"
#include "ConsistencyChecker.h"
#include "EfficiencyModule.h"

using namespace std;

// Small helper: build student → list of courses map from University
static void buildStudentCourseMap(const University& uni,
    map<string, vector<string>>& studentCourses) {
    vector<string> allCourses = uni.getAllCourseCodes();
    for (const string& c : allCourses) {
        vector<string> students = uni.getStudentsInCourse(c);
        for (const string& sid : students) {
            studentCourses[sid].push_back(c);
        }
    }
}

// ---------------------- Test 1: course code uniqueness ----------------------

static bool testNoDuplicateCourses(const University& uni) {
    cout << "[Test 1] No duplicate course codes (Module 1: Courses)\n";

    vector<string> codes = uni.getAllCourseCodes();
    set<string> seen;
    bool ok = true;

    for (const string& c : codes) {
        if (seen.find(c) != seen.end()) {
            cout << "  FAIL: Duplicate course code found: " << c << "\n";
            ok = false;
        }
        else {
            seen.insert(c);
        }
    }

    if (ok) cout << "  PASS\n";
    cout << "\n";
    return ok;
}

// --------------- Test 2: every course with students is in the course set ----

static bool testEnrollmentsReferToValidCourses(const University& uni) {
    cout << "[Test 2] Enrollments refer to valid courses (Modules 1, 2, 5)\n";

    vector<string> codes = uni.getAllCourseCodes();
    set<string> codeSet(codes.begin(), codes.end());

    map<string, vector<string>> studentCourses;
    buildStudentCourseMap(uni, studentCourses);

    bool ok = true;

    for (const auto& sc : studentCourses) {
        const string& sid = sc.first;
        const vector<string>& courses = sc.second;

        for (const string& c : courses) {
            if (codeSet.find(c) == codeSet.end()) {
                cout << "  FAIL: Student " << sid
                    << " enrolled in non-existing course: " << c << "\n";
                ok = false;
            }
        }
    }

    if (ok) cout << "  PASS\n";
    cout << "\n";
    return ok;
}

// -------- Test 3: prerequisite rules only use existing courses ---------------

static bool testPrereqCourseExistence(const University& uni,
    const Scheduler& scheduler) {
    cout << "[Test 3] All prerequisite pairs use existing courses (Module 3)\n";

    vector<string> codes = uni.getAllCourseCodes();
    set<string> codeSet(codes.begin(), codes.end());

    vector<pair<string, string>> prereqPairs = scheduler.getAllPrerequisitePairs();
    bool ok = true;

    for (const auto& p : prereqPairs) {
        const string& course = p.first;
        const string& prereq = p.second;

        if (codeSet.find(course) == codeSet.end()) {
            cout << "  FAIL: Prerequisite rule uses non-existing course: "
                << course << "\n";
            ok = false;
        }
        if (codeSet.find(prereq) == codeSet.end()) {
            cout << "  FAIL: Prerequisite rule uses non-existing prerequisite: "
                << prereq << "\n";
            ok = false;
        }
    }

    if (ok) cout << "  PASS\n";
    cout << "\n";
    return ok;
}

// ----------------- Test 4: prerequisite graph acyclic (Module 3) ------------

// color: 0 = unvisited, 1 = visiting, 2 = done
static bool dfsCycle(const string& node,
    map<string, vector<string>>& graph,
    map<string, int>& color) {
    color[node] = 1;

    const vector<string>& nbrs = graph[node];
    for (const string& nxt : nbrs) {
        if (color[nxt] == 1) {
            // back edge → cycle
            return true;
        }
        if (color[nxt] == 0) {
            if (dfsCycle(nxt, graph, color)) {
                return true;
            }
        }
    }

    color[node] = 2;
    return false;
}

static bool testPrereqAcyclic(const Scheduler& scheduler) {
    cout << "[Test 4] Prerequisite relation has no cycle (Strong Induction idea)\n";

    vector<pair<string, string>> prereqPairs = scheduler.getAllPrerequisitePairs();

    map<string, vector<string>> graph;
    for (const auto& p : prereqPairs) {
        const string& course = p.first;
        const string& prereq = p.second;
        graph[course].push_back(prereq);
        if (graph.find(prereq) == graph.end()) {
            graph[prereq] = vector<string>();
        }
    }

    map<string, int> color;
    for (const auto& entry : graph) {
        color[entry.first] = 0;
    }

    bool hasCycle = false;

    for (const auto& entry : graph) {
        const string& node = entry.first;
        if (color[node] == 0) {
            if (dfsCycle(node, graph, color)) {
                hasCycle = true;
                break;
            }
        }
    }

    if (hasCycle) {
        cout << "  WARNING: Cycle detected in prerequisite rules.\n";
        cout << "           This breaks strict induction ordering.\n\n";
        return false;
    }
    else {
        cout << "  PASS\n\n";
        return true;
    }
}

// -------- Test 5: conflict pairs refer to existing courses (Module 6, 9) ----

static bool testConflictPairsValid(const University& uni) {
    cout << "[Test 5] Conflict pairs use existing courses (Relations / Consistency)\n";

    vector<string> codes = uni.getAllCourseCodes();
    set<string> codeSet(codes.begin(), codes.end());

    vector<pair<string, string>> conflicts = uni.getCourseConflictPairs();
    bool ok = true;

    for (const auto& cp : conflicts) {
        const string& c1 = cp.first;
        const string& c2 = cp.second;

        if (codeSet.find(c1) == codeSet.end()) {
            cout << "  FAIL: Conflict rule uses non-existing course: "
                << c1 << "\n";
            ok = false;
        }
        if (codeSet.find(c2) == codeSet.end()) {
            cout << "  FAIL: Conflict rule uses non-existing course: "
                << c2 << "\n";
            ok = false;
        }
    }

    if (ok) cout << "  PASS\n";
    cout << "\n";
    return ok;
}

// ---------- Test 6: detect any direct violation of a conflict pair ----------

static bool testConflictViolationsDirect(const University& uni) {
    cout << "[Test 6] Direct conflict violations via sets (Modules 5, 6, 9)\n";

    vector<pair<string, string>> conflicts = uni.getCourseConflictPairs();
    if (conflicts.empty()) {
        cout << "  INFO: No conflict pairs defined; nothing to test.\n\n";
        return true;
    }

    map<string, vector<string>> studentCourses;
    buildStudentCourseMap(uni, studentCourses);

    bool ok = true;

    for (const auto& sc : studentCourses) {
        const string& sid = sc.first;
        const vector<string>& courses = sc.second;
        set<string> courseSet(courses.begin(), courses.end());

        for (const auto& cp : conflicts) {
            const string& c1 = cp.first;
            const string& c2 = cp.second;

            bool hasC1 = (courseSet.find(c1) != courseSet.end());
            bool hasC2 = (courseSet.find(c2) != courseSet.end());

            if (hasC1 && hasC2) {
                cout << "  WARNING: Student " << sid
                    << " enrolled in conflicting courses: "
                    << c1 << " and " << c2 << "\n";
                ok = false;
            }
        }
    }

    if (ok) {
        cout << "  PASS (no student currently enrolled in both sides of a conflict)\n";
    }
    cout << "\n";
    return ok;
}

// -------- Test 7: student course load not exploding (Module 7, 9) -----------

static bool testStudentMaxCourseLoad(const University& uni, int maxAllowed) {
    cout << "[Test 7] Student course load <= " << maxAllowed
        << " (Function-style mapping: student → set of courses)\n";

    map<string, vector<string>> studentCourses;
    buildStudentCourseMap(uni, studentCourses);

    bool ok = true;

    for (const auto& sc : studentCourses) {
        const string& sid = sc.first;
        int count = static_cast<int>(sc.second.size());
        if (count > maxAllowed) {
            cout << "  WARNING: Student " << sid
                << " taking " << count
                << " courses (exceeds limit " << maxAllowed << ")\n";
            ok = false;
        }
    }

    if (ok) {
        cout << "  PASS\n";
    }
    cout << "\n";
    return ok;
}

// -------- Test 8: simple sanity for "function-like" behavior -----------------
// Idea: If a course has no students, that's fine,
//       but if it has students, it should appear in the global course set.

static bool testStudentCourseRelationTotal(const University& uni) {
    cout << "[Test 8] Student–course relation is total over enrollments (Relations)\n";

    vector<string> codes = uni.getAllCourseCodes();
    set<string> codeSet(codes.begin(), codes.end());

    map<string, vector<string>> studentCourses;
    buildStudentCourseMap(uni, studentCourses);

    bool ok = true;

    for (const auto& sc : studentCourses) {
        const vector<string>& courses = sc.second;
        for (const string& c : courses) {
            if (codeSet.find(c) == codeSet.end()) {
                cout << "  FAIL: Enrollment uses course not in course set: "
                    << c << "\n";
                ok = false;
            }
        }
    }

    if (ok) {
        cout << "  PASS\n";
    }
    cout << "\n";
    return ok;
}

// ---------------------- Public runner: Module 12 -----------------------------

void TestingModule::runUnitTests(const University& uni,
    const Scheduler& scheduler) {
    cout << "\n========== Module 12: Unit Testing & Benchmarking ==========\n\n";

    int passed = 0;
    int failed = 0;

    if (testNoDuplicateCourses(uni))             passed++; else failed++;
    if (testEnrollmentsReferToValidCourses(uni)) passed++; else failed++;
    if (testPrereqCourseExistence(uni, scheduler)) passed++; else failed++;
    if (testPrereqAcyclic(scheduler))            passed++; else failed++;
    if (testConflictPairsValid(uni))             passed++; else failed++;
    if (testConflictViolationsDirect(uni))       passed++; else failed++;
    if (testStudentMaxCourseLoad(uni, 6))        passed++; else failed++;
    if (testStudentCourseRelationTotal(uni))     passed++; else failed++;

    cout << "Summary of unit tests:\n";
    cout << "  Passed: " << passed << "\n";
    cout << "  Failed: " << failed << "\n\n";

    cout << "Now running integrated consistency and efficiency checks\n";
    cout << "(Modules 9 and 10, using the current dataset):\n\n";

    int maxCoursesAllowed = 6;
    ConsistencyChecker::runFullConsistencyCheck(uni, scheduler, maxCoursesAllowed);

    EfficiencyModule::benchmarkPrerequisiteDFS(scheduler);
    EfficiencyModule::benchmarkConflictBitmask(uni);

    cout << "========== End of Module 12 tests and benchmarks ==========\n\n";
}