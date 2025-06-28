#ifndef CC1101_SUBGHZ_TYPES_H
#define CC1101_SUBGHZ_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// CC1101 Register Addresses
#define CC1101_IOCFG2       0x00    // GDO2 output pin configuration
#define CC1101_IOCFG1       0x01    // GDO1 output pin configuration
#define CC1101_IOCFG0       0x02    // GDO0 output pin configuration
#define CC1101_FIFOTHR      0x03    // RX FIFO and TX FIFO thresholds
#define CC1101_SYNC1        0x04    // Sync word, high byte
#define CC1101_SYNC0        0x05    // Sync word, low byte
#define CC1101_PKTLEN       0x06    // Packet length
#define CC1101_PKTCTRL1     0x07    // Packet automation control
#define CC1101_PKTCTRL0     0x08    // Packet automation control
#define CC1101_ADDR         0x09    // Device address
#define CC1101_CHANNR       0x0A    // Channel number
#define CC1101_FSCTRL1      0x0B    // Frequency synthesizer control
#define CC1101_FSCTRL0      0x0C    // Frequency synthesizer control
#define CC1101_FREQ2        0x0D    // Frequency control word, high byte
#define CC1101_FREQ1        0x0E    // Frequency control word, middle byte
#define CC1101_FREQ0        0x0F    // Frequency control word, low byte
#define CC1101_MDMCFG4      0x10    // Modem configuration
#define CC1101_MDMCFG3      0x11    // Modem configuration
#define CC1101_MDMCFG2      0x12    // Modem configuration
#define CC1101_MDMCFG1      0x13    // Modem configuration
#define CC1101_MDMCFG0      0x14    // Modem configuration
#define CC1101_DEVIATN      0x15    // Modem deviation setting
#define CC1101_MCSM2        0x16    // Main Radio Control State Machine configuration
#define CC1101_MCSM1        0x17    // Main Radio Control State Machine configuration
#define CC1101_MCSM0        0x18    // Main Radio Control State Machine configuration
#define CC1101_FOCCFG       0x19    // Frequency Offset Compensation configuration
#define CC1101_BSCFG        0x1A    // Bit Synchronization configuration
#define CC1101_AGCCTRL2     0x1B    // AGC control
#define CC1101_AGCCTRL1     0x1C    // AGC control
#define CC1101_AGCCTRL0     0x1D    // AGC control
#define CC1101_WOREVT1      0x1E    // High byte Event 0 timeout
#define CC1101_WOREVT0      0x1F    // Low byte Event 0 timeout
#define CC1101_WORCTRL      0x20    // Wake On Radio control
#define CC1101_FREND1       0x21    // Front end RX configuration
#define CC1101_FREND0       0x22    // Front end TX configuration
#define CC1101_FSCAL3       0x23    // Frequency synthesizer calibration
#define CC1101_FSCAL2       0x24    // Frequency synthesizer calibration
#define CC1101_FSCAL1       0x25    // Frequency synthesizer calibration
#define CC1101_FSCAL0       0x26    // Frequency synthesizer calibration
#define CC1101_RCCTRL1      0x27    // RC oscillator configuration
#define CC1101_RCCTRL0      0x28    // RC oscillator configuration
#define CC1101_FSTEST       0x29    // Frequency synthesizer calibration control
#define CC1101_PTEST        0x2A    // Production test
#define CC1101_AGCTEST      0x2B    // AGC test
#define CC1101_TEST2        0x2C    // Various test settings
#define CC1101_TEST1        0x2D    // Various test settings
#define CC1101_TEST0        0x2E    // Various test settings

// Status Registers
#define CC1101_PARTNUM      0x30    // Part number
#define CC1101_VERSION      0x31    // Current version number
#define CC1101_FREQEST      0x32    // Frequency offset estimate
#define CC1101_LQI          0x33    // Link quality estimate
#define CC1101_RSSI         0x34    // Received signal strength indication
#define CC1101_MARCSTATE    0x35    // Control state machine state
#define CC1101_WORTIME1     0x36    // High byte of WOR timer
#define CC1101_WORTIME0     0x37    // Low byte of WOR timer
#define CC1101_PKTSTATUS    0x38    // Current GDOx status and packet status
#define CC1101_VCO_VC_DAC   0x39    // Current setting from PLL calibration module
#define CC1101_TXBYTES      0x3A    // Underflow and number of bytes in TX FIFO
#define CC1101_RXBYTES      0x3B    // Overflow and number of bytes in RX FIFO
#define CC1101_RCCTRL1_STATUS 0x3C  // Last RC oscillator calibration result
#define CC1101_RCCTRL0_STATUS 0x3D  // Last RC oscillator calibration result

