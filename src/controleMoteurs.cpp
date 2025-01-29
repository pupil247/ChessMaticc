/**
 * @file      controleMoteurs.cpp
 * @authors   Antoine Arbour, Félix Parent
 * @brief     Tâche et méthode de contrôle moteur
 * @date      07/05/2024 
 * @version   0.1
 * @mainpage  Echec_Math_S3 
*/

#include "controleMoteurs.h"
#include "esp_err.h"

volatile bool flagInt0, flagInt1;
volatile bool flagOutOfBounds_X, flagOutOfBounds_Y; 
Motor *moteur1;
Motor *moteur2;
Motor *aimant;
motorStates_t stateMotor;
moveStates_t stateMove;
Partage *partage;

std::vector<std::vector<ChessPiece>>   chessBoard;

struct moveInStep_t {
  uint16_t to[2];
  uint16_t from[2];
};
moveInStep_t moveInStep;

bool IRAM_ATTR Timer0_ISR(void* arg){
  flagInt0 = 1;        
  return 0;
}

bool IRAM_ATTR Timer1_ISR(void* arg){
  flagInt1 = 1;      
  return 0;  
}


void IRAM_ATTR cptFinCourse_X_0_ISR(void* arg){  
  /* ISR pour capteur de fin de course En X */
  flagOutOfBounds_X=1;
  moteur1->setMotorStep(20);
  moteur1->setMotorDirection(1); 
  gpio_intr_disable(GPIO_NUM_2);
  ets_printf("int_X_ 0");
}


void IRAM_ATTR cptFinCourse_X_1_ISR(void* arg){
  /* ISR pour capteur de fin de course En X */
  flagOutOfBounds_X=1;
  moteur1->setMotorStep(20);
  moteur1->setMotorDirection(0);  
  gpio_intr_disable(GPIO_NUM_9);
  ets_printf("int_X_1");
}

void IRAM_ATTR cptFinCourse_Y_0_ISR(void* arg){
  /* ISR pour capteur de fin de course En Y */
  flagOutOfBounds_Y=1;
  moteur2->setMotorStep(20);
  moteur2->setMotorDirection(1);
  gpio_intr_disable(GPIO_NUM_12);
  ets_printf("int_Y_0");
}

void IRAM_ATTR cptFinCourse_Y_1_ISR(void* arg){ 
  /* ISR pour capteur de fin de course En Y */
  flagOutOfBounds_Y=1;
  moteur2->setMotorStep(20);
  moteur2->setMotorDirection(0);
  gpio_intr_disable(GPIO_NUM_11);
  ets_printf("int_Y_1");
}

