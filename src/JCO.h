#ifndef JCO_h
#define JCO_h
#include "Arduino.h"
#include "AsyncDelay.h"

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
    bool If(bool value){
      return this->Read()==value;
    }
};

class AnalogPin: public Pin {
  public:
    AnalogPin(int pin):Pin(pin){};
    long Read(){
      return analogRead(this->getPin());
    }
    void Write(int state){
      analogWrite(this->getPin(),state);
    }
    bool If(int value){
      return this->Read()==value;
    }
};

//-------------------------------------
//    Tools
//-------------------------------------

class DigitalActuator: public DigitalPin{
  public:
    DigitalActuator(int pin):DigitalPin(pin){}
    void On(){
      this->Write(true);
    }
    void Off(){
      this->Write(false);
    }
    void Switch(){
      if(this->If(true))this->Off();
      else if(this->If(false))this->On();
    }
};

class AnalogActuator: public AnalogPin{
    private:
        bool _state = false;
  public:
    AnalogActuator(int pin):AnalogPin(pin){}
    void Set(int number){
      this->Write(number);
    }
    bool If(int value){
        return this->_state==value;
    }
    void Switch(int rangeOn, int rangeOff){
      if(this->If(false))this->Set(rangeOn);
      else if(this->If(true))this->Set(rangeOff);
    }
};

//-------------------------------------
//    Actuadores
//-------------------------------------

class Led: public DigitalActuator{
  private:
    AsyncDelay TimerLed;
  public:
    Led(int pin):DigitalActuator(pin){}
    void useTimeBlink(unsigned long time){
      this->TimerLed.start(time, AsyncDelay::MILLIS);
    }
    void blink(){
      if(this->TimerLed.isExpired()){
        this->Switch();
        this->TimerLed.repeat();
      }
    }
};

class LedPWM:public AnalogActuator{
  private:
    AsyncDelay TimerLed;
  public:
    LedPWM(int pin):AnalogActuator(pin){}
    void to(int time, int start = 0,int end = 0){
      if(start==0&&end==0)Serial.println("test");
      else{
        unsigned long TimeDelay;
        TimeDelay = time/end;
        this->TimerLed.start(TimeDelay,AsyncDelay::MILLIS);
        while(start<end){
          if(this->TimerLed.isExpired()){
            this->Set(start);
            start++;
            this->TimerLed.repeat();
          }
        }
        TimeDelay = time/start;
        this->TimerLed.start(TimeDelay,AsyncDelay::MILLIS);
        while(start>end){
          if(this->TimerLed.isExpired()){
            this->Set(start);
            start--;
            this->TimerLed.repeat();
          }
        }
        this->TimerLed.restart();
      }
    }
    void blink(int time, int max = 255){
      this->to(time/2,0,max); 
      this->to(time/2,max,0);
    }
};

class Motor{
  private:
    DigitalActuator pinLeft = new DigitalActuator(0);
    DigitalActuator pinRight = new DigitalActuator(0);
  public:
  Motor(){}
  Motor(int pinLeft,int pinRight){
    this->pinLeft.setPin(pinLeft);
    this->pinRight.setPin(pinRight);
  }
  void use(int pinLeft,int pinRight){
    this->pinLeft.setPin(pinLeft);
    this->pinRight.setPin(pinRight);
  }
  void init(){
    this->pinLeft.Out();
    this->pinRight.Out();
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

//-------------------------------------
//    Debug
//-------------------------------------

class SerialConsole{
  private:
    int _baud = 9600;
  public:
    SerialConsole(){}
    SerialConsole(int baud){
      if(baud==0)this->_baud = 9600;
      else this->_baud = baud;
    }
    void init(){
      Serial.begin(this->_baud);
    }
    void ReadDigital(DigitalActuator obj){
      Serial.println(obj.Read());
    }
    void ReadAnalog(AnalogActuator obj){
      Serial.println(obj.Read());
    }
};


#endif
