/*
  Tank.cpp - Library for measuring and reporting fluid level
  Created by Christian Olsman 11 of March 2020.

*/

#include "Arduino.h"
#include "Tank.h"
//#define CALIBRATION

Tank::Tank(int pin, tN2kFluidType fluidType, int instance, float capacity, float voltage, float resistor, float resistor_tank_max, float resistance_mapping[][2], int length)
{
  init(pin, fluidType, instance, capacity, voltage, resistor, resistor_tank_max);
  _resistance_mapping = resistance_mapping;
  _length = length;

}

Tank::Tank(int pin, tN2kFluidType fluidType, int instance, float capacity, float voltage, float resistor, float resistor_tank_max)
{
  init( pin, fluidType, instance,  capacity,  voltage, resistor,  resistor_tank_max);
}

void Tank::init(int pin, tN2kFluidType fluidType, int instance, float capacity, float voltage, float resistor, float resistor_tank_max)
{
  pinMode(pin, INPUT);
  _pin = pin;
  _instance = instance;
  _levelPercent = 0;
  _capacity = capacity;
  _fluidType = fluidType;
  _voltage = voltage;
  _resistor = resistor;
  _resistor_tank_max = resistor_tank_max;
  _resistance_mapping = NULL;
  _pressure = 0;
  _temperature = 0;
  _viscosity = 0;
  _extinguishant = "";

}

void Tank::setInstance(int instance)
{
  _instance = instance;
}

int Tank::getInstance()
{
  return _instance;
}

float Tank::getCurrentVolume()
{
  return _capacity * getLevel();
}

float Tank::getPressure()
{
  return _pressure;
}

void Tank::setPressure(float pressure)
{
  _pressure = pressure;
}


float Tank::getTemperature()
{
  return _temperature;
}

void Tank::setTemperature(float temperature)
{
  _temperature = temperature;
}


float Tank::getViscosity()
{
  return _viscosity;
}

void Tank::setViscosity(float viscosity)
{
  _viscosity = viscosity;
}

String Tank::getExtinguishant()
{
  return _extinguishant;
}

void Tank::setExtinguishant(String extinguishant)
{
  _extinguishant = extinguishant;
}

void Tank::setFluidType(tN2kFluidType fluidType)
{
  _fluidType = fluidType;
}

tN2kFluidType Tank::getFluidType()
{
  return _fluidType;
}




float Tank::getLevel()
{
  int raw = 0;
  float R1 = 0;
  float Vout;
  raw = analogRead(_pin);
  if (raw)
  {
    Vout = raw * _voltage / 1023.0;
    R1 = (_resistor * _voltage / Vout) - _resistor;

    if (_resistance_mapping == NULL) // Linear fluid tank
    {
      _levelPercent = R1 / _resistor_tank_max * 100;
    }
    else
    {
      //Handle the tank max separately
      if (R1 >= _resistor_tank_max * 0.99)
      {
        _levelPercent = 100;

#ifdef CALIBRATION
        Serial.println( "R1: " + String(R1) + " level:" + String(_levelPercent));
#endif

      }
      else {
        for (int i = _length - 2; i >= 0; i--)
        {
          if (R1 > _resistance_mapping[i][0]) // The measured resistance R1 is between the mapped resistances on place i and i+1;
          {
            // do linear interpolation between i and i+1
            float xDiff = max(_resistance_mapping[i + 1][0] - _resistance_mapping[i][0], 0.001f); // Avoid divison by zero
            float yDiff = _resistance_mapping[i + 1][1] - _resistance_mapping[i][1];
            float k = yDiff / xDiff;

            _levelPercent = (_resistance_mapping[i][1] + (R1 - _resistance_mapping[i][0]) * k) / _capacity * 100;
            if (_levelPercent > 100) _levelPercent = 100; //Make sure we are not reporting more than 100%
            if (_levelPercent < 0) _levelPercent = 0; //Makre sure we are not reporting less than  0 %

#ifdef CALIBRATION

            Serial.println("Xdiff:" + String(xDiff) + " ydiff:" + String(yDiff) + " i:" + String(i) + "R1:" + String(R1) + " level:" + String(_levelPercent));
#endif
            break;
          }
        }
      }
    }
  }

  return round(_levelPercent);
}

void Tank::setCapacity(float capacity)
{
  _capacity = capacity;
}


float Tank::getCapacity()
{
  return _capacity;
}
