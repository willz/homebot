#include "planner.h"
#include <cstdint>
#include <set>
#include <string>
#include <cstdio>
#include <iostream>

using namespace std;

/*
bool Planner::Plan(vector<>& plan) {
    return true;
}
*/
void Planner::Plan() {
    IDA_STAR();
}

set<unsigned> small_objs;
set<unsigned> big_objs;
set<unsigned> locs;
State cur_state;
Op ops[100];
int opsnum;

bool IDA_STAR() {
    small_objs.clear();
    big_objs.clear();
    locs.clear();
    cur_state = gInitState;
    for (const auto& i : gTasks) {
        for (auto x : i.arg1) {
            locs.insert(gInitState.pos[x]);
            if (i.type == T_PICKUP || i.type == T_PUTDOWN ||
                i.type == T_PUTIN || i.type == T_TAKEOUT ||
                i.type == T_PUTON) {
                small_objs.insert(x);
            } else {
                big_objs.insert(x);
            }
        }
        if (i.type == T_GIVE || i.type == T_PUTON || i.type == T_PUTIN ||
            i.type == T_TAKEOUT) {
            for (auto x : i.arg2) {
                locs.insert(gInitState.pos[x]);
                if (i.type == T_GIVE) {
                    small_objs.insert(x);
                } else {
                    big_objs.insert(x);
                }
            }
        }
    }
    cout << "small objs: ";
    for (auto i : small_objs) {
        cout << i << " ";
    }
    cout << endl << "  big objs: ";
    for (auto i : big_objs) {
        cout << i << " ";
    }
    cout << endl;
    cout << "hold: " << gInitState.hold << endl;

    cout << "inside: " << endl;
    for (const auto& i : gInitState.inside) {
        cout << i.first << ": ";
        for (auto j : i.second) {
            cout << j << " ";
        }
        cout << endl;
    }

    Op op;
    op.op = (ACTION)0;
    int limit = CalcH();
    while (true) {
        int newLimit;
        cur_state = gInitState;
        if (DFS(0, limit, 0, op, newLimit)) {
            cout << "Find Ans!" << endl;
            ShowPlan();
            return true;
        }
        limit = newLimit;
    }
    return false;
}

int CalcH() {
    return 0;
}

