#include "cc1101_driver.h"
#include <furi_hal_spi.h>
#include <furi_hal_gpio.h>

#define CC1101_SPI_HANDLE &furi_hal_spi_bus_handle_external

// CC1101 Register definitions
#define CC1101_IOCFG0    0x00
#define CC1101_FREQ2     0x0D
#define CC1101_FREQ1     0x0E
#define CC1101_FREQ0     0x0F
#define CC1101_MDMCFG4   0x10
#define CC1101_MDMCFG3   0x11
#define CC1101_MDMCFG2   0x12
#define CC1101_DEVIATN   0x15
#define CC1101_MCSM0     0x18
#define CC1101_FOCCFG    0x19
#define CC1101_BSCFG     0x1A
#define CC1101_AGCCTRL2  0x1B
#define CC1101_AGCCTRL1  0x1C
#define CC1101_AGCCTRL0  0x1D
#define CC1101_FREND1    0x21
#define CC1101_FREND0    0x22
#define CC1101_FSCAL3    0x23
#define CC1101_FSCAL2    0x24
#define CC1101_FSCAL1    0x25
#define CC1101_FSCAL0    0x26
#define CC1101_RSSI      0x34
#define CC1101_MARCSTATE 0x35
#define CC1101_PKTSTATUS 0x38

// Command strobes
#define CC1101_SRES      0x30
#define CC1101_SFSTXON   0x31
#define CC1101_SXOFF     0x32
#define CC1101_SCAL      0x33
#define CC1101_SRX       0x34
#define CC1101_STX       0x35
#define CC1101_SIDLE     0x36

struct CC1101Driver {
    bool initialized;
    bool external_antenna;
    uint32_t current_frequency;
    uint8_t current_power;
    const GpioPin* cs_pin;
    const GpioPin* gdo0_pin;
    const GpioPin* gdo2_pin;
};

// GPIO pin definitions for external CC1101
static const GpioPin cc1101_gpio_cs = {&gpio_ext_pc3, 3};
static const GpioPin cc1101_gpio_gdo0 = {&gpio_ext_pb2, 2};
static const GpioPin cc1101_gpio_gdo2 = {&gpio_ext_pb3, 3};

CC1101Driver* cc1101_driver_alloc(void) {
    CC1101Driver* driver = malloc(sizeof(CC1101Driver));
    driver->initialized = false;
    driver->external_antenna = false;
    driver->current_frequency = 433920000;
    driver->current_power = 7;
    driver->cs_pin = &cc1101_gpio_cs;
    driver->gdo0_pin = &cc1101_gpio_gdo0;
    driver->gdo2_pin = &cc1101_gpio_gdo2;
    return driver;
}

void cc1101_driver_free(CC1101Driver* driver) {
    furi_assert(driver);
    if(driver->initialized) {
        cc1101_driver_deinit(driver);
    }
    free(driver);
}

static void cc1101_write_reg(CC1101Driver* driver, uint8_t reg, uint8_t value) {
    furi_hal_gpio_write(driver->cs_pin, false);
    furi_hal_spi_bus_tx(CC1101_SPI_HANDLE, &reg, 1, 1000);
    furi_hal_spi_bus_tx(CC1101_SPI_HANDLE, &value, 1, 1000);
    furi_hal_gpio_write(driver->cs_pin, true);
}

static uint8_t cc1101_read_reg(CC1101Driver* driver, uint8_t reg) {
    uint8_t value;
    reg |= 0x80; // Read bit
    furi_hal_gpio_write(driver->cs_pin, false);
    furi_hal_spi_bus_tx(CC1101_SPI_HANDLE, &reg, 1, 1000);
    furi_hal_spi_bus_rx(CC1101_SPI_HANDLE, &value, 1, 1000);
    furi_hal_gpio_write(driver->cs_pin, true);
    return value;
}

static void cc1101_write_strobe(CC1101Driver* driver, uint8_t strobe) {
    furi_hal_gpio_write(driver->cs_pin, false);
    furi_hal_spi_bus_tx(CC1101_SPI_HANDLE, &strobe, 1, 1000);
    furi_hal_gpio_write(driver->cs_pin, true);
}

