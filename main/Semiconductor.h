#ifndef Semi
#define Semi

#include "Arduino.h" 

struct Measurement {
  float voltage;
  float current;
  float baseCurrent;
};

class Semiconductor{
  public:
    Semiconductor(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);

    void On();
    void Off();

    void set_positive_ramp();
    void set_negative_ramp();

    virtual Measurement characterize() = 0;

  protected:
    int _powerPin;
    int _switchPin;
    int _basePin;
    int _voltagePin;
    int _currentPin;
    int list_pos[6];
    int list_neg[6];
    const signed int Base_R = 50000;
    const signed int Measure_R = 120;
    unsigned long prev_millis;
    signed int interval;
    Measurement results;
};

class NMOS : public Semiconductor {
  public:
    NMOS(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize();
};

class PMOS : public Semiconductor {
  public:
    PMOS(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize();
};

class DIODE : public Semiconductor {
  public:
    DIODE(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize();
};

class BJTN : public Semiconductor {
  public:
    BJTN(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize();
};

class BJTP : public Semiconductor {
  public:
    BJTP(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize();
};

class NFET : public Semiconductor {
  public:
    NFET(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize();
};

class PFET : public Semiconductor {
  public:
    PFET(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize();
};

#endif