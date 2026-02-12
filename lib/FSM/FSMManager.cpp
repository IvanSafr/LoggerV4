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
        delete _currentState; // Освобождаем память старого состояния
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
            else if (current == StateType::SCREEN_ON) {
                // В SCREEN_ON короткое нажатие просто меняет индекс экрана внутри DisplayUI
                // Но если это 3-й экран, можно вернуться в SLEEP или сбросить таймер
            }
            break;

        case FSMEvent::TIMEOUT:
            if (current == StateType::SCREEN_ON) _changeState(StateType::SLEEP);
            break;

        case FSMEvent::SENSOR_DONE:
            // После измерения решаем: слать данные или спать
            _changeState(StateType::SLEEP); 
            break;
            
        default:
            break;
    }
}

StateType FSMManager::getCurrentStateType() const {
    return _currentState ? _currentState->getType() : StateType::NONE;
}