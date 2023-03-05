#include "main.h"
#include "cata.hpp"

Motor cata(10, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);

bool toggle=false;
ADIButton limSwitch('C', false);
// void launch(){
//     cata.moveVoltage(12000);
//     while(limSwitch.isPressed()){
//         pros::delay(1);
//     }
//     while(!limSwitch.isPressed()){
//         pros::delay(1);
//     }
//     cata.moveVoltage(0);
// }
void updateCata(){
  
    if(controller.getDigital(ControllerDigital::R2)==1){
       while(limSwitch.isPressed()){
        cata.moveVoltage(12000);
       }
       cata.moveVoltage(0);
     
    }
      if((controller.getDigital(ControllerDigital::R2)==0)){
    if(!limSwitch.isPressed()){
        cata.moveVoltage(12000); //move down unless button is pressed
       }
       if(limSwitch.isPressed()){
        cata.moveVoltage(0);
       }
}
}