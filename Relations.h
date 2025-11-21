#ifndef RELATIONS_H
#define RELATIONS_H

#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

// Forward declaration to avoid circular include
class University;

class Relation {
private:
    // Relation R is a set of ordered pairs (a, b)
    vector<pair<string, string>> pairs;

public:
    void addPair(const string& a, const string& b);
    const vector<pair<string, string>>& getPairs() const;

    void print(const string& name) const;

    // Property checks
    bool isReflexive(const vector<string>& universe) const;
    bool isSymmetric() const;
    bool isTransitive() const;
    bool isAntisymmetric() const;

    bool isEquivalence(const vector<string>& universe) const;
    bool isPartialOrder(const vector<string>& universe) const;

    // Composition: this ∘ other
    Relation compose(const Relation& other) const;
};

class RelationsModule {
public:
    // Build Student -> Course relation from real enrollments
    static Relation buildStudentCourseRelation(const University& uni);

    // Analyze basic properties of Student -> Course relation
    static void analyzeStudentCourseRelation(const University& uni);
};

#endif