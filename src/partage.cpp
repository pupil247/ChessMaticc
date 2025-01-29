/**
 * @file        partage.cpp
 * @author      Félix Parent, Antoine Arbour
 * @brief       Fichier de définition de l'objet partage
 * @version     0.1
 * @date        2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "partage.h"

Partage::Partage() {
    this->chessBoard = new ChessBoard(STANDARD_BOARD_SIZE);
   // this->reedBoard = new std::vector<ReedLine>;
    this->gameState = gameStates_t::WAIT;
    this->gameMode = gameMode_t::PLAYING_GAME;
    this->updateGameState = 0;
    this->ledEffect  = "solid";
    this->updateLeds = false;
    this->requestForMoves = 0;

    pieceToMove.x = 255;
    pieceToMove.y = 255;
    pieceToCapture.x = 255;
    pieceToMove.y = 255;
    pieceToPromote.x = 255;
    pieceToPromote.y = 255;

    updateBoardMove = 0;

    this->mutexChessBoard = new Mutex();
    this->mutexGameState = new Mutex();
    this->mutexUpdateGameState = new Mutex();
    this->mutReedBoard = new Mutex();
    this->mutexI2C = new Mutex();
    this->mutexMove = new Mutex();
    this->mutexChessBoard->release();
    this->mutexGameState->release();
    this->mutexUpdateGameState->release();
    this->mutexChessBoard->release();
    this->mutexI2C->release();
    this->mutexMove->release();
    aimant = 0;
   }

Partage::~Partage(){
    delete chessBoard;
    delete mutexGameState;
    delete mutReedBoard;
    delete mutexUpdateGameState;
    delete mutexChessBoard; 
    //delete reedBoard;    
}

/*GameSTATE*/
void Partage::setUpdateGameState(bool state){
    TLock lock(mutexUpdateGameState);
    this->updateGameState = state;
}
bool Partage::getUpdateGameState(){
    TLock lock(mutexUpdateGameState);
    return this->updateGameState;    
}

void Partage::setGameState(gameStates_t gs){
    updateGameState = 1;
    TLock lock(mutexGameState);
    this->gameState = gs;
}

gameStates_t Partage::getGameState(){
    TLock lock(mutexGameState);
    return this->gameState;   
}

/*CHESS_BOARD*/
std::vector<std::vector<ChessPiece>> Partage::getChessBoard(){
    TLock lock(mutexChessBoard);
    return chessBoard->getChessBoard();
}

bool Partage::getUpdateLedEffect(){
    return updateLeds;
}
void Partage::setUpdateLedEffect(bool update){
    updateLeds = update;
}

void Partage::setLedEffect(std::string effect){
    ledEffect = effect;
}
std::string Partage::getLedEffect(){
    return ledEffect;
}
void Partage::setGameMode(gameMode_t mode){
    this->gameMode = mode;
}

gameMode_t Partage::getGameMode(){
    return this->gameMode;
}

ChessBoard* Partage:: getPtrChessBoard(){
    return chessBoard;
}

/*REED_BOARD*/
std::vector<ReedLine> Partage::getReedBoard(){
    TLock lock(mutReedBoard);
    return reedBoard;
}

void Partage::setReedBoard(std::vector<ReedLine> reed){
    TLock lock(mutReedBoard);
    reedBoard = reed;
}


/*MOVE*/
void Partage::setMove(uint8_t m[5]){
   TLock lock(mutexChessBoard);
   if(m[4])
   {
        move.prom=m[4];   
        // promotion à implémenter
   } 
   
    move.from[0]= chessBoard->getPlayerColor() == 0 ? 7 - (m[0] - 97) : m[0] - 97;
    move.to[0]= chessBoard->getPlayerColor() == 0 ? 7 - (m[2] - 97) : m[2] - 97;   
    move.from[1]= chessBoard->getPlayerColor() == 0 ? 7 - (m[1] - 49) : m[1] - 49;
    move.to[1]= chessBoard->getPlayerColor() == 0 ? 7 - (m[3] - 49) : m[3] - 49;         
    
    move.enPassant = false;
    move.gRoc = false;
    move.pRoc = false;

    if(chessBoard->getChessBoard()[move.from[1]][move.from[0]].getType() == ChessPieceType::PAWN && chessBoard->getChessBoard()[move.to[1]][move.to[0]].getType() == ChessPieceType::EMPTY && move.from[0] != move.to[0]){ //en passant
        ESP_LOGI("special move","en passant");
        move.enPassant = true;
    }
    
    if(chessBoard->getChessBoard()[move.from[1]][move.from[0]].getType() == ChessPieceType::KING && (std::abs(move.to[0] - move.from[0]) > 1)){
        if(chessBoard->getPlayerColor()){ //petit roc a droite
            if(move.from[0] < move.to[0]){
                ESP_LOGI("special move","petit roc");  
                move.pRoc = true;
            }
            else if(move.from[0] > move.to[0]){
                ESP_LOGI("special move","grand roc");
                move.gRoc = true;
            }
        }
        else { //petit roc a gauche
            if(move.from[0] < move.to[0]){
                ESP_LOGI("special move","grand roc");  
                move.gRoc = true;  
            }
            else if(move.from[0] > move.to[0]){
                ESP_LOGI("special move","petit roc");
                move.pRoc = true;
            }   
        }
        
    }
    
    //chessBoard->moveOpponentPiece(move);
}

