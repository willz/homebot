#include "homebot.h"
#include "domain.h"
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

    Move(5);
    Open(5);
    TakeOut(8, 5);
    ToPlate(8);
    Close(5);
    Move(3);
    FromPlate(8);
    PutDown(8);
}

void HomeBot::Fini()
{
    cout << "#(Devil): Fini" << endl;
}
