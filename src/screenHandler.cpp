#include "screenHandler.h"

ScreenHandler::ScreenHandler(void * shared){
    screen = (lcdI2C *)shared;
    partage = Partage::getInstance();
    pObjet = this;
}

ScreenHandler::~ScreenHandler(){

}

void ScreenHandler::start(){
    xTaskCreatePinnedToCore( (TaskFunction_t)task,    "Task lcd",    5000,      NULL,    1,    NULL,    0);
}

void ScreenHandler::task(){
    Partage * partage = pObjet->partage;
    lcdI2C * screen = pObjet->screen;
    std::string str;
    while(1){
        if(partage->getUpdateGameState()){
            //ESP_LOGI("lcd","update");
            partage->setUpdateGameState(0);
            partage->waitForI2CClear();
            switch(partage->getGameState()){
                case WAIT:
                ESP_LOGI("lcd","wait");
                    screen->clearDisplay();
                    //screen->setPos(0,5);
                    //screen->sendChar('0');
                    //screen->sendChar('0');
                    //screen->setPos(1,8);
                    //screen->sendChar('1');
                    screen->setPos(1,2);
                    //screen->sendChar('3');
                    //screen->setPos(2,6);
                    //screen->sendChar('2');
                    screen->sendString("WAITING FOR GAME");
    
                    break;
                case GAME_BEGIN:
                ESP_LOGI("lcd","begin");
                    screen->clearDisplay();
                    screen->setPos(1,4);
                    screen->sendString("BEGIN GAME");
                    //pObjet->screen->setPos(3,3);
                    //pObjet->screen->sendString("PLEASE PUT THE PIECES");
                    //pObjet->screen->setPos(4,0);
                    //pObjet->screen->sendString("AT THEIR RESPECTIVE PLACE");
                    break;
                case MOVEMENT_COMPUTER:
                ESP_LOGI("lcd","movement");
                    screen->clearDisplay();
                    screen->setPos(1,2);
                    screen->sendString("MOVEMENT COMPUTER");
                    break;
                case CHOICE_PLAYER:
                ESP_LOGI("lcd","choice player");
                    screen->clearDisplay();
                    screen->setPos(1,2);
                    screen->sendString("CHOICE PLAYER");
                    screen->setPos(2,0);
                    screen->sendString("Last computer move:");
                    screen->setPos(3,2);
                    screen->sendString("From:");
                    screen->setPos(3,8);
                    ESP_LOGI("test","%d,%d",partage->getMove().from[0],partage->getMove().from[1]);
                    str.push_back( partage->getPtrChessBoard()->getPlayerColor() == 0 ? (char)((7 - partage->getMove().from[0]) + 97) : (char)(partage->getMove().from[0] + 97));
                    str.push_back( partage->getPtrChessBoard()->getPlayerColor() == 0 ? (char)(( 7 - partage->getMove().from[1]) + 49) : (char)(partage->getMove().from[1] + 49));
                    screen->sendString(str.c_str());
                    str.clear();
                    screen->setPos(3,12);
                    screen->sendString("To:");
                    str.push_back( partage->getPtrChessBoard()->getPlayerColor() == 0 ? (char)(( 7 - partage->getMove().to[0]) + 97) : (char)(partage->getMove().to[0] + 97));
                    str.push_back( partage->getPtrChessBoard()->getPlayerColor() == 0 ? (char)(( 7 - partage->getMove().to[1]) + 49) : (char)(partage->getMove().to[1] + 49));
                    screen->setPos(3,16);
                    screen->sendString(str.c_str());
                    str.clear();
                    //TODO implement screen display for last computer move
                    break;
                
                case MOVE_PLAYER:
                ESP_LOGI("lcd","move player");
                    screen->clearDisplay();
                    screen->setPos(1,4);
                    screen->sendString("DROP PIECE");
                    screen->setPos(2,3);
                    screen->sendString("capture piece");
                    screen->setPos(3,5);
                    screen->sendString("if needed");
                    break;
                case CAPTURE_OPPONENT_PIECE:
                ESP_LOGI("lcd","capture");
                    screen->clearDisplay();
                    screen->setPos(1,4);
                    screen->sendString("DROP PIECE");
                    screen->setPos(2,2);
                    screen->sendString("dispose piece");
                    screen->setPos(3,5);
                   
                    break;
                case CHOICE_COMPUTER:
                ESP_LOGI("lcd","choice");
                    screen->clearDisplay();
                    screen->setPos(1,0);
                    screen->sendString("WAITING FOR COMPUTER");
                    if(partage->getGameMode() == gameMode_t::PLAYING_GAME){
                        screen->setPos(2,2);
                        screen->sendString("Last board move:");
                        screen->setPos(3,2);
                        screen->sendString("From:");
                        screen->setPos(3,8);
                        screen->sendString(partage->getPlayerMove().substr(0,2).c_str());
                        screen->setPos(3,12);
                        screen->sendString("To:");
                        screen->sendString(partage->getPlayerMove().substr(2,4).c_str());
                        screen->setPos(3,16);
                    }
                    
                    ESP_LOGI("test","end lcd");
                    //TODO implement screen display for last board move
                    break;
                case WAITING_PROMOTION:
                    screen->clearDisplay();
                    screen->setPos(1,2);
                    screen->sendString("WAITING FOR PROMOTION CHOICE");
                    screen->setPos(2,2);
                    screen->sendString("Current ");
                    //TODO implement screen display for move to promote
                    break;
                case CALIBRATE:
                ESP_LOGI("lcd","calibrate");
                    screen->clearDisplay();
                    screen->setPos(1,2);
                    screen->sendString("CALIBRATE");
                    break;
                default:
                    ESP_LOGI("lcd","erreur");
                    screen->clearDisplay();
                    screen->setPos(2,2);
                    screen->sendString("erreur");
                    break;
            };
            partage->clearI2C();
            
        }
        vTaskDelay(pdMS_TO_TICKS(100));    
    }
    
}

ScreenHandler* ScreenHandler::pObjet = nullptr;