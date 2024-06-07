#ifndef Sem
#define Sem
#include "Semiconductor.h"

Semiconductor::Semiconductor(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin){
  _powerPin = powerPin;
  _switchPin = switchPin;
  _basePin = basePin;
  _voltagePin = voltagePin;
  _currentPin = currentPin;

  list_pos[0] = 1861;
  list_pos[1] = 2234;
  list_pos[2] = 2482;
  list_pos[3] = 3103;
  list_pos[4] = 3723;
  list_pos[5] = 4096;

  list_neg[5] = 0;
  list_neg[4] = 620;
  list_neg[3] = 992;
  list_neg[2] = 1241;
  list_neg[1] = 1489;
  list_neg[0] = 1861;

  prev_millis = 0;
  interval = 100;
  pinMode(_powerPin, OUTPUT);
  pinMode(_switchPin, OUTPUT);
  pinMode(_basePin, OUTPUT);
  pinMode(_voltagePin, INPUT);
  pinMode(_currentPin, INPUT);
}

void Semiconductor::On(){
    digitalWrite(_powerPin, HIGH);
}

void Semiconductor::Off(){
    digitalWrite(_powerPin, LOW);
}

void Semiconductor::set_positive_ramp() {
  digitalWrite(_switchPin, LOW);
}

void Semiconductor::set_negative_ramp() {
  digitalWrite(_switchPin, HIGH);
}

NMOS::NMOS(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin) : Semiconductor(powerPin, switchPin, basePin, voltagePin, currentPin) {}

Measurement NMOS::characterize(){
  set_positive_ramp();

  for(int i =0; i<=5; i++){ 
    Off();
    analogWrite(_basePin, list_pos[i]);
    float base_voltage = (((((float)list_pos[i]*3.3 )/4096) - 1.5) *5);
    results.baseCurrent = base_voltage / Base_R;
    On();
    results.current = 0;

    unsigned long currentMillis = millis();

    if(currentMillis = prev_millis >= interval){
      results.voltage = analogRead(_voltagePin);
      for(int j =0; j<= 20; j++){
        results.current += analogRead(_currentPin);
      }
      results.current = (results.current / Measure_R)/20;
      prev_millis = currentMillis;
    }
  }
  return results;
}

PMOS::PMOS(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin) : Semiconductor(powerPin, switchPin, basePin, voltagePin, currentPin) {}

Measurement PMOS::characterize() {
  set_negative_ramp();  
  
  for(int i =0; i<=5; i++){ 
    Off();
    analogWrite(_basePin, list_neg[i]);
    float base_voltage = -1*(((((float)list_neg[i]*3.3 )/4096) - 1.5) *5);
    results.baseCurrent = base_voltage / Base_R;
    On();
    results.current = 0;
    unsigned long currentMillis = millis();

    if(currentMillis = prev_millis >= interval){
      results.voltage = -1*analogRead(_voltagePin);
      for(int j =0; j<= 20; j++){
        results.current += -1*analogRead(_currentPin);
      }
      results.current = (results.current / Measure_R)/20;
      prev_millis = currentMillis;
    }
  }
  return results;
}

DIODE::DIODE(int powerPin, int switchPin,int basePin, int voltagePin, int currentPin) : Semiconductor(powerPin, switchPin,basePin, voltagePin, currentPin) {}

Measurement DIODE::characterize() {
  set_positive_ramp();

  for(int i =0; i<=5; i++){ 
    On();
    results.current = 0;
    unsigned long currentMillis = millis();

    if(currentMillis = prev_millis >= interval){
      results.voltage = analogRead(_voltagePin);
      for(int j =0; j<= 20; j++){
        results.current += analogRead(_currentPin);
      }
      results.current = (results.current / Measure_R)/20;
      prev_millis = currentMillis;
    }
  }
  return results;
}

BJTN::BJTN(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin) : Semiconductor(powerPin, switchPin, basePin, voltagePin, currentPin) {}

Measurement BJTN::characterize() {
  set_positive_ramp();

  for(int i =0; i<=5; i++){ 
    Off();
    analogWrite(_basePin, list_pos[i]);
    float base_voltage = (((((float)list_pos[i]*3.3 )/4096) - 1.5) *5);
    results.baseCurrent = base_voltage / Base_R;
    On();

    unsigned long currentMillis = millis();
    signed int interval = 100;
    if(currentMillis = prev_millis >= interval){
      results.voltage = analogRead(_voltagePin);
      for(int j =0; j<= 20; j++){
        results.current += analogRead(_currentPin);
      }
      results.current = (results.current / Measure_R)/20;
      prev_millis = currentMillis;
    }
  }
  return results;
}


BJTP::BJTP(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin) : Semiconductor(powerPin, switchPin, basePin, voltagePin, currentPin) {}

Measurement BJTP::characterize() {
  set_negative_ramp();
  
  for(int i =0; i<=5; i++){ 
    Off();
    analogWrite(_basePin, list_neg[i]);
    float base_voltage = -1*(((((float)list_neg[i]*3.3 )/4096) - 1.5) *5);
    results.baseCurrent = base_voltage / Base_R;
    On();
    results.current = 0;
    unsigned long currentMillis = millis();

    if(currentMillis = prev_millis >= interval){
      results.voltage = analogRead(_voltagePin);
      for(int j =0; j<= 20; j++){
        results.current += analogRead(_currentPin);
      }
      results.current = (results.current / Measure_R)/20;
      prev_millis = currentMillis;
    }
  }
  return results;
}

NFET::NFET(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin) : Semiconductor(powerPin, switchPin, basePin, voltagePin, currentPin) {}

Measurement NFET::characterize() {
  set_positive_ramp();

  for(int i =0; i<=5; i++){ 
    Off();
    analogWrite(_basePin, list_pos[i]);
    float base_voltage = (((((float)list_pos[i]*3.3 )/4096) - 1.5) *5);
    results.baseCurrent = base_voltage / Base_R;
    On();
    results.voltage = analogRead(_voltagePin);
    results.current = 0;
    unsigned long currentMillis = millis();

    if(currentMillis = prev_millis >= interval){
      results.voltage = analogRead(_voltagePin);
      for(int j =0; j<= 20; j++){
        results.current += analogRead(_currentPin);
      }
      results.current = (results.current / Measure_R)/20;
      prev_millis = currentMillis;
    }
  }
  return results;
}

PFET::PFET(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin) : Semiconductor(powerPin, switchPin, basePin, voltagePin, currentPin) {}

Measurement PFET::characterize() {
  set_negative_ramp();
  
  for(int i =0; i<=5; i++){ 
    Off();
    analogWrite(_basePin, list_neg[i]);
    float base_voltage = -1*(((((float)list_neg[i]*3.3 )/4096) - 1.5) *5);
    results.baseCurrent = base_voltage / Base_R;
    On();
    results.current = 0;
    unsigned long currentMillis = millis();

    if(currentMillis = prev_millis >= interval){
      results.voltage = analogRead(_voltagePin);
      for(int j =0; j<= 20; j++){
        results.current += analogRead(_currentPin);
      }
      results.current = (results.current / Measure_R)/20;
      prev_millis = currentMillis;
    }
  }
  return results;
}

#endif