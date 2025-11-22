#include "EfficiencyModule.h"
#include "Scheduler.h"
#include "University.h"

using namespace std;

// We'll use a 64-bit mask. That gives us up to 64 courses.
const int MAX_COURSES_EFF = 64;

// --------------------- Helper for prerequisite graph ---------------------

static void buildPrereqGraphEff(const Scheduler& scheduler,
    map<string, vector<string>>& graph) {
    vector<pair<string, string>> prereqPairs = scheduler.getAllPrerequisitePairs();
    for (const auto& p : prereqPairs) {
        const string& course = p.first;
        const string& prereq = p.second;
        graph[course].push_back(prereq);
        if (graph.find(prereq) == graph.end()) {
            graph[prereq] = vector<string>();
        }
    }
}

// Plain recursive DFS to count total prerequisite nodes reachable from a course.
// We also count how many recursive calls happened (for benchmarking).
static int dfsPlain(const string& course,
    const map<string, vector<string>>& graph,
    int& callCounterPlain) {
    callCounterPlain++;

    auto it = graph.find(course);
    if (it == graph.end()) {
        return 0;
    }

    const vector<string>& prereqs = it->second;
    int total = 0;
    for (const string& p : prereqs) {
        total += 1 + dfsPlain(p, graph, callCounterPlain);
    }
    return total;
}

// Memoized DFS (dynamic programming).
// Uses a memo table so repeated subtrees are not recomputed.
static int dfsMemo(const string& course,
    const map<string, vector<string>>& graph,
    map<string, int>& memo,
    int& callCounterMemo) {
    callCounterMemo++;

    auto itMemo = memo.find(course);
    if (itMemo != memo.end()) {
        return itMemo->second;
    }

    auto it = graph.find(course);
    if (it == graph.end()) {
        memo[course] = 0;
        return 0;
    }

    const vector<string>& prereqs = it->second;
    int total = 0;
    for (const string& p : prereqs) {
        total += 1 + dfsMemo(p, graph, memo, callCounterMemo);
    }

    memo[course] = total;
    return total;
}

// --------------------- Benchmark 1: prerequisite DFS ----------------------

void EfficiencyModule::benchmarkPrerequisiteDFS(const Scheduler& scheduler) {
    cout << "=== Benchmark: Prerequisite DFS (Plain vs Memoized) ===\n";

    map<string, vector<string>> graph;
    buildPrereqGraphEff(scheduler, graph);

    if (graph.empty()) {
        cout << "No prerequisites defined. Nothing to benchmark.\n\n";
        return;
    }

    // Collect all course codes present in graph
    vector<string> allCourses;
    for (const auto& entry : graph) {
        allCourses.push_back(entry.first);
    }

    long long totalPlainCalls = 0;
    long long totalMemoCalls = 0;

    // Run plain DFS for each course
    for (const string& c : allCourses) {
        int callCounterPlain = 0;
        int result = dfsPlain(c, graph, callCounterPlain);
        (void)result; // avoid unused warning
        totalPlainCalls += callCounterPlain;
    }

    // Run memoized DFS for each course
    map<string, int> memo;
    for (const string& c : allCourses) {
        int callCounterMemo = 0;
        int result = dfsMemo(c, graph, memo, callCounterMemo);
        (void)result;
        totalMemoCalls += callCounterMemo;
    }

    cout << "\nTotal recursive calls (plain DFS):     " << totalPlainCalls << "\n";
    cout << "Total recursive calls (memoized DFS): " << totalMemoCalls << "\n\n";

    cout << "Interpretation:\n";
    cout << "- Plain recursion recomputes the same prerequisite chains many times.\n";
    cout << "- Memoized recursion (dynamic programming) saves results in a table\n";
    cout << "  and reuses them, which reduces the total number of recursive calls,\n";
    cout << "  especially for large prerequisite graphs with shared subchains.\n\n";
}

// --------------------- Benchmark 2: bitmask conflict checking ----------------

// Build mapping from course code to index [0, n)
static void buildCourseIndexMap(const University& uni,
    map<string, int>& courseIndex,
    vector<string>& indexToCourse) {
    vector<string> allCourses = uni.getAllCourseCodes();
    int idx = 0;
    for (const string& c : allCourses) {
        if (idx >= MAX_COURSES_EFF) {
            // Ignore extra courses beyond capacity
            break;
        }
        courseIndex[c] = idx;
        indexToCourse.push_back(c);
        idx++;
    }
}

