#ifndef FSM_DEFINITIONS_H
#define FSM_DEFINITIONS_H

#include <Arduino.h>

enum class StateType {
    SLEEP,
    SCREEN_ON,
    WORK,
    DATA_SEND,
    NET_SETUP,
    NONE
};

enum class FSMEvent {
    BTN_SHORT,
    BTN_LONG,
    BTN_DOUBLE,
    WORK_COMPLETE,  // <<< ИЗМЕНЕНО: Заменили SENSOR_DONE. Означает, что работа завершена, можно спать.
    TIME_TO_SEND,   // <<< НОВОЕ: Настало время для отправки данных (по счетчику N)
    TIME_TO_SYNC,   // <<< НОВОЕ: Настало время для синхронизации времени (по счетчику D)
    SEND_SUCCESS,
    SEND_FAIL,
    SYNC_SUCCESS, // Время успешно получено
    SYNC_FAIL,
    TIMEOUT
};

// Интерфейс состояния
class BaseState {
public:
    virtual ~BaseState() {}
    virtual void onEnter() = 0;
    virtual void onUpdate() = 0; // Тик
    virtual void onExit() = 0;
    virtual StateType getType() const = 0;
};

#endif