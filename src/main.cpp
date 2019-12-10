#include "main.h"
using namespace okapi;

//globals
int autonToUse = 1; //run auton 1 by default
bool trayInSetPosition = false;
Controller masterController;    //controller for okapi

//constants
const int SCR_WIDTH = 480;
const int SCR_HEIGHT = 272;
const int TRAY_MAX = 480;
const int TRAY_SCORE_CUBES = 135;
const int ARM_PICKUP_HEIGHT = 60;
const int ARM_PICKUP_AUTON = 45;
const int ARM_LOWSCORE_HEIGHT = 440;
const int ARM_HIGHSCORE_HEIGHT = 585;

//init start
//objects defined here
lv_obj_t * btnDisableAuton;
lv_obj_t * btnBlueAuton1;
lv_obj_t * btnRedAuton1;
lv_obj_t * btnBlueAuton2;
lv_obj_t * btnRedAuton2;
lv_obj_t * btnDriverSkills;
lv_obj_t * autonDescrpt;

//styles defined and created here
lv_style_t redBtnStyle = btnStyleBuilder(LV_COLOR_RED, LV_COLOR_WHITE);
lv_style_t redBtnPressedStyle = btnStyleBuilder(LV_COLOR_MAROON, LV_COLOR_WHITE);
lv_style_t blueBtnStyle =  btnStyleBuilder(LV_COLOR_BLUE, LV_COLOR_WHITE);
lv_style_t blueBtnPressedStyle = btnStyleBuilder(LV_COLOR_NAVY, LV_COLOR_WHITE);
lv_style_t driverBtnStyle = btnStyleBuilder(LV_COLOR_YELLOW, LV_COLOR_WHITE);
lv_style_t driverBtnPressedStyle = btnStyleBuilder(LV_COLOR_GRAY, LV_COLOR_WHITE);
lv_style_t regBtnStyle = btnStyleBuilder(LV_COLOR_SILVER, LV_COLOR_WHITE);
lv_style_t regBtnPressedStyle = btnStyleBuilder(LV_COLOR_GRAY, LV_COLOR_WHITE);

/**
 * Callback for when any button that is supposed to set a different auton. is
 * pressed. This function checks the ID of the pressed button, and sets
 * autonToUse to the appropriate value based on
 * which id the button has.
 * @param  btn - button passed from LVGL
 * @param  selectorVar - reference to autonToUse
 * @return LV_RES_OK if button is not deleted, for debugging purposes (not used)
 */
lv_res_t ifSelectorButtonPressed(lv_obj_t * btn) {
  int id = lv_obj_get_free_num(btn);

  autonToUse = id;

  return LV_RES_OK;
}

