/**
 * @file gpio.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief library to use a gpio input or output on esp32
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "driver/gpio.h"

namespace Gpio{
    /**
     * @brief Gpio class
     * 
     */
    class GpioBase{
    protected:
        const gpio_num_t _pin;
        const gpio_config_t _cfg;
    public:
        /**
         * @brief Construct a new Gpio Base object
         * 
         * @param pin 
         * @param config 
         */
        GpioBase(const gpio_num_t pin, const gpio_config_t& config) : _pin{pin}, _cfg{config}
        {
        };
        /**
         * @brief Destroy the Gpio Base object
         * 
         */
        virtual ~GpioBase(){};
        /**
         * @brief init the gpio object
         * 
         * @return esp_err_t 
         */
        [[nodiscard]] virtual esp_err_t init(void) = 0;
        /**
         * @brief return the state of the gpio input or output(to be implemented in child class)
         * 
         * @return true 
         * @return false 
         */
        virtual bool state(void) = 0;
    };
    /**
     * @brief gpio output class
     * 
     */
    class GpioOutput : public GpioBase{
    private:
        bool _state = false;
    public:
        /**
         * @brief init the gpio output
         * 
         * @return esp_err_t 
         */
        [[nodiscard]] esp_err_t init();
        /**
         * @brief set the gpio output
         * 
         * @param state 
         * @return esp_err_t 
         */
        esp_err_t set(const bool state);
        /**
         * @brief toggle the output of the gpio 
         * 
         * @return esp_err_t 
         */
        esp_err_t toggle();
        /**
         * @brief return the state of the gpio output
         * 
         * @return true 
         * @return false 
         */
        bool state(void){return _state;};
        /**
         * @brief Destroy the Gpio Output object
         * 
         */
        virtual ~GpioOutput(){};
        /**
         * @brief Construct a new Gpio Output object
         * 
         * @param pin 
         */
        GpioOutput(const gpio_num_t pin): 
            GpioBase{pin,
                gpio_config_t{
                    .pin_bit_mask = static_cast<uint64_t>(1) << pin,
                    .mode = GPIO_MODE_OUTPUT,
                    .pull_up_en = GPIO_PULLUP_DISABLE,
                    .pull_down_en = GPIO_PULLDOWN_ENABLE,
                    .intr_type = GPIO_INTR_DISABLE,
                }
            }
        {
        }

    };
    /**
     * @brief gpio input 
     * 
     */
    class GpioInput : public GpioBase{
    private:
        bool _state = false;
    public:
        /**
         * @brief Construct a new Gpio Input object with a gpiobase
         * 
         * @param pin 
         */
        GpioInput(const gpio_num_t pin): 
            GpioBase{pin,
                gpio_config_t{
                    .pin_bit_mask = static_cast<uint64_t>(1) << pin,
                    .mode = GPIO_MODE_INPUT,
                    .pull_up_en = GPIO_PULLUP_DISABLE,
                    .pull_down_en = GPIO_PULLDOWN_DISABLE,
                    .intr_type = GPIO_INTR_DISABLE, 
                }
            }
        {
        }
        /**
         * @brief Destroy the Gpio Input object
         * 
         */
        virtual ~GpioInput(){};
        /**
         * @brief init the gpio input
         * 
         * @return esp_err_t 
         */
        [[nodiscard]] esp_err_t init(void);
        /**
         * @brief return the state of the gpio input
         * 
         * @return true 
         * @return false 
         */
        bool state(void);
    };
}