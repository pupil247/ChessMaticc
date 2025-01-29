/**
 * @file    chessPiece.hpp
 * @author  Félix Parent, Antoine Arbour
 * @brief   Fichier de déclaration de la classe ChessPiece
 * @version 0.1
 * @date    2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "esp_log.h"

enum class ChessPieceType {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    EMPTY
};

// Enum to represent chess piece colors
enum class ChessPieceColor {
    WHITE,
    BLACK,
    EMPTY
};

// Class representing a ChessPiece
class ChessPiece {
public:

    /**
     * @brief Construct a new Chess Piece object
     * 
     * @param type 
     * @param color 
     */
    ChessPiece(ChessPieceType type, ChessPieceColor color)
    : type(type), color(color) {
     _f=0;
     _g=0;
     _h=0;  
    }

    /**
     * @brief Destroy the Chess Piece object
     * 
     */
    ~ChessPiece(){};    

    /**
     * @brief   Get the Color object
     * 
     * @return  ChessPieceColor 
     */
    ChessPieceColor getColor(){
        return this->color;
    }

    /**
     * @brief   Get the Type object
     * 
     * @return ChessPieceType 
     */
    ChessPieceType getType(){
        return this->type;
    }

    /**
     * @brief Set the Color object
     * 
     * @param color 
     */
    void setColor(ChessPieceColor color){
        this->color = color;
    }

    /**
     * @brief Set the Type object
     * 
     * @param type 
     */
    void setType(ChessPieceType type){
        this->type = type;    
    }


       

private:
    ChessPieceType type;
    ChessPieceColor color;
    uint8_t _h;
    float _f,_g;
    uint8_t x = 0;
    uint8_t y = 0;
};
