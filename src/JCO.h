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

class Button{
  private:
    Timer count;
    DigitalActuator pBtn;
  public:
    Button(int pin){
      this->pBtn.usePin(pin);
    }
    init(){
      this->pBtn.initPin();
    }
    start(){
      if(this->click()==true){
        this->count.use(millis());
      }
    }
    startMicros(){
      if(this->click()==true){
        this->count.use(micros());
      }
    }
    bool click(){
      return this->pBtn.Read();
    }
    int time(){
      return this->count.calc();
    }
};

//-------------------------------------
//    Actuadores
//-------------------------------------

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