void initialize() {
  //draw all buttons
  btnDisableAuton = createBtn(lv_scr_act(), 10, 25, 75, 50, 0, "None");
  btnBlueAuton1 = createBtn(lv_scr_act(), 125, 25, 75, 50, 1, "Blue 1");
  btnRedAuton1 = createBtn(lv_scr_act(), 255, 25, 75, 50, 2, "Red 1");
  btnBlueAuton2 = createBtn(lv_scr_act(), 125, 110, 75, 50, 3, "Blue 2");
  btnRedAuton2 = createBtn(lv_scr_act(), 255, 110, 75, 50, 4, "Red 2");
  btnDriverSkills = createBtn(lv_scr_act(), 10, 110, 75, 50, 5, "Skills");

  //set all button styles
  lv_btn_set_style(btnDisableAuton, LV_BTN_STYLE_REL, &regBtnStyle);
  lv_btn_set_style(btnDisableAuton, LV_BTN_STYLE_PR, &regBtnPressedStyle);
  lv_btn_set_style(btnBlueAuton1, LV_BTN_STYLE_REL, &blueBtnStyle);
  lv_btn_set_style(btnBlueAuton1, LV_BTN_STYLE_PR, &blueBtnPressedStyle);
  lv_btn_set_style(btnRedAuton1, LV_BTN_STYLE_REL, &redBtnStyle);
  lv_btn_set_style(btnRedAuton1, LV_BTN_STYLE_PR, &redBtnPressedStyle);
  lv_btn_set_style(btnBlueAuton2, LV_BTN_STYLE_REL, &blueBtnStyle);
  lv_btn_set_style(btnBlueAuton2, LV_BTN_STYLE_PR, &blueBtnPressedStyle);
  lv_btn_set_style(btnRedAuton2, LV_BTN_STYLE_REL, &redBtnStyle);
  lv_btn_set_style(btnRedAuton2, LV_BTN_STYLE_PR, &redBtnPressedStyle);
  lv_btn_set_style(btnDriverSkills, LV_BTN_STYLE_REL, &driverBtnStyle);
  lv_btn_set_style(btnDriverSkills, LV_BTN_STYLE_PR, &driverBtnPressedStyle);

  //set all button actions
  lv_btn_set_action(btnDisableAuton, LV_BTN_ACTION_CLICK, ifSelectorButtonPressed);
  lv_btn_set_action(btnBlueAuton1, LV_BTN_ACTION_CLICK, ifSelectorButtonPressed);
  lv_btn_set_action(btnRedAuton1, LV_BTN_ACTION_CLICK, ifSelectorButtonPressed);
  lv_btn_set_action(btnBlueAuton2, LV_BTN_ACTION_CLICK, ifSelectorButtonPressed);
  lv_btn_set_action(btnRedAuton2, LV_BTN_ACTION_CLICK, ifSelectorButtonPressed);
  lv_btn_set_action(btnDriverSkills, LV_BTN_ACTION_CLICK, ifSelectorButtonPressed);

  //make a label to tell user what auton is being run
  autonDescrpt = lv_label_create(lv_scr_act(), NULL);
  //making style for text
  static lv_style_t textStyle;
  lv_style_copy(&textStyle, &lv_style_plain);
  textStyle.text.color = LV_COLOR_WHITE;
  //set label text and align
  lv_obj_align(autonDescrpt, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, -50);
  lv_obj_set_width(autonDescrpt, SCR_WIDTH - 100);
  lv_label_set_text(autonDescrpt, "No autonomous will be run");
}



void disabled() {
  //inform the user which auton will be ran if a button is pressed
  while(1) {
  if (autonToUse == 0)
    lv_label_set_text(autonDescrpt, "No autonomous will be run\nRobot is currently disabled");
  else if (autonToUse == 1)
    lv_label_set_text(autonDescrpt, "Blue Auton 1 will be run\nRobot is currently disabled");
  else if (autonToUse == 2)
    lv_label_set_text(autonDescrpt, "Red Auton 1 will be run\nRobot is currently disabled");
  else if (autonToUse == 3)
    lv_label_set_text(autonDescrpt, "Blue Auton 2 will be run\nRobot is currently disabled");
  else if (autonToUse == 4)
    lv_label_set_text(autonDescrpt, "Red Auton 2 will be run\nRobot is currently disabled");
  else if (autonToUse == 5)
    lv_label_set_text(autonDescrpt, "Driver Skills\nRobot is currently disabled");
  pros::delay(250);
  }
}

void competition_initialize() {}


//auton start

//making the controllers, also used in opcontrol
auto chassis = ChassisControllerFactory::create({18,20}, {-17,-19},
                AbstractMotor::gearset::green, {10.6_cm, 23_cm});
auto rollers = AsyncControllerFactory::velIntegrated({1,-2});
auto trayControl = AsyncControllerFactory::posIntegrated(-6);
auto armControl = AsyncControllerFactory::posIntegrated(-3);

