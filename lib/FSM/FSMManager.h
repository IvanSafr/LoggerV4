#ifndef FSM_MANAGER_H
#define FSM_MANAGER_H

#include "FSM_Definitions.h"
// Do NOT include StateFactory.h here to avoid circular dependency

class BaseState; // Forward declaration

class FSMManager {
private:
    BaseState* _currentState = nullptr;
    void _changeState(StateType type);

public:
    FSMManager() {}
    void init(StateType initialState);
    void update();
    void handleEvent(FSMEvent event);
    StateType getCurrentStateType() const;
};

extern FSMManager fsm;

#endif