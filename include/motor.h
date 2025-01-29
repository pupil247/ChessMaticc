/**
 * @file        motor.h
 * @author      Antoine Arbour, Félix Parent
 * @brief       Fichier de déclaration de la classe moteur
 * @version     0.1
 * @date        2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#pragma once

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "gpio.h"
#include <esp_system.h>
#include <rom/ets_sys.h>
#include "mutex.h"
#include "tlock.h"

using namespace Gpio;

/* Nombre de pas pour une case */
#define SQUARE_X  192
#define HALF_SQUARE_X SQUARE_X/2
#define SQUARE_Y 250
#define HALF_SQUARE_Y SQUARE_Y/2

/* Mapping en Y*/
#define Y_0_CENTER_POS      0 
#define Y_1                 Y_0_CENTER_POS + SQUARE_Y
#define Y_2                 Y_0_CENTER_POS + (2 * SQUARE_Y)
#define Y_3                 Y_0_CENTER_POS + (3 * SQUARE_Y)
#define Y_4                 Y_0_CENTER_POS + (4 * SQUARE_Y)
#define Y_5                 Y_0_CENTER_POS + (5 * SQUARE_Y)
#define Y_6                 Y_0_CENTER_POS + (6 * SQUARE_Y)
#define Y_7                 Y_0_CENTER_POS + (7 * SQUARE_Y)

/* Mapping en X*/
#define X_0_CENTER_POS      0
#define X_1                 X_0_CENTER_POS + SQUARE_X
#define X_2                 X_0_CENTER_POS +  (2 * SQUARE_X)
#define X_3                 X_0_CENTER_POS +  (3 * SQUARE_X)
#define X_4                 X_0_CENTER_POS +  (4 * SQUARE_X)
#define X_5                 X_0_CENTER_POS +  (5 * SQUARE_X)
#define X_6                 X_0_CENTER_POS +  (6 * SQUARE_X)
#define X_7                 X_0_CENTER_POS +  (7 * SQUARE_X) - 20

/**
 * @name    motorinfos_t
 * @brief   strutcure contenu dans l'objet moteur. Contient les données necessaire pour controler les moteurs
*/
struct motorInfos_t{
    uint8_t currentSpeed;
    bool currentState;
    bool currentDirection; 
    uint8_t cptFinCourseID;
    bool cptFinCourseON;

    uint8_t currentStep;
    uint8_t A;
    uint8_t B;
    uint8_t A_;
    uint8_t B_;
    bool updateMotorInfos;
    uint16_t motorStep;
    uint16_t motorStepGridPos;
    uint8_t motorGridPos;
    bool toggle;
};
/**
 * @class   Motor
 * @brief   Objet de moteur, Contient des méthodes pour controler: sens, vitesse, on/off.
 *          Permet l'accès 
*/
class Motor{
    public:
        /**
         * @name        Motor
         * @param       uint8_t pinA,pinB,pinC,pinD,
         * @brief       Constructeur de l'objet Moteur
        */
        Motor(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD);
        
        /**
         * @name        Motor
         * @brief       Destructeur de l'objet Moteur
        */
        ~Motor();

        /**
         * @name        setMotorSpeed
         * @param       uint8_t speed
         * @brief       Permet le réglage de la vitesse
        */
        void setMotorSpeed(uint8_t speed);

        /**
         * @name        setMotorDirection
         * @param       bool: direction
         * @brief       Permet le réglage de la direction       
        */
        void setMotorDirection(bool direction);

        /**
         * @name        setMotorOnOff
         * @param       bool: state
         * @brief       Ajuste le flag on/off en fonction du paramètre: state       
        */
        void setMotorOnOff(bool state);

        /**
         * @name    setMotorUpdateStatus
         * @param   bool: state
         * @brief   Permet d'ajuster le flag: UpdateStatus qui signifie un changement de parametre dans l'objet moteur.
         *          Set 1 si changement de paramètre    
        */
        void setMotorUpdateStatus(bool state);

