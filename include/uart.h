/**
 * @file uart.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief uart library for esp32 
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "driver/uart.h"
#include <stdio.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <functional>
#define RX_BUFFER_LENGTH 128

/**
 * @brief serial class
 * 
 */
class SerialESP {

public:
    /**
     * @brief Construct a new Serial E S P object
     * 
     * @param channel 
     * @param txPin 
     * @param rxPin 
     */
    SerialESP(int channel = 0, int txPin = -1, int rxPin = -1);
    /**
     * @brief Destroy the Serial E S P object
     * 
     */
    ~SerialESP(){};
    /**
     * @brief begin the serail communication
     * 
     * @param baudRate 
     */
    void begin(int baudRate);
    /**
     * @brief check if data is received on uart
     * 
     * @return uint8_t 
     */
    uint8_t available();
    /**
     * @brief read a byte on the uart buffer
     * 
     * @return int 
     */
    int read();
    /**
     * @brief write on the uart
     * 
     * @param buffer 
     * @param bufferLength 
     */
    void write(const char * buffer, int bufferLength);
    /**
     * @brief write on the uart
     * 
     * @param str 
     */
    void write(std::string str);
    /**
     * @brief update the status of the serial port
     * 
     */
    void updateSerial();

    //create two serial interfaces
    static uint8_t cntInstance;
    static SerialESP * pObjet1;
    static SerialESP * pObjet2;
    
    /**
     * @brief update the status of the serial port for the two uart objects
     * 
     */
    static void uartCallBack();
    
private:
    
    uint8_t _dataLength; 
    int _channel; 
    int _txPin;
    int _rxPin;  
    int rxBuffer[RX_BUFFER_LENGTH];
    uint8_t _rxIndexIn;
    uint8_t _rxIndexOut;
};


