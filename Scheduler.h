#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <string>
#include <vector>
#include <map>

using namespace std;
class Scheduler 
{
private:
    vector<string> courses;                      // all course codes
    map<string, vector<string>> prerequisites;   // course -> its prereqs
    bool hasCycleUtil(const string& course,map<string, int>& state) const;
    void topoSortUtil(const string& course,map<string, bool>& visited,vector<string>& order) const;
public:
    Scheduler();
    vector<pair<string, string>> getAllPrerequisitePairs() const;
    void addCourse(const string& code);
    void addPrerequisite(const string& course, const string& prereq);
    bool canTakeCourse(const vector<string>& completed,const string& course) const;
    vector<string> generateSchedule() const;
    void printSchedule(const vector<string>& schedule) const;
    map<string, vector<string>> getPrerequisiteMap() const;

};
#endif
