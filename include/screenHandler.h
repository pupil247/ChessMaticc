/**
 * @file screenHandler.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief task handling the lcd screen controlled by i2c
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "partage.h"
#include "i2c.h"
#include "lcdi2c.h"

/**
 * @brief lcd screen task class
 * 
 */
class ScreenHandler{

private:
    Partage * partage;
    lcdI2C * screen;
    static ScreenHandler * pObjet;
    /**
     * @brief task loop running
     * 
     */
    static void task();
public:
    /**
     * @brief Construct a new Screen Handler object
     * 
     * @param shared 
     */
    ScreenHandler(void*shared);
    /**
     * @brief Destroy the Screen Handler object
     * 
     */
    ~ScreenHandler();
    /**
     * @brief start the lcd task
     * 
     */
    void start();
};