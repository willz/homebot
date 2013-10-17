#ifndef INCLUDED_PLANNER_H
#define INCLUDED_PLANNER_H

#include <vector>
#include "domain.h"
#include <string>

class Planner {
public:
    //bool Plan(std::vector<>& plan);
    void Plan();
    void SetDomain(const Domain& domain);

private:
    void _BuildGraph();
    Domain _domain;
    std::vector<unsigned> _locs; // locations
    std::vector<unsigned> _smallObjs;
    std::vector<unsigned> _bigObjs;
};

bool IDA_STAR();
int CalcH();
bool DFS(int g, int limit, int step, Op& preOp, int& newLimit);
bool IsGoalReached();

void AnalyzeInfo();
bool AnalyzeTask();
void GetObjsWithoutPos(const std::list<unsigned>& src, std::list<unsigned>& ret);
extern unsigned gObjToSense;
void RefreshEnv(const std::string& str);

extern Op ops[100];
extern int opsnum;

// debug function
void ShowState();
void ShowPlan();

#endif
