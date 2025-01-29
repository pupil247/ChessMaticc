#include "ReedBoardHandler.h"
#include "esp_log.h"

ReedBoardHandler::ReedBoardHandler(void * shared){
    reedBoard.push_back(ReedLine(ADD_REED1));
    reedBoard.push_back(ReedLine(ADD_REED2));
    reedBoard.push_back(ReedLine(ADD_REED3));
    reedBoard.push_back(ReedLine(ADD_REED4));
    reedBoard.push_back(ReedLine(ADD_REED5));
    reedBoard.push_back(ReedLine(ADD_REED6));
    reedBoard.push_back(ReedLine(ADD_REED7));
    reedBoard.push_back(ReedLine(ADD_REED8));

    reedBoardPast.push_back(ReedLine(ADD_REED1));
    reedBoardPast.push_back(ReedLine(ADD_REED2));
    reedBoardPast.push_back(ReedLine(ADD_REED3));
    reedBoardPast.push_back(ReedLine(ADD_REED4));
    reedBoardPast.push_back(ReedLine(ADD_REED5));
    reedBoardPast.push_back(ReedLine(ADD_REED6));
    reedBoardPast.push_back(ReedLine(ADD_REED7));
    reedBoardPast.push_back(ReedLine(ADD_REED8));
    pObjet = this;
    i2cHandle = (i2c*)shared;
}

ReedBoardHandler::~ReedBoardHandler(){

}

void ReedBoardHandler::start(){
    pObjet->partage = Partage::getInstance();
    xTaskCreatePinnedToCore( (TaskFunction_t)task,    "Task reed",    5000,      NULL,    4,    NULL,    1);
}