//making autonomous functions to allow for multiple possible auton. routines
void auton1(bool mirrorTurns) {
  unbrakeArm(); //do not brake the arm to allow okapi to work
  //blue by default, mirror turns for red if mirrorTurns = true
  chassis.setTurnsMirrored(mirrorTurns);
  chassis.setBrakeMode(AbstractMotor::brakeMode::coast);

  chassis.setMaxVelocity(120);
  chassis.moveDistance(25_cm);
  pros::delay(300);
  //set arm to pickup position
  armControl.setTarget(ARM_PICKUP_AUTON);
  armControl.waitUntilSettled();
  //now turn on rollers, then move forward to pick up cubes
  chassis.setMaxVelocity(50);
  rollers.setTarget(115);  //max speed for green cartridge = 200
  chassis.moveDistance(85_cm);
  chassis.waitUntilSettled();
  //now stop rollers, move back and turn torwards small goal
  rollers.setTarget(0);
  chassis.setMaxVelocity(105);
  chassis.moveDistance(-50_cm);
  chassis.setMaxVelocity(80);
  chassis.turnAngle(-350); //240 = 90 degrees
  chassis.waitUntilSettled();
  //now move slowly torwards goal and move rollers out to set cubes
  chassis.moveDistance(50_cm);
  chassis.waitUntilSettled();
  rollers.setTarget(-50);
  pros::delay(400);
  rollers.setTarget(0);
  //now move the tray forward, and place the cubes
  trayControl.setMaxVelocity(50);
  rollers.setTarget(-10);
  pros::delay(30);
  armControl.setTarget(0);
  armControl.waitUntilSettled();
  trayControl.setTarget(TRAY_MAX);
  trayControl.waitUntilSettled();
  rollers.setTarget(0);
  pros::delay(100);
  chassis.setMaxVelocity(20);
  chassis.moveDistance(5_cm);
  chassis.setMaxVelocity(80);
  //now move away with rollers
  rollers.setTarget(-115);
  chassis.moveDistance(-27_cm);
  rollers.setTarget(0);
}

void auton2(bool mirrorTurns) {
  unbrakeArm(); //do not brake the arm to allow okapi to work
  //blue by default, mirror turns for red if mirrorTurns = true
  chassis.setTurnsMirrored(mirrorTurns);
  chassis.setBrakeMode(AbstractMotor::brakeMode::coast);

  chassis.setMaxVelocity(120);
  chassis.moveDistance(25_cm);
  pros::delay(300);
  //set arm to pickup position
  armControl.setTarget(ARM_PICKUP_AUTON);
  armControl.waitUntilSettled();
  //now turn on rollers, then move forward to pick up cubes
  chassis.setMaxVelocity(50);
  rollers.setTarget(115);  //max speed for green cartridge = 200
  chassis.moveDistance(85_cm);
  chassis.waitUntilSettled();
  //now stop rollers, move back and turn torwards small goal
  rollers.setTarget(0);
  chassis.setMaxVelocity(105);
  chassis.moveDistance(-50_cm);
  chassis.setMaxVelocity(80);
  chassis.turnAngle(-350); //240 = 90 degrees
  chassis.waitUntilSettled();
  //now move slowly torwards goal and move rollers out to set cubes
  chassis.moveDistance(50_cm);
  chassis.waitUntilSettled();
  rollers.setTarget(-50);
  pros::delay(400);
  rollers.setTarget(0);
  //now spit a cube out
  rollers.setTarget(-100);
  pros::delay(500);
  rollers.setTarget(0);
  chassis.moveDistance(-20_cm);
}