void motorHandler(void * parameter) {
  
  while(1){
    switch(partage->getGameState())
    {
      case GAME_BEGIN:
        aimant->setAimant(0);
        break;
      case GAME_PAUSE:
        aimant->setAimant(0);
        break;
      case GAME_END:
        aimant->setAimant(0);
        break;
      case CHOICE_COMPUTER:
        aimant->setAimant(0);
        break;    
      case CHOICE_PLAYER:
        aimant->setAimant(0);
        break;  
      case CAPTURE_OPPONENT_PIECE:
        aimant->setAimant(0);
        break;
      case CAPTURE_PLAYER_PIECE:
        aimant->setAimant(0);
        break;
      case WAITING_PROMOTION:
        aimant->setAimant(0);
        break;
      case WAIT:
        moveMotors();
        aimant->setAimant(partage->getAimant());
        break;
      case MOVE_PLAYER:
        aimant->setAimant(0);
        break;          
      case CALIBRATE:
        calibrateMotors();
        partage->setGameState(gameStates_t::WAIT);
        break;  
      case MOVEMENT_COMPUTER:
        ESP_LOGI("MOVEMENTCOMPUTER","MOVEMENT_COMPUTER");
        Move_t move= partage->getMove();
        setMoveCoord(move);
        switch (stateMotor)
        {
          default:
          stateMotor=CAPTURE;
          break;
          case CAPTURE:
            if((partage->getPtrChessBoard()->isToTileEmpty(move) )){
              if(move.enPassant)
                stateMotor=CAPTURE_PATHING;
              else
                stateMotor=FROM_POSITION;  
            }
            else{
              stateMotor=CAPTURE_PATHING;
              ESP_LOGI("MOTOR_STATE","GOING_TO_CAPTURE_PATHING");
            }
            break;
          case CAPTURE_PATHING:
              if((partage->getPtrChessBoard()->isToTileEmpty(move) && !move.enPassant )){
                moteur2->doHalfSquare_Y();
                if(moveInStep.to[1] >= Y_7)
                  moteur2->setMotorDirection(0);
                else
                  moteur2->setMotorDirection(1);

                moteur2->setMotorOnOff(1);   

                aimant->setAimant(1); 
                do
                {
                  moveMotors();
                  vTaskDelay(1);
                } while (moteur2->getMotorInfos().motorStep !=0);
                moteur1->setMotorDirection(0);              
                moteur1->setMotorStep(abs( 0 - moteur1->getMotorGridPos()));
                moteur1->setMotorSpeed(60);
                moteur2->setMotorSpeed(60);
                moteur1->setMotorOnOff(1);
                do{
                  moveMotors();
                  vTaskDelay(1);
                }while ((moteur1->getMotorInfos().motorStep !=0));
                aimant->setAimant(0);
                stateMotor= FROM_POSITION;     
              }
              else{
                calibrateMotors();
                if(move.enPassant){ // si en passant 
                  uint16_t moveInStepEnPassant[2];
                  moveInStepEnPassant[0]= move.to[0]*SQUARE_X;
                  if(move.to[1]>move.from[1]){
                    moveInStepEnPassant[1]=(move.to[1]-1)*SQUARE_Y;
                    partage->getPtrChessBoard()->setPieceType(move.to[0],move.to[1]-1,ChessPieceType::EMPTY,ChessPieceColor::EMPTY);
                  }
                  else{
                    moveInStepEnPassant[1]=(move.to[1]+1)*SQUARE_Y;
                    partage->getPtrChessBoard()->setPieceType(move.to[0],move.to[1]+1,ChessPieceType::EMPTY,ChessPieceColor::EMPTY);
                  } 
                  goToCMD(moveInStepEnPassant);
                  partage->setEnPassant(0);   
                  move=partage->getMove();
                }
                else{
                  goToCMD(moveInStep.to);
                  partage->getPtrChessBoard()->setPieceType(move.to[0],move.to[1],ChessPieceType::EMPTY,ChessPieceColor::EMPTY);
                }
              }
            break;
          case FROM_POSITION:
                calibrateMotors();
                goToCMD(moveInStep.from);
                partage->getPtrChessBoard()->moveOpponentPiece(move);
                ESP_LOGI("MOTOR_STATE","GOING_TO_MOVE_PATHING");
                stateMotor=MOVE_PATHING;
              ESP_LOGI("MOTOR_STATE","FROM_POSITION");
            break;      
          case MOVE_PATHING:
                if(partage->getPtrChessBoard()->isPathToTileEmpty(move)){ // si le chemin est libre
                  ESP_LOGI("PATH","FREE");
                  if(moveInStep.to[0]> moteur1->getMotorGridPos())
                    moteur1->setMotorDirection(1);
                  else  
                    moteur1->setMotorDirection(0);
                  moteur1->setMotorSpeed(60);  
                  moteur1->setMotorStep(abs((moveInStep.to[0] /*- HALF_SQUARE_X*/) - moteur1->getMotorGridPos()));
                  if(moveInStep.to[1]> moteur2->getMotorGridPos())
                    moteur2->setMotorDirection(1);
                  else  
                    moteur2->setMotorDirection(0);
                  moteur2->setMotorSpeed(60);
                  moteur2->setMotorStep(abs((moveInStep.to[1] ) - moteur2->getMotorGridPos()));
                  ESP_LOGI("PATH","PAS UN CHEVAL");
                  moteur1->setMotorSpeed(50);
                  moteur1->setMotorOnOff(1);
                  aimant->setAimant(1);
                  moteur2->setMotorOnOff(1);
                  do
                  {
                    moveMotors();
                    vTaskDelay(1);
                  } while  ( (moteur1->getMotorInfos().motorStep !=0) || (moteur2->getMotorInfos().motorStep !=0));
                  aimant->setAimant(0);
                  stateMotor=MOVE_END;
                  ESP_LOGI("MOVE_STATE","MOVE_END");
                  break;   
                }
                else{ // si le chemin est pas libre on déplace sur les lignes 
                  //case GET_ON_LINE:
                  moteur2->doHalfSquare_Y();
                  if(moveInStep.from[1]>=Y_7)
                    moteur2->setMotorDirection(0);
                  else
                    moteur2->setMotorDirection(1);  
                  moteur1->doHalfSquare_X();
                  if(moveInStep.from[0] >=X_7 || moveInStep.to[0]==X_0_CENTER_POS ){
                    if(moveInStep.from[0 >= X_7])
                      moteur1->setMotorDirection(0);
                    else
                      moteur1->setMotorDirection(1);  
                  }
                  else
                    moteur1->setMotorDirection(1);
                  moteur1->setMotorOnOff(1);
                  aimant->setAimant(1);
                  moteur1->setMotorSpeed(40);
                  do
                  {
                    moveMotors();
                    vTaskDelay(1);
                  } while ((moteur1->getMotorInfos().motorStep !=0) );
                  moteur2->setMotorOnOff(1);
                  do
                  {
                    moveMotors();
                    vTaskDelay(1);
                  } while ((moteur2->getMotorInfos().motorStep !=0) );
                  aimant->setAimant(0);
                  //stateMove=MOVE_X;
                  //break;
                //case MOVE_X:
                  if(moveInStep.to[0]> moteur1->getMotorGridPos())
                    moteur1->setMotorDirection(1);
                  else  
                    moteur1->setMotorDirection(0);
                  if(moveInStep.to[0]==X_0_CENTER_POS) 
                  {
                    moteur1->setMotorStep(abs((moveInStep.to[0] + HALF_SQUARE_X)  ) - moteur1->getMotorGridPos());
                  } 
                  else
                    moteur1->setMotorStep(abs((moveInStep.to[0] - HALF_SQUARE_X) - moteur1->getMotorGridPos()));     
                  moteur1->setMotorSpeed(40);
                  moteur1->setMotorOnOff(1);
                  aimant->setAimant(1);
                  moteur1->setMotorOnOff(1);
                  do
                  {
                    moveMotors();
                    vTaskDelay(1);
                  } while ((moteur1->getMotorInfos().motorStep !=0));
                  ESP_LOGI("MOVE_STATE","GOING_TO_MOVE_Y");
                  aimant->setAimant(0);
                  //stateMove=MOVE_Y;
                //break;
                //case MOVE_Y:
                  if(moveInStep.to[1]> moteur2->getMotorGridPos())
                    moteur2->setMotorDirection(1);
                  else  
                    moteur2->setMotorDirection(0);
                  moteur2->setMotorSpeed(60);
                  moteur2->setMotorOnOff(1);  
                  moteur2->setMotorStep(abs((moveInStep.to[1] ) - moteur2->getMotorGridPos()));
                  ESP_LOGI("MOVE_STATE","GET_ON_TILE");
                  aimant->setAimant(1);
                  moteur2->setMotorOnOff(1);
                  do
                  {
                    moveMotors();
                    vTaskDelay(1);
                  } while ( (moteur2->getMotorInfos().motorStep !=0));
                  stateMove=GET_ON_TILE;
                  aimant->setAimant(0);
                //break;
                //case GET_ON_TILE:
                  moteur1->doHalfSquare_X();
                  if(moveInStep.to[0] ==X_0_CENTER_POS)
                      moteur1->setMotorDirection(0);
                  else
                    moteur1->setMotorDirection(1);
                  moteur1->doHalfSquare_X();
                  moteur1->setMotorSpeed(60);
                  moteur1->setMotorOnOff(1);
                  moteur2->setMotorSpeed(60);
                  moteur1->setMotorOnOff(1);
                  aimant->setAimant(1);
                  do
                  {
                    moveMotors();
                    vTaskDelay(1);
                  } while ((moteur1->getMotorInfos().motorStep !=0));
                  
                  aimant->setAimant(0);
                  //stateMove=GET_ON_LINE;
                  stateMotor=MOVE_END;
                  break;
                }         
        case MOVE_ON:
          break;
        case MOVE_END: 
          if(move.gRoc || move.pRoc){ // implémentation du roc , si c'est le cas on set le move de la tour et on recommence la séquence de positionnement
            stateMotor=FROM_POSITION;
            ESP_LOGI("move","%d,%d",move.to[1],move.to[0]);
            //ESP_LOGI("color","%d",partage->getChessBoard[move.to[1]][move.to[0]].getColor());
            if(partage->getChessBoard()[move.to[1]][move.to[0]].getColor()==ChessPieceColor::WHITE){
              if(move.pRoc){
                ESP_LOGI("petit roc","white");
                uint8_t tour[5]= {'h','1','f','1',0};
                partage->setMove(tour);
              }
              else if(move.gRoc){
                ESP_LOGI("grand roc","white");
                uint8_t tour[5]= {'a','1','d','1',0};
                partage->setMove(tour);
              }
            }
            else{
              if(move.pRoc){
                ESP_LOGI("petit roc","black");
                uint8_t tour[5]= {'h','8','f','8',0};
                partage->setMove(tour);
              }
              else if(move.gRoc){
                ESP_LOGI("grand roc","black");
                uint8_t tour[5]= {'a','8','d','8',0};
                partage->setMove(tour);
              }
            }
            break;
          }
          else{
            findReed(move);
            calibrateMotors();
            ESP_LOGI("MOTOR_STATE","MOVE_END");
            if(partage->getGameMode()== gameMode_t ::  PLAYING_GAME){
              partage->setGameState(CHOICE_PLAYER);
              stateMotor=CAPTURE;
            }
            else{
              std::vector<uint8_t> vctData;
              partage->setBoardMadeMove(1); 
              partage->setGameState(CHOICE_COMPUTER);
              stateMotor=CAPTURE;
            }
            break;
          } 
      }    
      break;
    } 
    
    vTaskDelay(1); // 1ms
  }
}

