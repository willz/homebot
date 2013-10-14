#ifndef INCLUDED_STRUCT_H
#define INCLUDED_STRUCT_H

enum STATE {SORT, COLOR, SIZE, TYPE, CLOSED, OPENED, HOLD, PLATE, AT};

enum ACTION {MOVE, PICKUP, PUTDOWN, TOPLATE, FROMPLATE, OPEN, CLOSE,
             PUTIN, TAKEOUT};

struct State {
    STATE state;
    int args[2];
};

struct Action {
    ACTION action;
    int args[2];
};

#endif
