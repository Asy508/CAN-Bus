#pragma once

#include <stdint.h>
#include "driver/spi_master.h"

class MCP2515
{
public:
    MCP2515();

    esp_err_t begin();
    esp_err_t reset();

    uint8_t readRegister(uint8_t address);

private:
    spi_device_handle_t spi;

    static constexpr int PIN_MOSI = 8;
    static constexpr int PIN_MISO = 9;
    static constexpr int PIN_SCLK = 7;
    static constexpr int PIN_CS   = 10;

    static constexpr uint8_t CMD_RESET = 0xC0;
    static constexpr uint8_t CMD_READ  = 0x03;
};