void initMotors(Partage *shared){
  partage=(Partage*)shared;
  chessBoard=partage->getChessBoard();
  flagInt1 = 0;
  flagInt0 = 0;
  moteur1 = new Motor(GPIO1_1,GPIO2_1,GPIO3_1,GPIO4_1);
  moteur2 = new Motor(GPIO1_2,GPIO2_2,GPIO3_2,GPIO4_2);
  aimant = new Motor(GPIO_AIMANT,NULL,NULL,NULL);
  partage->initShareMotor(moteur1,moteur2);
  moteur1->setMotorOnOff(0);
  moteur1->setMotorDirection(0);
  moteur1->setMotorSpeed(0);
  moteur1->setMotorOnOff(0);
  moteur2->setMotorDirection(0);
  moteur2->setMotorOnOff(0);
  esp_err_t intrErr;
  /*Capteur fin de course*/
  intrErr=  gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
  //ESP_LOGI(esp_err_to_name(intrErr),"logInt");
  intrErr= gpio_set_intr_type(GPIO_NUM_9,GPIO_INTR_LOW_LEVEL);
  intrErr=  gpio_isr_handler_add(GPIO_NUM_9,&cptFinCourse_X_1_ISR,NULL);
  intrErr= gpio_intr_enable(GPIO_NUM_9);
  intrErr= gpio_set_intr_type(GPIO_NUM_2,GPIO_INTR_LOW_LEVEL);
  intrErr=  gpio_isr_handler_add(GPIO_NUM_2,&cptFinCourse_X_0_ISR,NULL);
  intrErr= gpio_intr_enable(GPIO_NUM_2);
  intrErr= gpio_set_intr_type(GPIO_NUM_12,GPIO_INTR_LOW_LEVEL);
  intrErr=  gpio_isr_handler_add(GPIO_NUM_12,&cptFinCourse_Y_0_ISR,NULL);
  intrErr= gpio_intr_enable(GPIO_NUM_12);
  intrErr= gpio_set_intr_type(GPIO_NUM_11,GPIO_INTR_LOW_LEVEL);
  intrErr=  gpio_isr_handler_add(GPIO_NUM_11,&cptFinCourse_Y_1_ISR,NULL);
  intrErr= gpio_intr_enable(GPIO_NUM_11);
  /*End Capteur Fin de course*/

 /*Timer 0,1 Config*/
  const timer_config_t timer0_config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = 80  // 80 MHz timer base clock
  };
  
  const timer_config_t timer1_config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = 80  // 80 MHz timer base clock
  };
  esp_err_t timerLog;
  timer_init(TIMER_GROUP_0, TIMER_0, &timer0_config);
  timer_init(TIMER_GROUP_0, TIMER_1, &timer1_config);

  timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 10000);  
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_1, 10000);  
  timerLog=timer_isr_callback_add(TIMER_GROUP_0,TIMER_0,&Timer0_ISR,(void*)0,ESP_INTR_FLAG_IRAM);
  timer_isr_callback_add(TIMER_GROUP_0,TIMER_1,&Timer1_ISR,(void*)0,ESP_INTR_FLAG_IRAM); 
  timerLog = timer_start(TIMER_GROUP_0, TIMER_0);
  //ESP_LOGI(esp_err_to_name(timerLog),"allo");
  timer_start(TIMER_GROUP_0, TIMER_1);
 /*timer config end */ 

  xTaskCreatePinnedToCore( motorHandler,    "TaskMotors",    5000,      NULL,   50,    NULL,    1);

}

