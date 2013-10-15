#ifndef INCLUDED_DOMAIN_H
#define INCLUDED_DOMAIN_H

#include <vector>
#include <map>

const unsigned UNKNOWN_LOC = 0xffffffff;
const unsigned UNKNOWN = 0xffffffff;

enum ColorType {WHITE, RED, GREEN, YELLOW, BLUE, BLACK, UNKNOWN_COLOR};

enum SortType {HUMAN, PLANT, COUCH, CHAIR, SOFA, BED, TABLE, WORKSPACE,
               WORKTABLE, TEAPOY, DESK, TELEVISION, AIRCONDITIONER,
               WASHMACHINE, CLOSET, CUPBOARD, REFRIGERATOR, MICROWAVE,
               BOOK, CAN, REMOTECONTROL, BOTTLE, CUP, UNKNOWN_SORT};

enum SizeType {BIG, SMALL};

enum DoorType {UNKNOWN_DOOR, DOOR_OPEN, DOOR_CLOSED};

enum InfoType {I_ON, I_NEAR, I_PLATE, I_INSIDE, I_OPENED, I_CLOSED};

enum STATE {SORT, COLOR, SIZE, TYPE, CLOSED, OPENED, HOLD, PLATE, AT};

enum ACTION {MOVE, PICKUP, PUTDOWN, TOPLATE, FROMPLATE, OPEN, CLOSE,
             PUTIN, TAKEOUT};

enum TaskType {T_GIVE, T_PUTON, T_GOTO, T_PUTDOWN, T_PICKUP, T_OPEN,
               T_PUTIN, T_CLOSE, T_TAKEOUT};

enum ConsType {CONS_NOT, CONS_NOTNOT};

ColorType ColorStrToEnum(const char* str);
SortType SortStrToEnum(const char* str);
SizeType SizeStrToEnum(const char* str);
InfoType InfoStrToEnum(const char* str);
TaskType TaskStrToEnum(const char* str);

class Object {
public:
    Object();
    Object(unsigned x);
    unsigned id;
    ColorType color;
    SortType sort;
    SizeType size;
    DoorType door;
    unsigned loc;
    unsigned inside;
    bool isContainer;
};

class Task {
public:
    TaskType type;
    Object arg1;
    Object arg2;
};

class Info {
public:
    InfoType type;
    Object arg1;
    Object arg2;
};

class ConsTask {
public:
    ConsType type;
    Task task;
};

class ConsInfo {
public:
    ConsType type;
    Info info;
};

class Domain {
public:
    void SetEnv(const std::map<unsigned, Object>& objects, unsigned plate,
                unsigned hold);
    void GetObjects(const Object& filter, std::vector<unsigned>& result,
                    bool requireLoc) const;

private:
    std::map<unsigned, Object> _objects;
    unsigned _plate;
    unsigned _hold;
};

#endif
