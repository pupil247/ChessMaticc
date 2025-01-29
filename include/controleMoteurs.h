/**
 * @file        controleMoteurs.h
 * @author      Antoine Arbour, Félix Parent
 * @brief       Fichier de déclaration de la tâche moteur
 * @version     0.1
 * @date        2024-05-10
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

#pragma once

#include "motor.h"
#include <freertos/FreeRTOS.h>
#include "bluetoothHandler.h"
#include <freertos/task.h>
#include <esp_timer.h>
#include <driver/timer.h>
#include <partage.h>
#include "chessBoard.hpp"
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include "soc/soc.h"
#include "reedBoard.h"

#include "ioMapping.h"

#define LINE_0_CENTER
  
#define pdSecond pdMS_TO_TICKS(1000)
using namespace ioMapping;


extern volatile bool flagInt0;
extern volatile bool flagInt1;
extern Partage * partage;
//extern std::vector<std::vector<ChessPiece>>   chessBoard;

enum motorStates_t{
    WAITING_MOVE, 
    CAPTURE,
    CAPTURE_PATHING,
    FROM_POSITION,
    MOVE_PATHING,
    MOVE_ON,
    MOVE_END
};

enum moveStates_t{
    GET_ON_LINE,
    MOVE_X,
    MOVE_Y,
    GET_ON_TILE
};

extern moveStates_t stateMove;
extern motorStates_t stateMotor;
void initMotors(Partage *partage);

/**
* @param    void *arg
* @brief     ISR du timer0
*/
bool IRAM_ATTR Timer0_ISR(void* arg);

/**
* @param    void *arg
* @brief     ISR du timer1
*/
bool IRAM_ATTR Timer1_ISR(void* arg);

/**
* @param    void *arg
* @brief     Gestion ISR sur interruption externe 
* @example  Si le plateau en X touche au cpt, le sens du moteur1 est inverser et il recule de 20 pas 
*/
void IRAM_ATTR cptFinCourse_X_0_ISR(void* arg);

/**
* @param    void *arg
* @brief     Gestion ISR sur interruption externe 
* @example  Si le plateau en X touche au cpt, le sens du moteur1 est inverser et il recule de 20 pas 
*/
void IRAM_ATTR cptFinCourse_X_1_ISR(void* arg);

/**
* @param    void *arg
* @brief     Gestion ISR sur interruption externe 
* @example  Si le plateau en Y touche au cpt, le sens du moteur2 est inverser et il recule de 20 pas 
*/
void IRAM_ATTR cptFinCourse_Y_0_ISR(void* arg);

/**
* @param    void *arg
* @brief     Gestion ISR sur interruption externe 
* @example  Si le plateau en Y touche au cpt, le sens du moteur2 est inverser et il recule de 20 pas 
*/
void IRAM_ATTR cptFinCourse_Y_1_ISR(void* arg);

/**
* @param    void *parameter
* @brief     Tâche de gestion des moteurs Gestion des moteurs en fonction de l'état de la partie,
*            agit lorsqu'une cmd est reçu 
*/
void motorHandler(void * parameter);

/**
 * @param   Partage *shared :  objet de partage
 * @desc    Initialisation de la tâche de gestion moteur
 * @brief   Initialise les variables, états moteur, interruption et autres procédé requis au démarage de la tâche  
*/
void initMotors(Partage *shared);

/**
 * @name  updateMoteurs
 * @desc  Fait la mise à jour des timer en fonction de la vitesse dans les objets moteur 
*/
void updateMoteurs();

/**
 *  @brief  Déplacement des moteurs Fait la mise à jour de la position des moteurs si ils sont ON,
 *          réactive les interruption
*/
void moveMotors();

/**
 * @return  bool, 1 si terminé 
 * @brief   Calibration moteurs. Envoi les plateau vers 0 jusqu'a ce qu'il atteigne les capteurs de fin de course.
 *          Puis, place les plateau à l'origine
*/
bool calibrateMotors(void);

/**
 * @param   uint16_t cmd[2]
 * @brief   Positionne les moteurs en fonction d'une coordonée en uint16_t
*/
void goToCMD(uint16_t cmd[2]);

/**
 * @param   Move_T  move
 * @brief           Effectue la conversion d'une coordonnées x,y en uint8 vers le nombre de pas en uint16
*/
void setMoveCoord(Move_t move);

/**
 * @param   Move_t move
 * @return  bool   return 1 quand terminé  
 * @brief   Fonction tentant de corriger une erreur de mouvement, Effectue un mouvement carré qui,
 *          déplace son coin d'origine a chaque ittération et qui augmente sa surface afin de trouver
 *          le capteur.  
*/
bool findReed(Move_t move);



