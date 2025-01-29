#include "reedline.h"
#include "esp_log.h"

ReedLine::ReedLine(uint8_t i2cAdd) : _i2cAdd(i2cAdd)
{
    for(int i = 0; i < 8; i++)
        reedStatus.push_back(0);
    reedStatusInt = 0;
    
}

void ReedLine::setStatus(uint8_t status){
    reedStatus[0] = status & (1<<0) ?  false : true;
    reedStatus[1] = status & (1<<1) ?  false : true;
    reedStatus[2] = status & (1<<2) ?  false : true;
    reedStatus[3] = status & (1<<3) ?  false : true;
    reedStatus[4] = status & (1<<4) ?  false : true;
    reedStatus[5] = status & (1<<5) ?  false : true;
    reedStatus[6] = status & (1<<6) ?  false : true;
    reedStatus[7] = status & (1<<7) ?  false : true;
    reedStatusInt = status;
}


std::vector<bool> ReedLine::getStatus(){
    return reedStatus;
}

uint8_t ReedLine::getStatusInt(){
    return reedStatusInt;
}

uint8_t ReedLine::getAddr(){
    return _i2cAdd;
}
