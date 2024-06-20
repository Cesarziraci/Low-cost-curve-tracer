#ifndef SEMICONDUCTOR_H
#define SEMICONDUCTOR_H

#include <Arduino.h>

struct Measurement {
    float baseCurrent;
    float current;
    float voltage;
};

class Semiconductor {
public:
    Semiconductor(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    virtual ~Semiconductor() = default;
    virtual Measurement characterize() = 0;
    void On();
    void Off();
    void set_positive_ramp();
    void set_negative_ramp();

protected:
    int _powerPin;
    int _switchPin;
    int _basePin;
    int _voltagePin;
    int _currentPin;
    Measurement results;
    unsigned long prev_millis;
    unsigned long interval;
    const int list_pos[6] = {1861, 2234, 2482, 3103, 3723, 4096};
    const int list_neg[6] = {1861, 1489, 1241, 992, 620, 0};
    float Measure_R;
    float Base_R;
};

class NMOS : public Semiconductor {
public:
    NMOS(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize() override;
};

class PMOS : public Semiconductor {
public:
    PMOS(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize() override;
};

class DIODE : public Semiconductor {
public:
    DIODE(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize() override;
};

class BJTN : public Semiconductor {
public:
    BJTN(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize() override;
};

class BJTP : public Semiconductor {
public:
    BJTP(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize() override;
};

class NFET : public Semiconductor {
public:
    NFET(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize() override;
};

class PFET : public Semiconductor {
public:
    PFET(int powerPin, int switchPin, int basePin, int voltagePin, int currentPin);
    Measurement characterize() override;
};

#endif // SEMICONDUCTOR_H
