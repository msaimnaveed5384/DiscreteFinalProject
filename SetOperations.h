#ifndef SETOPERATIONS_H
#define SETOPERATIONS_H

#include<vector>
#include<string>
#include<set>
#include<iostream>
using namespace std;
class SetOperations 
{
public:
    // Convert vector to set (remove duplicates)
    static set<string> toSet(const vector<string>& v);

    // Convert set back to vector
    static vector<string> toVector(const set<string>& s);

    // A ∪ B
    static vector<string> setUnion(const vector<string>& A, const vector<string>& B);

    // A ∩ B
    static vector<string> setIntersection(const vector<string>& A, const vector<string>& B);

    // A \ B
    static vector<string> setDifference(const vector<string>& A, const vector<string>& B);

    // Is A ⊆ B ?
    static bool isSubset(const vector<string>& A, const vector<string>& B);

    // Power set 2^A
    static vector<vector<string>> powerSet(const vector<string>& A);

    // Print helper
    static void printSet(const vector<string>& S, const string& name);
};
#endif
