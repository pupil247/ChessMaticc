/**
 * @file ledBoardHandler.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "partage.h"
#include "Leds.h"

class LedBoardHandler{

private:
    Partage * partage;
    Leds * ledBoard;
    static LedBoardHandler * pObjet;
    /**
     * @brief ledboard task loop. check if new effect and set it, then run effect on the ledboard
     * 
     */
    static void task();
public:
    /**
     * @brief Construct a new Led Board Handler object
     * 
     * @param shared 
     */
    LedBoardHandler(void*shared);
    /**
     * @brief Destroy the Led Board Handler object
     * 
     */
    ~LedBoardHandler();
    /**
     * @brief start the led board task
     * 
     */
    void start();
};