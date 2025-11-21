#ifndef RELATIONS_H
#define RELATIONS_H

#include <vector>
#include <string>
#include <utility>
#include <set>
#include <iostream>
using namespace std;

class Relations
{
public:

    // Display helper
    static void printRelation(const vector<pair<string, string>>& R, const string& name);

    // Check properties
    static bool isReflexive(const vector<pair<string, string>>& R, const vector<string>& elements);
    static bool isSymmetric(const vector<pair<string, string>>& R);
    static bool isAntisymmetric(const vector<pair<string, string>>& R);
    static bool isTransitive(const vector<pair<string, string>>& R);

    // Composite property checks
    static bool isEquivalence(const vector<pair<string, string>>& R, const vector<string>& elements);
    static bool isPartialOrder(const vector<pair<string, string>>& R, const vector<string>& elements);

    // Composition R ∘ S
    static vector<pair<string, string>> compose(const vector<pair<string, string>>& R,
        const vector<pair<string, string>>& S);
};

#endif
