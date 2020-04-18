

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
    float getCurrentVolume();
    float getPressure();
    float getTemperature();
    float getViscosity();
    String getExtinguishant();
    void setPressure(float pressure);
    void setViscosity(float viscosity);
    void setTemperature(float temperature);
    void setExtinguishant(String extinguishant);
    
   

  private:
    tN2kFluidType _fluidType;
    int _instance;
    float _levelPercent;
    float _capacity;
    float _pressure;
    float _temperature;
    float _viscosity; 
    String _extinguishant;  //The preferred extinguishant to douse a fire in this tank
    int _pin;
    float _voltage;
    float _resistor;
    float _resistor_tank_max;
    float (*_resistance_mapping)[2];
    int _length;
    void init(int pin, tN2kFluidType fluidType, int instance, float capacity, float voltage, float resistor, float resistor_tank_max);
  
  };

#endif
