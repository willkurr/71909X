// This file contains prototypes for the functions in motor.cpp

#ifndef MOTOR_HPP
#define MOTOR_HPP

//globals
extern pros::Imu imuSensor;

bool turn(double toTurn, int speedCap, bool mirrorTurns);
bool turnToZero();
bool driveDistance(int cmToDrive, int velocity);

void setMotorBrakes();
void brakeRollers(bool brake);
void tank(int left, int right);
void setArm(int input);
bool moveArmTo(int input, int rpm);
double getArmPos();
void setTray(int input);
void setRollers(int input);
void unbrakeRollers();
void unbrakeArm();
void resetAutonMotors();
double getTrayPosition();
int map(int x, int inMin, int inMax, int outMin, int outMax);

#endif
