#include "JCO.h"
#include "Arduino.h"

Pin::Pin(uint8_t pin){
  this->_pin = pin;
}
Pin::Pin(void){
  this->_pin = 13;
}
uint8_t Pin::getPin(){
  return this->_pin;
}
void Pin::setPin(uint8_t pin){
  this->_pin = pin;
}
void Pin::Out(){
  pinMode(this->_pin,OUTPUT);
}
void Pin::In(){
  pinMode(this->_pin,INPUT);
}

DigitalPin::DigitalPin():Pin(){}
DigitalPin::DigitalPin(uint8_t pin):Pin(pin){}
bool DigitalPin::Read(){
  return digitalRead(this->getPin());
}
void DigitalPin::Write(uint8_t state){
  digitalWrite(this->getPin(),state);
}

AnalogPin::AnalogPin():Pin();
AnalogPin::AnalogPin(uint8_t pin):Pin(pin);
long AnalogPin::Read(){
  return analogRead(this->getPin());
}
void AnalogPin::Write(uint8_t state){
  analogWrite(this->getPin(),state);
}

Counter::Counter(){
  this->_state=0;  
}
void Counter::add(){
  this->_state++;
}
void Counter::back(){
  this->_state--;
}
void Counter::clear(){
  this->_state=0;  
}
unsigned long Counter::Read(){
  return this->_state;
}

Timer::Timer(){
  this->start = 0;
}
Timer::Timer(unsigned long start){
  this->start = start;
}
void Timer::use(unsigned long start){
  if(this->start!=start){
    this->start = start;
  }
}
unsigned long Timer::calc(){
  return millis()-(this->start);
}
unsigned long Timer::calcMicros(){
  return micros()-(this->start);
}

DigitalActuator::DigitalActuator();
DigitalActuator::DigitalActuator(uint8_t pin);
void DigitalActuator::usePin(uint8_t pin);
void DigitalActuator::initPin();
void DigitalActuator::On();
void DigitalActuator::Off();
void DigitalActuator::Switch();
bool DigitalActuator::Read();
void DigitalActuator::Write(bool state);

AnalogActuator::AnalogActuator();
AnalogActuator::AnalogActuator(uint8_t pin);
void AnalogActuator::usePin(uint8_t pin);
void AnalogActuator::initPin();
void AnalogActuator::Set(uint8_t number);
bool AnalogActuator::Read();

Relay::Relay(){}
Relay::Relay(uint8_t pin);
void Relay::invert();
void Relay::init();
void Relay::On();
void Relay::Off();
void Relay::useState(bool state);
bool Relay::state();

Motor::Motor();
Motor::Motor(uint8_t pinLeft,uint8_t pinRight);
void Motor::use(uint8_t pinLeft,uint8_t pinRight);
void Motor::init();
void Motor::left();
void Motor::right();
void Motor::stop();

Move::Move(Motor ML,Motor MR);
void Move::init();
void Move::Run();
void Move::Back();
void Move::Stop();
void Move::Left();
void Move::Right();

Button::Button(uint8_t pin);
Button::init();
void Button::start();
void Button::startMicros();
bool Button::click(),
uint8_t Button::time();

UltrasonidoSensor::UltrasonidoSensor(uint8_t Tpin,uint8_t Epin);
void UltrasonidoSensor::init();
void UltrasonidoSensor::SendPulse();
unsigned long UltrasonidoSensor::StatePulse();
float UltrasonidoSensor::cm();

LineSensor::LineSensor(uint8_t pin);
void LineSensor::init();
bool LineSensor::Value();

brightnessLevelSensor::brightnessLevelSensor(uint8_t pin);
void brightnessLevelSensor::init();
unsigned long brightnessLevelSensor::Read();