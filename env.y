%{
#include <map>
#include <cstdio>
#include <string>
#include <iostream>
#include "domain.h"

using namespace std;

static map<unsigned, Object> objects;
static vector<Task> tasks;
static vector<Info> infos;
static vector<ConsTask> consTasks;
static vector<ConsInfo> consInfos;
static unsigned plate = 0;
static unsigned hold = 0;


int yylex(void);
void yyerror(const char*) {
}

struct CondDef {
    string pred;
    string var;
    string param;
};

struct TaskPred {
    TaskType type;
    string param1;
    string param2;
};

struct InfoPred {
    InfoType type;
    string param1;
    string param2;
};

%}


%start start

%union {
    unsigned number;
    char* stringVal;
    CondDef* pCondDef;
    vector<CondDef>* pVecCondDef;
    TaskPred* pTaskPred;
    InfoPred* pInfoPred;
    Task* pTask;
    Info* pInfo;
}

%type <number> NUMBER
%type <stringVal> WORD
%type <pCondDef> condition
%type <pVecCondDef> condition_star
%type <pVecCondDef> condition_def
%type <pTaskPred> task_predicate
%type <pInfoPred> info_predicate
%type <pTask> task_combine
%type <pTask> task_definition
%type <pInfo> info_combine
%type <pInfo> info_definition

%token OPEN_PAREN
%token CLOSE_PAREN
%token DOMAIN_TOK
%token AT_TOK
%token SORT_TOK
%token COLOR_TOK
%token SIZE_TOK
%token TYPE_TOK
%token INSIDE_TOK
%token HOLD_TOK
%token PLATE_TOK
%token CLOSED_TOK
%token OPENED_TOK
%token NUMBER
%token WORD

%token INS_TOK
%token TASK_TOK
%token COND_TOK
%token CONS_NOT_TOK
%token CONS_NOTNOT_TOK
%token INFO_TOK
%token ON_TOK
%token NEAR_TOK

%%
start:
domain_definition
|
ins_definition
;

domain_definition:
OPEN_PAREN DOMAIN_TOK env_definition CLOSE_PAREN {
    cout << "domain" << endl;
}
;

env_definition:
state_definition env_definition
|
state_definition
;

state_definition:
OPEN_PAREN AT_TOK NUMBER NUMBER CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].loc = $4;
}
|
OPEN_PAREN PLATE_TOK NUMBER CLOSE_PAREN {
    plate = $3;
}
|
OPEN_PAREN HOLD_TOK NUMBER CLOSE_PAREN {
    hold = $3;
}
|
OPEN_PAREN SORT_TOK NUMBER WORD CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].sort = SortStrToEnum($4);
}
|
OPEN_PAREN COLOR_TOK NUMBER WORD CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].color = ColorStrToEnum($4);
}
|
OPEN_PAREN SIZE_TOK NUMBER WORD CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].size = SizeStrToEnum($4);
}
|
OPEN_PAREN TYPE_TOK NUMBER WORD CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].isContainer = true;
}
|
OPEN_PAREN CLOSED_TOK NUMBER CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].door = DOOR_CLOSED;
}
|
OPEN_PAREN OPENED_TOK NUMBER CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].door = DOOR_OPEN;
}
|
OPEN_PAREN INSIDE_TOK NUMBER NUMBER CLOSE_PAREN {
    map<unsigned, Object>::iterator it = objects.find($3);
    if (it == objects.end()) {
        objects[$3] = Object($3);
    }
    objects[$3].inside = $4;
}
;

ins_definition:
OPEN_PAREN INS_TOK ins_body_star CLOSE_PAREN {
    cout << "ins_definition" << endl;
}
;

ins_body_star:
/* empty */
ins_body ins_body_star {
    cout << "ins_body_star" << endl;
}
|
ins_body {
    cout << "ins_body" << endl;
}
;

ins_body:
info_definition {
    cout << "ins info" << endl;
}
|
task_definition {
    cout << "ins task" << endl;
}
|
cons_not_definition
|
cons_notnot_definition
;

info_definition:
OPEN_PAREN INFO_TOK info_combine CLOSE_PAREN {
    infos.push_back(*$3);
    delete $3;
}
;

info_predicate:
OPEN_PAREN ON_TOK WORD WORD CLOSE_PAREN {
    $$ = new InfoPred();
    $$->type = I_ON;
    $$->param1 = $3;
    $$->param2 = $4;
}
|
OPEN_PAREN PLATE_TOK WORD CLOSE_PAREN {
    $$ = new InfoPred();
    $$->type = I_PLATE;
    $$->param1 = $3;
}
|
OPEN_PAREN NEAR_TOK WORD WORD CLOSE_PAREN {
    $$ = new InfoPred();
    $$->type = I_NEAR;
    $$->param1 = $3;
    $$->param2 = $4;
}
|
OPEN_PAREN INSIDE_TOK WORD WORD CLOSE_PAREN {
    $$ = new InfoPred();
    $$->type = I_INSIDE;
    $$->param1 = $3;
    $$->param2 = $4;
}
|
OPEN_PAREN OPENED_TOK WORD CLOSE_PAREN {
    $$ = new InfoPred();
    $$->type = I_OPENED;
    $$->param1 = $3;
}
|
OPEN_PAREN CLOSED_TOK WORD CLOSE_PAREN {
    $$ = new InfoPred();
    $$->type = I_CLOSED;
    $$->param1 = $3;
}
;

