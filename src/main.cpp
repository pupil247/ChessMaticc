#include "main.h"
#define LOG_TAG "MAIN"
#include "esp_wifi.h"
#include <esp_mac.h>

static Main my_main;
char data = 0;
uint8_t value = 0;

extern "C" void app_main(void) {

    ESP_ERROR_CHECK(my_main.setup());

    while(true)
    {
        my_main.loop();
    }
}

esp_err_t Main::setup(){
    
    esp_err_t status(ESP_OK);
    vTaskDelay(pdMS_TO_TICKS(500));
    leds = std::make_unique<Leds>(ioMapping::PIN_LED_CONTROL,64);
    taskLeds = std::make_unique<LedBoardHandler>(leds.get());
    vTaskDelay(pdMS_TO_TICKS(500));
    char macAdd[6];
    esp_base_mac_addr_get((uint8_t*)macAdd);
    esp_read_mac((uint8_t*)macAdd, ESP_MAC_BT);
    ESP_LOGI("ESP MAC ADDRESS", "%02X:%02X:%02X:%02X:%02X:%02X",
         macAdd[0], macAdd[1], macAdd[2], macAdd[3], macAdd[4], macAdd[5]);
    partage = Partage::getInstance();
    vTaskDelay(pdMS_TO_TICKS(500));
    screen.init();
    vTaskDelay(pdMS_TO_TICKS(500));
    taskScreen.start();
   // dac.init();
   // initMotors(partage);
    //dac.setValue(50);
   
    //screen.clearDisplay();
    led.init();
    //uart1.begin(115200);
    //uart2.begin(115200);
    
 
    vTaskDelay(pdMS_TO_TICKS(500));
   leds->setEffect("rainbow");
   vTaskDelay(pdMS_TO_TICKS(500));
   bluetoothHandler.start();
   partage->setUpdateGameState(1);
   taskLeds->start();
   taskReeds.start();
   vTaskDelay(pdMS_TO_TICKS(500));
   initMotors(partage);
    //Bluetooth
    led.set(true);  
    return status;
}

void Main::loop(){
   //led.set(true);
    
    ESP_LOGI("test","test");
   /* while(uart1.available()){
        data = uart1.read();
        uart1.write(&data, 1);
    }
    while(uart2.available()){
        data = uart2.read();
        uart2.write(&data, 1);
    }*/
   vTaskDelay(pdSecond);
    //uart1.write("hello world");
    //uart2.write("hello world");
    //led.set(false);
    vTaskDelay(pdSecond); //1s
}



