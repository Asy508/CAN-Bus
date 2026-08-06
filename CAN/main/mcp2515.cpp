#include "mcp2515.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MCP2515";

MCP2515::MCP2515()
    : spi(nullptr)
{
}

esp_err_t MCP2515::begin()
{
    ESP_LOGI(TAG, "Initializing SPI...");

    spi_bus_config_t bus_config = {};

    bus_config.mosi_io_num = PIN_MOSI;
    bus_config.miso_io_num = PIN_MISO;
    bus_config.sclk_io_num = PIN_SCLK;

    bus_config.quadwp_io_num = -1;
    bus_config.quadhd_io_num = -1;

    esp_err_t ret = spi_bus_initialize(
        SPI2_HOST,
        &bus_config,
        SPI_DMA_CH_AUTO
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI bus initialization failed: %s",
                 esp_err_to_name(ret));

        return ret;
    }

    spi_device_interface_config_t device_config = {};

    // Start slow for debugging
    device_config.clock_speed_hz = 1 * 1000 * 1000;

    // MCP2515 uses SPI mode 0
    device_config.mode = 0;

    device_config.spics_io_num = PIN_CS;

    device_config.queue_size = 1;

    ret = spi_bus_add_device(
        SPI2_HOST,
        &device_config,
        &spi
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI device initialization failed: %s",
                 esp_err_to_name(ret));

        return ret;
    }

    ESP_LOGI(TAG, "SPI initialized successfully");

    return ESP_OK;
}


esp_err_t MCP2515::reset()
{
    uint8_t command = CMD_RESET;

    spi_transaction_t transaction = {};

    transaction.length = 8;
    transaction.tx_buffer = &command;

    esp_err_t ret = spi_device_transmit(
        spi,
        &transaction
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "MCP2515 reset failed: %s",
                 esp_err_to_name(ret));

        return ret;
    }

    ESP_LOGI(TAG, "MCP2515 RESET command sent");

    // Give MCP2515 some time after reset
    vTaskDelay(pdMS_TO_TICKS(10));

    return ESP_OK;
}


uint8_t MCP2515::readRegister(uint8_t address)
{
    uint8_t tx_data[3] = {
        CMD_READ,
        address,
        0x00
    };

    uint8_t rx_data[3] = {
        0x00,
        0x00,
        0x00
    };

    spi_transaction_t transaction = {};

    transaction.length = 24;

    transaction.tx_buffer = tx_data;
    transaction.rx_buffer = rx_data;

    esp_err_t ret = spi_device_transmit(
        spi,
        &transaction
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Register read failed: %s",
                 esp_err_to_name(ret));

        return 0xFF;
    }

    return rx_data[2];
}