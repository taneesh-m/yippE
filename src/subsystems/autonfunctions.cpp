#include "main.h"
#include "intake.hpp"
#include <utility>
okapi::IMU  inertial = IMU(16);
double initAngle=0;
// double targetF;
double prevPos=0;
void roller(){
    leftDrive.moveVelocity(-25);//set the drivetrain to move back at 25rpm
    rightDrive.moveVelocity(-25);//we do 25 rpm to reduce the torque needed to do the roller.
    pros::delay(400);
    // intakeMotor.moveVelocity(600); //move the roller at max speed
    // pros::delay(200); //wait half a second to allow roller to spin to our color 
    // leftDrive.moveVelocity(0);
    // rightDrive.moveVelocity(0);
    // intakeMotor.moveVelocity(0); //cut everyting besides flywheel

    //new 

  //  prevPos= intakeMotor.getPosition();
   intakeMotor.moveRelative( 606, 600);
   pros::delay(300);
//    while(intakeMotor.getPosition()<prevPos+360){
//     intakeMotor.moveVelocity(600);
//    }
//    intakeMotor.moveVelocity(0);
}
void driveForward(double distance) {  
    okapi::IterativePosPIDController drivePID = okapi::IterativeControllerFactory::posPID(0.9, 0., 0.0093); //create a new drive object with specified pid

    const double target = distance; //idk why not just use distance

    drivePID.setTarget(target); //tels PROS: to move the drive using pid to distance

    double orgPosX = drive->getState().x.convert(okapi::foot); //store the orginal position
    double orgPosY = drive->getState().y.convert(okapi::foot);

    double distTravelled = 0;

    while (abs(target-distTravelled) >= 0.2 || abs(leftDrive.getActualVelocity())>10) {//pid shit i think idk
    // condition :abs(target-distTravelled) >= 0.2
        double dx = drive->getState().x.convert(okapi::foot) - orgPosX;
        double dy = drive->getState().y.convert(okapi::foot) - orgPosY;

        distTravelled = sqrt(dx*dx + dy*dy);
        
        double vel = drivePID.step(distTravelled);

        drive -> getModel() -> tank(vel, vel);

        pros::delay(10);
         pros::lcd::set_text(1, std::to_string(drive->getState().x.convert(okapi::foot))); //displays the X coordinate on the LCD of the screen per tick
         pros::lcd::set_text(2, std::to_string(drive->getState().y.convert(okapi::foot))); //displays the X coordinate on the LCD of the screen per tick

    }
}

  


void driveBlorward(double distance, double scalar) {
    okapi::IterativePosPIDController drivePID = okapi::IterativeControllerFactory::posPID(0.75, 0.01, 0.01); //create a new drive object with specified pid

    const double target = distance; //idk why not just use distance

    drivePID.setTarget(target); //tels PROS: to move the drive using pid to distance

    double orgPosX = drive->getState().x.convert(okapi::foot); //store the orginal position
    double orgPosY = drive->getState().y.convert(okapi::foot);

    double distTravelled = 0; 

     while (abs(target-distTravelled) >= 0.2 || abs(leftDrive.getActualVelocity())>10) { //pid shit i think idk  
        double dx = drive->getState().x.convert(okapi::foot) - orgPosX;
        double dy = drive->getState().y.convert(okapi::foot) - orgPosY;

        distTravelled = sqrt(dx*dx + dy*dy);
        
        double vel = drivePID.step(distTravelled);

        drive -> getModel() -> tank(vel * scalar, vel * scalar);

        pros::delay(10);
    }

    drivePID.reset(); //reset everything to move relitive 
    drive -> getModel() -> tank(0, 0); //stop the drive once target is met
}

