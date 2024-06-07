#include "constants.h"
#include <unordered_map>
#include <functional>

unsigned int hash(const char* str);
void handle_Off();
void handle_On(Semiconductor* device);

std::unordered_map<unsigned int, std::function<void()> > commandMap = {
  {hash("ENCENDIDO;NMOS"), [&](){handle_On(&nmos);}},
  {hash("ENCENDIDO;PMOS"), [&](){handle_On(&pmos);}},
  {hash("ENCENDIDO;BJTN"), [&](){handle_On(&bjtn);}},
  {hash("ENCENDIDO;NFET"), [&](){handle_On(&nfet);}},
  {hash("ENCENDIDO;PFET"), [&](){handle_On(&pfet);}},
  {hash("ENCENDIDO;BJTP"), [&](){handle_On(&bjtp);}},
  {hash("ENCENDIDO;DIODE"), [&](){handle_On(&diode);}},
  {hash("APAGADO"),[](){handle_Off();}},
};

void setup(){
  Serial.begin(115200);
  semiconductor = nullptr;
  analogWriteResolution(12);
  analogReadResolution(12);
}

void loop(){

  switch(currentState){
    case IDLE:
      if (Serial.available()){
          String input = Serial.readStringUntil('\n');
          input.trim();
          unsigned int hashinput = hash(input.c_str());
          auto it = commandMap.find(hashinput);
          if(it != commandMap.end()){
            if(hashinput == hash("APAGADO")){
              currentState = TURNING_OFF;
            }else {
              currentState = CHARACTERIZING;
              it->second();
              };

            }else Serial.println("Comando no reconocido");
          }
      break;
      case CHARACTERIZING:{
        float Base_current = result.baseCurrent;
        float current = result.current;
        float Voltage = result.voltage;

        Serial.print("Voltage: ");
        Serial.println(Voltage); 

        Serial.print("Current: ");
        Serial.println(current); 

        Serial.print("Base Current: ");
        Serial.println(Base_current); 

        if (Serial.available()) {
          String input = Serial.readStringUntil('\n');
          input.trim();
          unsigned int hashinput = hash(input.c_str());
          if (hashinput == hash("APAGADO")) {
            currentState = TURNING_OFF;
          }
        }
        
        }
        break;

      case TURNING_OFF:
        handle_Off();
        currentState = IDLE;
        break;
        }
      }

unsigned int hash(const char* str) {
  unsigned int hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; 
  }
  return hash;
}

void handle_Off(){
  if(semiconductor != nullptr){
    semiconductor ->Off();
    delete semiconductor;
    semiconductor = nullptr;
    Serial.println('Sistema Apagado');

  }else Serial.print('No se ha encendido');
}

void handle_On(Semiconductor* device){
  if(semiconductor != nullptr){
    delete semiconductor;
  }
  semiconductor = device;
  Serial.println("Encendido");
  result = semiconductor ->characterize();
}