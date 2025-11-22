#include "PrerequisiteChecker.h"

using namespace std;

PrerequisiteChecker::PrerequisiteChecker(const map<string, vector<string>>& prereqMap) 
{
    prereq = prereqMap;
}

//recursive verification
bool PrerequisiteChecker::canReachBaseCourse(const string& current,const string& base,vector<string>& path) const 
{
    path.push_back(current);
    if (current == base) 
    {
        return true;
    }

    auto it = prereq.find(current);
    if (it == prereq.end()) 
    {
        path.pop_back();
        return false;
    }

    const vector<string>& prereqList = it->second;

    // Strong induction style: we explore all indirect prerequisites recursively
    for (const string& p : prereqList) 
    {
        if (canReachBaseCourse(p, base, path))
        {
            return true;
        }
    }

    // Backtrack if this path does not reach the base
    path.pop_back();
    return false;
}

bool PrerequisiteChecker::proveByInduction(const string& baseCourse,const string& targetCourse) const 
{
    cout << "=============================================\n";
    cout << " Induction / Strong Induction Proof\n";
    cout << " Base course:   " << baseCourse << "\n";
    cout << " Target course: " << targetCourse << "\n";
    cout << "=============================================\n\n";

    vector<string> path;
    bool ok = canReachBaseCourse(targetCourse, baseCourse, path);

    if (!ok) 
    {
        cout << "Result: The student CANNOT take \"" << targetCourse << "\" "
            << "starting from base course \"" << baseCourse << "\".\n\n";
        cout << "Reason (Strong Induction Check):\n";
        cout << "- There is no complete prerequisite chain from "
            << targetCourse << " down to " << baseCourse << ".\n";
        cout << "- Some direct or indirect prerequisite is missing.\n\n";
        return false;
    }

    // path currently is [target, ..., base] – reverse it to get base → ... → target
    vector<string> chain(path.rbegin(), path.rend());

    // Step 1: Base case
    cout << "Step 1: Base Case\n";
    cout << "We assume the student has completed the base course \""
        << baseCourse << "\".\n\n";

    // Step 2: Strong induction hypothesis
    cout << "Step 2: Strong Induction Hypothesis\n";
    cout << "Assume the student has completed ALL courses up to some level k in the chain.\n";
    cout << "In this example, the full prerequisite chain is:\n  ";
    for (int i = 0; i < chain.size();i++) 
    {
        cout << chain[i];
        if (i + 1 < chain.size()) 
        {
            cout << "  ->  ";
        }
    }
    cout << endl<<endl;

    // Step 3: Induction step – verify each link in the chain
    cout << "Step 3: Induction Step (Recursive Verification)\n";
    for (int i = 1; i < chain.size(); i++) 
    {
        cout << "- To take \"" << chain[i] << "\", ";
        cout << "the student must have already completed \"" << chain[i - 1] << "\".\n";
    }
    cout << "\nEach course in the chain depends on the previous one, "
        << "so by strong induction the student can reach the target course.\n\n";

    // Conclusion
    cout << "Conclusion:\n";
    cout << "If the student has completed \"" << baseCourse
        << "\" and all intermediate courses in the chain,\n";
    cout << "then they are allowed to take the advanced course \""
        << targetCourse << "\".\n\n";

    return true;
}
