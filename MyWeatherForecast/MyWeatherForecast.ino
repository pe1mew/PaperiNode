/*!

\file    WeatherForecastExample.ino
\author  Robert Poser, Remko Welling
\date    See version history
\version See version history

Version history:
----------------

Version | Date       | Who           | What
--------|------------|---------------|-------------------
1.0     | 19-12-2019 | Robert Poser  | Initial version
2.0     | 28-7-2020  | Remko Welling | Fork from master repo, Updated documentation to meet Doxygen specification, sanitize code.
2.1     | 30-7-2020  | Remko Welling | Sanitize variables, added Doxygen compatible tags. Removed unused variables, movede variables to app-struct
 

program description:
--------------------
This example demonstrates a simple weatherforcest demo. The supercap voltage v_scap is 
measured minutely while the ATmega328p processor is in deep sleep all remaining time. 
Triggering is done via external RTC to minimize current consumption during deep sleep 
phase. IF the voltage is charged above a certain limit (ie 4.2V) and a max. number of 
allowed Lora syncs is not exceeded, a download of is triggered from the TTN console
and the received content is printed on the Epaper Screen.

Supported Hardware:
-------------------
LoraPaper (with RFM95, PV cell, supercap & 1.1" EPD)

Libraries used in this sketch are based on the LoRaWAN stack from IDEETRON/NEXUS, for 
more infos please check this great source: https://github.com/Ideetron/Nexus-Low-Power

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.


*/

#include "paperinode.h"
#include "mcp7940.h"
#include "DS2401.h"
#include "spi_flash.h"
#include "RFM95.h"
#include "LoRaMAC.h"
#include "lorawan_def.h"
#include "Cayenne_LPP.h"
#include "PL_microEPD44.h"

#define DUMMY_DATA 25 ///< Some data to test if downlink data is received over LoRaWAN
/*!
 * GLOBAL VARIABLES 
 */
sAPP        appSettings;                    ///< Struct with application variables set to default settings
sLoRaWAN    lora;                           ///< See the lorapaper.h file for hardware settings of RFM95
sTimeDate   TimeDate;                       ///< RTC time and date variables
LORAMAC     lorawan (&lora);                ///< Initialize the LoRaWAN stack.  
CayenneLPP  LPP(&(lora.TX));                ///< Initialize the Low Power Protocol functions
PL_microEPD epd(EPD_CS, EPD_RST, EPD_BUSY); ///< Initialize the EPD.

volatile bool RTC_ALARM = false;            ///< Interrupt variable

/// \brief Interrupt vector for the alarm of the MCP7940 Real Time Clock.
/// \pre Do not use I2C functions or long delays here.
/// \param Interrupt vector
ISR(INT1_vect){
  RTC_ALARM = true;
}

/// \brief Interrupt vector for Timer1 which is used to time the Join and Receive windows for 
/// timeslot 1 and timelsot 2 Increment the timeslot counter variable for timing the
ISR(TIMER1_COMPA_vect){
  lora.timeslot++;
}

void setup(void){  
   analogReference(EXTERNAL);          // use AREF for reference voltage
   SPI.begin();                        // Initialize the SPI port
   SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
   
   pinMode(CAPVOLTAGE_PIN, INPUT);     // To measure V_Scap
   pinMode(SW_TFT, OUTPUT);            // Switch for V_Scap
   pinMode(DS2401, OUTPUT);            // Authenticating IC DS2401p+
   pinMode(RTC_MFP, INPUT);            // RTC MCP7940
   pinMode(RFM_DIO0, INPUT);           // RFM95W DIO0
   pinMode(RFM_DIO1, INPUT);           // RFM95W DIO1
   pinMode(RFM_DIO5, INPUT);           // RFM95W DIO5
   pinMode(RFM_NSS, OUTPUT);           // RFM95W NSS = CS
   pinMode(SPI_FLASH_CS, OUTPUT);
   digitalWrite(DS2401, HIGH);         // to save power...
   digitalWrite(SW_TFT, HIGH);         // to save power...
   digitalWrite(SPI_FLASH_CS, HIGH);   // to save power...
 
   PCMSK1 = 0;                         // Disable Pin change interrupts  
   PCICR = 0;                          // Disable Pin Change Interrupts
   delay(10);                          // Power on delay for the RFM module
   I2C_init();                         // Initialize I2C pins
   flash_power_down();                 // To save power...
   sei();                              // Enable Global Interrupts 
  
   epd.begin();                        // Turn ON & initialize 1.1" EPD
   epd.printText("Syncing with TTN...", 16, 20, 1);
   epd.update();                       // Update EPD
   epd.end();                          // to save power...
   digitalWrite(RFM_NSS, LOW);         // to save power...
   SPI.endTransaction();               // to save power...
   SPI.end();                          // to save power...
 
   appSettings.scapVoltage = analogRead(SCAP_PIN);            // 1st Dummy-read which always delivers strange values...(to skip)
   
   mcp7940_init(&TimeDate, appSettings.messageInterval);   // Generate minutely interrupt 
   RTC_ALARM = true;
}

