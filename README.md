# TankSensor
Arduino project for Teensy to publish Fluid tank levels on NMEA 2000 network

#How it works
The project is suppport resistive tank sensors e.g. 0-190 Ohm. The tanks could ether be of linear shape where dVolume/dResistance is constant, or nonlinear, where calibration points instead are used. 


#How to configure

For linear tanks: 
Use the constructor: Tank waterTank(20, N2kft_Water, 0, 100, 3.31, 217, 186.62)

For non linear tanks:
Define calibration points: e.g. float resistance_mapping [][2] = {{0, 0}, {10, 5}, {50, 10}, {100, 20}, {120, 50}, {150, 90}, {186, 100}};
Use the constructor: Tank waterTank(20, N2kft_Water, 0, 100, 3.31, 217, 186.62, resistance_mapping, sizeof(resistance_mapping) / sizeof(float) / 2);
