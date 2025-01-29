#pragma once

#include "partage.h"
#include "Leds.h"
#include "bluetooth.h"

enum rxState_t {
    STX,
    ID,
    SIZE,
    DATA,
    CHECKSUM,
    ETX
};

enum possibleCommands_t {
    START_GAME,
    END_GAME,
    WAIT_BOARD_MOVE,
    ASK_POSSIBLE_MOVES,
    MOTOR_STATE,
    MOTOR_SPEED,
    SET_POSITION,
    MOTOR_DIRECTION,
    SET_LED,
    PROMOTION,
    CALIBRATECMD,
    MOTOR_STEPS,
    SET_AIMANT
};

/**
 * @brief class for the bluetooth task
 * 
 */
class BluetoothHandler{

private:
    rxState_t rxState;
    Partage * partage;
    static BluetoothHandler * pObjet; //bluetooth handler object
    /**
     * @brief bluetooth task loop. receive the data and bluetooth and send commands if necessary
     * 
     */
    static void task();
    
public:
    /**
     * @brief Construct a new Bluetooth Handler object
     * 
     */
    BluetoothHandler();
    /**
     * @brief Destroy the Bluetooth Handler object
     * 
     */
    ~BluetoothHandler();
    /**
     * @brief start the task loop
     * 
     */
    void start();
};