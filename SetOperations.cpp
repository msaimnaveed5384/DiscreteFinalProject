#include "SetOperations.h"

using namespace std;

set<string> SetOperations::toSet(const vector<string>& v)
{
    set<string> s;
    for (const string& x : v) 
    {
        s.insert(x);  // duplicates are automatically removed
    }
    return s;
}

vector<string> SetOperations::toVector(const set<string>& s)
{
    vector<string> v;
    for (const string& x : s) 
    {
        v.push_back(x);
    }
    return v;
}

vector<string> SetOperations::setUnion(const vector<string>& A, const vector<string>& B)
{
    set<string> sA = toSet(A);
    set<string> sB = toSet(B);
    set<string> result = sA;

    for (const string& x : sB) 
    {
        result.insert(x);
    }

    return toVector(result);
}

vector<string> SetOperations::setIntersection(const vector<string>& A, const vector<string>& B) 
{
    set<string> sA = toSet(A);
    set<string> sB = toSet(B);
    set<string> result;

    for (const string& x : sA) 
    {
        if (sB.find(x) != sB.end()) 
        {
            result.insert(x);
        }
    }

    return toVector(result);
}

vector<string> SetOperations::setDifference(const vector<string>& A, const vector<string>& B) 
{
    set<string> sA = toSet(A);
    set<string> sB = toSet(B);
    set<string> result;

    for (const string& x : sA) 
    {
        if (sB.find(x) == sB.end()) 
        {
            result.insert(x);
        }
    }

    return toVector(result);
}

bool SetOperations::isSubset(const vector<string>& A, const vector<string>& B) 
{
    set<string> sA = toSet(A);
    set<string> sB = toSet(B);

    for (const string& x : sA) 
    {
        if (sB.find(x) == sB.end()) 
        {
            return false;
        }
    }
    return true;
}

vector<vector<string>> SetOperations::powerSet(const vector<string>& A) 
{
    // work on a unique version of A
    set<string> uniqueSet = toSet(A);
    vector<string> base = toVector(uniqueSet);

    vector<vector<string>> allSubsets;
    int n = static_cast<int>(base.size());
    int total = 1 << n;  // 2^n subsets bit shift operation.

    for (int mask = 0; mask < total; mask++) 
    {
        vector<string> subset;
        for (int i = 0; i < n; ++i)
        {
            if (mask & (1 << i)) 
            {
                subset.push_back(base[i]);
            }
        }
        allSubsets.push_back(subset);
    }

    return allSubsets;
}

void SetOperations::printSet(const vector<string>& S, const string& name)
{
    cout << name << " = { ";
    for (int i = 0; i < S.size();i++) 
    {
        cout << S[i];
        if (i + 1 < S.size()) 
        {
            cout << ", ";
        }
    }
    cout << " }\n";
}