// Command Strobes
#define CC1101_SRES         0x30    // Reset chip
#define CC1101_SFSTXON      0x31    // Enable and calibrate frequency synthesizer
#define CC1101_SXOFF        0x32    // Turn off crystal oscillator
#define CC1101_SCAL         0x33    // Calibrate frequency synthesizer and turn it off
#define CC1101_SRX          0x34    // Enable RX
#define CC1101_STX          0x35    // Enable TX
#define CC1101_SIDLE        0x36    // Exit RX / TX, turn off frequency synthesizer
#define CC1101_SAFC         0x37    // Perform AFC adjustment
#define CC1101_SWOR         0x38    // Start automatic RX polling sequence
#define CC1101_SPWD         0x39    // Enter power down mode when CSn goes high
#define CC1101_SFRX         0x3A    // Flush the RX FIFO buffer
#define CC1101_SFTX         0x3B    // Flush the TX FIFO buffer
#define CC1101_SWORRST      0x3C    // Reset real time clock
#define CC1101_SNOP         0x3D    // No operation

// FIFO Access
#define CC1101_TXFIFO       0x3F    // TX FIFO
#define CC1101_RXFIFO       0x3F    // RX FIFO

// Burst and single access
#define CC1101_WRITE_BURST  0x40
#define CC1101_READ_SINGLE  0x80
#define CC1101_READ_BURST   0xC0

// MARC States
typedef enum {
    CC1101_MARCSTATE_SLEEP          = 0x00,
    CC1101_MARCSTATE_IDLE           = 0x01,
    CC1101_MARCSTATE_XOFF           = 0x02,
    CC1101_MARCSTATE_VCOON_MC       = 0x03,
    CC1101_MARCSTATE_REGON_MC       = 0x04,
    CC1101_MARCSTATE_MANCAL         = 0x05,
    CC1101_MARCSTATE_VCOON          = 0x06,
    CC1101_MARCSTATE_REGON          = 0x07,
    CC1101_MARCSTATE_STARTCAL       = 0x08,
    CC1101_MARCSTATE_BWBOOST        = 0x09,
    CC1101_MARCSTATE_FS_LOCK        = 0x0A,
    CC1101_MARCSTATE_IFADCON        = 0x0B,
    CC1101_MARCSTATE_ENDCAL         = 0x0C,
    CC1101_MARCSTATE_RX             = 0x0D,
    CC1101_MARCSTATE_RX_END         = 0x0E,
    CC1101_MARCSTATE_RX_RST         = 0x0F,
    CC1101_MARCSTATE_TXRX_SWITCH    = 0x10,
    CC1101_MARCSTATE_RXFIFO_OVERFLOW = 0x11,
    CC1101_MARCSTATE_FSTXON         = 0x12,
    CC1101_MARCSTATE_TX             = 0x13,
    CC1101_MARCSTATE_TX_END         = 0x14,
    CC1101_MARCSTATE_RXTX_SWITCH    = 0x15,
    CC1101_MARCSTATE_TXFIFO_UNDERFLOW = 0x16
} cc1101_marcstate_t;

// Radio States
typedef enum {
    CC1101_STATE_IDLE = 0,
    CC1101_STATE_RX,
    CC1101_STATE_TX,
    CC1101_STATE_FSTXON,
    CC1101_STATE_CALIBRATE,
    CC1101_STATE_SETTLING,
    CC1101_STATE_RXFIFO_OVERFLOW,
    CC1101_STATE_TXFIFO_UNDERFLOW,
    CC1101_STATE_SLEEP
} cc1101_state_t;

// Modulation Types
typedef enum {
    CC1101_MOD_2FSK = 0,
    CC1101_MOD_GFSK,
    CC1101_MOD_ASK_OOK,
    CC1101_MOD_4FSK,
    CC1101_MOD_MSK
} cc1101_modulation_t;

// Data Rate Configuration
typedef struct {
    uint8_t mdmcfg4;    // DRATE_E and CHANBW_E
    uint8_t mdmcfg3;    // DRATE_M
} cc1101_datarate_config_t;

// Frequency Configuration
typedef struct {
    uint8_t freq2;      // FREQ[23:16]
    uint8_t freq1;      // FREQ[15:8]
    uint8_t freq0;      // FREQ[7:0]
} cc1101_frequency_config_t;

// Power Amplifier Table Entry
typedef struct {
    uint8_t power_setting;
    int8_t power_dbm;
} cc1101_pa_table_entry_t;

// Packet Configuration
typedef struct {
    uint8_t sync1;          // Sync word high byte
    uint8_t sync0;          // Sync word low byte
    uint8_t pktlen;         // Packet length
    uint8_t pktctrl1;       // Packet control 1
    uint8_t pktctrl0;       // Packet control 0
    uint8_t addr;           // Device address
} cc1101_packet_config_t;