void driveBackward(double distance, double scalar=1) {    
    okapi::IterativePosPIDController drivePID = okapi::IterativeControllerFactory::posPID(0.75, 0.01, 0.01); //create a new drive object with specified pid

    const double target = distance; //idk why not just use distance

    drivePID.setTarget(target); //tels PROS: to move the drive using pid to distance

    double orgPosX = drive->getState().x.convert(okapi::foot); //store the orginal position
    double orgPosY = drive->getState().y.convert(okapi::foot);

    double distTravelled = 0; 

    while (abs(target-distTravelled) >= 0.2 || abs(leftDrive.getActualVelocity())>10){ //pid shit i think idk
    // while(true){
        double dx = drive->getState().x.convert(okapi::foot) - orgPosX;
        double dy = drive->getState().y.convert(okapi::foot) - orgPosY;

        distTravelled = -sqrt(pow(dx,2) + pow(dy,2));
     
        double vel = drivePID.step(distTravelled);

        drive -> getModel() -> tank(vel * scalar, vel * scalar);

        pros::delay(10);
    }

    drivePID.reset(); //reset everything to move relitive 
    drive -> getModel() -> tank(0, 0); //stop the drive once target is met
}




void turnToAngle(double targetAngle){ //turn non-relitive to given target (degrees)
   // angle in degrees

    okapi::IterativePosPIDController rotatePID = okapi::IterativeControllerFactory::posPID(0.056, 0., 0.000906);
    //ani: 0.4553769998, 0.001, 0.01049997
    
    rotatePID.setTarget(targetAngle);

    // double initAngle = drive->getState().theta.convert(okapi::degree);
    double initAngle = inertial.controllerGet(); 

    while (abs(targetAngle - initAngle) >= 3 || abs(leftDrive.getActualVelocity())>10) {
        //condition: abs(targetAngle - initAngle) >= 3 || abs(leftDrive.getActualVelocity())>300
        // initAngle = drive->getState().theta.convert(okapi::degree);
        initAngle = inertial.controllerGet(); 
        double vel = rotatePID.step(initAngle);
        drive -> getModel() -> tank(vel, -vel);//turn faster by moving the other side the oppisite way
        pros::delay(20);
    }
    
    rotatePID.reset();


    drive -> getModel() -> tank(0, 0);

}

void driveToPoint(double posY, double posX,bool backward,double speed){
  double ogXPos=drive->getState().y.convert(okapi::foot); //get starting X position
  double ogYPos=drive->getState().x.convert(okapi::foot);//get starting Y position
  double distance = sqrt(pow((posX-ogXPos),2)+ pow((posY-ogYPos),2)); //calculate distance using distnace formula 
  double targetAngle = 0;

  
  if(backward==false){ //If driving shooter foward 
      if((posX-ogXPos)>=0 ){ //right
    targetAngle=((atan((posX-ogXPos)/(posY-ogYPos))*(180/3.14159)-90)*-1);  //invert and make it from 0 180
  }
  if(posX-ogXPos<0){ //left
    targetAngle=((atan((posX-ogXPos)/(posY-ogYPos))*(180/3.14159)+90)*-1); //invert and make it from 0 to -180 
  }
  turnToAngle(targetAngle);
  driveBlorward(distance,speed);
    }
  if(backward==true){ //If driving intake foward
      if((posX-ogXPos)<=0 ){ //swapped the signs in order to go backwards 
    targetAngle=((atan((posX-ogXPos)/(posY-ogYPos))*(180/3.14159)-90)*-1);  //invert and make it from 0 180
  }
  if(posX-ogXPos>0){  //swapped the signs in order to go backwards 
    targetAngle=((atan((posX-ogXPos)/(posY-ogYPos))*(180/3.14159)+90)*-1); //invert and make it from 0 to -180 
  }
    turnToAngle((targetAngle));
    driveBackward(-distance,speed);
  }
}

void shootAtPoint(double targetX,double targetY){
  //shoot at point tells the bot to trigger cata once it reaches the specified point, this is usefull for moving and shooting at the same time while also using PID and Odom
  //this function will not move the bot at all, and should be used before driveToPoint.

 double ogXPos=drive->getState().y.convert(okapi::foot); //get starting X position
 double ogYPos=drive->getState().x.convert(okapi::foot);//get starting Y position   /swap the X and Y to make it match the cartesian plane.


}
