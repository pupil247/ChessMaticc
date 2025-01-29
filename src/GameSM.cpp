/*#include "GameSM.h"

GameSM::GameSM(){
    partage = Partage::getInstance();
    partage->setGameState(WAIT);
    //xTaskCreatePinnedToCore( &run,    "Task Game State Machine",    5000,      NULL,    1,    NULL,    0);
};

GameSM::~GameSM(){

};

void GameSM::run(){
    switch(partage->getGameState()){
        case WAIT:
            if(0) //if user begin game
                partage->setGameState(GAME_BEGIN);

            break;
        case GAME_BEGIN:
            if(0) //if user choose its color
                partage->setGameState(CHOICE_PLAYER);
            break;
        case GAME_END:
            if(0) //if user acknowledge end of game
                partage->setGameState(WAIT);
            break;
        case GAME_PAUSE:
            if(0) //if player pause game
                partage->setGameState(WAIT);
            break;
        case MOVEMENT_COMPUTER:
            if(0) //if computer end move
                partage->setGameState(CHOICE_PLAYER);
            break;
        case CHOICE_PLAYER:
            if(0) //if player end its turn
                partage->setGameState(CHOICE_COMPUTER);
            break;
        case CHOICE_COMPUTER:
            if(0) //if received move from computer
                partage->setGameState(CHOICE_PLAYER);
            break;
    }
};*/

