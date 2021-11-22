#include <JCO.h>

Led led(13);

void setup(){
    led.Out();
    led.useTimeBlink(1000);
}

void loop(){
    led.blink();
}