#include "i2c.h"

i2c::i2c(uint8_t sda, uint8_t scl, uint8_t i2csel)
{
    i2cInstance._sda = sda;
    i2cInstance._scl = scl;
    i2cInstance._i2cSel = i2csel;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = i2cInstance._sda;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = i2cInstance._scl;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    conf.clk_flags = 0;
    i2c_param_config((i2c_port_t)(i2cInstance._i2cSel), &conf);
    i2c_driver_install((i2c_port_t)(i2cInstance._i2cSel), conf.mode, 0, 0, 0);
}

esp_err_t i2c::writeByte(uint8_t addr, uint8_t byte){
    i2c_cmd_handle_t hCmd = i2c_cmd_link_create();
    i2c_master_start(hCmd);
    i2c_master_write_byte(hCmd, (addr << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_EN);
    i2c_master_write_byte(hCmd, byte, I2C_ACK_CHECK_EN);
    i2c_master_stop(hCmd);
    esp_err_t ret = i2c_master_cmd_begin((i2c_port_t)(i2cInstance._i2cSel), hCmd, 0);
    i2c_cmd_link_delete(hCmd); 
    return ret;   
}

esp_err_t i2c::writeBytes(uint8_t addr, uint8_t * data, int dataLen){
    i2c_cmd_handle_t hCmd = i2c_cmd_link_create();
    i2c_master_start(hCmd);
    i2c_master_write_byte(hCmd, (addr << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_write(hCmd, data, dataLen, I2C_ACK_CHECK_EN);
    i2c_master_stop(hCmd);
    esp_err_t ret = i2c_master_cmd_begin((i2c_port_t)(i2cInstance._i2cSel), hCmd, 0);
    i2c_cmd_link_delete(hCmd);
    return ret;
}

esp_err_t i2c::readBytes(uint8_t addr, uint8_t * data, int dataLen){
    i2c_cmd_handle_t hCmd = i2c_cmd_link_create();
    for(int i = 0; i < 8; i++){

    }
    i2c_master_start(hCmd);
    i2c_master_write_byte(hCmd, (addr << 1) | I2C_MASTER_READ, 1);
    i2c_master_read(hCmd, data, dataLen, I2C_MASTER_LAST_NACK);
    i2c_master_stop(hCmd);
    esp_err_t ret = i2c_master_cmd_begin((i2c_port_t)(i2cInstance._i2cSel), hCmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(hCmd);
    return ret;
}


