/**
 * @file lcdI2C.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief This library controls a 16 x 4 lcd display control by i2c via a PCF8574 io expander
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#pragma once

#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <rom/ets_sys.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit
#define _I2CLCD_BUFFERSIZE 128
#define pdSecond pdMS_TO_TICKS(1000)

class lcdI2C {
public:
    lcdI2C() = delete;
    lcdI2C(const lcdI2C&) = delete;
    lcdI2C(i2c& i2cHandle, uint8_t lcdAddr, uint8_t rows, uint8_t col);
    
    ~lcdI2C();
    void init();

    //functions available
    esp_err_t sendChar(char car);
    esp_err_t sendString(const char *str);
    esp_err_t setPos(uint8_t row, uint8_t col);
    esp_err_t setBacklight(uint8_t state);
    esp_err_t clearDisplay();

private:
    i2c& i2cInstance;
    uint8_t lcdAddr;
    uint8_t _rows;
    uint8_t _col;
    uint8_t _i2cLCDData;  
    uint8_t _i2cLCDBufferIndexIn; 
    uint8_t _i2cLCDBuffer[_I2CLCD_BUFFERSIZE];
    uint8_t _backlightVal;
    uint8_t _displayFunction;
    uint8_t _displayControl;
    uint8_t _displayMode;

    //private methods
    esp_err_t _send8Bits(uint8_t data, uint8_t rs);
    esp_err_t _send4Bits(uint8_t data);
    esp_err_t _ioExpanderWrite(uint8_t data);
    esp_err_t _sendPulseEnable(uint8_t data);
    void _pushData(uint8_t data);
};