void updateMoteurs(){
  uint8_t s1,s2;
  moteur1->getMotorSpeed()==100 ? s1=99 : s1= moteur1->getMotorSpeed();
  moteur2->getMotorSpeed()==100 ? s2=99 : s2= moteur2->getMotorSpeed();
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 200 * (100 - s1)); 
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_1, 200 * (100 - s2));  

  moteur1->getMotorOnOff() ?  timer_start(TIMER_GROUP_0, TIMER_0) : timer_pause(TIMER_GROUP_0, TIMER_0);
  moteur2->getMotorOnOff() ?  timer_start(TIMER_GROUP_0, TIMER_1) : timer_pause(TIMER_GROUP_0, TIMER_1);
  moteur1->setMotorUpdateStatus(0);
  moteur2->setMotorUpdateStatus(0);
}

void moveMotors()
{ 
  if(moteur1->getMotorOnOff()){
    
    if(flagInt0){
      flagInt0 = 0;
     // ESP_LOGI("FLAGINT0","FLAGINT0");
      
      if(gpio_get_level(GPIO_NUM_9))
        gpio_intr_enable(GPIO_NUM_9); 
      if(gpio_get_level(GPIO_NUM_2                                                                                                                                                                                                                                                                                                         ))
        gpio_intr_enable(GPIO_NUM_2);
      moteur1->updatePosition();
    }

  }

  if(moteur2->getMotorOnOff()){
    if(flagInt1){
      flagInt1 = 0;
     // ESP_LOGI("MOTORON","MOTORON");
     // ESP_LOGI("FLAGINT0","FLAGINT0");
      moteur2->updatePosition();
      if(gpio_get_level(GPIO_NUM_12))
        gpio_intr_enable(GPIO_NUM_12);
      if(gpio_get_level(GPIO_NUM_11))  
       gpio_intr_enable(GPIO_NUM_11);
    } 
  }

  if(moteur1->getMotorUpdateStatus()||moteur2->getMotorUpdateStatus())
  {
    if(moteur1->getMotorInfos().cptFinCourseON==0)
      //flagOutOfBounds_X=0;
    if(moteur2->getMotorInfos().cptFinCourseON==0)
      //flagOutOfBounds_Y=0;  
    updateMoteurs();
  }
}  