task_definition:
OPEN_PAREN TASK_TOK task_combine CLOSE_PAREN {
    tasks.push_back(*$3);
    delete $3;
}
;

task_predicate:
OPEN_PAREN WORD WORD CLOSE_PAREN {
    cout << $2 << endl;
    $$ = new TaskPred();
    $$->type = TaskStrToEnum($2);
    $$->param1 = $3;
}
|
OPEN_PAREN WORD WORD WORD CLOSE_PAREN {
    cout << $2 << endl;
    $$ = new TaskPred();
    $$->type = TaskStrToEnum($2);
    $$->param1 = $3;
    $$->param2 = $4;
}
;

task_combine:
task_predicate condition_def {
    $$ = new Task();
    $$->type = $1->type;
    for (size_t i = 0; i < $2->size(); ++i) {
        CondDef cond = (*$2)[i];
        if (cond.var == $1->param1) {
            if (cond.pred == "sort") {
                $$->arg1.sort = SortStrToEnum(cond.param.c_str());
            } else if (cond.pred == "color") {
                $$->arg1.color = ColorStrToEnum(cond.param.c_str());
            } else {
                $$->arg1.isContainer = true;
            }
        }
        else {
            if (cond.pred == "sort") {
                $$->arg2.sort = SortStrToEnum(cond.param.c_str());
            } else if (cond.pred == "color") {
                $$->arg2.color = ColorStrToEnum(cond.param.c_str());
            } else {
                $$->arg2.isContainer = true;
            }
        }
    }
    delete $1;
    delete $2;
}
;

info_combine:
info_predicate condition_def {
    $$ = new Info();
    $$->type = $1->type;
    for (size_t i = 0; i < $2->size(); ++i) {
        CondDef cond = (*$2)[i];
        if (cond.var == $1->param1) {
            if (cond.pred == "sort") {
                $$->arg1.sort = SortStrToEnum(cond.param.c_str());
            } else if (cond.pred == "color") {
                $$->arg1.color = ColorStrToEnum(cond.param.c_str());
            } else {
                $$->arg1.isContainer = true;
            }
        }
        else {
            if (cond.pred == "sort") {
                $$->arg2.sort = SortStrToEnum(cond.param.c_str());
            } else if (cond.pred == "color") {
                $$->arg2.color = ColorStrToEnum(cond.param.c_str());
            } else {
                $$->arg2.isContainer = true;
            }
        }
    }
    delete $1;
    delete $2;
}
;

cons_not_definition:
OPEN_PAREN CONS_NOT_TOK task_definition CLOSE_PAREN {
    ConsTask cons;
    cons.type = CONS_NOT;
    cons.task = *$3;
    consTasks.push_back(cons);
    delete $3;
}
|
OPEN_PAREN CONS_NOT_TOK info_definition CLOSE_PAREN {
    ConsInfo cons;
    cons.type = CONS_NOT;
    cons.info = *$3;
    consInfos.push_back(cons);
    delete $3;
}
;

cons_notnot_definition:
OPEN_PAREN CONS_NOTNOT_TOK task_definition CLOSE_PAREN {
    ConsTask cons;
    cons.type = CONS_NOTNOT;
    cons.task = *$3;
    consTasks.push_back(cons);
    delete $3;
}
|
OPEN_PAREN CONS_NOTNOT_TOK info_definition CLOSE_PAREN {
    cout << " cons not not " << endl;
    ConsInfo cons;
    cons.type = CONS_NOTNOT;
    cons.info = *$3;
    consInfos.push_back(cons);
    delete $3;
}
;

condition_def:
OPEN_PAREN COND_TOK condition_star CLOSE_PAREN {
    $$ = $3;
}
;

condition_star:
condition condition_star {
    $2->push_back(*$1);
    $$ = $2;
    delete $1;
}
|
condition {
    $$ = new vector<CondDef>(1, *$1);
    delete $1;
}
;

condition:
OPEN_PAREN SORT_TOK WORD WORD CLOSE_PAREN {
    $$ = new CondDef();
    $$->pred = "sort";
    $$->var = $3;
    $$->param = $4;
}
|
OPEN_PAREN COLOR_TOK WORD WORD CLOSE_PAREN {
    $$ = new CondDef();
    $$->pred = "color";
    $$->var = $3;
    $$->param = $4;
}
|
OPEN_PAREN TYPE_TOK WORD WORD CLOSE_PAREN {
    $$ = new CondDef();
    $$->pred = "type";
    $$->var = $3;
    $$->param = $4;
}
;


/********** For Nature Language ***************/
/*
nl_task:
nl_sentence nl_task
|
nl_sentence
;

nl_sentence:
GIVE_TOK ME_TOK object_def
|
GIVE_TOK object_def TO_TOK ME_TOK
|
PUT_TOK object_def puton_prep object_def
|
PUT_TOK object_def DOWN_TOK
|
PUT_TOK DOWN_TOK object_def
|
PUT_TOK object_def IN_TOK object_def
|
GOTO_TOK object_def
|
PICKUP_TOK object_def
|
TAKEOUT_TOK object_def FROM_TOK object_def
;

puton_prep:
ON_TOK
|
NEAR_TOK
|
NEXT_TOK TO_TOK
|
DOWN_TOK TO_TOK
;
*/


%%


#include "lex_env.cpp"

void parse_env(const char* str, Domain& domain) {
    env_scan_string(str);
    yyparse();
    domain.SetEnv(objects, plate, hold);
}

void parse_task(const char* str, Domain& domain) {
    env_scan_string(str);
    yyparse();
}