void ReedBoardHandler::task(){
    Partage * partage = pObjet->partage;
    std::vector<ReedLine> reedBoard = pObjet->reedBoard;
    std::vector<ReedLine> reedBoardPast = pObjet->reedBoardPast;
    uint8_t value = 0;
    uint8_t add[8] = {ADD_REED1,ADD_REED2,ADD_REED3,ADD_REED4,ADD_REED5,ADD_REED6,ADD_REED7,ADD_REED8};
    std::vector<uint8_t> tmp;
    uint8_t state;
    std::vector<std::vector<ChessPiece>> chessBoard = partage->getChessBoard();
    
    vTaskDelay(pdMS_TO_TICKS(5000));
    uint8_t changeDetected = 0;
    while(1){
        
        partage->waitForI2CClear();
        if(pObjet ){ 
            for(int i = 0; i < 8; i++){ 
                
                pObjet->i2cHandle->readBytes(add[i],&value,1);

                if(reedBoardPast[i].getStatusInt() == value){
                    //check if not just reading error
                    pObjet->reedBoard[i].setStatus(value);
                }
                reedBoardPast[i].setStatus(value);
                    
            } 
                
        }
        partage->clearI2C();
        chessBoard = partage->getChessBoard();
        partage->setReedBoard(pObjet->reedBoard);

        for(int i = 0 ; i < chessBoard.size() && !changeDetected; i++){ //scan the board to see if the player moved a piece
            for(int j = 0 ; j < chessBoard[i].size() && !changeDetected; j++){
                    switch(partage->getGameState()){
                        case CHOICE_PLAYER:
                            
   
                        if(isBadPosition(i,j)){ //si une pièce est bougée : this means a piece has been selected
            
                                if(((chessBoard[i][j].getColor() == ChessPieceColor::BLACK )&& (partage->getPtrChessBoard()->getPlayerColor() == 0)) ||
                                    ((chessBoard[i][j].getColor() == ChessPieceColor::WHITE) && (partage->getPtrChessBoard()->getPlayerColor() == 1))){
                                        partage->setRequestForMoves(1);
                                        partage->setGameState(gameStates_t::MOVE_PLAYER);
                                        partage->setPieceToMove(i,j);
                                        changeDetected = 1; //le board enregistre que cette piece est deplacée pour etre jouée
                                }            
                            }
    
                            break;
                        case CAPTURE_OPPONENT_PIECE:
                            if(isPieceToCapture(i,j) && !isBadPosition(i,j)){ //si une pièce est bougée : this means a piece has been selected
                                partage->setGameState(gameStates_t::CHOICE_PLAYER); 

                                
                                partage->setPlayerMove(partage->getPieceToMove(),position_t(i,j));
                                Move_t move1 = partage->getMoveToPlay();
                                partage->getPtrChessBoard()->movePlayerPiece(move1);
                                partage->setPieceToMove(255,255);
                                partage->setPieceToCapture(255,255);
                                partage->deleteLegalPositions();
                                chessBoard = partage->getChessBoard();
                                if(chessBoard[i][j].getType() == ChessPieceType::PAWN && (i == 0 || i == 7)){
                                    ESP_LOGI("event","promotion");
                                    partage->setGameState(gameStates_t::WAITING_PROMOTION);
                                    partage->setRequestForPromotion(1);
                                    partage->setPieceToPromote(i,j,chessBoard[i][j].getColor());
                                }
                                else   
                                    partage->setGameState(gameStates_t::CHOICE_COMPUTER); 
                                
                                changeDetected = 1;
                            }
                            else if(!isBadPosition(i,j) && partage->getPieceToMove().x == i && partage->getPieceToMove().y == j){
                                //return to initial state
                                partage->setPieceToMove(255,255);
                                partage->setPieceToCapture(255,255);
                                partage->setPieceToPromote(255,255,ChessPieceColor::BLACK);

                                partage->deleteLegalPositions();
                                partage->setGameState(gameStates_t::CHOICE_PLAYER);
                                changeDetected = 1;
                                
                            }
                            
                            break;
                        case MOVE_PLAYER:
   
                            if(isBadPosition(i,j) && !isPieceToMove(i,j)){
                                if(chessBoard[i][j].getType() != ChessPieceType::EMPTY && !(((chessBoard[i][j].getColor() == ChessPieceColor::BLACK )&& (partage->getPtrChessBoard()->getPlayerColor() == 0)) ||
                                    ((chessBoard[i][j].getColor() == ChessPieceColor::WHITE) && (partage->getPtrChessBoard()->getPlayerColor() == 1))) && partage->isLegalPosition(i,j)){ //case pas vide et a l'adversaire
                                        partage->setPieceToCapture(i,j); //on passe en capture
                                
                                    partage->setGameState(gameStates_t::CAPTURE_OPPONENT_PIECE);
                                }
                                else if(partage->isLegalPosition(i,j)){ // si case vide c'est pour déplacement
                                    
                                    //check if petit roc
                                    
                                    partage->setPlayerMove(partage->getPieceToMove(),position_t(i,j));
                                    Move_t move = partage->getMoveToPlay();
                                    partage->getPtrChessBoard()->movePlayerPiece(move);
                                    
                                    partage->setPieceToMove(255,255);
                                    partage->setPieceToCapture(255,255);
                                    partage->setPieceToPromote(255,255,ChessPieceColor::EMPTY);
                                    
                                    partage->deleteLegalPositions();
                                    chessBoard = partage->getChessBoard();
                                    if(chessBoard[i][j].getType() == ChessPieceType::PAWN && (i == 0 || i == 7)){
                                        partage->setRequestForPromotion(1);
                                        partage->setGameState(gameStates_t::WAITING_PROMOTION);
                                        partage->setPieceToPromote(i,j,chessBoard[i][j].getColor());
                                    }
                                    else   
                                        partage->setGameState(gameStates_t::CHOICE_COMPUTER);    
                                } //si le joueur deplace une pièce qui est une pièce à capturer(en mauve), on passe en capture de pièce
                                 //on passe à l'autre mode
                                changeDetected = 1;                           
                            }
                            else if(!isBadPosition(i,j) && partage->getPieceToMove().x == i && partage->getPieceToMove().y == j){
                                //return to initial state
                                partage->setPieceToMove(255,255);
                                partage->setPieceToCapture(255,255);
                                partage->setPieceToPromote(255,255,ChessPieceColor::BLACK);
                                partage->deleteLegalPositions();
                                partage->setGameState(gameStates_t::CHOICE_PLAYER);
                                changeDetected = 1;
                                
                            } 

                            break;  
                        default:
                            break;
                    }
                    
            }
        }
        changeDetected = 0;
        for(int i = 0 ; i < chessBoard.size(); i++){ //scan the board to see if non valid move
            for(int j = 0 ; j < chessBoard[i].size(); j++){
                if(partage->getGameState() == MOVE_PLAYER || partage->getGameState() == CAPTURE_OPPONENT_PIECE || partage->getGameState() == CHOICE_PLAYER || partage->getGameState() == gameStates_t::GAME_BEGIN || partage->getGameState() == gameStates_t::CHOICE_COMPUTER){ //check if in good game state for errors
                                if(!isPieceToMove(i,j) && !isPieceToCapture(i,j) && isBadPosition(i,j)){ //if not piece to move or already a non valid move
                                    if(!isNonValid(i,j))
                                        addNonValid(i,j);
                                }
                                else if(isNonValid(i,j))
                                    removeNonValid(i,j);

                            }
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));      
    }  
    
}

bool ReedBoardHandler::isPieceToMove(uint8_t x, uint8_t y){
    return pObjet->partage->getPieceToMove().x == x && pObjet->partage->getPieceToMove().y == y;
}

bool ReedBoardHandler::isPieceToCapture(uint8_t x, uint8_t y){
    return pObjet->partage->getPieceToCapture().x == x && pObjet->partage->getPieceToCapture().y == y;
}

void ReedBoardHandler::addNonValid(uint8_t x, uint8_t y){ 
    pObjet->partage->addNonValid(x,y);  
}

void ReedBoardHandler::removeNonValid(uint8_t x, uint8_t y){ 
    pObjet->partage->removeNonValid(x,y);    
}

bool ReedBoardHandler::isNonValid(uint8_t x, uint8_t y){
    return pObjet->partage->isNonValid(x,y);
}

bool ReedBoardHandler::isBadPosition(uint8_t x, uint8_t y){
    std::vector<std::vector<ChessPiece>> chessBoardTmp = pObjet->partage->getChessBoard();
    return (chessBoardTmp[x][y].getType() != ChessPieceType::EMPTY && pObjet->reedBoard[x].getStatus()[y] == false) || (pObjet->reedBoard[x].getStatus()[y] == true &&
        chessBoardTmp[x][y].getType() == ChessPieceType::EMPTY);
        

}

ReedBoardHandler* ReedBoardHandler::pObjet = nullptr;