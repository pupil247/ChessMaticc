#include "gpio.h"

namespace Gpio
{
    [[nodiscard]] esp_err_t GpioBase::init(void)
    {
        esp_err_t status{ESP_OK};
        status = gpio_config(&_cfg);
        return status;
    }

    [[nodiscard]] esp_err_t GpioOutput::init(void)
    {
        esp_err_t status{GpioBase::init()};
        status = gpio_config(&_cfg);
        if(ESP_OK == status)
             status |= set(0);
        return status;
    }

    esp_err_t GpioOutput::set(const bool state){
        _state = state;
        return gpio_set_level(_pin, _state); 
    }

    esp_err_t GpioOutput::toggle(){
        _state = !_state;
        return gpio_set_level(_pin, _state); 
    }

     [[nodiscard]] esp_err_t GpioInput::init(void)
    {
        esp_err_t status{GpioBase::init()};
        status = gpio_config(&_cfg);
        if(ESP_OK == status)
            _state = gpio_get_level(_pin);
        return status;
    }

    bool GpioInput::state(void){
        _state = gpio_get_level(_pin);
        return _state; 
    }

    
}