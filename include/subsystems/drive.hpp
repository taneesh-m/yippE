#include "main.h"
using namespace okapi;
extern Motor rightFront;
extern Motor rightMiddle;
extern Motor rightBack;
extern Motor leftFront;
extern Motor leftMiddle;
extern Motor leftBack;
extern MotorGroup leftDrive;
extern MotorGroup rightDrive;
extern std::shared_ptr<OdomChassisController> drive;
extern Controller controller;
extern int state;
void updateDrive();
