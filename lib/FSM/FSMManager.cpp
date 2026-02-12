#include "FSMManager.h"
#include "StateFactory.h"
#include "AppConfig.h"
#include "states/State_ScreenOn.h" // Нужно для приведения типа
#include "DisplayUI.h"             // <<< НОВОЕ: Нужно для вызова DisplayUI.nextScreen()

FSMManager fsm;

void FSMManager::init(StateType initialState) {
    _changeState(initialState);
}

void FSMManager::update() {
    if (_currentState) {
        _currentState->onUpdate();
    }
}

void FSMManager::_changeState(StateType type) {
    if (_currentState) {
        if (_currentState->getType() == type) return;
        _currentState->onExit();
        delete _currentState;
    }

    _currentState = StateFactory::createState(type);

    if (_currentState) {
        _currentState->onEnter();
    }
}

void FSMManager::handleEvent(FSMEvent event) {
    if (!_currentState) return;

    StateType current = _currentState->getType();

    switch (event) {
        case FSMEvent::BTN_SHORT:
            if (current == StateType::SLEEP) {
                _changeState(StateType::SCREEN_ON);
            }
            else if (current == StateType::SCREEN_ON) {
                // <<< ИСПРАВЛЕНО: Заменен dynamic_cast на static_cast >>>
                // Это преобразование безопасно, так как мы находимся внутри проверки
                // current == StateType::SCREEN_ON.
                if (auto screenOnState = static_cast<State_ScreenOn*>(_currentState)) {
                    DisplayUI.nextScreen();      // Сначала переключаем экран
                    screenOnState->resetTimer(); // Затем сбрасываем таймер
                }
            }
            break;

        case FSMEvent::TIMEOUT:
            if (current == StateType::SCREEN_ON) _changeState(StateType::SLEEP);
            break;

        case FSMEvent::WORK_COMPLETE:
            _changeState(StateType::SLEEP);
            break;
        case FSMEvent::TIME_TO_SEND:
            _changeState(StateType::DATA_SEND);
            break;
        case FSMEvent::TIME_TO_SYNC:
            _changeState(StateType::NET_SETUP);
            break;
        case FSMEvent::SEND_SUCCESS:
        case FSMEvent::SEND_FAIL:
        case FSMEvent::SYNC_SUCCESS:
        case FSMEvent::SYNC_FAIL:
            _changeState(StateType::SLEEP);
            break;
        default:
            break;
    }
}

StateType FSMManager::getCurrentStateType() const {
    return _currentState ? _currentState->getType() : StateType::NONE;
}