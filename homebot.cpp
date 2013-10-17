#include "homebot.h"
#include "domain.h"
#include "planner.h"
#include <iostream>

using namespace std;

extern void parse_env(const char* str, Domain& domain);
extern void parse_task(const char* str, Domain& domain);

HomeBot::HomeBot() : Plug("HomeBot") {
    // blank line
}

// The extra plan
void HomeBot::Plan()
{
    cout << "#(Devil): Init" << endl;
    cout << "# EnvDes:\n" << GetEnvDes() << endl;
    cout << "# TaskDes:\n" << GetTaskDes() << endl;
    Domain domain;
    parse_env(GetEnvDes().c_str(), domain);
    parse_task(GetTaskDes().c_str(), domain);
    Planner planner;
    planner.SetDomain(domain);
    domain.Preprocess();

    ShowState();
    while (!AnalyzeTask()) {
        string str = AskLoc(gObjToSense);
        cout << str << endl;
        RefreshEnv(str);
        ShowState();
    }

    cout << "start IDA_STAR" << endl;
    IDA_STAR();

    ApplyPlan();
    /*
    Move(5);
    Open(5);
    TakeOut(8, 5);
    ToPlate(8);
    Close(5);
    Move(3);
    FromPlate(8);
    PutDown(8);
    */
}

bool HomeBot::ApplyPlan() {
    bool ret;
    auto& s = gInitState;
    for (int i = 0; i < opsnum; ++i) {
        if (ops[i].op == MOVE) {
            ret = Move(ops[i].arg1);
            cout << "[MOVE " << ops[i].arg1 << "|" << (ret ? "true" : "false") << "]"  << endl;
            if (ret) {
                s.pos[0] = ops[i].arg1;
                if (s.hold) {
                    s.pos[s.hold] = ops[i].arg1;
                }
                if (s.plate) {
                    s.pos[s.plate] = ops[i].arg1;
                }
            } else {
                return false;
            }
        } else if (ops[i].op == PICKUP) {
            ret = PickUp(ops[i].arg1);
            cout << "[PICKUP " << ops[i].arg1 << "|" << (ret ? "true" : "false") << "]" << endl;
            if (ret) {
                s.hold = ops[i].arg1;
            } else {
                return false;
            }
        } else if (ops[i].op == PUTDOWN) {
            ret = PutDown(ops[i].arg1);
            cout << "[PUTDOWN " << ops[i].arg1 << "|" << (ret ? "true" : "false") << "]" << endl;
            if (ret) {
                s.hold = 0;
            } else {
                return false;
            }
        } else if (ops[i].op == TOPLATE) {
            ret = ToPlate(ops[i].arg1);
            cout << "[TOPLATE " << ops[i].arg1 << "|" << (ret ? "true" : "false") << "]" << endl;
            if (ret) {
                s.plate = s.hold;
                s.hold = 0;
            } else {
                return false;
            }
        } else if (ops[i].op == FROMPLATE) {
            ret = FromPlate(ops[i].arg1);
            cout << "[FROMPLATE " << ops[i].arg1 << "|" << (ret ? "true" : "false") << "]" << endl;
            if (ret) {
                s.hold = s.plate;
                s.plate = 0;
            } else {
                return false;
            }
        } else if (ops[i].op == OPEN) {
            ret = Open(ops[i].arg1);
            cout << "[OPEN " << ops[i].arg1 << "|" << (ret ? "true" : "false") << "]" << endl;
            if (ret) {
                s.doorOpen.insert(ops[i].arg1);
            } else {
                return false;
            }
        } else if (ops[i].op == CLOSE) {
            cout << "[CLOSE " << ops[i].arg1 << "|" << (ret ? "true" : "false") << "]" << endl;
            ret = Close(ops[i].arg1);
            if (ret) {
                s.doorOpen.erase(ops[i].arg1);
            } else {
                return false;
            }
        } else if (ops[i].op == PUTIN) {
            cout << "[PUTIN " << ops[i].arg1 << " " << ops[i].arg2 << "|" << (ret ? "true" : "false") << "]" << endl;
            ret = PutIn(ops[i].arg1, ops[i].arg2);
            if (ret) {
                if (s.inside.count(ops[i].arg2) == 0) {
                    s.inside.insert(make_pair(ops[i].arg2, set<unsigned>()));
                }
                s.inside[ops[i].arg2].erase(ops[i].arg1);
                s.hold = 0;
            } else {
                return false;
            }
        } else if (ops[i].op == TAKEOUT) {
            cout << "[TAKEOUT " << ops[i].arg1 << " " << ops[i].arg2 << "|" << (ret ? "true" : "false") << "]" << endl;
            ret = TakeOut(ops[i].arg1, ops[i].arg2);
            if (ret) {
                s.inside[ops[i].arg2].erase(ops[i].arg1);
                s.hold = ops[i].arg1;
            } else {
                return false;
            }
        }
    }
    return true;
}

void HomeBot::Fini()
{
    cout << "#(Devil): Fini" << endl;
}
