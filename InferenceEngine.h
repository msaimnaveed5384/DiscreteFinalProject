#ifndef INFERENCE_ENGINE_H
#define INFERENCE_ENGINE_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

// A single logic rule: IF all conditions are true THEN conclusion is true
class LogicRule {
public:
    vector<string> conditions; // left side (IF ...)
    string conclusion;         // right side (THEN ...)

    LogicRule() {}

    LogicRule(const vector<string>& conds, const string& concl)
        : conditions(conds), conclusion(concl) {
    }
};

class InferenceEngine {
private:
    vector<string> facts;       // known facts (from University + inferred)
    vector<LogicRule> rules;    // rules added by user / system

public:
    // Add a fact (e.g. "ProfX_teaches_CS101", "CS101_in_LabA")
    void addFact(const string& fact);

    // Add a rule: IF conds THEN conclusion
    void addRule(const vector<string>& conds, const string& conclusion);

    // Check if a fact already exists in the knowledge base
    bool factExists(const string& fact) const;

    // Run forward-chaining inference until no new facts can be derived
    void runInference();

    // Check if any rule is violated (conditions true but conclusion false)
    void detectConflicts() const;

    // Print all rules (for debugging / viva)
    void printRules() const;

    // Print all facts
    void printFacts() const;

    const vector<string>& getFacts() const;
    const vector<LogicRule>& getRules() const;
};

#endif