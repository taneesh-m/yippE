#include "main.h"
//#include "drive.hpp"  
#include "okapi/api/odometry/point.hpp"
using namespace okapi;
int state=1; //state stores the brake mode of the drive where 1 is coast(default) and 2 is held 
Motor rightFront(7, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);// motor for the front(drive)
Motor rightMiddle(12, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);// the right motor on he top, back (drive)
Motor rightBack(10, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);// right motor on the bottom,back (drive)

Motor leftFront(1, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);//motor for the front (drive)
Motor leftMiddle(18, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);//the right motor on he top, back (drive)
Motor leftBack(5, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);// left motor on the bottom,back (drive)

MotorGroup leftDrive ({leftFront,leftMiddle,leftBack}); //define the left side of the drive as a motor group to allow cleaner code
MotorGroup rightDrive({rightFront,rightMiddle,rightBack});//define the left side of the drive as a motor group to allow cleaner code

//imu 7
// MotorGroup left(leftFront,leftTop,leftBottom);
  std::shared_ptr<OdomChassisController> drive =ChassisControllerBuilder()
    .withMotors(leftDrive, rightDrive) //defines the left and right side of the drive
    .withDimensions(  AbstractMotor::gearset::blue, {{3.25_in, 15_in}, okapi::imev5BlueTPR*  5./3.})
    .withOdometry() //specifies the tracking wheels dimentions
    .buildOdometry();
    //this creates a drive as an object with PID and the default odometry functions, most of the default odom functions suck tho so we make our own in odom files


  void updateDrive(){
      pros::lcd::set_text(1, std::to_string(drive->getState().x.convert(okapi::foot))); //displays the X coordinate on the LCD of the screen per tick
      pros::lcd::set_text(2, std::to_string(drive->getState().y.convert(okapi::foot))); //displays the X coordinate on the LCD of the screen per tick

    // drive -> getModel() -> tank(-translate1.power, -translate1.power);
    if(controller.getAnalog(ControllerAnalog::leftX)==0){
     drive -> getModel() -> arcade(0.5*controller.getAnalog(ControllerAnalog::rightY), 0.5 * controller.getAnalog((ControllerAnalog::rightX)));
    }
    
    if(controller.getAnalog(ControllerAnalog::rightX)==0){
    drive -> getModel() -> arcade(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog((ControllerAnalog::leftX)));
    }
    if (controller.getDigital(ControllerDigital::X) == 1){
      leftDrive.setBrakeMode(AbstractMotor::brakeMode::hold);
      rightDrive.setBrakeMode(AbstractMotor::brakeMode::hold);
      state=2;
    }
    else if (controller.getDigital(ControllerDigital::Y) == 1){
      leftDrive.setBrakeMode(AbstractMotor::brakeMode::coast);
      rightDrive.setBrakeMode(AbstractMotor::brakeMode::coast);
      state=1;
    }
 
  }
