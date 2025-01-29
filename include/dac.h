/**
 * @file dac.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief library to use a digital analog converter on a esp32. output a 0-100 duty pwm to be then process in a low pass filter
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "stdio.h"
#include <math.h>
#include "stdlib.h"
#include "gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief DAC class
 * 
 */
class DAC {

public:
    /**
     * @brief Construct a new DAC object
     * 
     * @param pinDAC 
     */
    DAC(uint8_t pinDAC):_pinDAC(pinDAC){
        _duty = 50;
    };
    /**
     * @brief Destroy the DAC object
     * 
     */
    ~DAC(){};
    /**
     * @brief init the dac
     * 
     */
    void init(){
        ledc_channel_config_t dacConfig = {
            .gpio_num = 4,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = LEDC_CHANNEL_0,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_2,
            .duty = 2048
        };

        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_12_BIT,
            .timer_num = LEDC_TIMER_2,
            .freq_hz = 15910
        };

        
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
        ESP_ERROR_CHECK(ledc_channel_config(&dacConfig));
        //setValue(50);
    };
    /**
     * @brief Get the Current Duty Value of the DAC
     * 
     * @return uint8_t 
     */
    uint8_t getValue(){
        return _duty;
    };
    /**
     * @brief Set the current Duty Value of the DAC
     * 
     * @param value 
     */
    void setValue(uint8_t value){ 
        uint32_t max_duty = (1 << LEDC_TIMER_12_BIT) - 1;
	    uint32_t duty = lroundf(value * (float)max_duty);
        ESP_ERROR_CHECK( ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty) );
	    ESP_ERROR_CHECK( ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0) );
        _duty = value;
    };
private:
    uint8_t _duty;
    uint8_t _pinDAC;
};


