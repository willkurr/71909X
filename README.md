# BRMHS1
This code is written for BRMHS' first competition robot for Tower Takeover.

This was programmed in C++ using PROS, an IDE running in Atom that allows users to program VEX V5 robot brains.

## Controller Documentation
Please refer to this documentation for the current control scheme.
* Starting a Match
  * Y Button - Enable opcontrol (follow prompt on controller)
* Driving Control
  * Left + Right stick tank control
  * B Button - Toggle between 80%(max) and 60% speed
* Cube Rollers
  * R2 Trigger  - Outtake Cubes slowly
  * R1 Trigger  - Intake Cubes
* Cube Roller Arm
  * L1 Trigger - Move Arm Up
  * L2 Trigger - Move Arm Down
* Cube Tray
  * A Button - Toggle tray in/out, also disables brakes on rollers to prevent tray from becoming stuck
  * Right Arrow - Fine tune tray forward (should be done before using arm to prevent arm getting stuck on tray)
  * Left Arrow - Fine tune tray back (will not go past 0
* Macros
  * Up Arrow - Drive forward and intake cubes
  * Down Arrow - Drive backwards slowly while out-taking cubes slowly


## Motor Ports
1 - Left Roller

2 - Right Roller

3 - Arm

16 - Tray

17 - Chassis Right Front

18 - Chassis Left Front

19 - Chassis Right Back

20 - Chassis left back

## Autonomous Selection
Currently, 3 buttons are provided on the robot screen to allow you to select an autonomous.

It is better to use these when the robot is in the disabled state (plugged in to competition controller), as the motors will not be holding; therefore you can easily access the screen.

## Starting Matches
IMPORTANT: when starting opcontrol or autonomous, you must make sure that all motors are in their starting positions, or else the robot will be unable to determine the correct positions.
This means the arm must be all the way down and the tray must be fully pushed back.

Whenever opcontrol is started, the user can move the motors however they'd like without the robot being messed up. When the user is done setting up the robot, they must press Y in order to activate opcontrol.

This feature was added to allow the user to access the display when the program begins.
