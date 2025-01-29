/**
 * @file reedBoard.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief reedswitches board class file
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "reedLine.h"
#include "i2c.h"
#include "partage.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define ADD_REED1 0b0111000
#define ADD_REED2 0b0111001
#define ADD_REED3 0b0111010
#define ADD_REED4 0b0111011
#define ADD_REED5 0b0111100
#define ADD_REED6 0b0111101
#define ADD_REED7 0b0111110
#define ADD_REED8 0b0111111

//forward declaration of class partage
class Partage;

/**
 * @brief Reed Switches status object
 * 
 */
class ReedBoard {
public:
    ReedBoard(i2c& i2cHandle);

    /**
     * @brief Get the ReedBoard object status
     * 
     * @return std::vector<ReedLine> 
     */
    std::vector<ReedLine> getReedStatus();

    /**
     * @brief init the reedboard
     * 
     */
    void init(); 
private:
    Partage * partage;
    i2c& _i2c;
    static ReedBoard* pObjet;
    std::vector<ReedLine> reedBoard;

    /**
     * @brief update the reed status by reading the i2c bus
     * @return void
     */
    static void updateReedStatus();
};