Move_t Partage:: getMove(){
    TLock lock(mutexChessBoard);
    return move;
}

void Partage:: setEnPassant(bool b){
    move.enPassant=b;
}

 void Partage:: initShareMotor(Motor *m,Motor *m2){
    motor1=m;
    motor2=m2;
 }

 void Partage:: addStepMotor1(uint16_t step){
    motor1->setMotorStep(step); 
 }

 void Partage:: addStepMotor2(uint16_t step){
    motor2->setMotorStep(step); 
 }

 void Partage:: motor1SetOnOff(bool b){
    motor1->setMotorOnOff(b);
 }

void Partage:: motor2SetOnOff(bool b){
    motor2->setMotorOnOff(b);
 }
 void Partage:: setSpeedMotor1(uint8_t speed){
    motor1->setMotorSpeed(speed);
 }

 void Partage:: setSpeedMotor2(uint8_t speed){
    motor2->setMotorSpeed(speed);
 }

void Partage:: directionStepMotor1(bool d){
    motor1->setMotorDirection(d);
}

void Partage:: directionStepMotor2(bool d){
    motor2->setMotorDirection(d);
}

void Partage::setAimant(bool state){
    this->aimant = state;
}

bool Partage::getAimant(){
    return this->aimant;
}

void Partage::setPieceToMove(uint8_t x, uint8_t y){
    this->pieceToMove.x = x;
    this->pieceToMove.y = y;
}
position_t Partage::getPieceToMove(){
    return this->pieceToMove;
}

std::string Partage::getPieceToMoveStr(){
    std::string move;
    ESP_LOGI("movex","%d",this->pieceToMove.x);
    ESP_LOGI("movey","%d",this->pieceToMove.y);
    
    uint8_t moveX = chessBoard->getPlayerColor() == 0 ? 7 - this->pieceToMove.x : this->pieceToMove.x;
    uint8_t moveY = chessBoard->getPlayerColor() == 0 ? 7 - this->pieceToMove.y : this->pieceToMove.y;
    switch(moveY){
        case 0:
            move.append("a");
            break;
        case 1:
            move.append("b");
            break;
        case 2:
            move.append("c");
            break;
        case 3:
            move.append("d");
            break;
        case 4:
            move.append("e");
            break;
        case 5:
            move.append("f");
            break;
        case 6:
            move.append("g");
            break;
        case 7:
            move.append("h");
            break;
        default:
            break;
    }
    switch(moveX){
        case 0:
            move.append("1");
            break;
        case 1:
            move.append("2");
            break;
        case 2:
            move.append("3");
            break;
        case 3:
            move.append("4");
            break;
        case 4:
            move.append("5");
            break;
        case 5:
            move.append("6");
            break;
        case 6:
            move.append("7");
            break;
        case 7:
            move.append("8");
            break;
        default:
            break;
    }
    return move;    
}

bool Partage::getRequestForMoves(){
    //TLock lock(mutexLegals);
    return this->requestForMoves;
}

void Partage::setRequestForMoves(bool request){
    //TLock lock(mutexLegals);
    this->requestForMoves = request;
}

bool Partage::getRequestForPromotion(){
    return this->requestForPromotion;
}

void Partage::setRequestForPromotion(bool request){
    this->requestForPromotion = request;
}

void Partage::addLegalPosition(uint8_t x,uint8_t y){
    position_t position = {x,y};
    bool alreadyThere = 0;
    for(int i = 0; i < legal.size(); i++){ //look if not already there
        if(legal[i].x == x && legal[i].y == y)
            alreadyThere = 1;
    } 
    if(!alreadyThere)
        legal.push_back(position);
}
void Partage::deleteLegalPositions(){
    legal.clear();
}
bool Partage::isLegalPosition(uint8_t x, uint8_t y){
    
    for(int i = 0; i < legal.size(); i++){ //look if not already there
        if(legal[i].x == x && legal[i].y == y)
            return 1;
    } 
    return 0;
}
void Partage::setPieceToCapture(uint8_t x, uint8_t y){
    this->pieceToCapture.x = x;
    this->pieceToCapture.y = y;
}
position_t Partage::getPieceToCapture(){
    return this->pieceToCapture;
}

