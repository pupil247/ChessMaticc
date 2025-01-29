/**
 * @file mutex.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief mutex handler for real time operations on esp32
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "stdio.h"
#include "esp_log.h"
class Mutex{

private:
    SemaphoreHandle_t xMutex;
public:
    Mutex(void){
        xMutex = xSemaphoreCreateMutex();
    };
    ~Mutex(){};
    void take(void){
        xSemaphoreTake(xMutex,portMAX_DELAY);
    };
    void release(void){
        xSemaphoreGive(xMutex);
    };

};