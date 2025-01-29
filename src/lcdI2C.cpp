#include "lcdI2C.h"


lcdI2C::lcdI2C(i2c& i2cHandle, uint8_t lcdAddr, uint8_t rows, uint8_t col) : i2cInstance(i2cHandle), lcdAddr(lcdAddr), _rows(rows), _col(col){
    _i2cLCDData = 0;
    _i2cLCDBufferIndexIn = 0;
    _backlightVal = LCD_BACKLIGHT;
}

lcdI2C::~lcdI2C(){
    
}

void lcdI2C::init(){

    _displayFunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (_rows > 1) {
		_displayFunction |= LCD_2LINE;
	}
    
    vTaskDelay(pdSecond/20);

    _ioExpanderWrite(_backlightVal);
    vTaskDelay(pdSecond/1);
    this->_send4Bits(0x03 << 4);
    ets_delay_us(4500);
    this->_send4Bits(0x03 << 4);
    ets_delay_us(4500);
    this->_send4Bits(0x03 << 4);
    ets_delay_us(150);
    this->_send4Bits(0x02 << 4);
    this->_send8Bits(LCD_FUNCTIONSET | _displayFunction, 0);
    _displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    this->_send8Bits(LCD_DISPLAYCONTROL | _displayControl, 0);
    this->_send8Bits(LCD_CLEARDISPLAY,0);
    ets_delay_us(2000);
    _displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    this->_send8Bits(LCD_ENTRYMODESET | _displayMode,0);
    ets_delay_us(2000);
}

esp_err_t lcdI2C::sendChar(char car){
    return _send8Bits(car,1);
}

esp_err_t lcdI2C::sendString(const char *str){
	esp_err_t ret = ESP_OK;
	while(*str)
	{
		ret = sendChar(*str);
		str++;
	}
    
	return  ret;
}

esp_err_t lcdI2C::setBacklight(uint8_t state){
    
    return state ? _send8Bits(LCD_BACKLIGHT,0) : _send8Bits(LCD_NOBACKLIGHT,0) ;    
}

esp_err_t lcdI2C::setPos(uint8_t row, uint8_t col){
    //nt row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) {
		row = _rows-1;    // we count rows starting w/0
	}
	esp_err_t ret = _send8Bits(LCD_SETDDRAMADDR | (col + row_offsets[row]),0);
    ets_delay_us(2000);
    return ret;
}

esp_err_t lcdI2C::clearDisplay(){
    
    _send8Bits(LCD_CLEARDISPLAY,0);
    ets_delay_us(2000);
    return ESP_OK;
}

esp_err_t lcdI2C::_send8Bits(uint8_t data, uint8_t rs){
    uint8_t msb = data & 0xF0;
    uint8_t lsb = (data << 4) & 0xF0;
    esp_err_t ret = _send4Bits(msb | rs);
    if(ESP_OK == ret)   
        ret = _send4Bits(lsb | rs);
    return ret;
}

esp_err_t lcdI2C::_send4Bits(uint8_t data){
    esp_err_t ret = _ioExpanderWrite(data);
    if(ESP_OK == ret)
        ret = _sendPulseEnable(data);
    return ret;
}

esp_err_t lcdI2C::_ioExpanderWrite(uint8_t data){
    esp_err_t ret = i2cInstance.writeByte(this->lcdAddr,data | _backlightVal);
    return ret;
}

esp_err_t lcdI2C::_sendPulseEnable(uint8_t data){
    esp_err_t ret = _ioExpanderWrite(data | En);
    ets_delay_us(1);
    ret = _ioExpanderWrite(data & ~En);
    ets_delay_us(50);
    return ret;
}

void lcdI2C::_pushData(uint8_t data) //remplie buffer pour transmettre d'un coup
{
	_i2cLCDData |= (1<<5); //enable high
	_i2cLCDData = ((_i2cLCDData & 0xF0) | (data >> 4)); //set data
	_i2cLCDBuffer[_i2cLCDBufferIndexIn++] = _i2cLCDData;
	_i2cLCDData &= ~(1<<5); //enable low
	_i2cLCDBuffer[_i2cLCDBufferIndexIn++] = _i2cLCDData;
	_i2cLCDData |= (1<<5); //enable high
	_i2cLCDData = ((_i2cLCDData & 0xF0) | (data)); //set data
	_i2cLCDBuffer[_i2cLCDBufferIndexIn++] = _i2cLCDData;
	_i2cLCDData &= ~(1<<5); //enable low
	_i2cLCDBuffer[_i2cLCDBufferIndexIn++] = _i2cLCDData;
}
