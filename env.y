%{
#include <map>
#include <cstdio>
#include <string>
#include <iostream>
#include "domain.h"

using namespace std;

static map<unsigned, Object> objects;
static vector<Task> tasks;
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

%}


%start start

%union {
    unsigned number;
    char* stringVal;
    CondDef* pCondDef;
    vector<CondDef>* pVecCondDef;
    TaskPred* pTaskPred;
}

%type <number> NUMBER
%type <stringVal> WORD
%type <pCondDef> condition
%type <pVecCondDef> condition_star
%type <pVecCondDef> condition_def
%type <pTaskPred> task_predicate

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
ins_body {
    cout << "ins_body" << endl;
}
|
ins_body ins_body_star {
    cout << "ins_body_star" << endl;
}
;

ins_body:
info_definition
|
task_definition
|
cons_not_definition
|
cons_notnot_definition
;

info_definition:
OPEN_PAREN INFO_TOK info_predicate condition_def CLOSE_PAREN
;

info_predicate:
OPEN_PAREN ON_TOK WORD WORD CLOSE_PAREN {
}
|
OPEN_PAREN PLATE_TOK WORD CLOSE_PAREN {
}
|
OPEN_PAREN NEAR_TOK WORD WORD CLOSE_PAREN {
}
|
OPEN_PAREN INSIDE_TOK WORD WORD CLOSE_PAREN {
}
|
OPEN_PAREN OPENED_TOK WORD CLOSE_PAREN {
}
|
OPEN_PAREN CLOSED_TOK WORD CLOSE_PAREN {
}
;

task_definition:
OPEN_PAREN TASK_TOK task_predicate condition_def CLOSE_PAREN {
    Task t;
    t.type = $3->type;
    for (size_t i = 0; i < $4->size(); ++i) {
        CondDef cond = (*$4)[i];
        if (cond.var == $3->param1) {
            if (cond.pred == "sort") {
                t.arg1.sort = SortStrToEnum(cond.param.c_str());
            } else if (cond.pred == "color") {
                t.arg1.color = ColorStrToEnum(cond.param.c_str());
            } else {
                t.arg1.isContainer = true;
            }
        }
        else {
            if (cond.pred == "sort") {
                t.arg2.sort = SortStrToEnum(cond.param.c_str());
            } else if (cond.pred == "color") {
                t.arg2.color = ColorStrToEnum(cond.param.c_str());
            } else {
                t.arg2.isContainer = true;
            }
        }
    }
    tasks.push_back(t);
    cout << tasks.size() << endl;
    delete $3;
    delete $4;
}
;

task_predicate:
OPEN_PAREN WORD WORD CLOSE_PAREN {
    $$ = new TaskPred();
    $$->type = TaskStrToEnum($2);
    $$->param1 = $3;
}
|
OPEN_PAREN WORD WORD WORD CLOSE_PAREN {
    $$ = new TaskPred();
    $$->type = TaskStrToEnum($2);
    $$->param1 = $3;
    $$->param2 = $4;
}
;

cons_not_definition:
OPEN_PAREN CONS_NOT_TOK task_predicate condition_def CLOSE_PAREN
|
OPEN_PAREN CONS_NOT_TOK info_predicate condition_def CLOSE_PAREN
;

cons_notnot_definition:
OPEN_PAREN CONS_NOTNOT_TOK task_predicate condition_def CLOSE_PAREN
|
OPEN_PAREN CONS_NOTNOT_TOK info_predicate condition_def CLOSE_PAREN
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
