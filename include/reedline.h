/**
 * @file reedline.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief reed line object, 8 reed switches on a pcf8754
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "i2c.h"
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief class reedline
 * 
 */
class ReedLine {
public:
    /**
     * @brief Construct a new Reed Line object
     * 
     * @param i2cAddr 
     */
    ReedLine(uint8_t i2cAddr);
    /**
     * @brief init the reed line object
     * 
     */
    void init();
    /**
     * @brief Get the i2c addr of the object
     * 
     * @return uint8_t 
     */
    uint8_t getAddr();
    /**
     * @brief Get the status of the reedline
     * 
     * @return std::vector<bool> 
     */
    std::vector<bool> getStatus();
    /**
     * @brief Get the status of the reedline
     * 
     * @return std::vector<bool> 
     */
    uint8_t getStatusInt();
    /**
     * @brief Set the status of the reedline
     * 
     */
    void setStatus(uint8_t);
private:
    std::vector<bool> reedStatus;
    uint8_t reedStatusInt;
    uint8_t _i2cAdd;
    
};