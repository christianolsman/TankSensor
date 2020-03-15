/*
  TankSensor.ino
  Module for measuring fluid level and report back over NMEA2000 using the NMEA 2000 library found here https://github.com/ttlappalainen/NMEA2000
  Created by Christian Olsman 11 of March 2020.

*/

#include <Arduino.h>
#include <NMEA2000_teensy.h>  // Comment if not running on a teensy
//#include <NMEA2000_CAN.h>  // Uncomment for running on e,g. ESP32.  This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMessages.h>
#include "Tank.h"
#include <Vector.h>

//#define CALIBRATION  
#define DataUpdatePeriod 2000

tNMEA2000_teensy NMEA2000;
long lastUpdate;

float resistance_mapping [][2] = {{0, 0}, {10, 5}, {50, 10}, {100, 20}, {120, 50}, {150, 90}, {186, 100}};
Tank waterTank(20, N2kft_Water, 0, 100, 3.31, 217, 186.62, resistance_mapping, sizeof(resistance_mapping) / sizeof(float) / 2);

void setup() {
  // Set Product information
  NMEA2000.SetN2kCANSendFrameBufSize(100);
  NMEA2000.SetProductInformation("00000001", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "Fluid level sensor",  // Manufacturer's Model ID§
                                 "1.0.0.1 (2020-03-10)",  // Manufacturer's Software version code
                                 "1.0.0.0 (2020-03-10)" // Manufacturer's Model version
                                );
  // Set device information
  NMEA2000.SetDeviceInformation(1, // Unique number. Use e.g. Serial number.
                                150, // Device function=Devices that measure/report fluid level. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                75, // Device class=Sensor Communication Interface. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );



  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly, 22);
  NMEA2000.Open();

#ifdef CALIBRATION
  Serial.begin(115200);
#endif

lastUpdate=millis();

}

void loop() {

  if ( lastUpdate + DataUpdatePeriod < millis() ) {
    lastUpdate = millis();

#ifdef CALIBRATION //Only call get level function and do not publish on network
    waterTank.getLevel();
#else
    tN2kMsg N2kMsg;
    SetN2kFluidLevel(N2kMsg, waterTank.getInstance(), waterTank.getFluidType(), waterTank.getLevel(), waterTank.getCapacity());
    NMEA2000.SendMsg(N2kMsg);
#endif
  }
}
