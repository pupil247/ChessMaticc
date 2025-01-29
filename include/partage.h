/**
 * @file    partage.h
 * @author  Félix Parent, Antoine Arbour
 * @brief   Fichier de déclaration de l'objet partage
 * @version 0.1
 * @date    2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "singleton.hpp"
#include "reedBoard.h"
#include "chessBoard.hpp"
#include "Move_t.h"
#include "motor.h"
#include <string>
#include "esp_log.h"
#define STANDARD_BOARD_SIZE 8

enum gameStates_t{
    WAIT,
    CALIBRATE,
    GAME_BEGIN,
    GAME_PAUSE,
    GAME_END,
    CHOICE_PLAYER,
    CHOICE_COMPUTER,
    MOVEMENT_COMPUTER,
    MOVE_PLAYER,
    CAPTURE_OPPONENT_PIECE,
    CAPTURE_PLAYER_PIECE,
    WAITING_PROMOTION
};

enum gameMode_t{
    PLAYING_GAME,
    WATCHING_GAME,
};

struct position_t{
    uint8_t x = 0;
    uint8_t y = 0;
};

struct promotePiece{
    uint8_t x = 0;
    uint8_t y = 0;
    ChessPieceColor color;
};

class Effect;
class Leds;

class Partage : public TSingleton<Partage>{

friend class TSingleton<Partage>;

private:
    Partage();
    ~Partage();
    /*Mutex*/
    Mutex * mutexGameState;
    Mutex * mutReedBoard;
    Mutex * mutexUpdateGameState;
    Mutex * mutexChessBoard;  
    Mutex * mutexI2C;
    Mutex *mutMotor1,*mutMotor2;
    Mutex * mutexLegals;
    Mutex * mutexMove;
    
    /*ChessBoard*/   
    std::vector<ReedLine> reedBoard;
    ChessBoard * chessBoard;
    
    /*Moteur*/
    Motor *motor1,*motor2;
    bool aimant;
   
    /*Game*/
    gameMode_t gameMode;   
    gameStates_t gameState;
    bool updateGameState;

    /*Mouvement*/
    Move_t move; 
    Move_t moveToPlay;
    position_t pieceToMove;
    position_t pieceToCapture;
    std::vector<position_t> nonValid;
    std::vector<position_t> legal;
    bool requestForMoves;
    bool requestForPromotion;
    promotePiece pieceToPromote;
    bool updateBoardMove;
    std::string playerMove;
    /*LEDS*/
    bool updateLeds;
    std::string ledEffect;

