#include "constants.h"
#include <unordered_map>
#include <functional>

unsigned int hash(const char* str);
void handle_Off();
void handle_On(const char* command);

std::unordered_map<unsigned int, std::function<void(const char*)>>  commandMap = {
    {hash("ENCENDIDO;NMOS"), handle_On},
    {hash("ENCENDIDO;PMOS"), handle_On},
    {hash("ENCENDIDO;BJTN"), handle_On},
    {hash("ENCENDIDO;NFET"), handle_On},
    {hash("ENCENDIDO;PFET"), handle_On},
    {hash("ENCENDIDO;BJTP"), handle_On},
    {hash("ENCENDIDO;DIODE"), handle_On},
    {hash("APAGADO"), [](const char*){ handle_Off(); }},
};

void setup() {
    Serial.begin(115200);
    semiconductor = nullptr;
    analogWriteResolution(12);
    analogReadResolution(12);
}

void loop() {
    static unsigned long prevMillis = 0;
    static const unsigned long interval = 10; // Intervalo de 1 segundo entre medidas
    
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        handle_command(input);
        input = "";
    }
    
    if (semiconductor != nullptr) {
        unsigned long currentMillis = millis();
        if (currentMillis - prevMillis >= interval) {
            prevMillis = currentMillis;
            Measurement results = semiconductor->characterize();
            
            Serial.print("Voltaje: ");
            Serial.print(results.voltage);
            Serial.print("  |  ");
            Serial.print("Corriente: ");
            Serial.println(results.current);
        }
    }
}

void handle_command(String command) {
    auto it = commandMap.find(hash(command.c_str()));
    if (it != commandMap.end()) {
        it->second(command.c_str());
    } else {
        Serial.println("Comando no reconocido");
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

void handle_Off() {
    if (semiconductor != nullptr) {
        semiconductor->Off();
        delete semiconductor;
        semiconductor = nullptr;
        Serial.println("Sistema Apagado");
    } else {
        Serial.println("No se ha encendido");
    }
}

void handle_On(const char* command) {
    if (semiconductor != nullptr) {
        delete semiconductor;
    }

    if (strcmp(command, "ENCENDIDO;NMOS") == 0) {
        semiconductor = &nmos;
    } else if (strcmp(command, "ENCENDIDO;PMOS") == 0) {
        semiconductor = &pmos;
    } else if (strcmp(command, "ENCENDIDO;BJTN") == 0) {
        semiconductor = &bjtn;
    } else if (strcmp(command, "ENCENDIDO;NFET") == 0) {
        semiconductor = &nfet;
    } else if (strcmp(command, "ENCENDIDO;PFET") == 0) {
        semiconductor = &pfet;
    } else if (strcmp(command, "ENCENDIDO;BJTP") == 0) {
        semiconductor = &bjtp;
    } else if (strcmp(command, "ENCENDIDO;DIODE") == 0) {
        semiconductor = &diode;
    }

    if (semiconductor != nullptr) {
        Serial.println("Encendido");
    }
}
