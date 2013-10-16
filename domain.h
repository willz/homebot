#ifndef INCLUDED_DOMAIN_H
#define INCLUDED_DOMAIN_H

#include <vector>
#include <map>
#include <set>
#include <cstring>

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

struct State {
    State() : plate(0), hold(0) {
        memset(pos, 0, MAX_OBJECT_NUM * sizeof(unsigned));
    }
    unsigned pos[MAX_OBJECT_NUM];
    unsigned plate;
    unsigned hold;
    std::set<unsigned> doorOpen;
    std::map<unsigned, unsigned> inside;
};

class Domain {
public:
    void SetEnv(const std::map<unsigned, Object>& objects, unsigned plate,
                unsigned hold);
    void SetTask(const std::vector<Task>& tasks);
    void SetInfo(const std::vector<Info>& infos, const std::vector<ConsTask>& consTasks,
                 const std::vector<ConsInfo>& consInfos);
    void GetObjects(const Object& filter, std::vector<unsigned>& result,
                    bool requireLoc) const;
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
extern std::vector<unsigned> gContainers;

#endif
