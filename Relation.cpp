#include "Relations.h"

void Relations::printRelation(const vector<pair<string, string>>& R, const string& name)
{
    cout << "\nRelation " << name << " = { ";
    for (auto& p : R)
        cout << "(" << p.first << ", " << p.second << ") ";
    cout << "}\n";
}

bool Relations::isReflexive(const vector<pair<string, string>>& R, const vector<string>& elements)
{
    for (auto& x : elements)
    {
        bool found = false;
        for (auto& p : R)
            if (p.first == x && p.second == x)
                found = true;

        if (!found) return false;
    }
    return true;
}

bool Relations::isSymmetric(const vector<pair<string, string>>& R)
{
    for (auto& p : R)
    {
        bool found = false;
        for (auto& q : R)
            if (q.first == p.second && q.second == p.first)
                found = true;

        if (!found) return false;
    }
    return true;
}

bool Relations::isAntisymmetric(const vector<pair<string, string>>& R)
{
    for (auto& p : R)
    {
        for (auto& q : R)
        {
            if (p.first == q.second && p.second == q.first)
            {
                if (p.first != p.second)  // xRy AND yRx but x ≠ y
                    return false;
            }
        }
    }
    return true;
}

bool Relations::isTransitive(const vector<pair<string, string>>& R)
{
    for (auto& p : R)
    {
        for (auto& q : R)
        {
            if (p.second == q.first)
            {
                bool found = false;
                for (auto& r : R)
                    if (r.first == p.first && r.second == q.second)
                    {
                        found = true;
                        break;
                    }
                if (!found) return false;
            }
        }
    }
    return true;
}

bool Relations::isEquivalence(const vector<pair<string, string>>& R, const vector<string>& elements)
{
    return isReflexive(R, elements) &&
        isSymmetric(R) &&
        isTransitive(R);
}

bool Relations::isPartialOrder(const vector<pair<string, string>>& R, const vector<string>& elements)
{
    return isReflexive(R, elements) &&
        isAntisymmetric(R) &&
        isTransitive(R);
}

vector<pair<string, string>> Relations::compose(
    const vector<pair<string, string>>& R,
    const vector<pair<string, string>>& S)
{
    vector<pair<string, string>> result;

    for (auto& a : R)
    {
        for (auto& b : S)
        {
            if (a.second == b.first)
                result.push_back({ a.first, b.second });
        }
    }
    return result;
}
