#ifndef STATE_FACTORY_H
#define STATE_FACTORY_H

#include "FSM_Definitions.h"

class StateFactory {
public:
    static BaseState* createState(StateType type);
};

#endif