#include "main.h"
using namespace okapi;

/**
 * This file contains all functions and definitions for the robot's motors. The
 * prototypes for every funtion in this file are declared in motor.hpp, which is
 * included in main.h.
 */

//PID Globals
const double turnKp = 5.5;
const double turnKi = 0;//0.03
const double turnKd = 215;

//Motor Definitions
Motor lBack (20);
Motor lFront (18);
Motor rBack (-19);
Motor rFront (-17);

pros::Motor tray(6, MOTOR_GEARSET_18, true);
pros::Motor arm(3, MOTOR_GEARSET_36, true);
pros::Motor left_roller(4, MOTOR_GEARSET_18);
pros::Motor right_roller(2, MOTOR_GEARSET_18, true);


//Inertial, defined as extern so it can be used in main too
pros::Imu imuSensor(8);

/**
 * Blocking function that drives a distance in cm with a velocity
 * @param cmToDrive - centimeters to drive, (+) for forward, (-) for backwards
 * @param velocity - a veloctiy that the movement will not exceed
 * @return true when the movement finished
 */
bool driveDistance(int cmToDrive, int velocity) {
  AbstractMotor::brakeMode prevBrake = lBack.getBrakeMode();
  AbstractMotor::encoderUnits prevEncode = lBack.getEncoderUnits();

  lBack.setBrakeMode(AbstractMotor::brakeMode::brake);
  lFront.setBrakeMode(AbstractMotor::brakeMode::brake);
  rBack.setBrakeMode(AbstractMotor::brakeMode::brake);
  rFront.setBrakeMode(AbstractMotor::brakeMode::brake);

  lBack.setEncoderUnits(AbstractMotor::encoderUnits::rotations);
  lFront.setEncoderUnits(AbstractMotor::encoderUnits::rotations);
  rBack.setEncoderUnits(AbstractMotor::encoderUnits::rotations);
  rFront.setEncoderUnits(AbstractMotor::encoderUnits::rotations);

  double revsToTravel = (cmToDrive / 2.54)/(4 * 3.14159);

  lBack.moveRelative(revsToTravel, velocity);
  lFront.moveRelative(revsToTravel, velocity);
  rBack.moveRelative(revsToTravel, velocity);
  rFront.moveRelative(revsToTravel, velocity);

  if (cmToDrive > 0)
    while (lBack.getPositionError() > 1/360.0)
      pros::delay(20);
  else
    while (lBack.getPositionError() < -1/360.0)
      pros::delay(20);

  lBack.setBrakeMode(prevBrake);
  lFront.setBrakeMode(prevBrake);
  rBack.setBrakeMode(prevBrake);
  rFront.setBrakeMode(prevBrake);

  lBack.setEncoderUnits(prevEncode);
  lFront.setEncoderUnits(prevEncode);
  rBack.setEncoderUnits(prevEncode);
  rFront.setEncoderUnits(prevEncode);

  return true;
}

bool turnToZero() {
  double currentPos = imuSensor.get_rotation();
  if (currentPos == 0) return false;
  if (currentPos > 0)
    turn(-currentPos,127,false);
  else
    turn(currentPos,127,false);
  return true;
}

/**
 * Blocking function that turns an angle with a voltage cap
 * toTurn - the angle to turn in degrees
 * speedCap - the nonzero max allowable voltage for the turn from 1 to 127
 */
bool turn(double toTurn, int speedCap, bool mirrorTurns) {
  toTurn = mirrorTurns ? -toTurn : toTurn;
  if (speedCap < 0) speedCap = -speedCap;
  if (speedCap > 127) speedCap = 127;

  if (toTurn == imuSensor.get_rotation()) return true;

  bool isSettled = false;
  double target = toTurn + imuSensor.get_rotation();
  double pos, output;
  int currentTime, prevTime;
  int curTimeI, prevTimeI;
  double elapsedTime;
  double elapTimeI;
  double error, prevError;
  double cumError, rateError;
  bool wasNoError = false;
  bool useIntegral = false;
  okapi::Timer timer;

  while (!isSettled) {
    //calculate
    currentTime = pros::millis();
    elapsedTime = (double)(currentTime - prevTime);

    error = target - imuSensor.get_rotation();
    rateError = (error - prevError) / elapsedTime;

    //antiwindup for integral
    if (useIntegral) {
      //integral needs its own time because integral does not start when normal elapsed time count starts
      curTimeI = pros::millis();
      elapTimeI = (double)(curTimeI - prevTimeI);
      cumError += error * elapsedTime;
      //calculate with integral
      output = turnKp*error + turnKi*cumError + turnKd*rateError;
      //the integral's previous time
      prevTimeI = curTimeI;
    }
    else
      output = turnKp*error + turnKd*rateError;

    if (output > speedCap) output = speedCap;
    if (output < -speedCap) output = -speedCap;

    lBack.moveVoltage(map(output,-127,127,-12000,12000));
    lFront.moveVoltage(map(output,-127,127,-12000,12000));
    rBack.moveVoltage(map(-output,-127,127,-12000,12000));
    rFront.moveVoltage(map(-output,-127,127,-12000,12000));

    prevError = error;
    prevTime = currentTime;
    if (abs(output) < 2) useIntegral = true;
    pros::delay(5);
    std::cout<<output<<std::endl;
    if (-15.0 < output && output < 15) {
      if (!wasNoError) {
        wasNoError = true;
        timer.placeMark();
      }
      else {
        if (timer.getDtFromMark() >= 100_ms)
          isSettled = true;
      }
    }
    else wasNoError = false;

  }
  //absolutely make sure the motors aren't moving before continuing
  while (!(lBack.isStopped() | lFront.isStopped() | rBack.isStopped() | rFront.isStopped()))
    pros::delay(20);

  return true;
}

//Motor functions
//Set brake mode = hold for all non-chassis motors
void setMotorBrakes() {
  left_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  right_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  tray.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void brakeRollers(bool brake) {
  if (brake) {
    left_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    right_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }
  else {
    left_roller.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    right_roller.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  }
}


void tank(int left, int right) {
  lBack.moveVoltage(map(left,-127,127,-12000,12000));
  lFront.moveVoltage(map(left,-127,127,-12000,12000));
  rBack.moveVoltage(map(right,-127,127,-12000,12000));
  rFront.moveVoltage(map(right,-127,127,-12000,12000));
}

//Gets the position of the arm in its encoder units
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

/**
 * Moves arm to an absolute position in its encoder units with a velocity, this
 * function is blocking so it will work with other motor commands
 * @param input - absolute position to move to in the motor's encoder units
 * @param rpm - the velocity in rpm to move at
 */
bool moveArmTo(int input, int rpm) {
  arm.move_absolute(input,rpm);
  while (!arm.is_stopped())
    pros::delay(50);
  return true;
}

//Gets the position of the tray
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

/**
 * Linearly maps an input value in a given range to a different range
 * @param x - the input number
 * @param inMin - lowest possible bound for x
 * @param inMax - highest possible bound for x
 * @param outMin - lowest bound of range to map to
 * @param outMax - highest bound of range to map to
 * @return the mapped value
 */
int map(int x, int inMin, int inMax, int outMin, int outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
