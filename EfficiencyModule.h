#ifndef EFFICIENCY_MODULE_H
#define EFFICIENCY_MODULE_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class University;
class Scheduler;

class EfficiencyModule {
public:
    // Benchmark 1:
    // Compare plain recursive DFS vs memoized DFS
    // for traversing the prerequisite graph.
    static void benchmarkPrerequisiteDFS(const Scheduler& scheduler);

    // Benchmark 2:
    // Use integer bitmasks to check student course conflicts efficiently.
    // Shows how bit manipulation speeds up set operations conceptually.
    static void benchmarkConflictBitmask(const University& uni);
};

#endif