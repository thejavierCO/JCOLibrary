#ifndef JCO_h
#define JCO_h

#include "Arduino.h"

class Pin{
  private:
    uint8_t _pin;
  public:
    Pin(uint8_t pin);
    Pin(void);
    uint8_t getPin();
    void setPin(uint8_t pin);
    void Out();
    void In();
};

class DigitalPin: public Pin {
  public:
    DigitalPin():Pin();
    DigitalPin(uint8_t pin):Pin(pin);
    bool Read();
    void Write(uint8_t state);
};

class AnalogPin: public Pin {
  public:
    AnalogPin():Pin();
    AnalogPin(uint8_t pin):Pin(pin);
    long Read();
    void Write(uint8_t state);
};

class Counter{
  private:
    unsigned long _state;
  public:
    Counter();
    void add();
    void back();
    void clear();
    unsigned long Read();
};

class Timer{
  private:
    unsigned long start;
  public:
    Timer();
    Timer(unsigned long start);
    void use(unsigned long start);
    unsigned long calc();
    unsigned long calcMicros();
};

class DigitalActuator{
  private:
    DigitalPin dpin;
  public:
    DigitalActuator();
    DigitalActuator(uint8_t pin);
    void usePin(uint8_t pin);
    void initPin();
    void On();
    void Off();
    void Switch();
    bool Read();
    void Write(bool state);
};

class AnalogActuator{
    private:
        AnalogPin apin;
  public:
    AnalogActuator();
    AnalogActuator(uint8_t pin);
    void usePin(uint8_t pin);
    void initPin();
    void Set(uint8_t number);
    bool Read();
};

class Relay{
  private:
    bool _invert;
    DigitalActuator _pin;
  public:
    Relay(){}
    Relay(uint8_t pin);
    void invert();
    void init();
    void On();
    void Off();
    void useState(bool state);
    bool state();
};

class Motor{
  private:
    DigitalActuator pinLeft;
    DigitalActuator pinRight;
  public:
  Motor();
  Motor(uint8_t pinLeft,uint8_t pinRight);
  void use(uint8_t pinLeft,uint8_t pinRight);
  void init();
  void left();
  void right();
  void stop();
};

class Move{
  private:
    Motor MA;
    Motor MB;
  public:
    Move(Motor ML,Motor MR);
    void init();
    void Run();
    void Back();
    void Stop();
    void Left();
    void Right();
};

class Button{
  private:
    Timer count;
    DigitalPin pBtn;
    bool block = false;
  public:
    Button(uint8_t pin);
    init();
    void start();
    void startMicros();
    bool click(),
    uint8_t time();
};

class UltrasonidoSensor{
  private:
    const float VelocidadSonido;
    DigitalPin TriggerPin;
    DigitalPin EchoPin;
    Timer time;
  public:
    UltrasonidoSensor(uint8_t Tpin,uint8_t Epin);
    void init();
    void SendPulse();
    unsigned long StatePulse();
    float cm();
};

class LineSensor{
  private:
    DigitalPin PinS;
  public:
    LineSensor(uint8_t pin);
    void init();
    bool Value();
};

class brightnessLevelSensor{
  private:
    AnalogPin _pin;
  public:
    brightnessLevelSensor(uint8_t pin);
    void init();
    unsigned long Read();
};

#endif
