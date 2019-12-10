// This file contains prototypes for the functions in motor.cpp

#ifndef MOTOR_HPP
#define MOTOR_HPP

void setMotorBrakes();
void setArm(int input);
double getArmPos();
void setTray(int input);
void setRollers(int input);
void unbrakeRollers();
void unbrakeArm();
void resetAutonMotors();

double getTrayPosition();

#endif
