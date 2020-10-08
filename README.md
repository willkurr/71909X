# 71909X
This code is written for my robot for the 2019-2020 VEX robotics game: Tower Takeover.

This was programmed in C++ using PROS, an IDE running in Atom that allows users to program VEX V5 robot brains.

## Controller Documentation
Please refer to this documentation for the current control scheme.
* Starting a Match
  * Hold Y Button - Enable opcontrol (follow prompt on controller)
* Driving Control
  * Left + Right Stick - tank control
  * B Button - Toggle between 80%(max) and 60% speed (anything over 80 is too fast so it's speed capped)
* Cube Rollers
  * R2 Trigger  - Outtake Cubes slowly
  * R1 Trigger  - Intake Cubes
* Cube Roller Arm
  * L1 Trigger - Move Arm Up
  * L2 Trigger - Move Arm Down
  * Y Button - Move arm to optimal position for cube pickup
* Cube Tray
  * A Button - Toggle tray in/out, also disables brakes on rollers to prevent tray from becoming stuck and moves arm down fully
* Macros
  * Up Arrow - Drive straight and intake cubes
  * Down Arrow - Drive backwards and outtake cubes
  * Right Arrow - High tower scoring position
  * Left Arrow - Low tower scoring position
  * X Button - Just drive straight


## Motor Ports
4 - Left Roller (was 1 but had static kill a port at states)

2 - Right Roller

3 - Arm

6 - Tray

17 - Chassis Right Front

18 - Chassis Left Front

19 - Chassis Right Back

20 - Chassis left back

## Autonomous Selection
6 Color-coded buttons are provided on the screen for autonomous selection. The auton routines are as follows:
* Blue 1 - Start near unprot. zone, pick up preload plus row of 4, then stack.
* Blue 2 - Start near protected zone, pick up preload plus one cube, stack those 2 cubes
* Red 1 - Blue 1 with mirrored turns. One or two of the distances are changed since apparently our school's field is messed up on red
* Red 2- Blue 2 mirrored
* Driver Skills - Start on blue near unprot. zone. Does blue 1, then scores 3 towers (did not work at states due to poor distance PID tuning, but at least it stacked 5 cubes)

## Starting Matches
The robot must be COMPLETELY STILL when the program is turned on! You cannot move the bot until the display does not display "CALIBRATING IMU." This is due to the fact that this year I was able to get my hands on the new VEX IMU, which is being used to make precise turns with PID (actaully just PD since I can't make anti-windup for I). IMPORTANT: The IMU takes 2 seconds to calibrate when the program starts. Thus, driver skills can't be run from the controller because 2 seconds gets wasted. The same is true for programming skills (especially since you have to hit a button on the screen to run it). In addition, DO NOT move the robot after the program is started. Any shift in the angle of the robot will mess up auton.

When starting opcontrol or autonomous, you must make sure that all motors are in their starting positions, or else the robot will be unable to determine the correct positions. This means the arm must be all the way down and the tray must be fully pushed back.

Whenever the program is started, the user can move the motors however they'd like without the robot being messed up. When the user is done setting up the robot, they must hold Y in order to activate opcontrol. This means after autonomous runs, Y needs to be held when the match starts.
