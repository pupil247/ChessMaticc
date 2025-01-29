#include "ledBoardHandler.h"
#include "esp_log.h"

LedBoardHandler::LedBoardHandler(void * shared){
    ledBoard = (Leds *)shared;
    pObjet = this;
}

LedBoardHandler::~LedBoardHandler(){

}

void LedBoardHandler::start(){
    pObjet->partage = Partage::getInstance();
    xTaskCreatePinnedToCore( (TaskFunction_t)task,    "Task lcd",    5000,      NULL,    2,    NULL,    1);
}

void LedBoardHandler::task(){
    Leds * ledBoard = pObjet->ledBoard;
    Partage * partage = pObjet->partage;
    while(1){
        if(partage->getUpdateLedEffect()){
            
            ledBoard->setEffect(partage->getLedEffect());
            partage->setUpdateLedEffect(false);
        }
        ledBoard->run();
        
        if(partage->getLedEffect().contains("static"))
            vTaskDelay(pdMS_TO_TICKS(10));
        else
            vTaskDelay(pdMS_TO_TICKS(100));  
    }
}

LedBoardHandler* LedBoardHandler::pObjet = nullptr;