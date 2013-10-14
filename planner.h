#ifndef INCLUDED_PLANNER_H
#define INCLUDED_PLANNER_H

#include <vector>

class Planner {
public:
    bool Plan(std::vector<>& plan);

private:
    void _BuildGraph();
    std::vector<unsigned> _locs; // locations
    std::vector<unsigned> _smallObjs;
    std::vector<unsigned> _bigObjs;
};

#endif
