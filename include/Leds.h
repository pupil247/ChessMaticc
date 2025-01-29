/**
 * @file Leds.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief This code is from https://github.com/mirronelli/neopixel and adapted to match our project needs
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "neopixel.h"
#include "effects/effect.h"
#include "effects/snake.h"
#include "effects/stars.h"
#include "effects/police.h"
#include "effects/rainbow.h"
#include "effects/effectFactory.h"
#include <string>
#include <memory>

class Leds {
public:
    Leds(uint8_t gpio,uint8_t ledCount);
    ~Leds(){delete pixels;};
    void setBoard();
    void setEffect(std::string effect);
    void setFrameDelay(uint8_t delayMS);
    void run();

private:
    Pixels * pixels;
    Effect * currentEffect;
    uint8_t gpio_num;
    uint8_t ledCount;
    uint8_t frameDelay;
};