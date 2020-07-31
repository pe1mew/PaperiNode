/// \file paperinonode.h
/// \brief Hardware and program settings for PaperiNode
/// \date 31-7-2020
/// \author  Robert Poser, Remko Welling

#ifndef PAPERINODE_DEMOBOARD_H_
#define PAPERINODE_DEMOBOARD_H_

#include <SPI.h>
#include "I2C.h"
#include "spi_functions.h"
#include "progmem.h"
#include "LowPower.h"

/*!
 * Defines for various hardware
 */
#define CAPVOLTAGE_PIN  7   ///< Analog input pin for measuring CAP voltage
#define EPD_RST         A2  ///< Reset for EPD
#define EPD_BUSY        A1  ///< Busy for EPD
#define EPD_CS          A3  ///< Chip Select for EPD
#define HV_BST          9   ///<
#define SW_TFT          A0  ///< Enable scap measurement
#define SCAP_PIN        A7  ///< Analog in from SCAP measurement trough 4,05 M resistor divider.
#define DS2401          2   ///< One wire pin for the Maxim DS2401 Silicon Serial Number
#define RTC_MFP         3   ///< 
#define RFM_DIO0        4   ///< RFM95 DIO0
#define RFM_DIO1        5   ///< RFM95 DIO1
#define RFM_DIO5        6   ///< RFM95 DIO5
#define RFM_NSS         10  ///< Chip Select for RFM 95
#define SPI_FLASH_CS    8   ///< Chip Select for FLash memory

/*!
 * Struct to contain various app variables with default values.
 */
typedef struct{
  uint8_t   messageCounter = 0;             ///< Transmit counter resest at reset or after programming 
  uint16_t  messageInterval = 1;            ///<  Variable to set the number of Timer2 timer ticks between LoRaWAN messages.
  uint16_t  failedDownlinkCounter = 0;      ///< counter of failed downloads & max_syncs 
  uint16_t  scapVoltage = 0;                ///< V_Supercap, counter of failed downloads & max_syncs
  uint16_t  d = { 0 };                      ///< \todo Unknown variable
}sAPP;	

#endif 
