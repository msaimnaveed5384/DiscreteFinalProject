#include "GroupManager.h"
#include <iostream>

using namespace std;

Group::Group(int id) 
{
    groupId = id;
    facultyId = "";
}
int Group::getGroupId() const 
{
    return groupId;
}

const vector<string>& Group::getStudentIds() const 
{
    return studentIds;
}

string Group::getFacultyId() const 
{
    return facultyId;
}

void Group::addStudent(const string& studentId) 
{
    // avoid duplicates
    for (const string& id : studentIds) 
    {
        if (id == studentId) 
        {
            return;
        }
    }
    studentIds.push_back(studentId);
}

void Group::setFacultyId(const string& id) 
{
    facultyId = id;
}

GroupManager::GroupManager() 
{
    // nothing for now
}

Group* GroupManager::findGroup(int groupId) 
{
    for (Group& g : groups) 
    {
        if (g.getGroupId() == groupId) 
        {
            return &g;
        }
    }
    return nullptr;
}

const Group* GroupManager::findGroup(int groupId) const
{
    for (const Group& g : groups) 
    {
        if (g.getGroupId() == groupId)
        {
            return &g;
        }
    }
    return nullptr;
}


void GroupManager::createGroup(int groupId) 
{
    // avoid duplicate group IDs
    if (findGroup(groupId) != nullptr) 
    {
        cout << "Group with this ID already exists\n";
        return;
    }
    groups.push_back(Group(groupId));
    cout << "Group " << groupId << " created\n";
}

void GroupManager::addStudentToGroup(int groupId, const string& studentId) 
{
    Group* g = findGroup(groupId);
    if (g == nullptr)
    {
        cout << "Group not found\n";
        return;
    }

    g->addStudent(studentId);
    cout << "Student " << studentId << " added to group " << groupId << ".\n";
}

void GroupManager::assignFacultyToGroup(int groupId, const string& facultyId) 
{
    Group* g = findGroup(groupId);
    if (g == nullptr) 
    {
        cout << "Group not found!\n";
        return;
    }

    g->setFacultyId(facultyId);
    cout << "Faculty " << facultyId << " assigned to group " << groupId << ".\n";
}

void GroupManager::listGroups() const 
{
    if (groups.empty()) 
    {
        cout << "No groups created yet\n";
        return;
    }

    cout << "Groups:"<<endl;
    for (const Group& g : groups) {
        cout << "Group ID: " << g.getGroupId() << endl;
        cout << "  Students: ";
        const vector<string>& ids = g.getStudentIds();
        if (ids.empty()) 
        {
            cout << "None";
        }
        else
        {
            for (const string& id : ids) 
            {
                cout << id << " ";
            }
        }
        cout <<endl<< "  Faculty: ";
        if (g.getFacultyId().empty()) 
        {
            cout << "None";
        }
        else 
        {
            cout << g.getFacultyId();
        }
        cout << endl;
    }
}


// Generate all combinations of students of a given size
void GroupManager::generateStudentCombinations(const vector<string>& studentIds,int groupSize) const 
{
    if (studentIds.empty()) 
    {
        cout << "No students available to form groups\n";
        return;
    }

    if (groupSize <= 0 || groupSize > (int)studentIds.size()) 
    {
        cout << "Invalid group size. It must be between 1 and " << studentIds.size() << ".\n";
        return;
    }

    vector<string> current;
    int count = 0;

    cout << "All possible groups of size " << groupSize << ":\n";
    generateCombUtil(studentIds, groupSize, 0, current, count);

    cout << "Total number of possible groups C("
        << studentIds.size() << ", " << groupSize << ") = "
        << count << "\n";
}

void GroupManager::generateCombUtil(const vector<string>& ids,int groupSize,int start,vector<string>& current,int& count) const {
    // If we formed a full group, print it
    if ((int)current.size() == groupSize) 
    {
        cout << "{ ";
        for (const string& id : current)
        {
            cout << id << " ";
        }
        cout << "}"<<endl;
        count++;
        return;
    }

    // If remaining elements are not enough to complete the group, stop
    if (start >= (int)ids.size()) 
    {
        return;
    }

    for (int i = start; i < (int)ids.size(); i++) 
    {
        current.push_back(ids[i]);                        // choose this student
        generateCombUtil(ids, groupSize, i + 1, current, count); // recurse
        current.pop_back();                              // backtrack
    }
}