bool DFS(int g, int limit, int step, Op& preOp, int& newLimit) {
    newLimit = 0x7fffffff;
    int cost = g + CalcH();
    // judge cost before call IsGoalReached,
    // because may be the last operation violate the constrain and
    // get much cost
    if (cost > limit) {
        newLimit = cost;
        return false;
    }
    if (IsGoalReached()) {
        opsnum = step;
        return true;
    }
    for (int op = 1; op <= 9; ++op) {
        if (preOp.op == op) {
            continue;
        }
        int nextCost = 0x7fffffff;
        ops[step].op = ACTION(op);
        auto& pos = cur_state.pos;
        if (op == MOVE) {
            // plate and move, move and plate is the same
            if (preOp.op == FROMPLATE || preOp.op == TOPLATE) {
                continue;
            }
            for (auto loc : locs) {
                if (loc == pos[0]) {
                    continue;
                }
                unsigned tmp = pos[0];
                pos[0] = loc;
                // TODO and unknown:
                if (cur_state.hold) {
                    pos[cur_state.hold] = loc;
                }
                if (cur_state.plate) {
                    pos[cur_state.plate] = loc;
                }
                ops[step].arg1 = loc;
                if (DFS(g + 4, limit, step + 1, ops[step], nextCost)) {
                    return true;
                }
                if (nextCost < newLimit) {
                    newLimit = nextCost;
                }

                // restore
                pos[0] = tmp;
                if (cur_state.hold) {
                    pos[cur_state.hold] = tmp;
                }
                if (cur_state.plate) {
                    pos[cur_state.plate] = tmp;
                }
            }
        } else if (op == PICKUP) {
            if (cur_state.hold) {
                continue;
            }
            for (auto i : small_objs) {
                if (preOp.op == PUTDOWN && preOp.arg1 == i) {
                    continue;
                }
                if (cur_state.plate == i) {
                    continue;
                }
                bool is_inside = false;
                for (const auto& c : cur_state.inside) {
                    if (c.second.count(i)) {
                        is_inside = true;
                        break;
                    }
                }
                if (is_inside) {
                    continue;
                }
                if (pos[0] == pos[i]) {
                    cur_state.hold = i;
                    ops[step].arg1 = i;
                    if (DFS(g + 2, limit, step + 1, ops[step], nextCost)) {
                        return true;
                    }
                    if (nextCost < newLimit) {
                        newLimit = nextCost;
                    }
                    cur_state.hold = 0;
                }
            }
        } else if (op == PUTDOWN) {
            if (cur_state.hold == 0) {
                continue;
            }
            if (preOp.op == PICKUP && preOp.arg1 == cur_state.hold) {
                continue;
            }
            unsigned tmp = cur_state.hold;
            cur_state.hold = 0;
            ops[step].arg1 = tmp;
            int cur_cost = 2;
            if (gNotPutdown.count(tmp)) {
                cur_cost += 10;
            }
            if (DFS(g + cur_cost, limit, step + 1, ops[step], nextCost)) {
                return true;
            }
            if (nextCost < newLimit) {
                newLimit = nextCost;
            }
            cur_state.hold = tmp;
        } else if (op == TOPLATE) {
            if (preOp.op == FROMPLATE) {
                continue;
            }
            if (cur_state.hold && !cur_state.plate) {
                ops[step].arg1 = cur_state.hold;
                cur_state.plate = cur_state.hold;
                cur_state.hold = 0;
                if (DFS(g + 2, limit, step + 1, ops[step], nextCost)) {
                    return true;
                }
                if (nextCost < newLimit) {
                    newLimit = nextCost;
                }
                cur_state.hold = cur_state.plate;
                cur_state.plate = 0;
            }
        } else if (op == FROMPLATE) {
            if (preOp.op == TOPLATE) {
                continue;
            }
            if (!cur_state.hold && cur_state.plate) {
                ops[step].arg1 = cur_state.plate;
                cur_state.hold = cur_state.plate;
                cur_state.plate = 0;
                if (DFS(g + 2, limit, step + 1, ops[step], nextCost)) {
                    return true;
                }
                if (nextCost < newLimit) {
                    newLimit = nextCost;
                }
                cur_state.plate = cur_state.hold;
                cur_state.hold = 0;
            }
        } else if (op == OPEN) {
            if (preOp.op == CLOSE || cur_state.hold) {
                continue;
            }
            for (auto i : gContainers) {
                // same loc and not open
                if (pos[i] == pos[0] && cur_state.doorOpen.count(i) == 0) {
                    cur_state.doorOpen.insert(i);
                    ops[step].arg1 = i;
                    if (DFS(g + 2, limit ,step + 1, ops[step], nextCost)) {
                        return true;
                    }
                    if (nextCost < newLimit) {
                        newLimit = nextCost;
                    }
                    cur_state.doorOpen.erase(i);
                }
            }
        } else if (op == CLOSE) {
            if (preOp.op == OPEN || cur_state.hold) {
                continue;
            }
            for (auto i : gContainers) {
                // same loc and not open
                if (pos[i] == pos[0] && cur_state.doorOpen.count(i)) {
                    cur_state.doorOpen.erase(i);
                    ops[step].arg1 = i;
                    if (DFS(g + 2, limit ,step + 1, ops[step], nextCost)) {
                        return true;
                    }
                    if (nextCost < newLimit) {
                        newLimit = nextCost;
                    }
                    cur_state.doorOpen.insert(i);
                }
            }

        } else if (op == PUTIN) {
            if (!cur_state.hold) {
                continue;
            }
            for (auto c : gContainers) {
                if (pos[c] == pos[0] && cur_state.doorOpen.count(c)) {
                    if (cur_state.inside.count(c) == 0) {
                        cur_state.inside.insert(make_pair(c, set<unsigned>()));
                    }
                    unsigned tmp = cur_state.hold;
                    if (tmp > 1000) {
                        cout << "PUTIN INSIDE" << endl;
                    }
                    cur_state.inside[c].insert(tmp);
                    cur_state.hold = 0;
                    ops[step].arg1 = tmp;
                    ops[step].arg2 = c;

                    if (DFS(g + 2, limit ,step + 1, ops[step], nextCost)) {
                        return true;
                    }
                    if (nextCost < newLimit) {
                        newLimit = nextCost;
                    }

                    cur_state.hold = tmp;
                    cur_state.inside[c].erase(tmp);
                }
            }
        } else if (op == TAKEOUT) {
            if (cur_state.hold) {
                continue;
            }
            for (auto c : gContainers) {
                if (pos[c] == pos[0] && cur_state.doorOpen.count(c) && cur_state.inside.count(c)) {
                    auto inside_objs = cur_state.inside[c];
                    for (auto i : inside_objs) {
                        cur_state.hold = i;
                        cur_state.inside[c].erase(i);
                        ops[step].arg1 = i;
                        ops[step].arg2 = c;
                        if (DFS(g + 2, limit ,step + 1, ops[step], nextCost)) {
                            return true;
                        }
                        if (nextCost < newLimit) {
                            newLimit = nextCost;
                        }
                        cur_state.inside[c].insert(cur_state.hold);
                        if (cur_state.hold > 1000) {
                            cout << "TAKEOUT INSIDE" << endl;
                        }
                        cur_state.hold = 0;
                    }
                }
            }
        }
    }
    return false;
}

