#include "main.h"
using namespace okapi;

//globals
int autonToUse = 1; //run auton 1 by default
Controller masterController;    //controller for okapi

//constants
const int SCR_WIDTH = 480;
const int SCR_HEIGHT = 272;
const int TRAY_MAX = 3700;
const int TRAY_SCORE_CUBES = 900;
const int ARM_PICKUP_HEIGHT = 260; //THE LESS ACCURATE VALUE. for quick n' dirty movements
const int ARM_PICKUP_AUTON = 260; //ACCURATE value. use this plese. :)
const int ARM_LOWSCORE_HEIGHT = 2500;
const int ARM_HIGHSCORE_HEIGHT = 3100;

const int ARM_TRAY_OPEN_HEIGHT = 2000;
const int TRAY_TRAY_OPEN_HEIGHT = 700;

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

  //calibrate the Imu
  imuSensor.reset();
  pros::delay(200);
  while (imuSensor.is_calibrating()) {
    lv_label_set_text(autonDescrpt, "STOP, IMU CALIBRATING");
    pros::delay(20);
  }
  lv_label_set_text(autonDescrpt, "No autonomous will be run");
}



void disabled() {
  //inform the user which auton will be ran if a button is pressed
  lv_label_set_text(autonDescrpt, NULL);//update the label
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

void competition_initialize() {
  //inform the user which auton will be ran if a button is pressed
  lv_label_set_text(autonDescrpt, NULL);//update the label
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


//auton start

//making the controllers, also used in opcontrol
auto chassis = ChassisControllerBuilder()
                .withMotors({18,20},{17,19})
                .withDimensions(AbstractMotor::gearset::green, {{10.6_cm,23_cm}, imev5GreenTPR})
                .build();
auto rollers = AsyncVelControllerBuilder().withMotor({4,2}).build();
auto trayControl = AsyncPosControllerBuilder().withMotor(6).build();
auto armControl = AsyncPosControllerBuilder().withMotor(3).build();

//making autonomous functions to allow for multiple possible auton. routines
void auton1(bool mirrorTurns) {
  unbrakeArm();
  brakeRollers(true);

  //deploy
  trayControl->setTarget(TRAY_TRAY_OPEN_HEIGHT);
  armControl->setTarget(ARM_TRAY_OPEN_HEIGHT);
  armControl->waitUntilSettled();

  //set arm and rollers, then pick up first row
  armControl->setTarget(ARM_PICKUP_AUTON);
  armControl->waitUntilSettled();
  trayControl->setTarget(0);
  rollers->setTarget(180);
  driveDistance(100,80);
  rollers->setTarget(0);
  pros::delay(250);
  //backup to goal
  driveDistance(-48,100);

  pros::delay(350);
  turn(-imuSensor.get_rotation()-135,127,mirrorTurns);
  //turn(-135,80,false);

  //move forward to goal, then set stack
  driveDistance(45,100);
  rollers->setTarget(-55);
  pros::delay(500);
  rollers->setTarget(0);
  brakeRollers(false);
  armControl->setTarget(0);
  pros::delay(50);
  trayControl->setMaxVelocity(90);
  trayControl->setTarget(TRAY_MAX);
  trayControl->waitUntilSettled();
  tank(30,30);
  pros::delay(750);
  tank(0,0);
  pros::delay(250);

  //back away, move tray down
  trayControl->setMaxVelocity(200);
  trayControl->setTarget(0);
  rollers->setTarget(-70);
  pros::delay(100);
  driveDistance(-20,40);
  rollers->setTarget(0);
}

void redAuton1(bool mirrorTurns) {
  unbrakeArm();
  brakeRollers(true);

  //deploy
  trayControl->setTarget(TRAY_TRAY_OPEN_HEIGHT);
  armControl->setTarget(ARM_TRAY_OPEN_HEIGHT);
  armControl->waitUntilSettled();

  //set arm and rollers, then pick up first row
  armControl->setTarget(ARM_PICKUP_AUTON);
  armControl->waitUntilSettled();
  trayControl->setTarget(0);
  rollers->setTarget(180);
  driveDistance(100,80);
  rollers->setTarget(0);
  pros::delay(250);
  //backup to goal
  driveDistance(-44,100);

  pros::delay(350);
  turn(-imuSensor.get_rotation()-135,127,mirrorTurns);
  //turn(-135,80,false);

  //move forward to goal, then set stack
  driveDistance(45,100);
  rollers->setTarget(-55);
  pros::delay(500);
  rollers->setTarget(0);
  brakeRollers(false);
  armControl->setTarget(0);
  pros::delay(50);
  trayControl->setMaxVelocity(90);
  trayControl->setTarget(TRAY_MAX);
  trayControl->waitUntilSettled();
  tank(30,30);
  pros::delay(750);
  tank(0,0);
  pros::delay(250);

  //back away, move tray down
  trayControl->setMaxVelocity(200);
  trayControl->setTarget(0);
  rollers->setTarget(-70);
  pros::delay(100);
  driveDistance(-20,40);
  rollers->setTarget(0);
}

void auton2(bool mirrorTurns) {
  unbrakeArm();
  brakeRollers(true);

  //deploy
  trayControl->setTarget(TRAY_TRAY_OPEN_HEIGHT);
  armControl->setTarget(ARM_TRAY_OPEN_HEIGHT);
  armControl->waitUntilSettled();

  //set arm and rollers, then pick up first row
  armControl->setTarget(ARM_PICKUP_AUTON);
  armControl->waitUntilSettled();
  trayControl->setTarget(0);
  rollers->setTarget(180);
  driveDistance(45,80);
  rollers->setTarget(0);
  pros::delay(250);

  //drive back, turn to big goal
  driveDistance(-33,80);
  turn(90,127,mirrorTurns);

  //drive to goal, set stack
  driveDistance(62,80);
  rollers->setTarget(-55);
  pros::delay(500);
  rollers->setTarget(0);
  brakeRollers(false);
  armControl->setTarget(0);
  pros::delay(50);
  trayControl->setMaxVelocity(90);
  trayControl->setTarget(TRAY_MAX);
  trayControl->waitUntilSettled();
  tank(30,30);
  pros::delay(750);
  tank(0,0);
  pros::delay(250);

  //back away, move tray down
  trayControl->setMaxVelocity(200);
  trayControl->setTarget(0);
  rollers->setTarget(-70);
  pros::delay(100);
  driveDistance(-20,40);
  rollers->setTarget(0);
}

void driverSkills() {
  unbrakeArm();
  brakeRollers(true);

  //deploy
  trayControl->setTarget(TRAY_TRAY_OPEN_HEIGHT);
  armControl->setTarget(ARM_TRAY_OPEN_HEIGHT);
  armControl->waitUntilSettled();

  //set arm and rollers, then pick up first row
  armControl->setTarget(ARM_PICKUP_AUTON);
  armControl->waitUntilSettled();
  trayControl->setTarget(0);
  rollers->setTarget(180);
  driveDistance(100,80);
  rollers->setTarget(0);

  //backup to goal
  driveDistance(-48,100);
  pros::delay(350);
  turn(-imuSensor.get_rotation()-135,127,false);
  //turn(-135,80,false);

  //move forward to goal, then set stack
  driveDistance(45,100);
  rollers->setTarget(-55);
  pros::delay(500);
  rollers->setTarget(0);
  brakeRollers(false);
  armControl->setTarget(0);
  pros::delay(50);
  trayControl->setMaxVelocity(90);
  trayControl->setTarget(TRAY_MAX);
  trayControl->waitUntilSettled();
  tank(30,30);
  pros::delay(750);
  tank(0,0);
  pros::delay(250);

  //back away, move tray down
  rollers->setTarget(-70);
  pros::delay(100);
  driveDistance(-20,40);
  rollers->setTarget(0);
  driveDistance(-10,80);
  trayControl->setMaxVelocity(200);
  trayControl->setTarget(0);

  //turn to pick up purple cube, pick it up
  turn(-imuSensor.get_heading()+90,127,false);
  //turn(-135,110,false);
  trayControl->waitUntilSettled();
  armControl->setTarget(ARM_PICKUP_AUTON);
  rollers->setTarget(180);
  pros::delay(400); //make sure chassis drives straight
  driveDistance(80,70);
  pros::delay(1000);
  rollers->setTarget(-50);
  pros::delay(750);
  rollers->setTarget(0);

  //move arm up, put cube in tower
  trayControl->setTarget(TRAY_SCORE_CUBES);
  armControl->setTarget(ARM_HIGHSCORE_HEIGHT);
  armControl->waitUntilSettled();
  turn(90-imuSensor.get_heading(),127,false); //straighten up
  driveDistance(10,80);
  pros::delay(1000);
  rollers->setTarget(-80);
  pros::delay(1000);
  rollers->setTarget(0);

  //back up, arm down, turn 90 degrees
  driveDistance(-10,80);
  armControl->setTarget(ARM_PICKUP_AUTON);
  armControl->waitUntilSettled();
  trayControl->setTarget(0);
  turn(-45,127,false);
  driveDistance(-10,80);
  turn(-45,127,false);

  //pickup next cube
  rollers->setTarget(180);
  driveDistance(25,80);
  pros::delay(1000);
  rollers->setTarget(-50);
  pros::delay(750);
  rollers->setTarget(0);

  //turn to alliance goal, set cube
  turn(-(360+imuSensor.get_rotation())-80,127,false);
  //turn(-80,127,false);
  driveDistance(60,80);
  trayControl->setTarget(TRAY_SCORE_CUBES);
  armControl->setTarget(ARM_LOWSCORE_HEIGHT);
  armControl->waitUntilSettled();
  driveDistance(20,80);
  pros::delay(1000);
  rollers->setTarget(-90);
  pros::delay(1000);
  rollers->setTarget(0);

  //back away, move arm down, turn to final goal
  driveDistance(-45,80);
  armControl->setTarget(ARM_PICKUP_AUTON);
  armControl->waitUntilSettled();
  trayControl->setTarget(0);
  turn(80,100,false);

  //drive to cube, pick it up
  rollers->setTarget(180);
  driveDistance(50,80);
  pros::delay(1000);
  rollers->setTarget(-50);
  pros::delay(750);
  rollers->setTarget(0);
  driveDistance(-15,80);

  //set cube
  trayControl->setTarget(TRAY_SCORE_CUBES);
  armControl->setTarget(ARM_LOWSCORE_HEIGHT);
  armControl->waitUntilSettled();
  driveDistance(15,80);
  pros::delay(1000);
  rollers->setTarget(-90);
  pros::delay(1000);
  rollers->setTarget(0);
  driveDistance(-20,80);
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
      redAuton1(true); //true to mirror turns
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
  bool leftToggle  = false;


	while (1) {
    //ensure that the macros are not enabled
    //if (!leftToggle && !rightToggle) {
      //moving arm up and down manually
    	if (master.get_digital(DIGITAL_L1))
  			setArm(50);
    	else if (master.get_digital(DIGITAL_L2))
    		setArm(-50);
      //bring arm to pickup height when Y is pressed
      else if (master.get_digital(DIGITAL_Y)) {
        moveArmTo(ARM_PICKUP_AUTON,50);
      }
      //bring arm to 0 when a is pressed
      else if (master.get_digital(DIGITAL_A)) {
          moveArmTo(0,80);
      }
    //}
    //bring arm to high scoring height if right is pressed, back to pickup if pressed again
    else if (master.get_digital(DIGITAL_RIGHT) && !leftToggle) {
      rightToggle = !rightToggle;
      if (rightToggle) {
        while (getArmPos() < ARM_HIGHSCORE_HEIGHT)
          setArm(100);
      }
      if (!rightToggle) {
        while (getArmPos() > ARM_PICKUP_HEIGHT)
          setArm(-100);
          pros::delay(20);
      }
    }
    //bring arm to low scoring height if left is pressed, back to pickup if pressed again
    else if (master.get_digital(DIGITAL_LEFT) && !rightToggle) {
      leftToggle = !leftToggle;
      if (leftToggle) {
        while (getArmPos() < ARM_LOWSCORE_HEIGHT)
          setArm(100);
      }
      if (!leftToggle) {
        while (getArmPos() > ARM_PICKUP_HEIGHT)
          setArm(-100);
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
			if (aToggle) {
        unbrakeRollers();
        while (getTrayPosition() < TRAY_MAX * (3/4.0)) {
          setTray(100);
        //  setRollers(-20);
        }
        while (getTrayPosition() < TRAY_MAX) {
          setTray(80);
        //  setRollers(-15);
        }
        setRollers(0);
        setTray(0);
			}
			else {
        unbrakeRollers();
        while (getTrayPosition() > 0.0) {
          setTray(-200);
        }
        setTray(0);
        setMotorBrakes();
			}

		}

		//if right or left arrow is pressed, and the tray is not in setting position, move tray to allow for cube scoring
	  else if ((master.get_digital(DIGITAL_RIGHT) || master.get_digital(DIGITAL_LEFT)) && !aToggle) {
      scoreToggle = !scoreToggle;
      if (scoreToggle) {
        while (!(getArmPos() >= 65)) pros::delay(20); //wait for arm to be at right height
        while (getTrayPosition() < TRAY_SCORE_CUBES)
          setTray(200);
      }
      if (!scoreToggle) {
        pros::delay(750);
        while (getTrayPosition() > 0.0)
          setTray(-200);
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
			setRollers(-60);
		else if (master.get_digital(DIGITAL_R1) || master.get_digital(DIGITAL_UP))
      setRollers(150);
    else if (master.get_digital(DIGITAL_DOWN))
      setRollers(-100);
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
    std::cout<<"imu at: " << imuSensor.get_rotation()<<std::endl;

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
		tank(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)*maxPercentSpeed,master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)*maxPercentSpeed);
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
    //macros for picking up cubes/backing away from stacks, related in roller task
    if (master.get_digital(DIGITAL_UP) || master.get_digital(DIGITAL_X))
      tank(127*0.4,127*0.4);
    else if (master.get_digital(DIGITAL_DOWN))
      tank(-127*0.4,-127*0.4);

		//delay to prevent resources from being wasted
		pros::delay(20);
	}
}