public:

    /**
     *  @param      bool
     *  @brief      Permet de signaler un changement de l'état de la partie
    */
    void setUpdateGameState(bool);

    /**
     *  @return     bool
     *  @brief      Retourne l'indicateur de changement de l'état de la partie
    */
    bool getUpdateGameState();

    /**
     *  @param      gameStates_t
     *  @brief      Permet de régler l'état de la partie
    */
    void setGameState(gameStates_t);

    /**
     *  @return     gameStates_t
     *  @brief      Retourne l'état actuel de la partie
    */
    gameStates_t getGameState();

    void setLeds(Leds*);
    Leds* getPtrLeds();
    
    /**
     *  @return     bool
     *  @brief      Retourne un indicateur signifiant si y'a eu un changement de l'effet sur les LEDS
    */
    bool getUpdateLedEffect();

    /**
     *  @param     bool
     *  @brief     Permet de regler un indicateur signifiant si y'a eu un changement de l'effet sur les LEDS
    */
    void setUpdateLedEffect(bool);

    /**
     *  @param      string
     *  @brief      Permet de régler l'effet actif des LEDS, à l'aide du paramètre en string
    */
    void setLedEffect(std::string);

    /**
     *  @return     string
     *  @brief      Retourne l'effet actif des LEDS 
    */
    std::string getLedEffect();

    /**
     *  @param      gameMode_t: mode
     *  @brief      Permet de régler le mode de jeu     
    */
    void setGameMode(gameMode_t mode);

    /**
     *  @return     gameMode_t
     *  @brief      Retourne le mode de jeu actuel            
    */

   /**
    * @brief Get the Game Mode object
    * 
    * @return gameMode_t 
    */
    gameMode_t getGameMode();

    
    std::vector<position_t> getNonValid();
    void addNonValid(uint8_t,uint8_t);
    void removeNonValid(uint8_t,uint8_t);
    bool isNonValid(uint8_t x, uint8_t y);

    void setPieceToMove(uint8_t x, uint8_t y);
    position_t getPieceToMove();
    std::string getPieceToMoveStr();
    void setPieceToCapture(uint8_t x, uint8_t y);
    position_t getPieceToCapture();

    bool getRequestForMoves();
    void setRequestForMoves(bool);
    bool getRequestForPromotion();
    void setRequestForPromotion(bool);

    promotePiece getPieceToPromote();
    void setPieceToPromote(uint8_t x, uint8_t y, ChessPieceColor color);

    void addLegalPosition(uint8_t,uint8_t);
    void deleteLegalPositions();
    bool isLegalPosition(uint8_t,uint8_t);

    //player move
    bool boardMadeItsMove();
    void setBoardMadeMove(bool);
    void setPlayerMove(position_t from, position_t to);

    /**
     *  @return     string
     * 
     *  @brief      Retourne le mouvement effectué par le joueur sous forme de string
    */
    std::string getPlayerMove();

    /**
     *  @return     move_t
     *  @brief      retourne le mouvement à jouer 
    */
    Move_t getMoveToPlay();

    //opponent move

    /**
     *  @param      uint8_t: m[5]
     * 
     *  @brief      Permet d'écrire en mémoire le mouvement passé en paramètre
    */
    void setMove(uint8_t m[5]);

    /**
     * @return      Move_t
     * 
     * @brief       Retourne la dernière commande de mouvement stockée en mémoire
    */
    Move_t getMove();

    /**
     * @param       bool: b
     * 
     * @brief       Permet de set un indicateur signifiant que la commande de mouvement est un "En passant"
     *              (mouvement spécial) 
    */
    void setEnPassant(bool  b);

    /**
     * @param       uint8_t: moveX, moveY
     * 
     * @brief       Permet la conversion de la commande de mouvement sous forme de String
    */
    std::string getPositionStr(uint8_t moveX,uint8_t moveY);

    //ChessBoard an reedboard
    /**
     * @return      vector<ChessPiece>
     * 
     * @brief       Retourne le vecteur contenant les données relatives aux pièces d'échec 
    */
    std::vector<std::vector<ChessPiece>> getChessBoard();

    /**
     * @return      *ChessBoard
     * 
     * @brief       Retourne un pointeur Chessboard. Donne accès au méthodes de l'objet ChessBoard
    */
    ChessBoard* getPtrChessBoard();

    /**
     * @return      vector<ReedLine>
     * 
     * @brief       Retourne le vecteur contenant les données relatives aux capteur de pièces
    */
    std::vector<ReedLine> getReedBoard();

    /**
     * @param       vector<ReedLine>
     * 
     * @brief       Permet d'écrire dans l'objet ReedLine avec le paramètre passé.
    */
    void setReedBoard(std::vector<ReedLine>);

    //Motors
    
    /**
     *  @param      Motor: * m,*m2 
     *  
     *  @brief      Initialisation des pointeur moteur dans l'objet partage  
    */
    void initShareMotor(Motor *m,Motor *m2);

    /**
     * @param       uint16_t: step
     * 
     * @brief       Ajoute le nombre de pas désiré dans au moteur 1 
    */
    void addStepMotor1(uint16_t step);

    /**
     * @param       uint16_t: step
     * 
     * @brief       Ajoute le nombre de pas désiré dans au moteur 2 
    */
    void addStepMotor2(uint16_t step);

    /**
     * @param       bool: b
     * 
     * @brief       Permet la mise en marche et l'arrêt du moteur 1 
    */
    void motor1SetOnOff(bool b);

    /**
     * @param       bool: b
     * 
     * @brief       Permet la mise en marche et l'arrêt du moteur 2 
    */
    void motor2SetOnOff(bool b);

    /**
     * @param       bool: b
     * 
     * @brief       Permet le choix du sens de  rotation du moteur 1 
    */
    void directionStepMotor1(bool d);

    /**
     * @param       bool: b
     * 
     * @brief       Permet le choix du sens de rotation du moteur 2 
    */
    void directionStepMotor2(bool d);

    /**
     * @param       uint8_t: speed
     * 
     * @brief       Permet le l'ajustement de vitesse du moteur 1 de 0 à 100
    */
    void setSpeedMotor1(uint8_t speed);

    /**
     * @param       uint8_t: speed
     * 
     * @brief       Permet le l'ajustement de vitesse du moteur 2 de 0 à 100
    */
    void setSpeedMotor2(uint8_t speed);

    /**
     *  @param      bool
     * 
     *  @brief      Permet l'activation et déSactivation de l'aimant
    */
    void setAimant(bool);

     /**
     *  @return     bool
     * 
     *  @brief      Retourne l'état actuel de l'aimant
    */
    bool getAimant();
    
    /**
     *  @brief      Efface les données sur le bus I2C
    */
    void clearI2C();

    /**
     *  @name       waiForI2Cclear
     *  @brief      Fonction permettant la synchronisation des operations sur le bus I2C 
    */
    void waitForI2CClear();
    

};