bool IsGoalReached() {
    auto& pos = cur_state.pos;
    for (const auto& i : gTasks) {
        bool found = false;
        if (i.type == T_GOTO) {
            // TODO: only one big object?
            if (pos[0] != i.arg1.front()) {
                return false;
            }
        } else if (i.type == T_GIVE) {
            unsigned human = i.arg1.front();
            for (auto x : i.arg2) {
                if (pos[x] == pos[human] && cur_state.hold != x && cur_state.plate != x) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        } else if (i.type == T_PUTON) {
            unsigned big_obj = i.arg2.front();
            for (auto x : i.arg1) {
                if (pos[x] == pos[big_obj] && cur_state.hold != x && cur_state.plate != x) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        } else if (i.type == T_PUTDOWN) {
            for (auto x : i.arg1) {
                if (cur_state.hold != x && cur_state.plate != x) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        } else if (i.type == T_PICKUP) {
            for (auto x : i.arg1) {
                if (cur_state.hold == x || cur_state.plate == x) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        } else if (i.type == T_OPEN) {
            for (auto x : i.arg1) {
                if (cur_state.doorOpen.count(x)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        } else if (i.type == T_CLOSE) {
            for (auto x : i.arg1) {
                if (cur_state.doorOpen.count(x) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        } else if (i.type == T_PUTIN) {
            unsigned big_obj = i.arg2.front();
            for (auto x : i.arg1) {
                if (cur_state.inside.count(big_obj) && cur_state.inside[big_obj].count(x)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        } else if (i.type == T_TAKEOUT) {
            unsigned big_obj = i.arg2.front();
            for (auto x : i.arg1) {
                if (cur_state.inside.count(big_obj) == 0 || cur_state.inside[big_obj].count(x) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    return true;
}

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


void AnalyzeInfo() {
    bool changed = true;
    auto& info = gInitState.info;
    auto& pos = gInitState.pos;
    unsigned bigObj;
    while (!info.empty() && changed) {
        changed = false;
        for (auto it = info.begin(); it != info.end(); ) {
            switch (it->type) {
            case I_OPENED:
                changed = true;
                gInitState.doorOpen.insert(it->arg1.front());
                it = info.erase(it);
                break;
            case I_CLOSED:
                // TODO: can do nothing?
                break;
            case I_PLATE:
                for (auto it1 = it->arg1.begin(); it1 != it->arg1.end(); ) {
                    // known and not at robot
                    if (pos[*it1] != UNKNOWN_LOC && pos[*it1] != pos[0]) {
                        it1 = it->arg1.erase(it1);
                    } else {
                        ++it1;
                    }
                }
                if (it->arg1.size() == 1) {
                    changed = true;
                    gInitState.plate = it->arg1.front();
                    pos[gInitState.plate] = pos[0];
                    it = info.erase(it);
                }
                break;
            case I_NEAR:
                bigObj = it->arg2.front();
                for (auto it1 = it->arg1.begin(); it1 != it->arg1.end(); ) {
                    if (pos[*it1] != UNKNOWN_LOC && pos[*it1] != pos[bigObj]) {
                        it1 = it->arg1.erase(it1);
                    } else {
                        ++it1;
                    }
                }
                // remaning is the same place
                if (it->arg1.size() == 1) {
                    changed = true;
                    pos[it->arg1.front()] = pos[bigObj];
                    it = info.erase(it);
                }
                break;
            case I_ON:
                bigObj = it->arg2.front();
                for (auto it1 = it->arg1.begin(); it1 != it->arg1.end(); ) {
                    // not at the bigobj pos or in plate or hand
                    if ((pos[*it1] != UNKNOWN_LOC && pos[*it1] != pos[bigObj]) ||
                        *it1 == gInitState.plate || *it1 == gInitState.hold) {
                        it1 = it->arg1.erase(it1);
                    } else {
                        ++it1;
                    }
                }
                // remaning is the same place
                if (it->arg1.size() == 1) {
                    changed = true;
                    pos[it->arg1.front()] = pos[bigObj];
                    it = info.erase(it);
                }
                break;
            case I_INSIDE:
                bigObj = it->arg2.front();
                for (auto it1 = it->arg1.begin(); it1 != it->arg1.end(); ) {
                    if (pos[*it1] != UNKNOWN_LOC && pos[*it1] != pos[bigObj]) {
                        it1 = it->arg1.erase(it1);
                    } else {
                        ++it1;
                    }
                }
                // remaning is the same place
                if (it->arg1.size() == 1) {
                    changed = true;
                    pos[it->arg1.front()] = pos[bigObj];
                    if (gInitState.inside.count(bigObj) == 0) {
                        gInitState.inside.insert(make_pair(bigObj, set<unsigned>()));
                    }
                    cout << "arg1.front() " << it->arg1.front() << endl;
                    gInitState.inside[bigObj].insert(it->arg1.front());
                    pos[it->arg1.front()] = pos[bigObj];
                    it = info.erase(it);
                }
            }

            if (!changed) {
                ++it;
            } else {
                break;
            }
        }
    }
}

bool gStopAnalyzeTask;
unsigned gObjToSense;  // If goal cannot reach, sense this object
bool AnalyzeTask() {
    gStopAnalyzeTask = false;
    // If there is one task having no object(pos not known) to do with, 
    // just sense
    map<unsigned, unsigned> no_pos_objs;  // <id, count>
    for (auto t : gTasks) {
        // goto, close, open no need to analyze, those can be done.
        if (t.type == T_GOTO || t.type == T_CLOSE || t.type == T_OPEN) {
            continue;
        }
        list<unsigned> ret;
        bool can_done;
        if (t.type == T_GIVE) {
            GetObjsWithoutPos(t.arg2, ret);
            can_done = (t.arg2.size() == ret.size()) ? false : true;
        } else {
            GetObjsWithoutPos(t.arg1, ret);
            can_done = (t.arg1.size() == ret.size()) ? false : true;
        }
        if (!can_done) {
            for (auto i : ret) {
                if (no_pos_objs.count(i) == 0) {
                    no_pos_objs.insert(make_pair(i, 1));
                } else {
                    no_pos_objs[i]++;
                }
            }
        }
    }
    if (!no_pos_objs.empty()) {
        pair<unsigned, unsigned> max_item = *no_pos_objs.begin();
        for (auto i : no_pos_objs) {
            if (i.second > max_item.second) {
                max_item = i;
            }
        }
        gObjToSense = max_item.first;
        return false;
    }
    // Well, now all tasks have at least one object to choose. But,
    // the object may not choose because of some constrains, or,
    // there is competition between two task on the same object
    return true;
}

inline void GetObjsWithoutPos(const list<unsigned>& src, list<unsigned>& ret) {
    for (auto i : src) {
        if (gInitState.pos[i] == UNKNOWN_LOC) {
            ret.push_back(i);
        }
    }
}

void DFSAnalyzeTask(int task_idx) {
    if (gStopAnalyzeTask || task_idx == (int)gTasks.size()) {
        return;
    }
    auto& task = gTasks[task_idx];
    // goto, close, open no need to analyze, those can be done.
    if (task.type == T_GIVE) {
        
    }
}

void RefreshEnv(const string& str) {
    Cons info;
    unsigned id1, id2;
    if (str[0] == 'a') {
        // at
        sscanf(str.substr(3).c_str(), "%u,%u", &id1, &id2);
        gInitState.pos[id1] = id2;
    }
    if (str == "unknown") {
        // TODO
    } else if (str[0] == 'o') {
        // on
        info.type = I_ON;
        sscanf(str.substr(2).c_str(), "%u %u", &id1, &id2);
    } else if (str[0] == 'n') {
        info.type = I_NEAR;
        sscanf(str.substr(4).c_str(), "%u %u", &id1, &id2);
    } else if (str[0] == 'i') {
        info.type = I_INSIDE;
        sscanf(str.substr(7).c_str(), "%u,%u", &id1, &id2);
        cout << str.substr(7) << endl;
        cout << "Get inside " << id1 << " " << id2 << endl;
    }
    info.arg1.push_back(id1);
    info.arg2.push_back(id2);
    gInitState.info.push_back(info);
    AnalyzeInfo();
}


void ShowState() {
    fprintf(stdout, "obj: ");
    for (int i = 0; i <= 10; ++i) {
        fprintf(stdout, "%4d ", i);
    }
    fprintf(stdout, "\nloc: ");
    for (int i = 0; i <= 10; ++i) {
        fprintf(stdout, "%4d ", gInitState.pos[i]);
    }
    fprintf(stdout, "\n");
}

void ShowPlan() {
    cout  << "opsnum: " << opsnum << endl;
    for (int i = 0; i < opsnum; ++i) {
        cout << ops[i].op << " " << ops[i].arg1 << endl;
    }
}