bool calibrateMotors(void){
  moteur1->setMotorOnOff(1);
  moteur2->setMotorOnOff(1);
  moteur1->setMotorDirection(0); 
  moteur2->setMotorDirection(0); 
  moteur1->setMotorStep(32000);
  moteur2->setMotorStep(32000);
  moteur1->setMotorSpeed(80);
  moteur2->setMotorSpeed(90);

  flagOutOfBounds_X=0;
  flagOutOfBounds_Y=0;
  
  do{
    moveMotors();
    vTaskDelay(1);    
  }while ((flagOutOfBounds_X==0) || (flagOutOfBounds_Y == 0));
 
  flagOutOfBounds_X=0;
  flagOutOfBounds_Y=0;
  moteur1->setMotorStep(40);
  moteur2->setMotorStep(40);
  moteur1->setMotorDirection(1);
  moteur2->setMotorDirection(1);
  moteur1->setMotorOnOff(1);
  moteur2->setMotorOnOff(1);
  
  do{
    moveMotors();
    vTaskDelay(1);
  }while((moteur1->getMotorInfos().motorStep != 0)||(moteur2->getMotorInfos().motorStep !=0));
  moteur1->setMotorOnOff(0);
  moteur2->setMotorOnOff(0);
  moteur1->setStepGridPos(0);
  moteur2->setStepGridPos(0);
  return 1;
}


