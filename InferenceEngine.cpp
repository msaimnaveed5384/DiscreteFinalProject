#include "InferenceEngine.h"

void InferenceEngine::addFact(const string& fact) {
    // Avoid duplicates manually using simple loop
    for (const string& f : facts) {
        if (f == fact) {
            return; // fact already exists
        }
    }

    facts.push_back(fact);
}

// --------------------------
// Add a new rule
// --------------------------
void InferenceEngine::addRule(const vector<string>& conds, const string& conclusion) {
    LogicRule rule(conds, conclusion);
    rules.push_back(rule);
}

// --------------------------
// Check if a fact exists
// --------------------------
bool InferenceEngine::factExists(const string& fact) const {
    for (const string& f : facts) {
        if (f == fact) {
            return true;
        }
    }
    return false;
}

// --------------------------
// Forward chaining inference
// --------------------------
void InferenceEngine::runInference() {
    bool newFactAdded = true;

    // Keep applying rules as long as new facts are generated
    while (newFactAdded) {
        newFactAdded = false;

        for (const LogicRule& rule : rules) {
            bool allConditionsTrue = true;

            // Check if ALL conditions are satisfied
            for (const string& cond : rule.conditions) {
                if (!factExists(cond)) {
                    allConditionsTrue = false;
                    break;
                }
            }

            // If conditions hold, conclusion must also become a fact
            if (allConditionsTrue && !factExists(rule.conclusion)) {
                addFact(rule.conclusion);
                newFactAdded = true;

                cout << "Inferred new fact: " << rule.conclusion << "\n";
            }
        }
    }
}

// --------------------------
// Detect rule violations (conflicts)
// --------------------------
void InferenceEngine::detectConflicts() const {
    cout << "\n=== Checking Conflicts in Logic Rules ===\n";

    for (const LogicRule& rule : rules) {
        bool allConditionsTrue = true;

        // Check all conditions of the rule
        for (const string& cond : rule.conditions) {
            if (!factExists(cond)) {
                allConditionsTrue = false;
                break;
            }
        }

        // If all conditions are true but conclusion is false => violation
        if (allConditionsTrue && !factExists(rule.conclusion)) {
            cout << "CONFLICT: Rule violated: IF ";

            for (size_t i = 0; i < rule.conditions.size(); i++) {
                cout << rule.conditions[i];
                if (i + 1 < rule.conditions.size()) cout << " AND ";
            }

            cout << " THEN " << rule.conclusion << "\n";
        }
    }

    cout << "=== Conflict Check Complete ===\n\n";
}

// --------------------------
// Print all rules
// --------------------------
void InferenceEngine::printRules() const {
    cout << "\n=== Logic Rules ===\n";

    for (size_t i = 0; i < rules.size(); i++) {
        cout << i + 1 << ". IF ";

        for (size_t j = 0; j < rules[i].conditions.size(); j++) {
            cout << rules[i].conditions[j];
            if (j + 1 < rules[i].conditions.size()) cout << " AND ";
        }

        cout << " THEN " << rules[i].conclusion << "\n";
    }

    cout << "===================\n\n";
}

// --------------------------
// Print all facts
// --------------------------
void InferenceEngine::printFacts() const {
    cout << "\n=== Facts in Knowledge Base ===\n";

    for (const string& fact : facts) {
        cout << "- " << fact << "\n";
    }

    cout << "===============================\n\n";
}

const vector<string>& InferenceEngine::getFacts() const {
    return facts;
}

const vector<LogicRule>& InferenceEngine::getRules() const {
    return rules;
}