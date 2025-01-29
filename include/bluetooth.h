/**
 * @file bluetooth.h
 * @author Felix Parent (parent.felix@gmail.com)
 * @brief this library is a modified example based on https://github.com/Zeni241/ESP32-NimbleBLE-For-Dummies for bluetooth nimble communication with the esp32
 * @version 0.1
 * @date 2024-05-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "stdio.h"
#include <string>
#include "sdkconfig.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "bleprph.h"
#include <vector>
#define RX_BUFFER_SIZE 128
#define ETX_CHARACTER 126


namespace Bluetooth {

    void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg);
    //static int bleprph_gap_event(struct ble_gap_event *event, void *arg);

    void stopBLE();
    
    void initBLE();
    void sendData(std::string dataToSend);
    void sendData(uint8_t * dataToSend, uint8_t dataLength);
    void sendCommand(uint8_t id, uint8_t size, std::vector<uint8_t> data);
    char readBuffer();
    bool dataAvailable();
    uint8_t calculateChecksum(uint8_t id, uint8_t size, std::vector<uint8_t> data);

}