std::vector<position_t> Partage::getNonValid(){
    return nonValid;
}
void Partage::addNonValid(uint8_t x,uint8_t y){
    position_t position = {x,y};
    bool alreadyThere = 0;
    for(int i = 0; i < nonValid.size(); i++){ //look if not already there
        if(nonValid[i].x == x && nonValid[i].y == y)
            alreadyThere = 1;
    } 
    if(!alreadyThere)
        nonValid.push_back(position);
}
void Partage::removeNonValid(uint8_t x ,uint8_t y){
    for(int i = 0; i < nonValid.size(); i++){
        if(nonValid[i].x == x && nonValid[i].y == y)
            nonValid.erase(nonValid.begin() + i);
    }    
}
bool Partage::isNonValid(uint8_t x, uint8_t y){
    bool isNonValid = false;
    for(int i = 0; i < nonValid.size(); i++){
        if(nonValid[i].x == x && nonValid[i].y == y)
            isNonValid = true;
    }
    return isNonValid;
}

bool Partage::boardMadeItsMove(){
    TLock lock(mutexMove);
    return this->updateBoardMove;
}

void Partage::setBoardMadeMove(bool state){
    TLock lock(mutexMove);
    this->updateBoardMove = state;
}

void Partage::setPlayerMove(position_t from, position_t to){
    TLock lock(mutexMove);
    this->updateBoardMove = true;
    this->playerMove = getPositionStr(from.x,from.y) + getPositionStr(to.x,to.y);

    //set move to play
    moveToPlay.from[0] = from.y;
    moveToPlay.from[1] = from.x;
    moveToPlay.to[0] = to.y;
    moveToPlay.to[1] = to.x;

    moveToPlay.enPassant = false;
    moveToPlay.gRoc = false;
    moveToPlay.pRoc = false;

    if(chessBoard->getChessBoard()[moveToPlay.from[1]][moveToPlay.from[0]].getType() == ChessPieceType::PAWN && chessBoard->getChessBoard()[moveToPlay.to[1]][moveToPlay.to[0]].getType() == ChessPieceType::EMPTY && moveToPlay.from[0] != moveToPlay.to[0]){ //en passant
        ESP_LOGI("special move","en passant");
        moveToPlay.enPassant = true;
    }
    
    if(chessBoard->getChessBoard()[moveToPlay.from[1]][moveToPlay.from[0]].getType() == ChessPieceType::KING && (std::abs(moveToPlay.to[0] - moveToPlay.from[0]) > 1)){
        if(chessBoard->getPlayerColor()){ //petit roc a droite
            if(moveToPlay.from[0] < moveToPlay.to[0]){
                ESP_LOGI("special move","petit roc");  
                moveToPlay.pRoc = true;
            }
            else if(moveToPlay.from[0] > moveToPlay.to[0]){
                ESP_LOGI("special move","grand roc");
                moveToPlay.gRoc = true;
            }
        }
        else { //petit roc a gauche
            if(moveToPlay.from[0] < moveToPlay.to[0]){
                ESP_LOGI("special move","grand roc");  
                moveToPlay.gRoc = true;  
            }
            else if(moveToPlay.from[0] > moveToPlay.to[0]){
                ESP_LOGI("special move","petit roc");
                moveToPlay.pRoc = true;
            }   
        }
        
    }

}

std::string Partage::getPlayerMove(){
    TLock lock(mutexMove);
    this->updateBoardMove = false;
    return this->playerMove;  
}

Move_t Partage::getMoveToPlay(){
    TLock lock(mutexMove);
    return moveToPlay;  
}

std::string Partage::getPositionStr(uint8_t moveX,uint8_t moveY){
    std::string move;
    moveX = chessBoard->getPlayerColor() == 0 ? 7 - moveX : moveX;
    moveY = chessBoard->getPlayerColor() == 0 ? 7 - moveY : moveY;
    switch(moveY){
        case 0:
            move.append("a");
            break;
        case 1:
            move.append("b");
            break;
        case 2:
            move.append("c");
            break;
        case 3:
            move.append("d");
            break;
        case 4:
            move.append("e");
            break;
        case 5:
            move.append("f");
            break;
        case 6:
            move.append("g");
            break;
        case 7:
            move.append("h");
            break;
        default:
            break;
    }
    switch(moveX){
        case 0:
            move.append("1");
            break;
        case 1:
            move.append("2");
            break;
        case 2:
            move.append("3");
            break;
        case 3:
            move.append("4");
            break;
        case 4:
            move.append("5");
            break;
        case 5:
            move.append("6");
            break;
        case 6:
            move.append("7");
            break;
        case 7:
            move.append("8");
            break;
        default:
            break;
    }
    return move;    
}



void Partage::waitForI2CClear(){
    mutexI2C->take();
}

void Partage::clearI2C(){
     mutexI2C->release();    
}

void Partage::setPieceToPromote(uint8_t x, uint8_t y, ChessPieceColor color){
    this->pieceToPromote.x = x;
    this->pieceToPromote.y = y;
    this->pieceToPromote.color = color;
}

promotePiece Partage::getPieceToPromote(){
    return this->pieceToPromote;
}