void setMoveCoord(Move_t move){
  switch (move.to[0])
  {
  case 0:
    moveInStep.to[0]=  X_0_CENTER_POS;
    break;
  case 1:
    moveInStep.to[0]= X_1;
    break;
  case 2:
    moveInStep.to[0]= X_2;
    break;
  case 3:
    moveInStep.to[0]= X_3;
    break;    
  case 4:
    moveInStep.to[0]= X_4;
    break;
  case 5:
    moveInStep.to[0]= X_5;
    break;
  case 6:
    moveInStep.to[0]= X_6;
    break;  
  case 7:
    moveInStep.to[0]= X_7;
    break;      
  default:
    moveInStep.to[0]=0;
    break;
  }
  switch (move.to[1])
  {
   case 0:
    moveInStep.to[1]= Y_0_CENTER_POS;
    break;
  case 1:
    moveInStep.to[1]= Y_1;
    break;
  case 2:
    moveInStep.to[1]= Y_2;
    break;
  case 3:
    moveInStep.to[1]= Y_3;
    break;    
  case 4:
    moveInStep.to[1]= Y_4;
    break;
  case 5:
    moveInStep.to[1]= Y_5;
    break;
  case 6:
    moveInStep.to[1]= Y_6;
    break;  
  case 7:
    moveInStep.to[1]= Y_7;
    break;      
  default:
    moveInStep.to[1]=0;
    break;
  }
  switch (move.from[0])
  {
 case 0:
    moveInStep.from[0]= X_0_CENTER_POS;
    break;
  case 1:
    moveInStep.from[0]= X_1;
    break;
  case 2:
    moveInStep.from[0]= X_2;
    break;
  case 3:
    moveInStep.from[0]= X_3;
    break;    
  case 4:
    moveInStep.from[0]= X_4;
    break;
  case 5:
    moveInStep.from[0]= X_5;
    break;
  case 6:
    moveInStep.from[0]= X_6;
    break;  
  case 7:
    moveInStep.from[0]= X_7;
    break;      
  default:
    moveInStep.from[0]=0;
    break;
  }
  switch (move.from[1])
  {
   case 0:
    moveInStep.from[1]= Y_0_CENTER_POS;
    break;
  case 1:
    moveInStep.from[1]= Y_1;
    break;
  case 2:
    moveInStep.from[1]= Y_2;
    break;
  case 3:
    moveInStep.from[1]= Y_3;
    break;    
  case 4:
    moveInStep.from[1]= Y_4;
    break;
  case 5:
    moveInStep.from[1]= Y_5;
    break;
  case 6:
    moveInStep.from[1]= Y_6;
    break;  
  case 7:
    moveInStep.from[1]= Y_7;
    break;      
  default:
    moveInStep.from[1]=0;
    break;
  }
}

