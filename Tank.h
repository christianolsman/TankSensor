

/*
  Tank.h - Library for fluid levels in tanks
  Created by Christian Olsman 11 of March 2020.
 
*/
#ifndef Tank_h
#define Tank_h


#include "Arduino.h"
#include <N2kMessages.h>

class Tank{
  public:
    Tank(int pin,tN2kFluidType fluidType,int instance, float capacity,float voltage, float resistor, float resistor_tank_max);
Tank(int pin,tN2kFluidType fluidType,int instance, float capacity,float voltage, float resistor, float resistor_tank_max, float resistance_mapping[][2],int length);
   //  Tank(int pin,tN2kFluidType fluidType,int instance, float capacity,float voltage, float resistor, float resistor_tank_max, float &resistance_mapping[][]);

    void setInstance(int instance);
    int getInstance();
    void setFluidType(tN2kFluidType fluidType);
    tN2kFluidType getFluidType();
    float getLevel();
    void setCapacity(float capacityLitres);
    float getCapacity();
   

  private:
    tN2kFluidType _fluidType;
    int _instance;
    float _levelPercent;
    float _capacity; 
    int _pin;
    float _voltage;
    float _resistor;
    float _resistor_tank_max;
    float (*_resistance_mapping)[2];
    int _length;
    void init(int pin, tN2kFluidType fluidType, int instance, float capacity, float voltage, float resistor, float resistor_tank_max);
  
  };

#endif

// Input:
//  - Instance              Tank instance. Different devices handles this a bit differently. So it is best to have instance unique over
//                          all devices on the bus.
//  - FluidType             Defines type of fluid. See definition of tN2kFluidType
//  - Level                 Tank level in % of full tank.
//  - Capacity              Tank Capacity in litres
// Output:
//  - N2kMsg                NMEA2000 message ready to be send.