void loop(){ 
   if(RTC_ALARM == true){              // Catch the minute alarm from the RTC. 
      RTC_ALARM = false;               // Clear the boolean.
        
      mcp7940_reset_minute_alarm(appSettings.messageInterval);        
      mcp7940_read_time_and_date(&TimeDate);    
        
      digitalWrite(SW_TFT, LOW);       // Turn ON voltage divider 
      delay(1);                        // To stabilize analogRead
      appSettings.scapVoltage = analogRead(SCAP_PIN);         // Measure V_scap
      digitalWrite(SW_TFT, HIGH);      // Turn OFF voltage divider 
      
      if (appSettings.scapVoltage >= 640) {             // Proceed only if (Vscap > 4,2V)--> DEFAULT!
         if (++appSettings.messageCounter %2) {        // Every two hours...  
            if (appSettings.messageCounter >= 4){       // Change wakeup from minutely to every 2hours
               appSettings.messageInterval = 58;
            }

            // Compose payload
            LPP.clearBuffer();         // Form a payload according to the LPP standard to 
            LPP.addDigitalOutput(0x00, appSettings.messageCounter);
            LPP.addAnalogInput(0x00, appSettings.scapVoltage * 3.3/1023*2);

            // Prepare display
            SPI.begin();
            SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
            epd.begin(false);          // Turn ON EPD without refresh to save power

            // Transmit and receive LoRaWAN data
            lorawan.init();            // Init the RFM95 module
            lora.RX.Data[6] = DUMMY_DATA;      // Dummy value to check lateron if downlink data was received
            lorawan.LORA_send_and_receive();  // LoRaWAN send_and_receive
            // Test if downlink data is received
            if (lora.RX.Data[6] == DUMMY_DATA){
               // No data received, increase no-data-received counter 
               appSettings.failedDownlinkCounter++;                       
            }
            
            epd.printText("Current Weather ", 1, 2, 1);         // Header line

            // Print houer for which weather is presented.
            // When hour or minute is below 10, add preceding space to align presentation on EPD.
            String leadingHour = "";
            if(lora.RX.Data[3] < 10) {
               leadingHour = " ";
            }
            String leadingMinute = "";
            if(lora.RX.Data[4] < 10) {
               leadingMinute = "0";
            }
            epd.printText(leadingHour + String(lora.RX.Data[3]) + ":" + leadingMinute + String(lora.RX.Data[4]), 110, 2, 1);

            // Print temperature on EPD.
            // When temperature is between -1 and 10 degrees, add preceding space to align presentation on EPD.
            String leadingTemp = "";
            if(int8_t(lora.RX.Data[0]) > -1 && int8_t(lora.RX.Data[0]) < 10) {
               leadingTemp = " ";
            }
            epd.printText(leadingTemp + String(int8_t(lora.RX.Data[0])), 11, 16, 3);  // Temperature
            epd.printText("o", 53, 12, 2);
            epd.printText("C", 65, 16, 3);
      
            // Print humidity on EPD.
            // When humidity is between -1 and 10 degrees, add preceding space to align presentation on EPD.
            String leadingHumi = "";
            if(int8_t(lora.RX.Data[1]) < 10) {
               leadingHumi = " ";
            }
            epd.printText(leadingHumi + String(uint8_t(lora.RX.Data[1])), 11, 44, 3);  // Humidity
            epd.printText("%", 65, 44, 3);

                                       // Just to demonstrate how to write little
                                       // icons; here it will always be the same 
                                       // independent of the weather forecast :-)
            //epd.drawBitmapLM(87, 15, wIcon_sunny, 24, 24);

            epd.fillRectLM(87 , 41, 4, 1, EPD_BLACK);
            epd.fillRectLM(92 , 41, 4, 1, EPD_BLACK);
            epd.fillRectLM(97 , 41, 4, 1, EPD_BLACK);
            epd.fillRectLM(102, 41, 4, 1, EPD_BLACK);
            epd.fillRectLM(107, 41, 4, 1, EPD_BLACK);
            uint8_t r1 = uint8_t(lora.RX.Data[7])  * 2;
            uint8_t r2 = uint8_t(lora.RX.Data[8])  * 2;
            uint8_t r3 = uint8_t(lora.RX.Data[9])  * 2;
            uint8_t r4 = uint8_t(lora.RX.Data[10]) * 2;
            uint8_t r5 = uint8_t(lora.RX.Data[11]) * 2;
            if(r1 > 20){
               r1 = 20;
            }
            if(r2 > 20){
               r2 = 20;
            }
            if(r3 > 20){
               r3 = 20;
            }
            if(r4 > 20){
               r4 = 20;
            }
            if(r5 > 20){
               r5 = 20;
            }
            epd.fillRectLM(87 , 39, 4, r1, EPD_BLACK);
            epd.fillRectLM(92 , 39, 4, r2, EPD_BLACK);
            epd.fillRectLM(97 , 39, 4, r3, EPD_BLACK);
            epd.fillRectLM(102, 39, 4, r4, EPD_BLACK);
            epd.fillRectLM(107, 39, 4, r5, EPD_BLACK);
              
            // Plot last known voltage
            epd.printText("V " + String(appSettings.scapVoltage*3.3/1023*2, 1),
                          115, 20, 1);  
            // Print how many syncs have been tried
            epd.printText("U " + String(appSettings.messageCounter/2+1),
                          115, 30, 1); 
            // Print how many downlinks were empty
            epd.printText("D " + String(appSettings.messageCounter/2+1 - appSettings.failedDownlinkCounter),
                          115, 40, 1); 
      
            epd.update();              // Send the framebuffer and do the update
            epd.end();                 // To save power...
            digitalWrite(RFM_NSS, LOW);// To save power...
            SPI.endTransaction();      // To save power...
            SPI.end();                 // To save power...
         }
      }
   }else{
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); // To save power... 
   }
}
