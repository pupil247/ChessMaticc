/**
 * @file        chessBoard.cpp
 * @author      Félix Parent , Antoine Arbour
 * @brief       Fichier de définition de la classe chessBoard 
 * @version     0.1
 * @date        2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "chessBoard.hpp"

void ChessBoard::beginGame(bool playerColor){
    this->playerColor = playerColor;
    chessBoard.clear();
    std::vector<ChessPiece> emptyVector;
        for(int i = 0; i < 8; i++)
            chessBoard.push_back(emptyVector);
        chessBoard[0].push_back(ChessPiece(ChessPieceType::ROOK, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::KNIGHT, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::BISHOP, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[0].push_back(ChessPiece(playerColor ? ChessPieceType::QUEEN : ChessPieceType::KING, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[0].push_back(ChessPiece(playerColor ? ChessPieceType::KING : ChessPieceType::QUEEN, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::BISHOP, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::KNIGHT, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[0].push_back(ChessPiece(ChessPieceType::ROOK, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        for(int i = 0; i < 8; i++){
            chessBoard[1].push_back(ChessPiece(ChessPieceType::PAWN, playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        }
        for(int i = 2; i < 6; i++){
            for(int j = 0; j < 8; j++){
                chessBoard[i].push_back(ChessPiece(ChessPieceType::EMPTY, ChessPieceColor::EMPTY));
            }    
        }

        chessBoard[7].push_back(ChessPiece(ChessPieceType::ROOK, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::KNIGHT, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::BISHOP, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(playerColor ? ChessPieceType::QUEEN : ChessPieceType::KING, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(playerColor ? ChessPieceType::KING : ChessPieceType::QUEEN, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::BISHOP, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::KNIGHT, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        chessBoard[7].push_back(ChessPiece(ChessPieceType::ROOK, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        if(chessBoard[7][1].getColor() == ChessPieceColor::BLACK){
            ESP_LOGI("color at begin","black");
        }else{
            ESP_LOGI("color at begin","white");
        }
        
        for(int i = 0; i < 8; i++){
            chessBoard[6].push_back(ChessPiece(ChessPieceType::PAWN, !playerColor ? ChessPieceColor::WHITE : ChessPieceColor::BLACK));
        }

}

void ChessBoard::endGame(){

}

bool ChessBoard::getPlayerColor(){
    TLock lock(mutPlayerColor);
    return this->playerColor;
}
bool ChessBoard::getCurrentTurn(){
    TLock lock(mutCurrentTurn);
    return this->currentTurn;
}
void ChessBoard::movePlayerPiece(Move_t move){         // déplacement à sample. Réalisé par le joueur
    TLock lock(mutChessBoard);
    if(move.enPassant == true){
        chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);   
        chessBoard[move.from[1]][move.to[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.to[0]].setColor(ChessPieceColor::EMPTY);     
    }
    else if(move.gRoc == true){
        chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);
        if(playerColor){ //grand roc a gauche
            ESP_LOGI("grand roc","gauche");
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());    
            chessBoard[move.from[1]][move.to[0] - 2].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] - 2].setColor(ChessPieceColor::EMPTY);  
        }
        else { //grand roc a droite
            ESP_LOGI("grand roc","droite");
            chessBoard[move.from[1]][move.to[0] - 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] - 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(ChessPieceColor::EMPTY); 
        }

    }
    else if(move.pRoc == true){
        chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);
         
        if(playerColor){
            ESP_LOGI("petit roc","droite");
            chessBoard[move.from[1]][move.to[0] - 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] - 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(ChessPieceColor::EMPTY);     
        }
        else{
            ESP_LOGI("petit roc","gauche");
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());
            chessBoard[move.from[1]][move.to[0] - 1].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] - 1].setColor(ChessPieceColor::EMPTY);    
        }
        
    }
    else{
        chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);
    }
    if(chessBoard[7][1].getColor() == ChessPieceColor::BLACK){
            ESP_LOGI("color of roc piece","black");
    }else if(chessBoard[7][1].getColor() == ChessPieceColor::WHITE){
        ESP_LOGI("color of roc piece","white");
    }
    else{
        ESP_LOGI("color of roc piece","empty");
    }
    
}
void ChessBoard::moveOpponentPiece(){       // déplacement automatisé. Réalisé par le CPU
    TLock lock(mutChessBoard);   
}

void ChessBoard::moveOpponentPiece(Move_t move){ 
    
              
    TLock lock(mutChessBoard);
    if(move.enPassant == true){
       /* chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);   
        chessBoard[move.from[1]][move.to[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.to[0]].setColor(ChessPieceColor::EMPTY);     
    }
    else if(move.gRoc == true){
        chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);
        if(playerColor){ //grand roc a gauche
            ESP_LOGI("grand roc","gauche");
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());    
            chessBoard[move.from[1]][move.to[0] - 2].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] - 2].setColor(ChessPieceColor::EMPTY);  
        }
        else { //grand roc a droite
            ESP_LOGI("grand roc","droite");
            chessBoard[move.from[1]][move.to[0] - 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] - 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(ChessPieceColor::EMPTY); 
        }*/

    }
    else if(move.pRoc == true){
       /* chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);
         
        if(playerColor){
            ESP_LOGI("petit roc","droite");
            chessBoard[move.from[1]][move.to[0] - 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] - 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(ChessPieceColor::EMPTY);     
        }
        else{
            ESP_LOGI("petit roc","gauche");
            chessBoard[move.from[1]][move.to[0] + 1].setType(ChessPieceType::ROOK);
            chessBoard[move.from[1]][move.to[0] + 1].setColor(chessBoard[move.to[1]][move.to[0]].getColor());
            chessBoard[move.from[1]][move.to[0] - 1].setType(ChessPieceType::EMPTY);
            chessBoard[move.from[1]][move.to[0] - 1].setColor(ChessPieceColor::EMPTY);    
        }*/
        
    }
    //else{
        
        chessBoard[move.to[1]][move.to[0]].setType(chessBoard[move.from[1]][move.from[0]].getType());
        chessBoard[move.to[1]][move.to[0]].setColor(chessBoard[move.from[1]][move.from[0]].getColor());
        chessBoard[move.from[1]][move.from[0]].setType(ChessPieceType::EMPTY);
        chessBoard[move.from[1]][move.from[0]].setColor(ChessPieceColor::EMPTY);
    //}
    if(chessBoard[7][1].getColor() == ChessPieceColor::BLACK){
            ESP_LOGI("color of roc piece","black");
    }else if(chessBoard[7][1].getColor() == ChessPieceColor::WHITE){
        ESP_LOGI("color of roc piece","white");
    }
    else{
        ESP_LOGI("color of roc piece","empty");
    }
    if(move.prom != 0){
        ESP_LOGI("promotion","promotion has been made");
        switch(move.prom){
            case 'r':
                chessBoard[move.to[1]][move.to[0]].setType(ChessPieceType::ROOK);
                break;
            case 'q':
                chessBoard[move.to[1]][move.to[0]].setType(ChessPieceType::QUEEN);
                break;
            case 'b':
                chessBoard[move.to[1]][move.to[0]].setType(ChessPieceType::BISHOP);
                break;
            case 'k':
                chessBoard[move.to[1]][move.to[0]].setType(ChessPieceType::KNIGHT);
                break;
            default:
                break;
        };
    }
    
    
    
}

