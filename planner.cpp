#include "planner.h"
#include <cstdint>
#include <set>

using namespace std;

/*
bool Planner::Plan(vector<>& plan) {
    return true;
}
*/

void Planner::SetDomain(const Domain& domain) {
    _domain = domain;
}

// action:    [0-5]       [6-18]    [19-31]
//         action type    param 1   param 2
// state:     [0]         [1-5]     [6-18]    [19-31]
//       pos(0)/neg(1)  state type  param 1   param 2
void Planner::_BuildGraph() {
    vector<set<uint32_t> > layers;
    // init layer 0
    int l = 1; // start from layer 1
    set<uint32_t> actionUsed;
    while (true) {
        layers.push_back(set<uint32_t>()); // action layer
        layers.push_back(layers[l - 1]); // state layer
        // move
        for (size_t i = 0; i < _locs.size(); ++i) {
            uint32_t pre = 1 + (AT << 1) + (_locs[i] << 6);
            if (layers[l - 1].count(pre)) {
                uint32_t action = MOVE + (_locs[i] << 6);
                layers[l].insert(action);
                // add effect
                layers[l + 1].insert(pre - 1);
            }
        }
        // pickup
        for (size_t i = 0; i < _smallObjs.size(); ++i) {

        }
        // putdown
        for (size_t i = 0; i < _smallObjs.size(); ++i) {
            uint32_t pre = (HOLD << 1) + (_smallObjs[i] << 6);
            if (layers[l - 1].count(pre)) {
                uint32_t action = PUTDOWN + (_smallObjs[i] << 6);
                layers[l].insert(action);
                // add effect
                layers[l + 1].insert(HOLD << 1);
            }
        }
        // toplate
        for (size_t i = 0; i < _smallObjs.size(); ++i) {
            uint32_t pre = (HOLD << 1) + (_smallObjs[i] << 6);
            if (layers[l - 1].count(pre) && layers[l - 1].count(PLATE << 1)) {
                uint32_t action = TOPLATE + (_smallObjs[i] << 6);
                layers[l].insert(action);
                layers[l + 1].insert(HOLD << 1);
                layers[l + 1].insert((PLATE << 1) + (_smallObjs[i] << 6));
            }
        }
        // fromplate
        for (size_t i = 0; i < _smallObjs.size(); ++i) {
            uint32_t pre = (PLATE << 1) + (_smallObjs[i] << 6);
            if (layers[l - 1].count(pre) && layers[l - 1].count(HOLD << 1)) {
                uint32_t action = FROMPLATE + (_smallObjs[i] << 6);
                layers[l].insert(action);
                layers[l + 1].insert(PLATE << 1);
                layers[l + 1].insert((HOLD << 1) + (_smallObjs[i] << 6));
            }
        }
        // open
        for (size_t i = 0; i < _bigObjs.size(); ++i) {
            //uint32_t pos = GetObjPos(_bigObjs[i]);
            // TODO !!!!
            uint32_t pos = 0;
            uint32_t pre1 = (OPENED << 1) + (pos << 6);
            uint32_t pre2 = (AT << 1) + (pos << 19);
            if (layers[l - 1].count(pre1) && layers[l - 1].count(pre2)) {
                uint32_t action = OPEN + (_bigObjs[i] << 6);
                layers[l].insert(action);
                layers[l + 1].insert((CLOSED << 1) + (_bigObjs[i] << 6));
            }
        }
        // close
        // putin
        // takeout

        if (layers[l - 1].size() == layers[l + 1].size()) {
            // level off
            break;
        }
        l += 2;
    }
}