void driverSkills() {
  unbrakeArm(); //do not brake the arm to allow okapi to work
  chassis.setMaxVelocity(90);
  //blue by default, mirror turns for red if mirrorTurns = true
  chassis.setBrakeMode(AbstractMotor::brakeMode::coast);

  //push the preload forward then move back
  chassis.moveDistance(20_cm);
  //chassis.moveDistance(-5_cm);
  //now move arm up and down to unleash the tray
  armControl.setTarget(350);
  armControl.waitUntilSettled();
  armControl.setTarget(ARM_PICKUP_AUTON);
  armControl.waitUntilSettled();
  chassis.waitUntilSettled();
  //now turn on rollers, then move forward to pick up cubes
  chassis.setMaxVelocity(60);
  rollers.setTarget(115);  //max speed for green cartridge = 200
  pros::delay(500);
  chassis.moveDistance(82_cm);
  chassis.waitUntilSettled();
  //now stop rollers, move back and turn torwards small goal
  rollers.setTarget(0);
  chassis.setMaxVelocity(90);
  chassis.moveDistance(-35_cm);
  chassis.setMaxVelocity(80);
  chassis.turnAngle(-350);
  chassis.waitUntilSettled();
  //now move slowly torwards goal and outtake cubes to lower them
  chassis.moveDistance(42_cm);
  chassis.waitUntilSettled();
  rollers.setTarget(-50);
  pros::delay(400);
  rollers.setTarget(0);
  //now move the tray forward, and place the cubes
  unbrakeRollers();
  trayControl.setMaxVelocity(30);
  trayControl.setTarget(TRAY_MAX);
  rollers.setTarget(-20);
  trayControl.waitUntilSettled();
  rollers.setTarget(0);
  pros::delay(100);
  chassis.setMaxVelocity(20);
  chassis.moveDistance(5_cm);
  pros::delay(1000);
  chassis.setMaxVelocity(80);
  armControl.setTarget(0);
  armControl.waitUntilSettled();
  pros::delay(100);
  //now move away and move tray down
  chassis.setMaxVelocity(45);
  chassis.moveDistance(-29_cm);
  chassis.setMaxVelocity(60);
  trayControl.setMaxVelocity(80);
  trayControl.setTarget(0);
  trayControl.waitUntilSettled();
  armControl.setTarget(ARM_PICKUP_HEIGHT);
  //now turn towards tower and move to pick up the purple cube
  chassis.turnAngle(-140_deg);
  chassis.moveDistance(63_cm);
  //now pick up purple cube
  rollers.setTarget(110);
  chassis.moveDistance(4_cm);
  rollers.setTarget(0);
  //now back up, move tray forward and arm up to scoring position
  chassis.moveDistance(-5_cm);
  trayControl.setMaxVelocity(50);
  trayControl.setTarget(TRAY_SCORE_CUBES);
  trayControl.waitUntilSettled();
  armControl.setTarget(ARM_HIGHSCORE_HEIGHT);
  armControl.waitUntilSettled();
  chassis.moveDistance(14_cm);
  //set the cube
  rollers.setTarget(-70);
  pros::delay(1500);
  rollers.setTarget(0);
  //backup, turn towards other small goal to pick up green cube
  chassis.setMaxVelocity(60);
  chassis.moveDistance(-52_cm);
  armControl.setTarget(ARM_PICKUP_HEIGHT);
  chassis.turnAngle(-90_deg);
  chassis.moveDistance(62_cm);
  //now pick up the green cubes
  rollers.setTarget(110);
  chassis.moveDistanceAsync(5_cm);
  pros::delay(500);
  rollers.setTarget(0);
  chassis.waitUntilSettled();
  pros::delay(250);
  rollers.setTarget(0);
  //move arm to scoring position
  chassis.moveDistance(-5_cm);
  armControl.setTarget(ARM_LOWSCORE_HEIGHT);
  armControl.waitUntilSettled();
  chassis.moveDistance(8_cm);
  //now move forward and score the cube
  rollers.setTarget(-70);
  pros::delay(1000);
  rollers.setTarget(0);
  chassis.moveDistance(-20_cm);
}

void autonomous() {
	//select which autonomous should be ran
	switch(autonToUse){
    case 0:
      break;
		case 1:
			auton1(false); //false to not mirror turns
			break;
    case 2:
      auton1(true); //true to mirror turns
      break;
    case 3:
      auton2(false);
      break;
    case 4:
      auton2(true);
      break;
    case 5:
      driverSkills();
      break;
	}
}


