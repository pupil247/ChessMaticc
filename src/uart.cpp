#include "uart.h"
SerialESP::SerialESP(int channel, int txPin, int rxPin) : _channel(channel), _txPin(txPin), _rxPin(rxPin){
    _dataLength = 0;
    _rxIndexOut = 0;
    _rxIndexIn = 0;
    if(cntInstance++ == 0)
        pObjet1 = this;
    else if(cntInstance == 1)
        pObjet2 = this;
    //sinon on ne traite pas le troisième uart
}


uint8_t SerialESP::available(){
    return _dataLength;
}

void SerialESP::begin(int baudRate){
    uart_config_t uart1_config = {
        .baud_rate = baudRate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    //uart1
    ESP_ERROR_CHECK(uart_driver_install(_channel, RX_BUFFER_LENGTH * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(_channel, &uart1_config));
    ESP_ERROR_CHECK(uart_set_pin(_channel, _txPin, _rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    xTaskCreatePinnedToCore( (TaskFunction_t)uartCallBack,    "uart_echo_task",    5000,      NULL,    1,    NULL,    1);
}

int SerialESP::read(){
    uint8_t rxData = 0;
    if(_dataLength > 0){
        _dataLength--;

        rxData = rxBuffer[_rxIndexOut++];
        if(_rxIndexOut >= RX_BUFFER_LENGTH) //gestion du débordement
			_rxIndexOut = 0;
        return rxData;
    }
    return -1;
}

void SerialESP::write(std::string str){
    uart_write_bytes(_channel, str.c_str(), str.length());
}

void SerialESP::write(const char * buffer, int bufferLength){
    uart_write_bytes(_channel, buffer, bufferLength);
}

void SerialESP::updateSerial(){
    
        uint8_t rxBuff[RX_BUFFER_LENGTH + 1];
        int rxBytes = uart_read_bytes(_channel,rxBuff,RX_BUFFER_LENGTH,20/portTICK_PERIOD_MS);
        if(rxBytes > 0){
            for(int i = 0; i < rxBytes; i++){
                rxBuffer[_rxIndexIn++] = rxBuff[i];
                _dataLength++;   
                if(_rxIndexIn >= RX_BUFFER_LENGTH)
                    _rxIndexIn = 0;
            }
        }
}

void SerialESP::uartCallBack(){
    while(1){
        if(pObjet1)
            pObjet1->updateSerial();
        if(pObjet2)
            pObjet2->updateSerial();
        //ESP_LOGI("test","hello world");
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }    
}

SerialESP* SerialESP::pObjet1 = nullptr;
SerialESP* SerialESP::pObjet2 = nullptr;
uint8_t SerialESP::cntInstance = 0;







