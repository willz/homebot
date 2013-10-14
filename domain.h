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

enum InfoType {I_ON, I_NEAR, I_PLATE, I_INSIDE, I_OPENED, I_CLOSED};

enum STATE {SORT, COLOR, SIZE, TYPE, CLOSED, OPENED, HOLD, PLATE, AT};

enum ACTION {MOVE, PICKUP, PUTDOWN, TOPLATE, FROMPLATE, OPEN, CLOSE,
             PUTIN, TAKEOUT};

ColorType ColorStrToEnum(const char* str);
SortType SortStrToEnum(const char* str);
SizeType SizeStrToEnum(const char* str);
InfoType InfoStrToEnum(const char* str);

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
    bool isContainer;
    bool opened;
};

class Task {
public:

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