//opcontrol start
//task function for controlling the arm, runs as a task in opcontrol()
//THIS IS FOR THE ARM!!! NOT THE TRAY.
void armControlFunc(void *) {
	pros::Controller master (pros::E_CONTROLLER_MASTER);
  setMotorBrakes();
  bool rightToggle = false;
  bool leftToggle  = true;

	while (1) {
    //moving arm up and down manually
  	if (master.get_digital(DIGITAL_L1))
			setArm(100);
  	else if (master.get_digital(DIGITAL_L2))
  		setArm(-100);
    //bring arm to pickup height when Y is pressed
    else if (master.get_digital(DIGITAL_Y)) {
      while (getArmPos() < 80)
        setArm(50);
      while (getArmPos() > ARM_PICKUP_HEIGHT)
        setArm(-40);
    }
    //bring arm to 0 when a is pressed
    else if (master.get_digital(DIGITAL_A)) {
      while (getArmPos() > 0)
        setArm(-80);
    }
    //bring arm to high scoring height if right is pressed, back to pickup if pressed again
    else if (master.get_digital(DIGITAL_RIGHT)) {
      rightToggle = !rightToggle;
      if (rightToggle) {
        while (getArmPos() < ARM_HIGHSCORE_HEIGHT)
          setArm(70);
          pros::delay(20);
      }
      else {
        while (getArmPos() > ARM_PICKUP_HEIGHT)
          setArm(-70);
          pros::delay(20);
      }
    }
    //bring arm to low scoring height if left is pressed, back to pickup if pressed again
    else if (master.get_digital(DIGITAL_LEFT)) {
      leftToggle = !leftToggle;
      if (leftToggle) {
        while (getArmPos() < ARM_LOWSCORE_HEIGHT)
          setArm(70);
          pros::delay(20);
      }
      else {
        while (getArmPos() > ARM_PICKUP_HEIGHT)
          setArm(-70);
          pros::delay(20);
      }
    }
  	else
  		setArm(0);

		pros::delay(30);
	}
}

//task function for controlling the tray, runs as a task in opcontrol()
//THIS IS FOR THE TRAY!!! NOT THE ARM
void trayControlFunc(void *) {
	pros::Controller master (pros::E_CONTROLLER_MASTER);
	setMotorBrakes();
	bool aToggle = false;
  bool scoreToggle = false;
	int pos = 0;

	while (1) {
		//if the a button is pressed, toggle the tray in and out.
		if (master.get_digital(DIGITAL_A)) {
			aToggle = !aToggle;
      trayInSetPosition = !trayInSetPosition;
			if (aToggle) {
        unbrakeRollers();
        while (getTrayPosition() < TRAY_MAX * (3/4.0)) {
          setTray(50);
          setRollers(-10);
        }
        while (getTrayPosition() < TRAY_MAX) {
          setTray(30);
          setRollers(-10);
        }
        setRollers(0);
        setTray(0);
			}
			else {
        unbrakeRollers();
        while (getTrayPosition() > 0.0) {
          setTray(-80);
        }
        setTray(0);
        setMotorBrakes();
			}

		}

		//if right or left arrow is pressed, and the tray is not in setting position, move tray to allow for cube scoring
	  else if ((master.get_digital(DIGITAL_RIGHT) || master.get_digital(DIGITAL_LEFT)) && !trayInSetPosition) {
      scoreToggle = !scoreToggle;
      if (scoreToggle) {
        while (getTrayPosition() < TRAY_SCORE_CUBES)
          setTray(60);
          pros::delay(15);
      }
      else {
        pros::delay(500);
        while (getTrayPosition() > 0.0)
          setTray(-60);
      }
		}

    else
      setTray(0);

		pros::delay(30);
	}
}

void rollControlFunc(void*) {
  pros::Controller master (pros::E_CONTROLLER_MASTER);

  while(1) {
    //moving rollers based on trigger buttons
    if (master.get_digital(DIGITAL_R2))
			setRollers(-30);
		else if (master.get_digital(DIGITAL_R1))
			setRollers(110);
    //macros
    else if (master.get_digital(DIGITAL_UP))
      setRollers(110);
    else if (master.get_digital(DIGITAL_DOWN))
      setRollers(-120);
    //if no button pressed, don't move
		else
			setRollers(0);

    pros::delay(30);
  }
}

