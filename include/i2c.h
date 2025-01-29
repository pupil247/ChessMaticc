/**
 * @file i2c.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief library to use i2c on the esp32 using the sdk methods of esp idf
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define I2C_ACK_CHECK_EN 1  //validation de la donnée(ack) du slave)

/**
 * @brief i2c class
 * 
 */
class i2c {
public:
    struct i2c_t{
        uint8_t _sda; 
        uint8_t _scl; 
        uint8_t _i2cSel;    
    };
    /**
     * @brief Construct a new default i2c object(do nothing if no parameters)
     * 
     */
    i2c(){};
    /**
     * @brief Destroy the i2c object
     * 
     */
    ~i2c(){};
    /**
     * @brief Construct a new i2c object on selected pins and i2c interface
     * 
     * @param sda 
     * @param scl 
     * @param i2csel 
     */
    i2c(uint8_t sda, uint8_t scl, uint8_t i2csel);
    /**
     * @brief write a byte on the i2c bus from a certain adress
     * 
     * @param addr 
     * @param byte 
     * @return esp_err_t 
     */
    esp_err_t writeByte(uint8_t addr, uint8_t byte);
    /**
     * @brief write data buffer on the i2c bus from a certain adress
     * 
     * @param addr 
     * @param data 
     * @param dataLen 
     * @return esp_err_t 
     */
    esp_err_t writeBytes(uint8_t addr, uint8_t * data, int dataLen);
    /**
     * @brief read buffer on the i2c bus from a certain adress
     * 
     * @param addr 
     * @param data 
     * @param dataLen 
     * @return esp_err_t 
     */
    esp_err_t readBytes(uint8_t addr, uint8_t * data, int dataLen);

private:
    i2c_t i2cInstance;
};
