#include "object.h"
#include <map>

using namespace std;

static map<char const*, COLOR> colorMap;
static map<char const*, SORT> sortMap;

COLOR ColorStrToEnum(const char* str) {
    if (colorMap.empty()) {
        colorMap['white'] = WHITE;
        colorMap['red'] = RED;
        colorMap['green'] = GREEN;
        colorMap['yellow'] = YELLOW;
        colorMap['blue'] = BLUE;
        colorMap['black'] = BLACK;
    }
    return colorMap[str];
}

SORT SortStrToEnum(const char* str) {
    if (sortMap.empty()) {
        sortMap['human'] = HUMAN;
        sortMap['plant'] = PLANT;
        sortMap['couch'] = COUCH;
        sortMap['chair'] = CHAIR;
        sortMap['sofa'] = SOFA;
        sortMap['bed'] = BED;
        sortMap['table'] = TABLE;
        sortMap['workspace'] = WORKSPACE;
        sortMap['worktable'] = WORKTABLE;
        sortMap['teapoy'] = TEAPOY;
        sortMap['desk'] = DESK;
        sortMap['television'] = TELEVISION;
        sortMap['airconditioner'] = AIRCONDITIONER;
        sortMap['washmachine'] = WASHMACHINE;
        sortMap['closet'] = CLOSET;
        sortMap['cupboard'] = CUPBOARD;
        sortMap['refrigerator'] = REFRIGERATOR;
        sortMap['microwave'] = MICROWAVE;
        sortMap['book'] = BOOK;
        sortMap['can'] = CAN;
        sortMap['remotecontrol'] = REMOTECONTROL;
        sortMap['bottle'] = BOTTLE;
        sortMap['cup'] = CUP;
    }
    return sortMap[str];
}
