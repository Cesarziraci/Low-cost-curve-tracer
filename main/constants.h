#ifndef Constants
#define Constants

#include "Semiconductor.h"

#define STATE_PIN 1
#define SWITCH_PIN 2
#define Analog_pin A0
#define V A1
#define A A2

enum State {
  IDLE,
  CHARACTERIZING,
  TURNING_OFF

};

State currentState = IDLE;
Measurement result;
Semiconductor* semiconductor = nullptr;
NMOS nmos(STATE_PIN,SWITCH_PIN,Analog_pin,V,A);
PMOS pmos(STATE_PIN,SWITCH_PIN,Analog_pin,V,A);
BJTN bjtn(STATE_PIN,SWITCH_PIN,Analog_pin,V,A);
BJTP bjtp(STATE_PIN,SWITCH_PIN,Analog_pin,V,A);
NFET nfet(STATE_PIN,SWITCH_PIN,Analog_pin,V,A);
PFET pfet(STATE_PIN,SWITCH_PIN,Analog_pin,V,A);
DIODE diode(STATE_PIN,SWITCH_PIN,Analog_pin,V,A);

#endif