// Complete CC1101 Configuration
typedef struct {
    // IO Configuration
    uint8_t iocfg2;
    uint8_t iocfg1;
    uint8_t iocfg0;
    
    // FIFO and Sync
    uint8_t fifothr;
    cc1101_packet_config_t packet;
    
    // Channel and Frequency
    uint8_t channr;
    uint8_t fsctrl1;
    uint8_t fsctrl0;
    cc1101_frequency_config_t frequency;
    
    // Modem Configuration
    uint8_t mdmcfg4;
    uint8_t mdmcfg3;
    uint8_t mdmcfg2;
    uint8_t mdmcfg1;
    uint8_t mdmcfg0;
    uint8_t deviatn;
    
    // Main Radio Control State Machine
    uint8_t mcsm2;
    uint8_t mcsm1;
    uint8_t mcsm0;
    
    // Frequency Offset and Bit Sync
    uint8_t foccfg;
    uint8_t bscfg;
    
    // AGC Control
    uint8_t agcctrl2;
    uint8_t agcctrl1;
    uint8_t agcctrl0;
    
    // Wake on Radio
    uint8_t worevt1;
    uint8_t worevt0;
    uint8_t worctrl;
    
    // Front End Configuration
    uint8_t frend1;
    uint8_t frend0;
    
    // Frequency Synthesizer Calibration
    uint8_t fscal3;
    uint8_t fscal2;
    uint8_t fscal1;
    uint8_t fscal0;
    
    // Test Settings
    uint8_t test2;
    uint8_t test1;
    uint8_t test0;
} cc1101_config_t;

// Status Information
typedef struct {
    uint8_t rssi_raw;       // Raw RSSI value
    int16_t rssi_dbm;       // RSSI in dBm
    uint8_t lqi;            // Link Quality Indicator
    bool crc_ok;            // CRC status
} cc1101_rx_info_t;

// Packet Structure
typedef struct {
    uint8_t length;         // Packet length
    uint8_t *data;          // Packet data
    cc1101_rx_info_t rx_info; // Reception information (for RX)
} cc1101_packet_t;

// Callback function types
typedef void (*cc1101_rx_callback_t)(cc1101_packet_t *packet);
typedef void (*cc1101_tx_callback_t)(bool success);

// Error codes
typedef enum {
    CC1101_OK = 0,
    CC1101_ERROR_INVALID_PARAM,
    CC1101_ERROR_SPI,
    CC1101_ERROR_TIMEOUT,
    CC1101_ERROR_FIFO_OVERFLOW,
    CC1101_ERROR_FIFO_UNDERFLOW,
    CC1101_ERROR_NOT_INITIALIZED,
    CC1101_ERROR_INVALID_STATE
} cc1101_error_t;

// Sub-GHz band definitions
#define CC1101_FREQ_315MHZ      315000000UL
#define CC1101_FREQ_433MHZ      433000000UL
#define CC1101_FREQ_868MHZ      868000000UL
#define CC1101_FREQ_915MHZ      915000000UL

// Common data rates (bps)
#define CC1101_DATARATE_1200    1200
#define CC1101_DATARATE_2400    2400
#define CC1101_DATARATE_4800    4800
#define CC1101_DATARATE_9600    9600
#define CC1101_DATARATE_19200   19200
#define CC1101_DATARATE_38400   38400
#define CC1101_DATARATE_76800   76800
#define CC1101_DATARATE_153600  153600

// FIFO sizes
#define CC1101_FIFO_SIZE        64
#define CC1101_MAX_PACKET_SIZE  255

// Frequency resolution
#define CC1101_FREQ_RESOLUTION  396.728515625f  // Hz per LSB at 26MHz crystal

// RSSI offset
#define CC1101_RSSI_OFFSET      74

// Macros for frequency calculation
#define CC1101_FREQ_TO_REG(freq_hz) ((uint32_t)((freq_hz) / CC1101_FREQ_RESOLUTION))
#define CC1101_REG_TO_FREQ(reg_val) ((uint32_t)((reg_val) * CC1101_FREQ_RESOLUTION))

// Macros for RSSI conversion
#define CC1101_RSSI_TO_DBM(rssi_raw) ((int16_t)((rssi_raw < 128) ? \
    ((int16_t)rssi_raw / 2) - CC1101_RSSI_OFFSET : \
    ((int16_t)(rssi_raw - 256) / 2) - CC1101_RSSI_OFFSET))

// GDO pin configurations
#define CC1101_GDO_RX_FIFO_THR      0x00
#define CC1101_GDO_RX_FIFO_THR_PKT  0x01
#define CC1101_GDO_TX_FIFO_THR      0x02
#define CC1101_GDO_TX_FIFO_FULL     0x03
#define CC1101_GDO_RX_FIFO_OVER     0x04
#define CC1101_GDO_TX_FIFO_UNDER    0x05
#define CC1101_GDO_SYNC_WORD        0x06
#define CC1101_GDO_PKT_RECEIVED     0x07
#define CC1101_GDO_CCA              0x08
#define CC1101_GDO_PLL_LOCK         0x0A
#define CC1101_GDO_SERIAL_CLOCK     0x0B
#define CC1101_GDO_SERIAL_DATA_SYNC 0x0C
#define CC1101_GDO_SERIAL_DATA_ASYNC 0x0D
#define CC1101_GDO_CARRIER_SENSE    0x0E
#define CC1101_GDO_CRC_OK           0x0F

#ifdef __cplusplus
}
#endif

#endif // CC1101_SUBGHZ_TYPES_H