        /**
         * @name    setMotorStep
         * @param   uint16_t: stepNumber
         * @brief   Permet d'ajouter le nombre de pas à effectuer     
        */
        void setMotorStep(uint16_t stepNumber);

        void setCptFinCourse(bool,uint8_t);

        /*Aimant*/

        /**
         * @name    setAimant
         * @param   bool: b
         * @brief   Permet l'activation, désactivation de l'aimant    
        */
        void setAimant(bool b );

        /**
         * @name    getMotorSpeed
         * @return  uint8_t: speed
         * @brief   Permet d'obtenir la vitesse actuel du moteur 
        */
        uint8_t getMotorSpeed();

        /**
         * @name    getMotorDirection
         * @return  bool: direction.    return 1 si vers l'avant 
         * @brief   Permet d'obtenir la direction actuel du moteur.  
        */
        bool getMotorDirection();

        /**
         * @name    getMotorOnOff
         * @return  bool: On/off.    return 1 si ON 
         * @brief   Permet d'obtenir l'état on/off actuel du moteur.  
        */
        bool getMotorOnOff();

        /**
         * @name    getMotorDirection
         * @return  bool: updateStatus.    return 1 si un changement de statut 
         * @brief   Permet de savoir si un changement de paramètre dans l'objet moteur à été effectué.  
        */
        bool getMotorUpdateStatus();
        
        /**
         * @name    updatePosition
         * @brief   Méthode qui effectue la séquence requise pour obtenir un mouvement continue dans un sens pour un
         *          moteur pas à pas en fonction de la direction écrite dans l'objet moteur  
        */
        void updatePosition(void);

        void updatePos();

        /**
         * @name    getMotorInfos
         * @return  motorInfos_t: motorInfos.    structure de données du moteur 
         * @brief   Permet d'obtenir la structure de données qui contient l'ensemble des informations liées au contrôle
         *          du moteur.  
        */
        motorInfos_t getMotorInfos();
        
        /**
         * @name    getMotorGridPos
         * @return  uint16_t: positio     
         * @brief   Retourne le nombre de pas effectué à partir du zero calibré. Retourne la position en nb de pas
         *          du moteur  
        */
        uint16_t getMotorGridPos();
        
        /**
         * @name    updateGridPos 
         * @brief   Mets à jour le nombre de pas effectué à partir de zero en fonction de la direction.  
        */  
        void updateGridPos();

        /**
         * @name    updateGridPos 
         * @param   uint16_t: step
         * @brief   Force le nombre de pas en mémoire à la valeur passée en paramètre.  
        */
        void setStepGridPos(uint16_t step);

        /**
         * @name    doOneSquare_X 
         * @brief   Écrit le nombre de pas requis pour bouger d'une case en X dans, l'objet moteur.  
        */  
        void doOneSquare_X();

        /**
         * @name    doOneSquare_y 
         * @brief   Écrit le nombre de pas requis pour bouger d'une case en y, dans l'objet moteur.  
        */
        void doOneSquare_Y();

        /** 
         * @name    doHalfSquare_X 
         * @brief   Écrit le nombre de pas requis pour bouger d'une demi case en X, dans l'objet moteur.  
        */
        void doHalfSquare_X();

        /**
         * @name    doHalfSquare_y 
         * @brief   Écrit le nombre de pas requis pour bouger d'une demi case en y, dans l'objet moteur.  
        */
        void doHalfSquare_Y();

        /**
         * @name    do360 
         * @brief   Écrit le nombre de pas requis pour faire un tour complet, dans l'objet moteur.  
        */
        void do360();
        
        /**
         * @name    do360 
         * @brief   Écrit le nombre de pas requis pour faire un demi tour, dans l'objet moteur.  
        */
        void do180();
        
        
    private:    
        motorInfos_t motorInfos;
        GpioOutput * pinA;
        GpioOutput * pinB;
        GpioOutput * pinA_;
        GpioOutput * pinB_;
        Mutex *mutMoteur;
       
};