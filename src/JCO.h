#ifndef JCO_h
#define JCO_h

#include "Arduino.h"



//-------------------------------------
//    Basic Class
//-------------------------------------
class Pin{
  private:
    int _pin = 0;
  public:
    Pin(int pin){
      this->_pin = pin;
    }
    Pin(void){}
    int getPin(){
      return this->_pin;
    }
    void setPin(int pin){
      this->_pin = pin;
    }
    void Out(){
      pinMode(this->_pin,OUTPUT);
    }
    void In(){
      pinMode(this->_pin,INPUT);
    }
};

class DigitalPin: public Pin {
  public:
    DigitalPin():Pin(){}
    DigitalPin(int pin):Pin(pin){};
    bool Read(){
      return digitalRead(this->getPin());
    }
    void Write(bool state){
      digitalWrite(this->getPin(),state);
    }
};

class AnalogPin: public Pin {
  public:
    AnalogPin():Pin(){}
    AnalogPin(int pin):Pin(pin){};
    long Read(){
      return analogRead(this->getPin());
    }
    void Write(int state){
      analogWrite(this->getPin(),state);
    }
};

//-------------------------------------
//    Tools
//-------------------------------------
class Counter{
  private:
    unsigned long _state = 0;
  public:
    Counter(){}
    void add(){
      this->_state++;
    }
    void back(){
      this->_state--;
    }
    void clear(){
      this->_state=0;  
    }
    unsigned long Read(){
      return this->_state;
    }
};

class Timer{
  private:
    unsigned long start = 0;
  public:
    Timer(){}
    Timer(unsigned long start){
      this->start = start;
    }
    void use(unsigned long start){
      if(this->start!=start){
        this->start = start;
      }
    }
    unsigned long calc(){
      return millis()-(this->start);
    }
    unsigned long calcMicros(){
      return micros()-(this->start);
    }
};

class DigitalActuator{
  private:
    DigitalPin dpin;
  public:
    DigitalActuator(){}
    DigitalActuator(int pin){
      this->dpin.setPin(pin);
    }
    void usePin(int pin){
      this->dpin.setPin(pin);
    }
    void initPin(){
      this->dpin.Out();
    }
    void On(){
      this->dpin.Write(true);
    }
    void Off(){
      this->dpin.Write(false);
    }
    void Switch(){
      if(this->Read()==true)this->Off();
      else if(this->Read()==false)this->On();
    }
    bool Read(){
      return this->dpin.Read();
    }
    void Write(bool state){
      this->dpin.Write(state);
    }
};

class AnalogActuator{
    private:
        AnalogPin apin;
  public:
    AnalogActuator(){}
    AnalogActuator(int pin){
      this->apin.setPin(pin);
    }
    void usePin(int pin){
      this->apin.setPin(pin);
    }
    void initPin(){
      this->apin.Out();
    }
    void Set(int number){
      this->apin.Write(number);
    }
    bool Read(){
      return this->apin.Read();
    }
};


//-------------------------------------
//    Actuadores
//-------------------------------------

class Led{
  private:
    DigitalActuator _pinD;
    AnalogActuator _pinA;
  public:
    Led(int pin){
      _pinA.usePin(pin);
      _pinD.usePin(pin);
    }
    void start(){
      _pinA.initPin();
      _pinD.initPin();
    }
    void On(){
      _pinD.On();
    }
    void Off(){
      _pinD.Off();
    }
    void Use(int value){
      _pinA.Set(value);
    }
};

class Relay{
  private:
    bool _invert = false;
    DigitalActuator _pin;
  public:
    Relay(){}
    Relay(int pin){this->_pin.usePin(pin);}
    void invert(){this->_invert = true;}
    void init(){
      this->_pin.initPin();
    }
    void On(){
      if(this->_invert==true)this->_pin.Off();
      else this->_pin.On();
    }
    void Off(){
      if(this->_invert==true)this->_pin.On();
      else this->_pin.Off();
    }
    void useState(bool state){
      if(state==true)this->_pin.On();
      else this->_pin.Off();
    }
    bool state(){
      return this->_pin.Read();
    }
};

class Motor{
  private:
    DigitalActuator pinLeft;
    DigitalActuator pinRight;
  public:
  Motor(){}
  Motor(int pinLeft,int pinRight){
    this->pinLeft.usePin(pinLeft);
    this->pinRight.usePin(pinRight);
  }
  void use(int pinLeft,int pinRight){
    this->pinLeft.usePin(pinLeft);
    this->pinRight.usePin(pinRight);
  }
  void init(){
    this->pinLeft.initPin();
    this->pinRight.initPin();
  }
  void left(){
    this->pinLeft.On();
    this->pinRight.Off();
  }
  void right(){
    this->pinLeft.Off();
    this->pinRight.On();
  }
  void stop(){
    this->pinLeft.Off();
    this->pinRight.Off();
  }
};

class Move{
  private:
    Motor MA;
    Motor MB;
  public:
    Move(Motor ML,Motor MR){
      this->MA = ML;
      this->MB = MR;
    }
    void init(){
      this->MA.init();
      this->MB.init();
    }
    void Run(){
      this->MA.left();
      this->MB.left();
    }
    void Back(){
      this->MA.right();
      this->MB.right();
    }
    void Stop(){
      this->MA.stop();
      this->MB.stop();
    }
    void Left(){
      this->MA.left();
      this->MB.right();
    }
    void Right(){
      this->MA.right();
      this->MB.left();
    }
};

//-------------------------------------
// Sensors
//-------------------------------------


class Button{
  private:
    Timer count;
    DigitalPin pBtn;
    bool block = false;
  public:
    Button(int pin){
      this->pBtn.setPin(pin);
    }
    init(){
      this->pBtn.In();
    }
    void start(){
      if(this->pBtn.Read()){
        if(this->block==false){
          this->count.use(millis());
          this->block = true;
        }
      }else this->block = false;
    }
    void startMicros(){
      if(this->click()){
        if(this->block==false){
          this->count.use(micros());
          this->block = true;
        }
      }else this->block = false;
    }
    bool click(){
      return this->time()>25;
    }
    int time(){
      return this->count.calc();
    }
};

class UltrasonidoSensor{
  private:
    const float VelocidadSonido = 34000.0;
    DigitalPin TriggerPin;
    DigitalPin EchoPin;
    Timer time;
  public:
    UltrasonidoSensor(int Tpin,int Epin){
      this->TriggerPin.setPin(Tpin);
      this->EchoPin.setPin(Epin);
    }
    void init(){
      this->TriggerPin.Out();
      this->EchoPin.In();
    }
    void SendPulse(){
      this->TriggerPin.Write(false);
      delayMicroseconds(2);
      this->TriggerPin.Write(true);
      delayMicroseconds(10);
      this->TriggerPin.Write(false);
    }
    unsigned long StatePulse(){
     return pulseIn(this->EchoPin.getPin(), HIGH);
    }
    float cm(){
      return this->StatePulse() * 0.000001 * this->VelocidadSonido / 2.0;
    }
};

class LineSensor{
  private:
    DigitalPin PinS;
  public:
    LineSensor(int pin){
      this->PinS.setPin(pin);
    }
    void init(){
      this->PinS.In();
    }
    bool Value(){
      return this->PinS.Read();
    }
};

class brightnessLevelSensor{
  private:
    AnalogPin _pin;
  public:
    brightnessLevelSensor(int pin){
      this->_pin.setPin(pin);
    }
    void init(){
      this->_pin.In();
    }
    unsigned long Read(){
      return this->_pin.Read();
    }
};
//-------------------------------------
//    Debug
//-------------------------------------


#endif
