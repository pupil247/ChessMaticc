#include "BluetoothHandler.h"
#include "esp_log.h"

BluetoothHandler::BluetoothHandler(){ 
    rxState = STX;
    pObjet = this;
}

BluetoothHandler::~BluetoothHandler(){

}

void BluetoothHandler::start(){
    pObjet->partage = Partage::getInstance();
    xTaskCreatePinnedToCore( (TaskFunction_t)task,    "Task bluetooth",    5000,      NULL,    1,    NULL,    0);
}

void BluetoothHandler::task(){
    
    Bluetooth::initBLE();
    char car = 0;
    std::string str;
    uint8_t idCmd = 0;
    uint8_t cmdSize = 0;
    std::vector<uint8_t> data;
    
    bool trameValide = 0;
    Partage * partage = pObjet->partage;

    uint8_t onOff;
    uint8_t red;
    uint8_t green; 
    uint8_t blue;  
    uint8_t speedEffect;
    std::string effectName;
    uint8_t moveX = 0;
    uint8_t moveY = 0;
    bool responseSent = 0;

    while(1){
        
        if(Bluetooth::dataAvailable()){
            while(Bluetooth::dataAvailable()){
                car = Bluetooth::readBuffer();
                switch(pObjet->rxState){
                    case STX:
                        if(car == 60){
                            pObjet->rxState = ID;
                        }
                        break;
                    case ID:
                        idCmd = car;
                        pObjet->rxState = SIZE;
                        break;
                    case SIZE:
                        cmdSize = car;
                        if(cmdSize > 0)
                            pObjet->rxState = DATA;
                        else
                            pObjet->rxState = CHECKSUM;    
                        break;
                    case DATA:
                        if(cmdSize > 0)
                            data.push_back(car);
                        if(data.size() >= cmdSize){
                            pObjet->rxState = CHECKSUM;
                        }
                        break;
                    case CHECKSUM:
                        if(Bluetooth::calculateChecksum(idCmd,cmdSize,data) != car){
                            data.clear();
                            pObjet->rxState = STX;
                        }
                        else
                            pObjet->rxState = ETX;
                        break;
                    case ETX:
                        if(car == 62)
                            trameValide = 1;
                        else
                            data.clear();
                        pObjet->rxState = STX;
                        break;
                    default:
                        break;

                }
                if(trameValide){
                    trameValide = 0;
                    switch(idCmd){
                        case possibleCommands_t::START_GAME:
                            ESP_LOGI("CMD RECEIVED", "START GAME");
                            if(cmdSize == 1){
                                Bluetooth::sendCommand(0,1,data); //success begin game 
                                vTaskDelay(pdMS_TO_TICKS(1000));
                                responseSent = 0;
                                partage->setPieceToCapture(255,255);
                                partage->setPieceToMove(255,255);
                                partage->deleteLegalPositions();
                                partage->setPieceToPromote(255,255,ChessPieceColor::EMPTY);
                                if(data[0] == 0 || data[0] == 1){
                                    partage->getPtrChessBoard()->beginGame((bool)data[0]);
                                    ESP_LOGI("start game","playing game");
                                    partage->setGameMode(gameMode_t::PLAYING_GAME);
                                    partage->setGameState(gameStates_t::GAME_BEGIN);
                                    partage->getPtrChessBoard()->beginGame(data[0]);
                                    if(data[0] != 0) //if player is beginning
                                        partage->setGameState(gameStates_t::CHOICE_PLAYER);
                                }
                                else{
                                    partage->setGameMode(gameMode_t::WATCHING_GAME);
                                    partage->setGameState(gameStates_t::GAME_BEGIN);
                                    partage->getPtrChessBoard()->beginGame(0);
                                   
                                }
                                partage->setLedEffect("static");
                                partage->setUpdateLedEffect(1);
                                partage->setUpdateGameState(1);     
                            }
                            else
                                ESP_LOGI("ERROR", "ERROR DURING START GAME");    
                            break;
                            break;
                        case possibleCommands_t::END_GAME:
                            if(partage->getGameState() != gameStates_t::WAIT){
                                responseSent = 0;
                                ESP_LOGI("CMD RECEIVED", "END GAME");
                                partage->setPieceToCapture(255,255);
                                partage->setPieceToMove(255,255);
                                partage->deleteLegalPositions();
                                
                                partage->setLedEffect("stars:64;1;99000;10;43;222;225;0");
                                partage->setUpdateLedEffect(true);
                                partage->setPieceToPromote(255,255,ChessPieceColor::EMPTY);
                                partage->setGameState(gameStates_t::WAIT);
                            }
                            break;
                        case possibleCommands_t::WAIT_BOARD_MOVE:
                            ESP_LOGI("CMD RECEIVED", "REQUEST FOR MOVE");
                            
                            if(cmdSize == 5 && (partage->getGameState() == gameStates_t::CHOICE_COMPUTER || partage->getGameState() == gameStates_t::GAME_BEGIN)){
                                Move_t lastMove = partage->getMove();
                                partage->setMove(data.data());
                                //if redundent request and response has already been sent, stuck in wrong state
                                //resend board move
                                if((lastMove.from[1] == partage->getMove().from[1]) && (lastMove.from[0] == partage->getMove().from[0]) && (lastMove.to[0] == partage->getMove().to[0]) && (lastMove.to[1] == partage->getMove().to[1]) && responseSent){
                                        
                                    partage->setBoardMadeMove(1);
                                    ESP_LOGI("no response","resending move made by board");
                                }
                                else{ //it means the request is not sent or the app made a new move
                                    responseSent = 0;

                                    ESP_LOGI("GAME MODE:", "%d",partage->getGameMode());
                                    if(partage->getGameMode() == gameMode_t::PLAYING_GAME){
                                        
                                        Move_t moveBoard = partage->getMove();
                                        ESP_LOGI("move made by board", "from: %d%d, to: %d%d", moveBoard.from[0],moveBoard.from[1],moveBoard.to[0],moveBoard.to[1]);
                                        partage->setGameState(gameStates_t::MOVEMENT_COMPUTER);
                                    }  
                                    else{                             

                                        partage->setGameState(gameStates_t::MOVEMENT_COMPUTER);
                                        Move_t moveBoard = partage->getMove();
                                        ESP_LOGI("move made by board", "from: %d%d, to: %d%d", moveBoard.from[0],moveBoard.from[1],moveBoard.to[0],moveBoard.to[1]);
                                    }
                                }
                            }
                            
                            else
                                ESP_LOGI("ERROR", "ERROR DURING MOVE REQUEST");  
                            break;
                            break;
                        case possibleCommands_t::ASK_POSSIBLE_MOVES: //response possible moves
                            ESP_LOGI("CMD RECEIVED", "POSSIBLE MOVES");
                            if(cmdSize && !(cmdSize % 4) && partage->getGameState() != gameStates_t::WAIT){
                                for(int i = 0; i < (cmdSize / 4); i++){
                                    switch(data[i * 4 +2]){
                                        case 'a':
                                            moveY = 0;
                                            break;
                                        case 'b':
                                            moveY = 1;
                                            break;
                                        case 'c':
                                            moveY = 2;
                                            break;
                                        case 'd':
                                            moveY = 3;
                                            break;
                                        case 'e':
                                            moveY = 4;
                                            break;
                                        case 'f':
                                            moveY = 5;
                                            break;
                                        case 'g':
                                            moveY = 6;
                                            break;
                                        case 'h':
                                            moveY = 7;
                                            break;
                                        default:
                                            moveY = 255;
                                            break;
                                    }
                                    switch(data[i * 4 +3]){
                                        case '1':
                                            moveX = 0;
                                            break;
                                        case '2':
                                            moveX = 1;
                                            break;
                                        case '3':
                                            moveX = 2; 
                                            break;
                                        case '4':
                                            moveX = 3;
                                            break;
                                        case '5':
                                            moveX = 4;
                                            break;
                                        case '6':
                                            moveX = 5;
                                            break;
                                        case '7':
                                            moveX = 6;
                                            break;
                                        case '8':
                                            moveX = 7;
                                            break;
                                        default:
                                            moveX = 255;
                                            break;
                                    }
                                    moveX = partage->getPtrChessBoard()->getPlayerColor() == 0 ? 7 - moveX : moveX;
                                    moveY = partage->getPtrChessBoard()->getPlayerColor() == 0 ? 7 - moveY : moveY;
                                    if(moveX != 255 && moveY != 255){
                                        partage->addLegalPosition(moveX,moveY);
                                    }
                                    ESP_LOGI("MOVE", "FROM: %c%c TO: %c%c",data[i * 4],data[i * 4+1],data[i * 4+2],data[i * 4+3]);
                                }
                            }
                            else
                                ESP_LOGI("ERROR", "ERROR DURING MOVE REQUEST");    
                            break;
                        case possibleCommands_t::MOTOR_STATE:
                            if(data.size() == 2 && partage->getGameState() == gameStates_t::WAIT){
                                if(data[0] == 1){
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 1");
                                    partage->addStepMotor1(1000);
                                    if(data[1] == 0){ 
                                         partage->motor1SetOnOff(1); 
                                        ESP_LOGI("CMD RECEIVED", "MOTOR ON");
                                    } 
                                    else {
                                        partage->motor1SetOnOff(0); 
                                        ESP_LOGI("CMD RECEIVED", "MOTOR OFF");
                                    }
                                    
                                }                                 
                                else{
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 2");
                                   
                                    if(data[1] == 0){ 
                                        partage->motor2SetOnOff(1); 
                                        partage->addStepMotor2(1000);
                                        ESP_LOGI("CMD RECEIVED", "MOTOR ON");
                                    } 
                                    else { 
                                        ESP_LOGI("CMD RECEIVED", "MOTOR OFF");
                                        partage->motor2SetOnOff(0); 
                                    }
                               
                               }
                                Bluetooth::sendCommand(4,2,data); 
                            }
                            break;
                        case possibleCommands_t::MOTOR_SPEED:
                            if(data.size() == 2 && partage->getGameState() == gameStates_t::WAIT){
                                if(data[0] == 1){
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 1");
                                    partage->setSpeedMotor1(data[1]);
                                }
                                    

                                else{
                                    partage->setSpeedMotor2(data[1]);
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 2");
                                }    
                                ESP_LOGI("CMD RECEIVED", "MOTOR SPEED %d", data[1]);

                                Bluetooth::sendCommand(5,2,data); 
                            }
                            break;
                        case possibleCommands_t::SET_POSITION:
                            if(data.size() == 2 && partage->getGameState() == gameStates_t::WAIT){
                                ESP_LOGI("CMD RECEIVED", "SET POSITION %d , %d",data[0],data[1]);
                                Bluetooth::sendCommand(6,2,data); 
                            }
                            break;
                        case possibleCommands_t::MOTOR_DIRECTION:
                            if(data.size() == 2 && partage->getGameState() == gameStates_t::WAIT){
                                if(data[0] == 1){
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 1");
                                    partage->directionStepMotor1(data[1]);
                                }
                                else{
                                    partage->directionStepMotor2(data[1]);
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 2");
                                }
                                    
                             
                                ESP_LOGI("CMD RECEIVED", "MOTOR DIRECTION %d",data[1]);
                                Bluetooth::sendCommand(7,2,data); 
                            }
                            break;
                        case possibleCommands_t::SET_LED:
                            if(partage->getGameState() == gameStates_t::WAIT){
                                ESP_LOGI("CMD RECEIVED", "SET LEDS");
                                
                                for(int i = 0; i < data.size(); i++){
                                    effectName.push_back(data[i]);
                                }
                                partage->setLedEffect(effectName);
                                partage->setUpdateLedEffect(true);
                                effectName.clear();
                                
                                Bluetooth::sendCommand(8,data.size(),data);
                            }
                            break;
                        case possibleCommands_t::PROMOTION:
                            
                            ESP_LOGI("CMD RECEIVED", "RECEIVED PROMOTION CHOICE");
                            if(data.size() && partage->getGameState() == gameStates_t::WAITING_PROMOTION){
                                char car = data[0];
                                ChessPieceType type = ChessPieceType::PAWN;
                                switch(car){
                                    case 'k':
                                        type = ChessPieceType::KNIGHT;
                                        break;
                                    case 'r':
                                        type = ChessPieceType::ROOK;
                                        break;
                                    case 'b':
                                        type = ChessPieceType::BISHOP;
                                        break;
                                    default:
                                        type = ChessPieceType::QUEEN;
                                }
                                partage->getPtrChessBoard()->setPieceType(partage->getPieceToPromote().y, partage->getPieceToPromote().x,type, partage->getPieceToPromote().color);
                                partage->setGameState(gameStates_t::CHOICE_COMPUTER);
                                partage->setPieceToPromote(255,255,ChessPieceColor::EMPTY);
                            }
                            break;
                        case possibleCommands_t::CALIBRATECMD:
                            ESP_LOGI("command","calibrate");
                            if(partage->getGameState() == gameStates_t::WAIT)
                                partage->setGameState(gameStates_t::CALIBRATE);
                            break;
                        case possibleCommands_t::MOTOR_STEPS:
                            ESP_LOGI("command","motor step: %d", data[1]);
                            if(data.size() == 2 && partage->getGameState() == gameStates_t::WAIT){
                                if(data[0] == 1){
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 1");
                                    partage->addStepMotor1(data[1] * 10);
                                }
                                else{
                                    partage->addStepMotor2(data[1] * 10);
                                    ESP_LOGI("CMD RECEIVED", "MOTOR 2");
                                }    
                                ESP_LOGI("CMD RECEIVED", "MOTOR STEP %d", data[1]);

                                Bluetooth::sendCommand(11,2,data); 
                            }
                            break;
                        case possibleCommands_t::SET_AIMANT:
                            ESP_LOGI("command","aimant: %d", data[0]);
                            if(data.size() == 1 && partage->getGameState() == gameStates_t::WAIT){  
                                partage->setAimant(data[0]);
                                Bluetooth::sendCommand(12,1,data); 
                            }
                            break;    
                        default:
                            break;
                    }
                    data.clear();
                }

            }
        
        }
        else if(partage->getRequestForMoves()){
            ESP_LOGI("bluetooth","send request for moves");
            partage->setRequestForMoves(0);
            std::vector<uint8_t> dataSend;
            std::string move = partage->getPieceToMoveStr();
            ESP_LOGI("bluetooth","%s", move.c_str());
            char car = 0;
            for(int i = 0; i < move.length(); i++){
                car = move[i];
                dataSend.push_back((uint8_t)car);   
            }
            //if(move.length() == 2)
                Bluetooth::sendCommand(3,dataSend.size(),dataSend);       
        }
        else if(partage->boardMadeItsMove()){
            responseSent = 1;
            ESP_LOGI("bluetooth","send move made by board");
            if(partage->getGameMode() == gameMode_t::PLAYING_GAME){
                std::vector<uint8_t> dataSend;
                std::string move = partage->getPlayerMove();
                ESP_LOGI("bluetooth","%s", move.c_str());
                char car = 0;
                for(int i = 0; i < move.length(); i++){
                    car = move[i];
                    dataSend.push_back((uint8_t)car);   
                }
                if(partage->getRequestForPromotion()){
                    partage->setRequestForPromotion(0);  
                    car = 1; 
                }
                else 
                    car = NULL;
                dataSend.push_back((uint8_t)car);   
                //if(move.length() == 2)
                    Bluetooth::sendCommand(2,dataSend.size(),dataSend); 
            }else{
                partage->setBoardMadeMove(0);
                std::vector<uint8_t> vctData;
                vctData.push_back(0);
                Bluetooth::sendCommand(2,1,vctData); //renvoie reponse que move a ete fait  
            }      
        } 
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


//static object pointer definition
BluetoothHandler* BluetoothHandler::pObjet = nullptr;