bool ChessBoard::isToTileEmpty(Move_t move){        // return 1 if ToTile is empty
    if(chessBoard[move.to[1]][move.to[0]].getType()==ChessPieceType::EMPTY)
        return 1;
    return 0; 
}
bool ChessBoard::isFromTileEmpty(Move_t move){        // return 1 if FromTile is empty
    if(chessBoard[move.from[1]][move.from[0]].getType()==ChessPieceType::EMPTY)
        return 1;
    return 0; 
}

bool ChessBoard::isPathToTileEmpty(Move_t move){
    if((move.from[1]!=move.to[1])  && (move.from[0] != move.to[0])){// mouvement non linéaire
       //ESP_LOGI("CHEVAL","MOUVEMENT NON LINÉRAIRE");
        if(chessBoard[move.to[1]][move.to[0]].getType() == ChessPieceType::KNIGHT){
           // dont get here 
           return 0;
           /* int8_t dirX= abs(move.from[0]- move.to[0]);
            int8_t dirY= abs(move.from[1]- move.to[1]);
            bool flagPath=1;
            if(dirY > dirX){ // mouvement de cheval en Y
                dirX= move.to[0] - move.from[0];
                dirY= move.to[1] - move.from[1];
                if(dirY>0){ // cheval vers le haut
                    ESP_LOGI("CHEVAL","VERS LE HAUT");
                    if(dirX > 0){ // a droite
                        for ( uint8_t i = move.from[1]; i < move.to[1] ; i++)  // ligne original
                        {
                            if(chessBoard[i][move.to[0] -1 ].getType()!= ChessPieceType::EMPTY)
                                flagPath=0;      
                        }
                        if(flagPath==0){ // ligne to
                            flagPath=1;
                            for ( uint8_t i = move.from[1]; i < move.to[1] ; i++)
                            {
                                if(chessBoard[i][move.to[0]].getType()!= ChessPieceType::EMPTY)
                                    flagPath=0;      
                            }
                        }
                    return flagPath;
                    }
                    else{//a gauche
                        ESP_LOGI("Cheval","En avant a gauche");
                        for ( uint8_t i = move.from[1]; i <= move.to[1]  ; i++){  // ligne original{
                            if(chessBoard[i][move.from[0]].getType()!= ChessPieceType::EMPTY)
                                flagPath=0;    
                            setChevalMove(1);      //bouge y en premier
                        }
                        if(flagPath==0){ // ligne to
                            flagPath=1;
                            for ( uint8_t i = move.from[1]; i <= move.to[1] ; i++)
                            {
                                if(chessBoard[i][move.to[0]].getType()!= ChessPieceType::EMPTY)
                                    flagPath=0; 
                                setChevalMove(0);      // bouge X en premier    
                            }
                        }
                        return flagPath;   
                    }
                }
                else{
                    if(dirX > 0){
                        for ( uint8_t i = move.from[1]; i > move.to[1] ; i--)  // ligne original
                        {
                            if(chessBoard[i][move.to[0] -1 ].getType()!= ChessPieceType::EMPTY)
                                flagPath=0;      
                        }
                        if(flagPath==0){ // ligne to
                            flagPath=1;
                            for ( uint8_t i = move.from[1]; i > move.to[1] ; i--)
                            {
                                if(chessBoard[i][move.to[0]].getType()!= ChessPieceType::EMPTY)
                                    flagPath=0;      
                            }
                        }
                        return flagPath;
                    }    
                    else{
                        for ( uint8_t i = move.from[1]; i > move.to[1] ; i--)  // ligne original
                        {
                            if(chessBoard[i][move.to[0] + 1 ].getType()!= ChessPieceType::EMPTY)
                                flagPath=0;      
                        }
                        if(flagPath==0){ // ligne to
                            flagPath=1;
                            for ( uint8_t i = move.from[1]; i > move.to[1] ; i--)
                            {
                                if(chessBoard[i][move.to[0]].getType()!= ChessPieceType::EMPTY)
                                    flagPath=0;      
                            }
                        }
                        return flagPath;
                    }    
                    
                }
                 

            }
            else{   // cheval en X
                return 0;
            }   */ 
        }
        else
            return 1;
    }
        
    
    else{
        if((move.from[1]!=move.to[1]) && (move.from[0] == move.to[0]) ){ // si le move est en Y   
            if(move.from[1]> move.to[1]){ // si le move est par en bas
            ESP_LOGI("PATH","PAR en BAS");
                for (uint8_t i = move.from[1]; i > move.to[1]; i--)
                {
                    if(chessBoard[i][move.to[0]].getType()!= ChessPieceType::EMPTY)
                        return 0;
                }
                return 1;
            }
            else{   // si le move est par en haut
                for (uint8_t i = move.from[1]; i < move.to[1]; i++){
                    if(chessBoard[i][move.to[0]].getType() != ChessPieceType::EMPTY)
                        return 0;
                }
                return 1;
            }
        }
        else if((move.from[1]==move.to[1]) && (move.from[0] != move.to[0])){ // si le move est en X
            if(move.from[0]> move.to[0] ){// si move gauche
                for (uint8_t i = move.from[0]/*-1*/; i > move.to[0] ; i--)// jai changer sa pt que sa bogue
                {
                    ESP_LOGI("PATH","BONNE ENDROIT");
                    if(chessBoard[move.to[1]][i].getType() != ChessPieceType::EMPTY){
                        ESP_LOGI("PATH","NOT EMPTY");    
                        return 0;
                    }
                        
                }
                return 1;
            }
            else{   //si move par en haut
                ESP_LOGI("PATH_FROM_TILE","MOVE_FROM_BLACK");
                for (uint8_t i = move.from[0]; i < move.to[0]; i++)
                {
                    if(chessBoard[move.to[1]][i].getType() != ChessPieceType::EMPTY)
                        return 0;
                }
                return 1;
            }
        }
        else
            return 0;

    }
    return 0;

}
    


void ChessBoard:: setPieceType(uint8_t x, uint8_t y, ChessPieceType type, ChessPieceColor color){   // Promotion perhaps
    TLock lock(mutChessBoard);
    chessBoard[y][x].setType(type);
    chessBoard[y][x].setColor(color);

}

std::vector<std::vector<ChessPiece>> ChessBoard::getChessBoard(){
    TLock lock(mutChessBoard);
    return chessBoard;   
}



void ChessBoard ::setChevalMove(bool b){
    chevalMove=b;
}

bool ChessBoard :: getChevalMove(){
    return chevalMove;
}