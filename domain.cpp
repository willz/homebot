#include "domain.h"
#include <cstring>

using namespace std;

static map<const char*, ColorType> colorMap;
static map<const char*, SortType> sortMap;
static map<const char*, InfoType> infoMap;

ColorType ColorStrToEnum(const char* str) {
    if (colorMap.empty()) {
        colorMap["white"] = WHITE;
        colorMap["red"] = RED;
        colorMap["green"] = GREEN;
        colorMap["yellow"] = YELLOW;
        colorMap["blue"] = BLUE;
        colorMap["black"] = BLACK;
    }
    return colorMap[str];
}

SortType SortStrToEnum(const char* str) {
    if (sortMap.empty()) {
        sortMap["human"] = HUMAN;
        sortMap["plant"] = PLANT;
        sortMap["couch"] = COUCH;
        sortMap["chair"] = CHAIR;
        sortMap["sofa"] = SOFA;
        sortMap["bed"] = BED;
        sortMap["table"] = TABLE;
        sortMap["workspace"] = WORKSPACE;
        sortMap["worktable"] = WORKTABLE;
        sortMap["teapoy"] = TEAPOY;
        sortMap["desk"] = DESK;
        sortMap["television"] = TELEVISION;
        sortMap["airconditioner"] = AIRCONDITIONER;
        sortMap["washmachine"] = WASHMACHINE;
        sortMap["closet"] = CLOSET;
        sortMap["cupboard"] = CUPBOARD;
        sortMap["refrigerator"] = REFRIGERATOR;
        sortMap["microwave"] = MICROWAVE;
        sortMap["book"] = BOOK;
        sortMap["can"] = CAN;
        sortMap["remotecontrol"] = REMOTECONTROL;
        sortMap["bottle"] = BOTTLE;
        sortMap["cup"] = CUP;
    }
    return sortMap[str];
}

SizeType SizeStrToEnum(const char* str) {
    if (strncmp(str, "big", 3) == 0) {
        return BIG;
    }
    return SMALL;
}

InfoType InfoStrToEnum(const char* str) {
    if (infoMap.empty()) {
        infoMap["on"] = I_ON;
        infoMap["near"] = I_NEAR;
        infoMap["plate"] = I_PLATE;
        infoMap["inside"] = I_INSIDE;
        infoMap["opened"] = I_OPENED;
        infoMap["closed"] = I_CLOSED;
    }
    return infoMap[str];
}

Object::Object()
    : color(UNKNOWN_COLOR), sort(UNKNOWN_SORT), loc(UNKNOWN_LOC)
    , isContainer(false) {
}

Object::Object(unsigned x)
    : id(x), color(UNKNOWN_COLOR), sort(UNKNOWN_SORT), loc(UNKNOWN_LOC)
    , isContainer(false) {
}

void Domain::SetEnv(const map<unsigned, Object>& objects, unsigned plate,
                    unsigned hold) {
    _objects = objects;
    _plate = plate;
    _hold = hold;
}

void Domain::GetObjects(const Object& filter, vector<unsigned>& result,
                        bool requireLoc) const {

}
