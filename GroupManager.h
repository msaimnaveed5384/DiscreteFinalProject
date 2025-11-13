#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H
#include <string>
#include <vector>

using namespace std;
class Group 
{
private:
    int groupId;
    vector<string> studentIds; // IDs of students in this group
    string facultyId;          // ID of assigned faculty advisor

public:
    Group(int id = 0);

    int getGroupId() const;
    const vector<string>& getStudentIds() const;
    string getFacultyId() const;

    void addStudent(const string& studentId);
    void setFacultyId(const string& id);
};

class GroupManager 
{
private:
    vector<Group> groups;

    Group* findGroup(int groupId);
    const Group* findGroup(int groupId) const;

public:
    GroupManager();

    void createGroup(int groupId);
    void addStudentToGroup(int groupId, const string& studentId);
    void assignFacultyToGroup(int groupId, const string& facultyId);
    void listGroups() const;
};
#endif