bool cc1101_driver_init(CC1101Driver* driver, bool external_antenna) {
    furi_assert(driver);
    
    driver->external_antenna = external_antenna;
    
    // Initialize GPIO pins
    furi_hal_gpio_init(driver->cs_pin, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
    furi_hal_gpio_init(driver->gdo0_pin, GpioModeInput, GpioPullNo, GpioSpeedVeryHigh);
    furi_hal_gpio_init(driver->gdo2_pin, GpioModeInput, GpioPullNo, GpioSpeedVeryHigh);
    
    furi_hal_gpio_write(driver->cs_pin, true);
    
    // Acquire SPI bus
    furi_hal_spi_acquire(CC1101_SPI_HANDLE);
    
    // Reset CC1101
    cc1101_write_strobe(driver, CC1101_SRES);
    furi_delay_ms(10);
    
    // Configure CC1101 registers
    cc1101_write_reg(driver, CC1101_IOCFG0, 0x06);    // GDO0 output pin config
    cc1101_write_reg(driver, CC1101_MDMCFG4, 0xF8);   // Modem configuration
    cc1101_write_reg(driver, CC1101_MDMCFG3, 0x83);   // Modem configuration
    cc1101_write_reg(driver, CC1101_MDMCFG2, 0x32);   // Modem configuration (OOK)
    cc1101_write_reg(driver, CC1101_DEVIATN, 0x00);   // Modem deviation
    cc1101_write_reg(driver, CC1101_MCSM0, 0x18);     // Main Radio Control State Machine
    cc1101_write_reg(driver, CC1101_FOCCFG, 0x16);    // Frequency Offset Compensation
    cc1101_write_reg(driver, CC1101_BSCFG, 0x6C);     // Bit synchronization
    cc1101_write_reg(driver, CC1101_AGCCTRL2, 0x43);  // AGC control
    cc1101_write_reg(driver, CC1101_AGCCTRL1, 0x40);  // AGC control
    cc1101_write_reg(driver, CC1101_AGCCTRL0, 0x91);  // AGC control
    cc1101_write_reg(driver, CC1101_FREND1, 0x56);    // Front end RX configuration
    cc1101_write_reg(driver, CC1101_FREND0, 0x10);    // Front end TX configuration
    cc1101_write_reg(driver, CC1101_FSCAL3, 0xE9);    // Frequency synthesizer calibration
    cc1101_write_reg(driver, CC1101_FSCAL2, 0x2A);    // Frequency synthesizer calibration
    cc1101_write_reg(driver, CC1101_FSCAL1, 0x00);    // Frequency synthesizer calibration
    cc1101_write_reg(driver, CC1101_FSCAL0, 0x1F);    // Frequency synthesizer calibration
    
    driver->initialized = true;
    return true;
}

void cc1101_driver_deinit(CC1101Driver* driver) {
    furi_assert(driver);
    
    if(driver->initialized) {
        cc1101_write_strobe(driver, CC1101_SIDLE);
        furi_hal_spi_release(CC1101_SPI_HANDLE);
        driver->initialized = false;
    }
}

void cc1101_driver_set_frequency(CC1101Driver* driver, uint32_t frequency) {
    furi_assert(driver);
    
    driver->current_frequency = frequency;
    
    // Calculate frequency registers
    uint32_t freq_reg = (uint32_t)((float)frequency / 26000000.0f * 65536.0f);
    
    cc1101_write_reg(driver, CC1101_FREQ2, (freq_reg >> 16) & 0xFF);
    cc1101_write_reg(driver, CC1101_FREQ1, (freq_reg >> 8) & 0xFF);
    cc1101_write_reg(driver, CC1101_FREQ0, freq_reg & 0xFF);
}

void cc1101_driver_set_power(CC1101Driver* driver, uint8_t power) {
    furi_assert(driver);
    driver->current_power = power;
    // Power setting implementation would go here
}

void cc1101_driver_start_rx(CC1101Driver* driver) {
    furi_assert(driver);
    cc1101_write_strobe(driver, CC1101_SRX);
}

void cc1101_driver_stop_rx(CC1101Driver* driver) {
    furi_assert(driver);
    cc1101_write_strobe(driver, CC1101_SIDLE);
}

void cc1101_driver_start_tx(CC1101Driver* driver) {
    furi_assert(driver);
    cc1101_write_strobe(driver, CC1101_STX);
}

void cc1101_driver_stop_tx(CC1101Driver* driver) {
    furi_assert(driver);
    cc1101_write_strobe(driver, CC1101_SIDLE);
}

float cc1101_driver_get_rssi(CC1101Driver* driver) {
    furi_assert(driver);
    uint8_t rssi_raw = cc1101_read_reg(driver, CC1101_RSSI);
    if(rssi_raw >= 128) {
        return ((float)(rssi_raw - 256) / 2.0f) - 74.0f;
    } else {
        return ((float)rssi_raw / 2.0f) - 74.0f;
    }
}

bool cc1101_driver_is_signal_detected(CC1101Driver* driver) {
    furi_assert(driver);
    return cc1101_driver_get_rssi(driver) > -70.0f; // Threshold
}

