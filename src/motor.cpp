/**
 * @file      motor.cpp
 * @authors   Antoine Arbour, Félix Parent
 * @date      07/05/2024 
 * @version   0.1
 * @mainpage  Echec_Math_S3 
*/

#include "motor.h"
#include "esp_log.h"

Motor::Motor(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD){
    motorInfos.A = pinA;
    motorInfos.B = pinB;
    motorInfos.A_ = pinC;
    motorInfos.B_ = pinD;

    motorInfos.currentState = 0; //off
    motorInfos.currentDirection = 1; //forward
    motorInfos.currentSpeed = 50; //50%
    motorInfos.currentStep = 0;
    motorInfos.updateMotorInfos = 0;
    motorInfos.motorStep = 0;
    motorInfos.motorGridPos=0;
    motorInfos.motorStepGridPos=0;
    motorInfos.toggle=0;
    motorInfos.cptFinCourseID=0;
    motorInfos.cptFinCourseON=0;
    mutMoteur= new Mutex;
    this->pinA = new GpioOutput((gpio_num_t)pinA); 
    this->pinB = new GpioOutput((gpio_num_t)pinC);
    this->pinA_ = new GpioOutput((gpio_num_t)pinB);
    this->pinB_ = new GpioOutput((gpio_num_t)pinD);
    this->pinA->init();
    this->pinA_->init();
    this->pinB->init();
    this->pinB_->init();


}
Motor::~Motor(){
    delete pinA;
    delete pinB;  
    delete pinA_;  
    delete pinB_;     
    delete mutMoteur; 
}

void Motor::setMotorSpeed(uint8_t speed){
    if(speed >= 0 && speed <= 100)
        this->motorInfos.currentSpeed = speed;
    this->motorInfos.updateMotorInfos = 1;
}

void Motor::setMotorDirection(bool direction){
    this->motorInfos.currentDirection = direction;
    this->motorInfos.updateMotorInfos = 1;
}

void Motor::setMotorOnOff(bool state){
    TLock(this->mutMoteur);
    
    /*if(this-> motorInfos.cptFinCourseON)
    {   
        if(this->motorInfos.cptFinCourseID==0){
            if(this->motorInfos.currentDirection == 1)
            {
                this->motorInfos.currentState=state;
                this->motorInfos.cptFinCourseON=0;
            }
                
            else
                this->motorInfos.currentState=0;    
        }
        else{
            ESP_LOGI("setMotor","SetMotor");
            if(this->motorInfos.currentDirection == 0)
                this->motorInfos.currentState=state;
            else
                this->motorInfos.currentState=0;
        }
    }
    else{*/
        this->motorInfos.currentState = state;
        
    /*}*/
    this->motorInfos.updateMotorInfos = 1;
 
    if(!state){
        this->pinA->set(0);
        this->pinA_->set(0);
        this->pinB->set(0);
        this->pinB_->set(0);
    } 
}

void Motor::setMotorStep(uint16_t stepNumber){
    this->motorInfos.motorStep = stepNumber;
}

void Motor::setMotorUpdateStatus(bool state){
    this->motorInfos.updateMotorInfos  = state;
}

uint8_t Motor::getMotorSpeed(){
    return this->motorInfos.currentSpeed;
}

bool Motor::getMotorDirection(){
    return this->motorInfos.currentDirection;
}

bool Motor::getMotorOnOff(){
    TLock(this->mutMoteur);
    if(this->motorInfos.currentState == 0)
    {
        this->pinA->set(0);
        this->pinA_->set(0);
        this->pinB->set(0);
        this->pinB_->set(0);
    }
    return this->motorInfos.currentState;
}

bool Motor::getMotorUpdateStatus(){
    return this->motorInfos.updateMotorInfos;
}

motorInfos_t Motor::getMotorInfos(){
    return this->motorInfos;
}

void Motor::setCptFinCourse(bool onOff,uint8_t id){
    this->motorInfos.cptFinCourseON=onOff;
    this->motorInfos.cptFinCourseID=id;
}

uint16_t Motor:: getMotorGridPos(){
    return this->motorInfos.motorStepGridPos;
}

void Motor::updatePosition(void){
   TLock(this->mutMoteur);
    if(this->motorInfos.motorStep>0){
        updateGridPos();
        this->motorInfos.motorStep--;
        switch(this->motorInfos.currentStep){
            case 0:
                this->pinB_->set(0);
                this->pinA_->set(0);
                this->pinA->set(1); 
                this->pinB->set(1);
                this->motorInfos.currentDirection ? this->motorInfos.currentStep++ : this->motorInfos.currentStep = 4;  
                break;
            case 1:
                this->pinA->set(0);
                this->pinB_->set(0);
                this->pinA_->set(1); 
                this->pinB->set(1);  
                this->motorInfos.currentDirection ? this->motorInfos.currentStep++ : this->motorInfos.currentStep--; 
                break;
            case 2:
                this->pinB->set(0);
                this->pinA->set(0);
                this->pinB_->set(1); 
                this->pinA_->set(1);
                this->motorInfos.currentDirection ? this->motorInfos.currentStep++ : this->motorInfos.currentStep--; 
                break;
            default:
                this->pinA_->set(0);
                this->pinB->set(0);
                this->pinA->set(1);
                this->pinB_->set(1); 
                this->motorInfos.currentDirection ? this->motorInfos.currentStep = 0 : this->motorInfos.currentStep=2;
                break;
        }
    }
    else
    {
        this->setMotorOnOff(0);
    }
  
}

void Motor :: setAimant(bool b){
    TLock(this->mutMoteur);
    this->pinA->set(b);
}

void Motor::do360(){
    this->motorInfos.motorStep = 200; 
}

void Motor::do180(){
    this->motorInfos.motorStep = 100;    
}

void Motor::doOneSquare_X(){
    //a square worth of steps + updatePOS
     this->motorInfos.motorStep = SQUARE_X; 
}

void Motor::doOneSquare_Y(){
    //a square worth of steps + updatePOS
     this->motorInfos.motorStep = SQUARE_Y; 
}

void Motor:: doHalfSquare_X(){
    // half square steps 
    this->motorInfos.motorStep= HALF_SQUARE_X;
}

void Motor:: doHalfSquare_Y(){
    // half square steps 
    this->motorInfos.motorStep= HALF_SQUARE_Y;

}

void Motor :: updateGridPos()
{
    if(this->motorInfos.currentDirection)
        this->motorInfos.motorStepGridPos++;
    else
        this->motorInfos.motorStepGridPos--; 
}

void Motor :: setStepGridPos(uint16_t step){
    this->motorInfos.motorStepGridPos= step;
}