#include "FSMManager.h"
#include "StateFactory.h"
#include "AppConfig.h"

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

    // Глобальная логика переходов
    switch (event) {
        case FSMEvent::BTN_SHORT:
            if (current == StateType::SLEEP) _changeState(StateType::SCREEN_ON);
            // ... (другие обработки кнопок)
            break;

        case FSMEvent::TIMEOUT:
            if (current == StateType::SCREEN_ON) _changeState(StateType::SLEEP);
            break;

        // <<< НОВОЕ: Обработка событий после измерения >>>
        case FSMEvent::WORK_COMPLETE:
            _changeState(StateType::SLEEP);
            break;
        case FSMEvent::TIME_TO_SEND:
            _changeState(StateType::DATA_SEND);
            break;
        case FSMEvent::TIME_TO_SYNC:
            _changeState(StateType::NET_SETUP);
            break;

        // <<< НОВОЕ: Обработка событий после сетевых операций. Возвращаемся в сон. >>>
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