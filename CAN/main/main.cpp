#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "mcp2515.h"


static const char *TAG = "CAN_A";


extern "C" void app_main()
{
    ESP_LOGI(TAG, "================================");
    ESP_LOGI(TAG, " ESP32-C5 MCP2515 TEST");
    ESP_LOGI(TAG, " Step 1: SPI + RESET + READ");
    ESP_LOGI(TAG, "================================");

    MCP2515 mcp2515;

    // --------------------------------
    // Initialize SPI
    // --------------------------------

    ESP_ERROR_CHECK(
        mcp2515.begin()
    );

    // --------------------------------
    // Reset MCP2515
    // --------------------------------

    ESP_ERROR_CHECK(
        mcp2515.reset()
    );

    // --------------------------------
    // Read MCP2515 CANSTAT register
    //
    // Address:
    // 0x0E = CANSTAT
    // --------------------------------

    uint8_t canstat = mcp2515.readRegister(0x0E);

    ESP_LOGI(
        TAG,
        "CANSTAT = 0x%02X",
        canstat
    );

    // --------------------------------
    // Read MCP2515 CANCTRL register
    //
    // Address:
    // 0x0F = CANCTRL
    // --------------------------------

    uint8_t canctrl = mcp2515.readRegister(0x0F);

    ESP_LOGI(
        TAG,
        "CANCTRL = 0x%02X",
        canctrl
    );

    ESP_LOGI(TAG, "Test finished.");

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}