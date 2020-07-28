![PaperiNode](images/Screenshot_2020-07-16-RobPo-PaperiNode.jpg?raw=true "PaperiNode")

# PaperiNode, an Arduino-compatible flexible low power energy-harvesting LoRa-enabled electronic paper display
https://www.electronics-lab.com/paperinode-arduino-compatible-flexible-low-power-energy-harvesting-lora-enabled-electronic-paper-display/ 
Retrieved from hackster.io on July 28, 2020 by Remko Welling

Robert Poser has posted his project on GitHub called PaperiNode, which is a flexible, low power, energy-harvesting, LoRa-enabled, electronic paper display. PaperiNode is ideal for the Paperino (an easy to use micro EPD breakout-board for the Photon or other Arduino-compatible microcontrollers. The eInk-based ePaper display mimics the appearance of natural paper and is capable of holding text and images indefinitely, even without electricity). This makes Paperino perfect for your next battery-driven, a connected project where the display content changes rarely.

## Specifications include:

  - MCU: ATmega328pb (16MHz, 32KB FLASH, 2KB SRAM)
  - External RTC: MCP7940M
  - LoRa Chip: RFM95W, Antenna EU_863_870 or u.FL connector for external antenna
  - EPD: 1.1″ Plastic Logic, 148x70pixel
  - Flash: Winbond W25X40Cl, 4Mbit
  - Low power design: Deep Sleep 2.4uA
  - PV cells: IXYS SLMD121H04L
  - Energy-harvesting PMIC: E-Peas AEM10941 w/ MPPT
  - Storage device: EDLC supercap 400mF
  
![PaperiNodePWR](images/Screenshot_2020-07-16-RobPo-PaperiNode1.png?raw=true "PaperiNodePWR")

## PaperiNode Power Consumption

The PaperiNode has more to offer than just being a flexible EPD breakout board. It is also an Arduino-compatible development board and comes with complete example code and EPD drivers that are capable of running nonstop just from the small amount of power it harnesses from its two IXYS SLMD121H04L solar panels which are mounted on the rear of the device. The process of harnessing the solar energy from the two photovoltaic panels is carried out via a dedicated E-Peas AEM10941 MPPT PMIC, which also acts as a buffer to keep the balance topped up on a large 400mF HA230F EDLC supercapacitor from Cap XX. This supercapacitor device enables the device to store some charge for later use when there are no clear skies for a solar charge, and the solar cells aren’t able to gather the peak Tx current needed by the radio. The supercapacitor also offers enough headroom for some radio capabilities.

The device features the ATmega328PB, which is a very popular MCU in the Arduino family. It features 32KB of flash / 2KB of SRAM, running at 16MHz, and it is housed inside SMD TQFP part instead of the PDIP. The ATmega functions well at low power, and when it’s turned fully off. This is possible via the external RTC IC it features, called the Microchip MCP7940M. The Microchip MCP7940M  is capable of sending a configurable interrupt signal via its MFP pin. This enables you to design your applications to utilize only a minute amount of power. One remarkable thing about the EPD module is that it does not need power to hold a static image, couple with the fact that the ATmega remains idle in ” sleep” mode, just waiting for an interrupt from the RTC to turn it on, there is really no power consumption, just about 2.4uA between display updates. One thing that is very important for graphics application is memory space. The ATmega offers 32KB of flash which can be sufficient for application code, but small for image data, font sets. This problem can be solved through the Winbond W25X40Cl 4Mbit FLASH memory found on the schematic.

The device is an open-source device. The device has a second revision which is the successor of the first edition, and contains the following changes:

  - Reduced deep sleep current
  - SPI pins exposed on bottom side to connect external sensors etc.
  - SPI flash added to store measurement data or upto seven pictures

Finally, the device features a DS2401, which is a Silicon Serial Number IC. It is a hard-coded 48-bit serial number, which is unique across all devices. The advantage of this is that it saves you time programming each device with a UUID, and enables a unique identity for each device, making identification of each device very easy. The PaperiNode is now available on Tindie for $59, and you can find support for the project on GitHub.