void autonPrinterFunc(void*) {
  while (1) {
    //print which autonomous will currently be run
    if (autonToUse == 0)
      lv_label_set_text(autonDescrpt, "No autonomous will be run");
    else if (autonToUse == 1)
      lv_label_set_text(autonDescrpt, "Blue auton 1 will be run");
    else if (autonToUse == 2)
      lv_label_set_text(autonDescrpt, "Red auton 1 will be run");
    else if (autonToUse == 3)
      lv_label_set_text(autonDescrpt, "Blue auton 2 will be run");
    else if (autonToUse == 4)
      lv_label_set_text(autonDescrpt, "Red auton 2 will be run");
      else if (autonToUse == 5)
        lv_label_set_text(autonDescrpt, "Driver skills will be run");

    //long delay to give lvgl time to react to changes
    pros::delay(250);
  }
}
//cancer

void opcontrol() {
	//init the controller
	pros::Controller master (pros::E_CONTROLLER_MASTER);

  //wait for Y to be pressed before starting opcontrol, allows for setup
  while (!master.get_digital(DIGITAL_Y)) {
    if (autonToUse == 0)
      lv_label_set_text(autonDescrpt, "No autonomous will be run\nPress Y to begin");
    else if (autonToUse == 1)
      lv_label_set_text(autonDescrpt, "Blue auton 1 will be run\nPress Y to begin");
    else if (autonToUse == 2)
      lv_label_set_text(autonDescrpt, "Red auton 1 will be run\nPress Y to begin");
    else if (autonToUse == 3)
      lv_label_set_text(autonDescrpt, "Blue auton 2 will be run\nPress Y to begin");
    else if (autonToUse == 4)
      lv_label_set_text(autonDescrpt, "Red auton 2 will be run\nPress Y to begin");
      else if (autonToUse == 5)
        lv_label_set_text(autonDescrpt, "Driver skills will be run\nPress Y to begin");

    master.set_text(0,0,"Hit Y to Begin");
    master.set_text(0,0,"Hit Y to Begin"); //known bug, must print twice to ensure it displays

    std::cout<<"arm at: " << getArmPos()<<std::endl;
    std::cout<<"tray at: " << getTrayPosition()<<std::endl;

    pros::delay(500);
  }

  //init variables
	double maxPercentSpeed = 0.8;			//run at 80% speed by default
  master.set_text(0,0,"Speed:    80%");
  pros::delay(25);
  master.set_text(0,0,"Speed:    80%"); //ensure it is printed
	bool bWasPressed = false;

	//init brakes and motor positions
	setMotorBrakes();

	//init tasks
	pros::Task armControlTask(armControlFunc,(void*)NULL,"Arm Control");
	pros::Task trayControlTask(trayControlFunc,(void*)NULL,"Tray Control");
  pros::Task rollerControlTask(rollControlFunc,(void*)NULL,"Roller Control");
  pros::Task autonPrinterTask(autonPrinterFunc,(void*)NULL,"Auton Printer");

	while (true) {
		//tank control
		chassis.tank(masterController.getAnalog(ControllerAnalog::leftY) * maxPercentSpeed,
                 masterController.getAnalog(ControllerAnalog::rightY) * maxPercentSpeed);
		//if B is pressed, toggle between speeds
		if (master.get_digital(DIGITAL_B)) {
			bWasPressed = !bWasPressed;
			if (bWasPressed) {
				maxPercentSpeed = 0.6;
				master.set_text(0,0,"Speed:     60%");
        pros::delay(250);
        master.set_text(0,0,"Speed:     60%"); //insure that it is printed
			}
			else if (!bWasPressed) {
				maxPercentSpeed = 0.8;
				master.set_text(0,0,"Speed:    80%");
        pros::delay(250);
        master.set_text(0,0,"Speed:    80%"); //insure that it is printed
			}
		}
    //macros for picking up cubes/backing away from stacks, also in roller task
    if (master.get_digital(DIGITAL_UP) || master.get_digital(DIGITAL_X)) {
      chassis.tank(0.40, 0.40);
    }
    else if (master.get_digital(DIGITAL_DOWN)) {
      chassis.tank(-0.55, -0.55);
    }

		//delay to prevent resources from being wasted
		pros::delay(20);
	}
}
