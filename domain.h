#ifndef INCLUDED_DOMAIN_H
#define INCLUDED_DOMAIN_H

#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <list>

const unsigned UNKNOWN_LOC = 0xffffffff;
const unsigned UNKNOWN = 0xffffffff;

enum ColorType {WHITE, RED, GREEN, YELLOW, BLUE, BLACK, UNKNOWN_COLOR};

enum SortType {HUMAN, PLANT, COUCH, CHAIR, SOFA, BED, TABLE, WORKSPACE,
               WORKTABLE, TEAPOY, DESK, TELEVISION, AIRCONDITIONER,
               WASHMACHINE, CLOSET, CUPBOARD, REFRIGERATOR, MICROWAVE,
               BOOK, CAN, REMOTECONTROL, BOTTLE, CUP, UNKNOWN_SORT};

enum SizeType {BIG, SMALL};

enum DoorType {UNKNOWN_DOOR, DOOR_OPENED, DOOR_CLOSED};

enum InfoType {I_ON, I_NEAR, I_PLATE, I_INSIDE, I_OPENED, I_CLOSED};

enum STATE {SORT, COLOR, SIZE, TYPE, CLOSED, OPENED, HOLD, PLATE, AT};

enum ACTION {UNKNOWN_ACTION = 0, MOVE, PICKUP, PUTDOWN, TOPLATE, FROMPLATE, OPEN, CLOSE,
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
    unsigned loc;
    unsigned inside;
    DoorType door;
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

const int MAX_OBJECT_NUM = 50;

struct Cons {
    InfoType type;
    std::list<unsigned> arg1;
    std::list<unsigned> arg2;
};

// expand task
struct ETask {
    TaskType type;
    std::list<unsigned> arg1;
    std::list<unsigned> arg2;
};

struct Op {
    ACTION op;
    unsigned arg1;
    unsigned arg2;
};

struct State {
    State() : plate(UNKNOWN), hold(UNKNOWN) {
        memset(pos, 1, MAX_OBJECT_NUM * sizeof(unsigned));
    }
    State(const State& s) {
        memcpy(pos, s.pos, MAX_OBJECT_NUM * sizeof(unsigned));
        plate = s.plate;
        hold = s.hold;
        doorOpen = s.doorOpen;
        inside = s.inside;
        info = s.info;
    }
    unsigned pos[MAX_OBJECT_NUM];
    unsigned plate;
    unsigned hold;
    std::set<unsigned> doorOpen;
    std::map<unsigned, std::set<unsigned>> inside;
    std::list<Cons> info;
};

class Domain {
public:
    void SetEnv(const std::map<unsigned, Object>& objects, unsigned plate,
                unsigned hold);
    void SetTask(const std::vector<Task>& tasks);
    void SetInfo(const std::vector<Info>& infos, const std::vector<ConsTask>& consTasks,
                 const std::vector<ConsInfo>& consInfos);
    void GetObjects(const Object& filter, std::list<unsigned>& result,
                    bool requireLoc = false) const;
    void Preprocess();

private:
    std::map<unsigned, Object> _objects;
    unsigned _plate;
    unsigned _hold;
    std::vector<Task> _tasks;
    std::vector<Info> _infos;
    std::vector<ConsTask> _consTasks;
    std::vector<ConsInfo> _consInfos;
};

extern State gInitState;
extern std::vector<Cons> gNotC;
extern std::vector<Cons> gNotNotC;
extern std::vector<unsigned> gContainers;
extern std::vector<ETask> gTasks;

#endif
