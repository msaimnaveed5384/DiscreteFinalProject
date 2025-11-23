#ifndef PROOF_MODULE_H
#define PROOF_MODULE_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>

using namespace std;

class Scheduler;
class InferenceEngine;
class LogicRule;

class ProofModule {
public:
    // 1) Step-by-step proof that a course's prerequisites chain is satisfied
    // completedCourses = list of course codes the student has already passed
    static void provePrerequisiteChain(const Scheduler& scheduler,
        const string& targetCourse,
        const vector<string>& completedCourses);

    // 2) Step-by-step explanation of all logic rules vs current facts
    static void explainLogicInference(const InferenceEngine& engine);

    // 3) Verify prerequisite relation consistency (no self-loops / cycles)
    static void verifyRelationalConsistency(const Scheduler& scheduler);
};

#endif