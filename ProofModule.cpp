#include "ProofModule.h"
#include "Scheduler.h"
#include "InferenceEngine.h"

using namespace std;

// ---------- Helpers for prerequisites proof ----------

static void buildPrereqGraph(const Scheduler& scheduler,
    map<string, vector<string>>& graph) {
    vector<pair<string, string>> prereqPairs = scheduler.getAllPrerequisitePairs();
    for (const auto& p : prereqPairs) {
        const string& course = p.first;
        const string& prereq = p.second;
        graph[course].push_back(prereq);
        // ensure prereq exists as key as well, even if it has no further prereqs
        if (graph.find(prereq) == graph.end()) {
            graph[prereq] = vector<string>();
        }
    }
}

static bool proveCourseRecursive(const string& course,
    const map<string, vector<string>>& graph,
    const set<string>& completed,
    set<string>& visiting,
    set<string>& proven,
    int depth) {
    // indentation for readability
    for (int i = 0; i < depth; i++) cout << "  ";

    cout << "Analyzing course: " << course << "\n";

    // cycle detection
    if (visiting.find(course) != visiting.end()) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "Found a cycle involving " << course << " (prerequisite loop).\n";
        return false;
    }

    // if already proven earlier, reuse that
    if (proven.find(course) != proven.end()) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "Already proven that prerequisites for " << course << " are satisfied.\n";
        return true;
    }

    visiting.insert(course);

    auto it = graph.find(course);
    if (it == graph.end() || it->second.empty()) {
        // No prerequisites for this course
        if (completed.find(course) != completed.end()) {
            for (int i = 0; i < depth; i++) cout << "  ";
            cout << "Base case: student has already completed " << course << ".\n";
        }
        else {
            for (int i = 0; i < depth; i++) cout << "  ";
            cout << "Base case: " << course << " has no prerequisites.\n";
        }
        visiting.erase(course);
        proven.insert(course);
        return true;
    }

    // There are prerequisites
    const vector<string>& prereqs = it->second;

    for (const string& p : prereqs) {
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "To take " << course << ", student must have completed " << p << ".\n";

        // recurse on prereq
        bool ok = proveCourseRecursive(p, graph, completed, visiting, proven, depth + 1);
        if (!ok) {
            for (int i = 0; i < depth; i++) cout << "  ";
            cout << "Cannot prove prerequisites for " << course << " because of issues with " << p << ".\n";
            visiting.erase(course);
            return false;
        }
    }

    // if all prerequisites proven
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << "Induction step: since all prerequisites for " << course
        << " are satisfied, student is allowed to take " << course << ".\n";

    visiting.erase(course);
    proven.insert(course);
    return true;
}

// ---------- Public: step-by-step prerequisite proof ----------

void ProofModule::provePrerequisiteChain(const Scheduler& scheduler,
    const string& targetCourse,
    const vector<string>& completedCourses) {
    cout << "=== Proof of Prerequisite Chain for " << targetCourse << " ===\n";

    map<string, vector<string>> graph;
    buildPrereqGraph(scheduler, graph);

    set<string> completedSet;
    for (const string& c : completedCourses) {
        completedSet.insert(c);
    }

    set<string> visiting;
    set<string> proven;

    bool result = proveCourseRecursive(targetCourse, graph,
        completedSet, visiting, proven, 0);

    cout << "\nConclusion: ";
    if (result) {
        cout << "All prerequisite requirements for " << targetCourse << " are structurally satisfied.\n";
    }
    else {
        cout << "There are problems in the prerequisite chain for " << targetCourse << ".\n";
    }
    cout << "=============================================\n\n";
}

// ---------- Public: explain logic inference ----------

void ProofModule::explainLogicInference(const InferenceEngine& engine) {
    cout << "=== Step-by-Step Logic Rule Explanation ===\n\n";

    const vector<string>& facts = engine.getFacts();
    const vector<LogicRule>& rules = engine.getRules();

    // print facts first
    cout << "Known facts:\n";
    for (const string& f : facts) {
        cout << "  - " << f << "\n";
    }
    cout << "\n";

    // helper lambda to check fact in list
    auto factExistsLocal = [&facts](const string& fact) -> bool {
        for (const string& f : facts) {
            if (f == fact) return true;
        }
        return false;
        };

    // go through each rule
    for (size_t i = 0; i < rules.size(); i++) {
        const LogicRule& r = rules[i];

        cout << "Rule " << (i + 1) << ": IF ";
        for (size_t j = 0; j < r.conditions.size(); j++) {
            cout << r.conditions[j];
            if (j + 1 < r.conditions.size()) cout << " AND ";
        }
        cout << " THEN " << r.conclusion << "\n";

        bool allCondTrue = true;

        // check each condition
        for (const string& cond : r.conditions) {
            bool exists = factExistsLocal(cond);
            cout << "  Check condition \"" << cond << "\": "
                << (exists ? "present in facts.\n" : "NOT present in facts.\n");
            if (!exists) {
                allCondTrue = false;
            }
        }

        bool conclTrue = factExistsLocal(r.conclusion);
        cout << "  Check conclusion \"" << r.conclusion << "\": "
            << (conclTrue ? "present in facts.\n" : "NOT present in facts.\n");

        if (allCondTrue && conclTrue) {
            cout << "  Verdict: All conditions hold and conclusion holds. Rule is satisfied.\n\n";
        }
        else if (allCondTrue && !conclTrue) {
            cout << "  Verdict: All conditions hold but conclusion is false. Rule is VIOLATED.\n\n";
        }
        else {
            cout << "  Verdict: Conditions do not fully hold. Rule does not trigger.\n\n";
        }
    }

    cout << "=== End of Logic Explanation ===\n\n";
}

// ---------- Public: verify relational consistency (prereq graph) ----------

static bool dfsCycle(const string& course,
    const map<string, vector<string>>& graph,
    set<string>& visiting,
    set<string>& visited) {
    visiting.insert(course);

    auto it = graph.find(course);
    if (it != graph.end()) {
        const vector<string>& prereqs = it->second;
        for (const string& p : prereqs) {
            if (p == course) {
                cout << "Self-loop detected: " << course << " has itself as a prerequisite.\n";
                return true;
            }
            if (visiting.find(p) != visiting.end()) {
                cout << "Cycle detected involving: " << course << " and " << p << ".\n";
                return true;
            }
            if (visited.find(p) == visited.end()) {
                if (dfsCycle(p, graph, visiting, visited)) {
                    return true;
                }
            }
        }
    }

    visiting.erase(course);
    visited.insert(course);
    return false;
}

void ProofModule::verifyRelationalConsistency(const Scheduler& scheduler) {
    cout << "=== Verifying Prerequisite Relation Consistency ===\n";

    map<string, vector<string>> graph;
    buildPrereqGraph(scheduler, graph);

    set<string> visited;
    set<string> visiting;

    bool hasProblem = false;

    for (const auto& entry : graph) {
        const string& course = entry.first;
        if (visited.find(course) == visited.end()) {
            if (dfsCycle(course, graph, visiting, visited)) {
                hasProblem = true;
                break;
            }
        }
    }

    if (!hasProblem) {
        cout << "No cycles or self-loops detected in prerequisite graph.\n";
        cout << "Relation is structurally consistent.\n";
    }

    cout << "===============================================\n\n";
}