// Build per-student vector of enrolled courses (for naive check)
static void buildStudentCourseMapSimple(const University& uni,
    map<string, vector<string>>& studentCourses) {
    vector<string> allCourses = uni.getAllCourseCodes();
    for (const string& c : allCourses) {
        vector<string> students = uni.getStudentsInCourse(c);
        for (const string& sid : students) {
            studentCourses[sid].push_back(c);
        }
    }
}

// Build per-student bitmask representing enrolled courses.
// Each bit in an unsigned long long corresponds to a course index.
static void buildStudentBitmasks(const University& uni,
    const map<string, int>& courseIndex,
    map<string, unsigned long long>& studentMask) {
    vector<string> allCourses = uni.getAllCourseCodes();

    for (const string& courseCode : allCourses) {
        auto it = courseIndex.find(courseCode);
        if (it == courseIndex.end()) {
            continue; // beyond MAX_COURSES_EFF
        }
        int idx = it->second;
        unsigned long long bit = (1ULL << idx);

        vector<string> students = uni.getStudentsInCourse(courseCode);
        for (const string& sid : students) {
            studentMask[sid] |= bit;
        }
    }
}

void EfficiencyModule::benchmarkConflictBitmask(const University& uni) {
    cout << "=== Benchmark: Conflict Checking (Naive vs Bitmask) ===\n";

    // Get course conflict pairs from University
    vector<pair<string, string>> conflictPairs = uni.getCourseConflictPairs();
    if (conflictPairs.empty()) {
        cout << "No course conflict pairs defined. Nothing to benchmark.\n\n";
        return;
    }

    // Build course index mapping
    map<string, int> courseIndex;
    vector<string> indexToCourse;
    buildCourseIndexMap(uni, courseIndex, indexToCourse);

    if (courseIndex.empty()) {
        cout << "No courses available. Nothing to benchmark.\n\n";
        return;
    }

    // Naive approach: use student -> vector<course> mapping
    map<string, vector<string>> studentCourses;
    buildStudentCourseMapSimple(uni, studentCourses);

    if (studentCourses.empty()) {
        cout << "No student enrollments found. Nothing to benchmark.\n\n";
        return;
    }

    // Bitmask approach: student -> unsigned long long mask
    map<string, unsigned long long> studentMask;
    buildStudentBitmasks(uni, courseIndex, studentMask);

    long long naiveComparisons = 0;
    long long bitmaskChecks = 0;
    int violationsNaive = 0;
    int violationsBitmask = 0;

    // -------- Naive method --------
    for (const auto& sc : studentCourses) {
        const string& sid = sc.first;
        const vector<string>& courses = sc.second;

        for (const auto& cp : conflictPairs) {
            const string& c1 = cp.first;
            const string& c2 = cp.second;

            bool hasC1 = false;
            bool hasC2 = false;

            for (const string& c : courses) {
                naiveComparisons++;
                if (c == c1) hasC1 = true;
                if (c == c2) hasC2 = true;
            }

            if (hasC1 && hasC2) {
                violationsNaive++;
            }
        }
    }

    // -------- Bitmask method --------
    for (const auto& sm : studentMask) {
        const string& sid = sm.first;
        unsigned long long mask = sm.second;

        for (const auto& cp : conflictPairs) {
            auto it1 = courseIndex.find(cp.first);
            auto it2 = courseIndex.find(cp.second);
            if (it1 == courseIndex.end() || it2 == courseIndex.end()) {
                continue;
            }
            int idx1 = it1->second;
            int idx2 = it2->second;

            unsigned long long bit1 = (1ULL << idx1);
            unsigned long long bit2 = (1ULL << idx2);
            unsigned long long conflictMask = bit1 | bit2;

            bitmaskChecks++;

            // Both bits present if (mask & conflictMask) == conflictMask
            if ((mask & conflictMask) == conflictMask) {
                violationsBitmask++;
            }
        }
    }

    cout << "\nNaive approach:\n";
    cout << "  String comparisons performed: " << naiveComparisons << "\n";
    cout << "  Violations found:             " << violationsNaive << "\n\n";

    cout << "Bitmask approach:\n";
    cout << "  Bitmask checks performed:     " << bitmaskChecks << "\n";
    cout << "  Violations found:             " << violationsBitmask << "\n\n";

    cout << "Interpretation:\n";
    cout << "- Naive method checks each conflict pair by scanning all enrolled\n";
    cout << "  courses for a student using string comparisons.\n";
    cout << "- Bitmask method encodes each student's course set inside an\n";
    cout << "  unsigned long long, where each bit represents a course.\n";
    cout << "- Then conflict checks become a single '&' and '==' operation,\n";
    cout << "  which is much closer to O(1) per check and uses bit manipulation\n";
    cout << "  as suggested by the module's 'bit manipulation' hint.\n\n";
}