void goToCMD(uint16_t cmd[2]){
  if(cmd[0]> moteur1->getMotorGridPos())
    moteur1->setMotorDirection(1);
  else  
    moteur1->setMotorDirection(0);

  if(cmd[1]> moteur2->getMotorGridPos())
    moteur2->setMotorDirection(1);
  else  
    moteur2->setMotorDirection(0);
     
  moteur1->setMotorStep(abs(cmd[0] - moteur1->getMotorGridPos()));
  moteur2->setMotorStep(abs(cmd[1] - moteur2->getMotorGridPos()));
  moteur1->setMotorSpeed(90);
  moteur2->setMotorSpeed(90);
  moteur1->setMotorOnOff(1);
  moteur2->setMotorOnOff(1);
  do
  {
    moveMotors();
    vTaskDelay(1);
  } while ((moteur1->getMotorInfos().motorStepGridPos != cmd[0]) || (moteur2->getMotorInfos().motorStepGridPos != cmd[1])  );
  aimant->setAimant(0);
  moteur1->setMotorOnOff(0);
  moteur2->setMotorOnOff(0);
  ESP_LOGI("GOTO_CMD","OVER");   
}

bool findReed( Move_t move){  // fait un carré pour trouver le capteur 
  uint16_t step=5;
  uint8_t nbTry=0;
  uint8_t squareSeq=0;
  do 
  {
    vTaskDelay(300);    
    if ( partage->getReedBoard()[move.to[1]].getStatus()[move.to[0]] ){
      ESP_LOGI("move","perfect pitch");
      // si reed allumé
      return 1;
    }
    ESP_LOGI("move","bad movement");
    switch (squareSeq)
    {
      case 0:
        step+=5;
        moteur2->setMotorDirection(0);
        moteur2->setMotorStep(step);
        moteur2->setMotorOnOff(1);
        squareSeq++;
        break;
      case 1:
        moteur1->setMotorDirection(1);
        moteur1->setMotorStep(step);
        moteur1->setMotorOnOff(1);
        squareSeq++;
        break;
      case 2:
        moteur2->setMotorDirection(1);
        moteur2->setMotorStep(step);
        moteur2->setMotorOnOff(1);
        squareSeq++;
        break;
      case 3:
        moteur1->setMotorDirection(0);
        moteur1->setMotorStep(step);
        moteur1->setMotorOnOff(1);
        squareSeq++;
        break;
              
      default:
        moteur1->setMotorDirection(0);
        moteur2->setMotorDirection(1);
        moteur1->setMotorStep(3);
        moteur2->setMotorStep(3);
        moteur1->setMotorOnOff(1);
        moteur2->setMotorOnOff(1);
        squareSeq=0;
        break;
    }
    aimant->setAimant(1);
    do
    {
      ESP_LOGI("move","adjust");
      moveMotors();
    } while ( (moteur1->getMotorInfos().motorStep != 0) || (moteur2->getMotorInfos().motorStep !=0) );
    aimant->setAimant(0);
  
  }while (nbTry++ < 8);
  
  return 0;
}