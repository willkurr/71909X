#include "main.h"

/**
 * This file contains all functions and definitions for the robot's motors. The
 * prototypes for every funtion in this file are declared in motor.hpp, which is
 * included in main.h.
 */

//Motor Definitions
pros::Motor left_back (20, MOTOR_GEARSET_18);
pros::Motor left_mid (18, MOTOR_GEARSET_18);
pros::Motor right_back (19, MOTOR_GEARSET_18, true);
pros::Motor right_mid (17, MOTOR_GEARSET_18, true);

pros::Motor tray(6, MOTOR_GEARSET_36, true);         //only motor to use 36:1
pros::Motor arm(3, MOTOR_GEARSET_36, true);
pros::Motor left_roller(1, MOTOR_GEARSET_18);
pros::Motor right_roller(2, MOTOR_GEARSET_18, true);

//Motor functions


//Set brake mode = hold for all non-chassis motors
void setMotorBrakes() {
  left_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  right_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  tray.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

double getArmPos() {
  return arm.get_position();
}

/**
 * Moves arm given an input
 * @param input - voltage to set arm to
 */
void setArm(int input) {
  arm.move_velocity(input);
}

double getTrayPosition() {
  return tray.get_position();
}

/**
 * Moves tray given an input
 * @param input - value from -127 to 127
 */
void setTray(int input) {
  tray.move_velocity(input);
}

/**
 * Moves rollers given an input
 * @param input - value from (whatever)
 */
void setRollers(int input) {
    left_roller.move_velocity(input);
    right_roller.move_velocity(input);
}

/**
 Stop rollers from braking
*/
void unbrakeRollers() {
  left_roller.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  right_roller.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

//Stop arm from braking (fixes autonomous issue of no arm movement)
void unbrakeArm() {
  arm.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

//Tares the position of all auton-critical motors
void resetAutonMotors() {
  tray.tare_position();
  arm.tare_position();
}
