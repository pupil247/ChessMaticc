/**
 * @file      Move_t.h
 * @authors   Antoine Arbour, Félix Parent
 * @date      07/05/2024 
 * @brief     Fichier de déclaration de la strucure Move_t afin de d'augmenter la porté de l'enum  
 * @version   0.1
 * @mainpage  Echec_Math_S3 
*/
#pragma once
#include <stdint.h>
#include <stdlib.h>

struct Move_t
{
    uint8_t from[2];  // [x,y]
    uint8_t to[2];
    uint8_t prom;
    bool enPassant;
    bool gRoc;
    bool pRoc;
};