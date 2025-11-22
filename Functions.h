#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

class University;

// A generic function f: A -> B represented as pairs (a, f(a))
class FunctionMapping {
private:
    vector<pair<string, string>> mapping; // (from, to)

public:
    void addPair(const string& from, const string& to);

    const vector<pair<string, string>>& getPairs() const;

    void print(const string& name) const;

    // Check if this is a valid function: each domain element maps to at most one codomain element
    bool isFunction() const;

    // Check if function is total on given domain: every element in domain appears exactly once
    bool isTotal(const vector<string>& domain) const;

    // Injective: no two different domain elements map to the same codomain value
    bool isInjective() const;

    // Surjective onto given codomain: every element in codomain has at least one preimage
    bool isSurjective(const vector<string>& codomain) const;

    // Bijective: both injective and surjective
    bool isBijective(const vector<string>& domain, const vector<string>& codomain) const;

    // Composition: g ∘ f (this: A->B, g: B->C) result: A->C
    FunctionMapping compose(const FunctionMapping& g) const;

    // Inverse mapping: swap (a, b) to (b, a) (may not be a function if not injective)
    FunctionMapping inverse() const;
};

// Module 7 helper functions
class FunctionsModule {
public:
    // Build Student -> Course function (primary course chosen from enrollments)
    static FunctionMapping buildStudentToCourseFunction(const University& uni);

    // Build Course -> Faculty function (each course assigned to one faculty, if available)
    static FunctionMapping buildCourseToFacultyFunction(const University& uni);

    // Build Faculty -> Room function (main room derived from courses they teach)
    static FunctionMapping buildFacultyToRoomFunction(const University& uni);

    // Analysis helpers
    static void analyzeCourseFacultyFunction(const University& uni);
    static void analyzeStudentCourseFunction(const University& uni);
    static void analyzeFacultyRoomFunction(const University& uni);
};

#endif
