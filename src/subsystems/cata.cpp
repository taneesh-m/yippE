#include "main.h"
#include "cata.hpp"

Motor cata(10, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
ADIButton limSwitch('D', false);
void launch(){
    cata.moveVoltage(12000);
    while(limSwitch.isPressed()){
        pros::delay(1);
    }
    while(!limSwitch.isPressed()){
        pros::delay(1);
    }
    cata.moveVoltage(0);
}
void updateCata(){
    if(controller.getDigital(ControllerDigital::R2)==1){
        launch();
    }
}