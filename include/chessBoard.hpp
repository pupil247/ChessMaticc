/**
 * @file        chessBoard.hpp
 * @author      Félix Parent, Antoine Arbour
 * @brief       
 * @version     0.1
 * @date        2024-05-10
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

#pragma once
#include <vector>
#include <stdio.h>
#include "chessPiece.hpp"
#include "mutex.h"
#include "tlock.h"
#include "Move_t.h"

// Class representing a ChessPiece
class ChessBoard { 
public: 
    /**
     * @brief Constructeur De l'objet chessBoard
     * 
     * @param boardSize 
     */
    ChessBoard(uint8_t boardSize){
        ESP_LOGI("mutex","chessboardinit");
        mutPlayerColor = new Mutex();
        mutChessBoard = new Mutex();
        mutCurrentTurn = new Mutex();
        
        mutPlayerColor->release();
        mutChessBoard->release();
        mutCurrentTurn->release();

        std::vector<ChessPiece> emptyVector;
        for(int i = 0; i < 8; i++)
            chessBoard.push_back(emptyVector);

        chessBoard[0].push_back(ChessPiece(ChessPieceType::KNIGHT, ChessPieceColor::WHITE));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::BISHOP, ChessPieceColor::WHITE));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::QUEEN, ChessPieceColor::WHITE));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::KING, ChessPieceColor::WHITE));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::BISHOP, ChessPieceColor::WHITE));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::KNIGHT, ChessPieceColor::WHITE));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::ROOK, ChessPieceColor::WHITE));
        for(int i = 0; i < 8; i++){
            chessBoard[1].push_back(ChessPiece(ChessPieceType::PAWN, ChessPieceColor::WHITE));
        }
        for(int i = 2; i < 6; i++){
            for(int j = 0; j < 8; j++){
                chessBoard[i].push_back(ChessPiece(ChessPieceType::EMPTY, ChessPieceColor::EMPTY));
            }    
        }

        chessBoard[7].push_back(ChessPiece(ChessPieceType::ROOK, ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::KNIGHT, ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::BISHOP, ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::QUEEN, ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::KING, ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::BISHOP, ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::KNIGHT, ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::ROOK, ChessPieceColor::BLACK));
        for(int i = 0; i < 8; i++){
            chessBoard[6].push_back(ChessPiece(ChessPieceType::PAWN, ChessPieceColor::BLACK));
        }

        

    }
    /**
     * @brief Destroy the Chess Board object
     * 
     */
    ~ChessBoard(){
        delete mutPlayerColor;
        delete mutChessBoard;
        delete mutCurrentTurn;    
    };
    
    /**
     * @brief Get the Chess Board object
     * 
     * @return std::vector<std::vector<ChessPiece>> 
     */
    std::vector<std::vector<ChessPiece>> getChessBoard();

    /**
     * @brief Permets d'amorcer la partie avec la couleur passé en paramètre
     * 
     * @param playerColor 
     */
    void beginGame(bool playerColor);

    /**
     * @brief   Permets de mettre fin à la partie en cours
     * 
     */
    void endGame();

    /**
     * @brief Get the Player Color object
     * 
     * @return true     white
     * @return false    black
     */
    bool getPlayerColor();

    /**
     * @brief   Retourne un indicateur qui permet de connaître quel coulour doit jouer
     * 
     * @return true     white
     * @return false    black
     */
    bool getCurrentTurn();

    /**
     * @brief   Permet de déplacer la pièce du joueur en fonction de la cmd de mouvement
     * 
     * @param   Move_t
     */
    void movePlayerPiece(Move_t);

    /**
     * @brief   Permet de déplacer la pièce de l'opposant en fonction de la cmd de mouvement
     * 
     */
    void moveOpponentPiece();

    /**
     * @brief Permets de déplacer la pièce de l'opposant en fonction de la cmd de mouvement  
     * 
     * @param move 
     */
    void moveOpponentPiece(Move_t move);

    /**
     * @brief Set the Piece Type object
     * 
     * @param x 
     * @param y 
     * @param type 
     * @param color 
     */
    void setPieceType(uint8_t x, uint8_t y, ChessPieceType type, ChessPieceColor color);

    /**
     * @brief   Permet de déterminer si la position visé par la commande est occupé 
     * 
     * @param   move 
     * @return  true    vide
     * @return  false   occupé
     */
    bool isToTileEmpty(Move_t move);

    /**
     * @brief   Permet de déterminer si la position d'origine du déplacement est occupé
     * 
     * @param move 
     * @return true     vide
     * @return false    occupé
     */
    bool isFromTileEmpty(Move_t move);

    /**
     * @brief   Permets de déterminer si le chemin vers la case est libre afin d'éviter une colision
     * 
     * @param move 
     * @return true     libre
     * @return false    obstrué
     */
    bool isPathToTileEmpty(Move_t move);

    void setChevalMove( bool b);
    bool getChevalMove();

private:
    Mutex * mutPlayerColor;
    Mutex * mutChessBoard;
    Mutex * mutCurrentTurn;

    bool activeGame;
    bool playerColor;
    bool currentTurn;
    bool chevalMove;
    std::vector<std::vector<ChessPiece>> chessBoard;    
};