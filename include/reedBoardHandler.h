/**
 * @file reedBoardHandler.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief task updating the status of the reedboard by reading on the i2c bus and updating the game state if necessary
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "partage.h"
#include "reedline.h"

/**
 * @brief task reedboard class
 * 
 */
class ReedBoardHandler{

private:
    i2c * i2cHandle;
    Partage * partage;
    std::vector<ReedLine> reedBoard;
    std::vector<ReedLine> reedBoardPast;
    static ReedBoardHandler * pObjet;
    
    /**
     * @brief task loop 
     * 
     */
    static void task();
    /**
     * @brief check if position is already marked as non valid
     * 
     * @param x 
     * @param y 
     * @return true 
     * @return false 
     */
    static bool isNonValid(uint8_t x, uint8_t y);
    /**
     * @brief check if status of a reed switch is in bad status according to the chessboard
     * 
     * @param x 
     * @param y 
     * @return true 
     * @return false 
     */
    static bool isBadPosition(uint8_t x, uint8_t y);
    /**
    * @brief check if position is set as piece to move    
    * 
    * @param x 
    * @param y 
    * @return true 
    * @return false 
    */
    static bool isPieceToMove(uint8_t x, uint8_t y);
    /**
    * @brief check if position is set as piece to capture    
    * 
    * @param x 
    * @param y 
    * @return true 
    * @return false 
    */
    static bool isPieceToCapture(uint8_t x, uint8_t y);
    /**
     * @brief add this position to the non valid list
     * 
     * @param x 
     * @param y 
     */
    static void addNonValid(uint8_t x, uint8_t y);
    /**
     * @brief remove this position from the non valid list
     * 
     * @param x 
     * @param y 
     */
    static void removeNonValid(uint8_t x, uint8_t y);

    position_t pieceToMove;
    position_t pieceToCapture;
    std::vector<position_t> nonValidMoves;

public:
    /**
     * @brief Construct a new Reed Board Handler object
     * 
     * @param shared 
     */
    ReedBoardHandler(void*shared);
    /**
     * @brief Destroy the Reed Board Handler object
     * 
     */
    ~ReedBoardHandler();
    /**
     * @brief start the reeboard handler task
     * 
     */
    void start();
};