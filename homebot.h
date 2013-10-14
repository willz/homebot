#ifndef INCLUDED_HOMEBOT_H
#define INCLUDED_HOMEBOT_H

#include "plug.hpp"

class HomeBot : public _home::Plug {
public:
    HomeBot();

protected:
    void Plan();
    void Fini();
};

#endif
