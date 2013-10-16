#ifndef INCLUDED_PLANNER_H
#define INCLUDED_PLANNER_H

#include <vector>
#include "domain.h"

class Planner {
public:
    //bool Plan(std::vector<>& plan);
    void SetDomain(const Domain& domain);

private:
    void _BuildGraph();
    Domain _domain;
    std::vector<unsigned> _locs; // locations
    std::vector<unsigned> _smallObjs;
    std::vector<unsigned> _bigObjs;
};

#endif
