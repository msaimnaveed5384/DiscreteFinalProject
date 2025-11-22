#include "Scheduler.h"
#include <iostream>
using namespace std;

Scheduler::Scheduler() {
    //nothing for now
}

void Scheduler::addCourse(const string& code) 
{
    //avoid duplicate courses
    for (const string& c : courses) 
    {
        if (c == code) 
        {
			/*cout << "Course already exists: " << code << endl;*/
            return;
        }
    }
    courses.push_back(code);
}

void Scheduler::addPrerequisite(const string& course, const string& prereq) 
{
    addCourse(course);   // make sure both exist
    addCourse(prereq);

    // avoid duplicate prereq entry
    vector<string>& prereqList = prerequisites[course];
    for (const string& p : prereqList)
    {
        if (p == prereq)
        {
			cout << "Prerequisite already exists: " << prereq << " for course " << course << endl;
            return;
        }
    }
    prereqList.push_back(prereq);
}


bool Scheduler::canTakeCourse(const vector<string>& completed,const string& course) const 
{
    auto it = prerequisites.find(course);
    if (it == prerequisites.end()) 
    {
        // no prerequisites
        return true;
    }

    const vector<string>& prereqList = it->second;

    for (const string& prereq : prereqList) 
    {
        bool found = false;
        for (const string& c : completed) 
        {
            if (c == prereq) {
                found = true;
                break;
            }
        }
        if (!found) 
        {
            return false; // some prereq missing
        }
    }

    return true;
}


bool Scheduler::hasCycleUtil(const string& course,map<string, int>& state) const 
{
    // state: 0 = unvisited, 1 = visiting, 2 = done
    auto it = state.find(course);
    if (it != state.end() && it->second == 1) 
    {
        // back edge -> cycle
        return true;
    }

    if (it != state.end() && it->second == 2) 
    {
        // already processed, no cycle from here
        return false;
    }

    state[course] = 1; // visiting

    auto pit = prerequisites.find(course);
    if (pit != prerequisites.end()) 
    {
        const vector<string>& prereqList = pit->second;
        for (const string& prereq : prereqList) 
        {
            if (hasCycleUtil(prereq, state)) 
            {
                return true;
            }
        }
    }

    state[course] = 2; // done
    return false;
}

void Scheduler::topoSortUtil(const string& course,map<string, bool>& visited,vector<string>& order) const {
    if (visited[course]) 
    {
        return;
    }
    visited[course] = true;

    auto it = prerequisites.find(course);
    if (it != prerequisites.end())
    {
        const vector<string>& prereqList = it->second;
        for (const string& prereq : prereqList) 
        {
            topoSortUtil(prereq, visited, order);
        }
    }
    order.push_back(course);
}


vector<string> Scheduler::generateSchedule() const 
{
    vector<string> order;

    // 1) check for cycles
    map<string, int> state;
    for (const string& c : courses) 
    {
        if (hasCycleUtil(c, state)) 
        {
            // cycle found, invalid plan
            return vector<string>(); // empty
        }
    }

    // 2) no cycle → topological order
    map<string, bool> visited;
    for (const string& c : courses)
    {
        visited[c] = false;
    }

    for (const string& c : courses) 
    {
        if (!visited[c]) 
        {
            topoSortUtil(c, visited, order);
        }
    }
    return order;
}
void Scheduler::printSchedule(const vector<string>& schedule) const 
{
    if (schedule.empty()) 
    {
        cout << "No valid schedule (possible cycle in prerequisites).\n";
        return;
    }

    cout << "Valid course order:\n";
    for (const string& c : schedule) 
    {
        cout << c << "\n";
    }
}

map<string, vector<string>> Scheduler::getPrerequisiteMap() const 
{
    return prerequisites;
}


vector<pair<string, string>> Scheduler::getAllPrerequisitePairs() const {
    vector<pair<string, string>> result;
    for (const auto& entry : prerequisites) {
        const string& course = entry.first;
        const vector<string>& prereqs = entry.second;
        for (const string& p : prereqs) {
            result.push_back({ course, p }); 
        }
    }
    return result;
}