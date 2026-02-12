#include "StateFactory.h"
#include "states/State_Sleep.h"
#include "states/State_Work.h"
#include "states/State_ScreenOn.h"
#include "states/State_DataSend.h"
#include "states/State_NetSetup.h"

BaseState* StateFactory::createState(StateType type) {
    switch (type) {
        case StateType::SLEEP:     return new State_Sleep();
        case StateType::WORK:      return new State_Work();
        case StateType::SCREEN_ON: return new State_ScreenOn();
        case StateType::DATA_SEND: return new State_DataSend();
        case StateType::NET_SETUP: return new State_NetSetup();
        default: return nullptr;
    }
}