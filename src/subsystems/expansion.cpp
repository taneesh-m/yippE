#include "main.h"
using namespace okapi;
bool Toggle=false;
bool Lock=true;
pros::ADIDigitalOut piston ('B');

void updateExpansion(){
    if(controller.getDigital(ControllerDigital::up) == 1){  
          if(Lock){
          Toggle=!Toggle;
          Lock=false;
          piston.set_value(Toggle);    
        }
    }
        if(controller.getDigital(ControllerDigital::up) == 0){
          Lock=true;
        }

}