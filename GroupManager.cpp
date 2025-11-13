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
