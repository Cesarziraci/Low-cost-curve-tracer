#define V_raw A1
#define A_raw A2
#define Base_current A0 
#define V_ramp A3
#define S1 3
#define S2 2

void READ();
void procesarComando(String comando);
void CicloTransistor(const int polaridad_V_Base);

const int V_ref = 5;
const int ADC_resolution = 4095;  // Resolución ADC 12 bits
const int DAC_resolution = 1023;  // Resolución DAC 10 bits
const int R1= 1000, R2=250000;
float B_fixed = 0; 
unsigned long previousMillis = 0;
const int readInterval = 6;

bool cicloActivo = false;
int polaridadActual = 0; //1 positivo, -1 negativo;
String semiconductorActivo = "";

typedef struct {
  String tipo;
  bool S1State = LOW;
  bool S2State = LOW;
  float B_value;

  void SemiconductorConfig(){
    float V_ramp_value;
    do {
      V_ramp_value = (float)analogRead(V_ramp) * 3 *(V_ref / ADC_resolution);
    } while (V_ramp_value < 0 || V_ramp_value > 2.0); 

    digitalWrite(S1, S1State ? HIGH:LOW);
    digitalWrite(S2, S2State ? HIGH:LOW);

  }

}Semiconductor;

Semiconductor DIODE, NMOS, PMOS, NFET, PFET, NBJT, PBJT;

void setup(){
  Serial.begin(115200);

  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  DIODE.tipo = "DIODE";
  DIODE.S1State = LOW;
  DIODE.S2State = LOW; 
  DIODE.B_value = 0.0;

  NMOS.tipo = "NMOS";
  NMOS.S1State = LOW;
  NMOS.S2State = LOW; 
  NMOS.B_value = 0.0;
  
  PMOS.tipo = "PMOS";
  PMOS.S1State = LOW;
  PMOS.S2State = HIGH; 
  PMOS.B_value = 0.0;
  
  NFET.tipo = "NFET";
  NFET.S1State = LOW;
  NFET.S2State = LOW; 
  NFET.B_value = 0.0;
  
  PFET.tipo = "PFET";
  PFET.S1State = LOW;
  PFET.S2State = HIGH; 
  PFET.B_value = 0.0;
  
  NBJT.tipo = "NBJT";
  NBJT.S1State = LOW;
  NBJT.S2State = LOW; 
  NBJT.B_value = 0.0;
  
  PBJT.tipo = "PBJT";
  PBJT.S1State = LOW;
  PBJT.S2State = HIGH; 
  PBJT.B_value = 0.0;
  
  digitalWrite(S1, LOW);
}

void loop(){
  if(Serial.available() > 0){
    String comando = Serial.readString();
    procesarComando(comando);
  }

  if(cicloActivo){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= readInterval){
      previousMillis = currentMillis;
      CicloTransistor(polaridadActual);
    }
  }
}

void READ(){
  // se multipilica por 3, debido al divisor de tension de factor 3.
  float V_fixed = (float)analogRead(V_raw) * (V_ref * 3)/(ADC_resolution);
  float A_buff = (float)analogRead(A_raw) * (V_ref * 3)/(ADC_resolution);

  // se debbe multiplicar por 2 por el divisor de tension equivalente
  float A_fixed = (A_buff/R1) * 2;

  Serial.print("X= "); Serial.print(V_fixed);
  Serial.print(",Y= "); Serial.print(A_fixed);
  Serial.print(" ,B= "); Serial.print(B_fixed, 6);
  Serial.print('\n');

}

void procesarComando(String comando) {
  Serial.println(comando);
  if (comando == "OFF") {
    DIODE.S1State = LOW;
    NMOS.S1State = LOW;
    PMOS.S1State = LOW;
    NFET.S1State = LOW;
    PFET.S1State = LOW;
    NBJT.S1State = LOW;
    PBJT.S1State = LOW;

    DIODE.SemiconductorConfig();
    NMOS.SemiconductorConfig();
    PMOS.SemiconductorConfig();
    NFET.SemiconductorConfig();
    PFET.SemiconductorConfig();
    NBJT.SemiconductorConfig();
    PBJT.SemiconductorConfig();

    cicloActivo = false;
    semiconductorActivo = "";
    polaridadActual = 0;
    
  }else if (comando.startsWith("ENCENDIDO;")){
    String tipo = comando.substring(10);

    DIODE.S1State = LOW;
    NMOS.S1State = LOW;
    PMOS.S1State = LOW;
    NFET.S1State = LOW;
    PFET.S1State = LOW;
    NBJT.S1State = LOW;
    PBJT.S1State = LOW;

    if (tipo == "DIODE") {
      DIODE.S1State = HIGH;
      DIODE.SemiconductorConfig();

      cicloActivo = true; // No se requiere CicloTransistor para diodo
      semiconductorActivo = "DIODE";
      polaridadActual = 0;
    }else if (tipo == "NMOS") {
      NMOS.S1State = HIGH;
      NMOS.SemiconductorConfig();

      cicloActivo = true;
      semiconductorActivo = "NMOS";
      polaridadActual = 1;
    }else if (tipo == "PMOS") {
      PMOS.S1State = HIGH;
      PMOS.SemiconductorConfig();

      cicloActivo = true;
      semiconductorActivo = "PMOS";
      polaridadActual = -1;

    }else if (tipo == "NFET") {
      NFET.S1State = HIGH;
      NFET.SemiconductorConfig();

      cicloActivo = true;
      semiconductorActivo = "NFET";
      polaridadActual = 1;

    } else if (tipo == "PFET") {
      PFET.S1State = HIGH;
      PFET.SemiconductorConfig();

      cicloActivo = true;
      semiconductorActivo = "PFET";
      polaridadActual = -1;

    } else if (tipo == "BJTN") {
      NBJT.S1State = HIGH;
      NBJT.SemiconductorConfig();

      cicloActivo = true;
      semiconductorActivo = "BJTN";
      polaridadActual = 1;

    } else if (tipo == "BJTP") {
      PBJT.S1State = HIGH;
      PBJT.SemiconductorConfig();

      cicloActivo = true;
      semiconductorActivo = "BJTP";
      polaridadActual = -1;

    }
  }
}

void CicloTransistor(const int polaridad_V_Base){
  static float V_base = 1.5; // Valor inicial de V_base
  static unsigned long lastReadMillis = 0;
  static unsigned long prev= 0;

  if (!cicloActivo) {
    return;
  }
  unsigned long cur = millis();
  if (cur - prev >= readInterval) {
    prev = cur; // Guardar el tiempo actual para la próxima lectura
    READ(); // Leer los valores
  }

  unsigned long cur1 = millis();
  if (cur1 - lastReadMillis >= 120) {
    lastReadMillis = cur1;

    if (polaridad_V_Base == 1) {
      V_base += 0.3; // Aumentar V_base
      if (V_base >= 3.0) {
        V_base = 1.5; 
      }
    } else if (polaridad_V_Base == -1) {
      V_base -= 0.3; // Disminuir V_base
      if (V_base <= 0.0) {
        V_base = 1.5; 
      }
    }
    B_fixed = V_base; //(V_base * 6) / R2; // Corriente de base para el cálculo de Beta
    // Convertir V_base a valor DAC
    int analog_value = (int)((V_base / V_ref) * DAC_resolution);
    analogWrite(Base_current, analog_value);  
  }
}