/**
 * @file ioMapping.h
 * @author Felix Parent (parent.felix@domain.com)
 * @brief io declarations
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <stdio.h>

namespace ioMapping{

    //i2c
    const uint8_t PIN_SDA = 48;
    const uint8_t PIN_SCL = 47;

    //led
    const uint8_t PIN_LED = 1;

    //ledBoard
    const uint8_t PIN_LED_CONTROL = 38;

    //motors
    const uint8_t GPIO1_1 = 4;
    const uint8_t GPIO2_1 = 5; 
    const uint8_t GPIO3_1 = 6;
    const uint8_t GPIO4_1 = 7;

    const uint8_t  GPIO1_2 = 21;
    const uint8_t  GPIO2_2 = 37;
    const uint8_t  GPIO3_2 = 36;
    const uint8_t  GPIO4_2 = 35;

    const uint8_t GPIO_AIMANT = 8;



};