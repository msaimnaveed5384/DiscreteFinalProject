#ifndef PREREQUISITECHECKER_H
#define PREREQUISITECHECKER_H

#include<string>
#include<vector>
#include<map>
#include<iostream>

using namespace std;

class PrerequisiteChecker 
{
private:
    map<string, vector<string>> prereq;

    bool canReachBaseCourse(const string& current,const string& base,vector<string>& path) const;

public:
    PrerequisiteChecker(const map<string, vector<string>>& prereqMap);

    bool proveByInduction(const string& baseCourse,const string& targetCourse) const;
};
#endif
