#include <JCO.h>

Motor ML(12,11);
Motor MR(10,9);
Move RobotMotors(ML,MR);

void setup(){
    RobotMotors.init();
}

void loop(){
    RobotMotors.run();
    delay(1000);
    RobotMotors.left();
    delay(1000);
    RobotMotors.right();
    delay(1000);
    RobotMotors.stop();
    delay(1000);
}