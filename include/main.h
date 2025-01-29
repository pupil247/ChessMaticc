#pragma once

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_wifi.h"
#include "gpio.h"
#include "i2c.h"
#include "lcdI2C.h"
#include "bluetooth.h"
#include "controleMoteurs.h"
#include "ioMapping.h"
#include "cJSON.h"
#include "sdkconfig.h"
#include "uart.h"
#include "dac.h"
#include "neopixel.h"
#include "partage.h"
#include "screenHandler.h"
#include "Leds.h"
#include "ledBoardHandler.h"
#include "reedBoardHandler.h"
#include "bluetoothHandler.h"
#include <memory>

#define pdSecond pdMS_TO_TICKS(1000)

extern Motor* moteur1;
extern Motor * moteur2;
extern Motor* aimant;

class Main final
{

public:
    ~Main(){
        delete moteur1;
        delete moteur2;
        delete aimant;
    };
    esp_err_t setup(void);
    void loop(void);
private:

    Partage * partage;
    Gpio::GpioOutput led{(gpio_num_t)ioMapping::PIN_LED};
    DAC dac{18};
    //SerialESP uart1{};
    //SerialESP uart2{2,17,16};
    i2c i2cInstance{(uint8_t)ioMapping::PIN_SDA,(uint8_t)ioMapping::PIN_SCL,0};
    lcdI2C screen{i2cInstance, 0x27, 20, 4};
    ReedBoardHandler taskReeds{&i2cInstance};
    std::unique_ptr<Leds> leds;
    ScreenHandler taskScreen{&screen};
    std::unique_ptr<LedBoardHandler> taskLeds;
    BluetoothHandler bluetoothHandler;

};