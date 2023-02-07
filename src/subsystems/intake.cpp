#include "main.h"
#include "okapi/impl/device/controllerUtil.hpp" //random ass import, idk what it does but code breaks without it 
// #include "pros/motors.h"
Motor intakeMotor(16, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
void updateIntake(){
    if (controller.getDigital(ControllerDigital::R1) == 1){
        intakeMotor.moveVelocity(600); //intake when positive
    }
    else{
        intakeMotor.moveVelocity(0);// stop if there is no input
    }
}