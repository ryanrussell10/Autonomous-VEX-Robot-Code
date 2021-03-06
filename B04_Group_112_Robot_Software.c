#pragma config(Sensor, in1,    infraCollector1, sensorReflection)
#pragma config(Sensor, in2,    infraCollector2, sensorReflection)
#pragma config(Sensor, in3,    infraCollector3, sensorReflection)
#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Sensor, dgtl3,  limitSwitch1,   sensorTouch)
#pragma config(Sensor, dgtl4,  limitSwitch2,   sensorTouch)
#pragma config(Sensor, dgtl5,  sonarSensor,    sensorSONAR_cm)
#pragma config(Sensor, dgtl8,  redLED1,        sensorDigitalOut)
#pragma config(Sensor, dgtl9,  redLED2,        sensorDigitalOut)
#pragma config(Sensor, dgtl10, redLED3,        sensorDigitalOut)
#pragma config(Sensor, dgtl11, redLED4,        sensorDigitalOut)
#pragma config(Sensor, I2C_1,  turnEncoder,    sensorNone)
#pragma config(Motor,  port1,           rightDriveMotor, tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port4,           turningMotor,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           armMotor,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          leftDriveMotor, tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// The declaration of the robot's various states through the use of enumerated types.
typedef enum {
	TURN_STATE = 0,
	FACING_TARGET_STATE,
	DRIVING_STATE,
	CABLE_CONNECTION_STATE,
	CORRECT_LEFT_STATE,
	CORRECT_RIGHT_STATE,
	WALL_BUMPED_STATE
} T_State;

// A constant value for the light threshold that the phototransistor picks up.
const int light_threshold = 100;

// A constant value defined for the wait1MSec function that corresponds to one second.
const int oneSecond = 1000;

// Booleans defining when each touch sensor has been activated.
bool button1_pushed = false;
bool button2_pushed = false;
bool limitSwitch1_pushed = false;
bool limitSwitch2_pushed = false;



// Initializes the status of the Red LED to OFF.
void initLightStatus()
{
	// Turn off the red LED
	SensorValue(redLED1) = 0;
}



// The following five functions are implemented here so they can be called later in the program:

// Rotates the rear turning wheel with the motor encoder roughly 90 degrees to allow for rotation of the robot.
void rotateTurningWheel() {
	motor[turningMotor] = 50;
	wait1Msec(oneSecond);
	motor[turningMotor] = 0;
}

// Straightens the rear turning wheel with the motor encoder after rotation of robot is performed.
void straightenTurningWheel() {
	motor[turningMotor] = -45;
	wait1Msec(oneSecond);
	motor[turningMotor] = 0;
}

// Drops the arm for cable connection.
void dropArm()
{
	motor[armMotor] = 20;
	wait1Msec(oneSecond * 3.15);
	motor[armMotor] = 0;
}

// Raises the arm once cable connection is complete.
void raiseArm()
{
	motor[armMotor] = -20;
	wait1Msec(oneSecond * 3.5);
	motor[armMotor] = 0;
}

// Reverses from target once the cable is connected to the target object.
void reverseFromTarget(int speed)
{
	motor[rightDriveMotor] = -speed;
	motor[leftDriveMotor] = -speed;
	wait1Msec(oneSecond * 2); //Change this value depending on the intended travel distance.
	motor[rightDriveMotor] = 0;
	motor[leftDriveMotor] = 0;
}



// Monitors the given input for the touch sensors.
void monitorInput() {
	if(SensorValue(button1) && !button1_pushed) {
		button1_pushed = true;
	}
	if(SensorValue(button2) && !button2_pushed) {
		button2_pushed = true;
	}
	if(SensorValue(limitSwitch1) && !limitSwitch1_pushed) {
		limitSwitch1_pushed = true;
	}
	if(SensorValue(limitSwitch2) && !limitSwitch2_pushed) {
		limitSwitch2_pushed = true;
	}
}

// Perform processing of measurements for the right phototransistor.
// Should be called with rate of at least 20 Hertz for proper detection of puck.
int monitorRightLight()
{
	static int minLevelIR1 = 4096;	// Minimum light level seen by IR sensor 1
	static int maxLevelIR1 = 0;			// Maximum light level seen by IR sensor 1
	static int diffLevelIR1 = 0;		// Delta between maximum and minimum seen in last 0.1 seconds

	int lightLevel1 = SensorValue[infraCollector3];

	// Check if 100 msecs have elapsed.
	if ( time1[T1] > 100 )  {

		// 100 msecs have elapsed.  Compute delta of light level.
		diffLevelIR1 = maxLevelIR1 - minLevelIR1;

		// Reset calculation for next 100 msecs.
		maxLevelIR1 = 0;
		minLevelIR1 = 4096;
		clearTimer(T1);

		} else {

		// Check for new minimum/maximum light levels.
		if (lightLevel1 < minLevelIR1) {
			minLevelIR1 = lightLevel1;
			} else if (lightLevel1 > maxLevelIR1) {
			maxLevelIR1 = lightLevel1;
		}
	}

	// Returns true or false depending on whether or not IR light level is above the threshold.
	return(diffLevelIR1);
}

// Perform processing of measurements for the left phototransistor.
// Should be called with rate of at least 20 Hertz for proper detection of puck.
int monitorLeftLight()
{
	static int minLevelIR1 = 4096;	// Minimum light level seen by IR sensor 1
	static int maxLevelIR1 = 0;			// Maximum light level seen by IR sensor 1
	static int diffLevelIR1 = 0;		// Delta between maximum and minimum seen in last 0.1 seconds

	int lightLevel1 = SensorValue[infraCollector1];

	// Check if 100 msecs have elapsed.
	if ( time1[T1] > 100 )  {

		// 100 msecs have elapsed.  Compute delta of light level.
		diffLevelIR1 = maxLevelIR1 - minLevelIR1;

		// Reset calculation for next 100 msecs.
		maxLevelIR1 = 0;
		minLevelIR1 = 4096;
		clearTimer(T1);

		} else {

		// Check for new minimum/maximum light levels.
		if (lightLevel1 < minLevelIR1) {
			minLevelIR1 = lightLevel1;
			} else if (lightLevel1 > maxLevelIR1) {
			maxLevelIR1 = lightLevel1;
		}
	}

	// Returns true or false depending on whether or not IR light level is above the threshold
	return(diffLevelIR1);
}

// Perform processing of measurements for the centre phototransistor.
// Should be called with rate of at least 20 Hertz for proper detection of puck.
int monitorCentreLight()
{
	static int minLevelIR1 = 4096;	// Minimum light level seen by IR sensor 1
	static int maxLevelIR1 = 0;			// Maximum light level seen by IR sensor 1
	static int diffLevelIR1 = 0;		// Delta between maximum and minimum seen in last 0.1 seconds

	int lightLevel1 = SensorValue[infraCollector2];

	// Check if 100 msecs have elapsed.
	if ( time1[T1] > 100 )  {

		// 100 msecs have elapsed.  Compute delta of light level.
		diffLevelIR1 = maxLevelIR1 - minLevelIR1;

		// Reset calculation for next 100 msecs.
		maxLevelIR1 = 0;
		minLevelIR1 = 4096;
		clearTimer(T1);

		} else {

		// Check for new minimum/maximum light levels.
		if (lightLevel1 < minLevelIR1) {
			minLevelIR1 = lightLevel1;
			} else if (lightLevel1 > maxLevelIR1) {
			maxLevelIR1 = lightLevel1;
		}
	}

	// Returns true or false depending on whether or not IR light level is above the threshold.
	return(diffLevelIR1);
}



// The main implementation of the Finite State Machine.
task main()
{
	// Initializes loop counters for the amount of times the FACING_TARGET_STATE and CABLE_CONNECTION_STATE have looped.
	// These are included so the robot doesn't constantly spin the turning wheel while performing other actions.
	int facingStateLoopCounter = 0;
	int cableStateLoopCounter = 0;

	// Initializes the robot state to LIGHT_OFF state.
	T_State robot_state = TURN_STATE;

	// Initializes the lights to off.
	initLightStatus();

	// Rotates the turning wheel to allow for rotation of the entire robot.
	rotateTurningWheel();

	while(true) {

		// Update sensor values (must be called at least 20 times a second for proper performance).
		int leftLightLevel = monitorLeftLight();
		int rightLightLevel = monitorRightLight();
		int centreLightLevel = monitorCentreLight();

		//Checks the sensor values for the four touch sensors in order to detect if they have been pushed.
		monitorInput();

		switch( robot_state ) {


			// The initial state where the robot turns and searches for the beacon using three phototransistors.
		case TURN_STATE:

			// Activate the drive motors in reverse to each other to rotate the robot until the beacon is detected.
			motor[leftDriveMotor] = -35;
			motor[rightDriveMotor] = 35;

			// Sets the robot_state to WALL_BUMPED_STATE if one of the touch sensors are activated.
			if(button1_pushed || button2_pushed || limitSwitch1_pushed || limitSwitch2_pushed) {

				robot_state = WALL_BUMPED_STATE;

				// Clear all button flags.
				button1_pushed = false;
				button2_pushed = false;
				limitSwitch1_pushed = false;
				limitSwitch2_pushed = false;
			}

			// If the centre phototransistor picks up a larger value than the other two phototransistors
			// the robot is facing the target and the state is changed to FACING_TARGET_STATE.
			if ((centreLightLevel > (leftLightLevel)) && (centreLightLevel > (rightLightLevel))
				&& (centreLightLevel > light_threshold)) {
				robot_state = FACING_TARGET_STATE;
				} else {
				robot_state = TURN_STATE;
			}


			break;


			// The state where the robot is facing the target object but has not yet started to move towards it.
		case FACING_TARGET_STATE:

			// Deactivate the drive motors, stopping the robot facing the target object.
			motor[leftDriveMotor] = 0;
			motor[rightDriveMotor] = 0;

			// Sets the robot_state to WALL_BUMPED_STATE if one of the touch sensors are activated.
			if(button1_pushed || button2_pushed || limitSwitch1_pushed || limitSwitch2_pushed) {

				robot_state = WALL_BUMPED_STATE;

				// Clear all button flags.
				button1_pushed = false;
				button2_pushed = false;
				limitSwitch1_pushed = false;
				limitSwitch2_pushed = false;
			}

			// If this is the first time this state has been looped, return the turning wheel to its centred position.
			if (facingStateLoopCounter == 0) {
				straightenTurningWheel();
			}
			facingStateLoopCounter++; // Increment the facingStateLoopCounter so the wheel doesn't straighten again.

			// Robot automatically enters DRIVING_STATE once it has completed the FACING_TARGET_STATE.
			// (the robot knows it is facing the target and can now begin to drive).
			robot_state = DRIVING_STATE;

			break;


			// The state in which the robot drives towards the target object and beacon.
		case DRIVING_STATE:

			// Activate both drive motors to drive forwards.
			motor[leftDriveMotor] = 75;
			motor[rightDriveMotor] = 75;

			// Sets the robot_state to WALL_BUMPED_STATE if one of the touch sensors are activated.
			if(button1_pushed || button2_pushed || limitSwitch1_pushed || limitSwitch2_pushed) {

				robot_state = WALL_BUMPED_STATE;

				// Clear all button flags.
				button1_pushed = false;
				button2_pushed = false;
				limitSwitch1_pushed = false;
				limitSwitch2_pushed = false;
			}

			// If the left phototransistor is picking up higher values than the others, robot must correct direction.
			if (leftLightLevel > (centreLightLevel + 40) && leftLightLevel > (rightLightLevel + 40)) {
				robot_state = CORRECT_LEFT_STATE;
			}

			// If the right phototransistor is picking up higher values than the others, robot must correct direction.
			if (rightLightLevel > (centreLightLevel + 40) && rightLightLevel > (leftLightLevel + 40)) {
				robot_state = CORRECT_RIGHT_STATE;
			}

			// If the robot is within the intended distance for connection, begin connection.
			if ((SensorValue(sonarSensor) < 31)&&(SensorValue(sonarSensor) > 28)) {
				// Deactivate the drive motors and enter CABLE_CONNECTION_STATE.
				motor[rightDriveMotor] = 0;
				motor[leftDriveMotor] = 0;
				robot_state = CABLE_CONNECTION_STATE;
			}

			break;


			// The state where the robot has gone off course to the left and must correct its direction.
		case CORRECT_LEFT_STATE:

			// Deactivate only the leftDriveMotor, turning the robot slightly right.
			motor[leftDriveMotor] = 0;

			// Sets the robot_state to WALL_BUMPED_STATE if one of the touch sensors are activated.
			if(button1_pushed || button2_pushed || limitSwitch1_pushed || limitSwitch2_pushed) {

				robot_state = WALL_BUMPED_STATE;

				// Clear all button flags.
				button1_pushed = false;
				button2_pushed = false;
				limitSwitch1_pushed = false;
				limitSwitch2_pushed = false;
			}

			// If the robot is now again centred and heading towards the beacon, return to DRIVING_STATE.
			if ((centreLightLevel > (leftLightLevel)) && (centreLightLevel > (rightLightLevel))
				&& (centreLightLevel > light_threshold)) {
				robot_state = DRIVING_STATE;
			}

			break;


			// The state where the robot has gone off course to the right and must correct its direction.
		case CORRECT_RIGHT_STATE:

			// Deactivate only the rightDriveMotor, turning the robot slightly left.
			motor[rightDriveMotor] = 0;

			// Sets the robot_state to WALL_BUMPED_STATE if one of the touch sensors are activated.
			if(button1_pushed || button2_pushed || limitSwitch1_pushed || limitSwitch2_pushed) {

				robot_state = WALL_BUMPED_STATE;

				// Clear all button flags.
				button1_pushed = false;
				button2_pushed = false;
				limitSwitch1_pushed = false;
				limitSwitch2_pushed = false;
			}

			// If the robot is now again centred and heading towards the beacon, return to DRIVING_STATE.
			if ( (centreLightLevel > (leftLightLevel)) && (centreLightLevel > (rightLightLevel))
				&& (centreLightLevel > light_threshold) ) {
				robot_state = DRIVING_STATE;
			}

			break;


			// The state where the robot is in position for cable connection and performs said connection.
		case CABLE_CONNECTION_STATE:

			// Ensure drive motors are deactivated.
			motor[rightDriveMotor] = 0;
			motor[leftDriveMotor] = 0;

			// Sets the robot_state to WALL_BUMPED_STATE if one of the touch sensors are activated.
			if(button1_pushed || button2_pushed || limitSwitch1_pushed || limitSwitch2_pushed) {

				robot_state = WALL_BUMPED_STATE;

				// Clear all button flags.
				button1_pushed = false;
				button2_pushed = false;
				limitSwitch1_pushed = false;
				limitSwitch2_pushed = false;
			}

			// If this is the first time looping through this state, perform cable connection.
			if (cableStateLoopCounter == 0) {
				dropArm(); // Call the dropArm function and wait half a second to drop the arm for connection.
				wait1Msec(oneSecond * 0.5);

				// Activate both drive motors and wait an eighth of a second before deactivating them to secure the cable.
				motor[leftDriveMotor] = 30;
				motor[rightDriveMotor] = 30;
				wait1Msec(oneSecond * 0.8);
				motor[leftDriveMotor] = 0;
				motor[rightDriveMotor] = 0;

				// Wait a second before reversing and signaling task completion.
				wait1Msec(oneSecond);
				reverseFromTarget(35);
				SensorValue(redLED1) = 1;
			}
			cableStateLoopCounter++; // Increment the cableStateLoopCounter so the cable connection does not occur again.

			break;


			// The state where one of the touch sensors is activated and the robot has bumped a wall.
		case WALL_BUMPED_STATE:

			// Activate the drive motors to reverse the robot away from the wall.
			motor[leftDriveMotor] = -35;
			motor[rightDriveMotor] = -35;

			// Wait a second before rotating the turning wheel
			wait1Msec(oneSecond * 2);
			rotateTurningWheel();

			facingStateLoopCounter = 0; // Reset counter so the wheel can once again straighten in the FACING_TARGET_STATE.

			robot_state = TURN_STATE; // Look for the beacon once again.

			break;


		default:
			// This should never happen.
			robot_state = TURN_STATE;

		} // switch( robot_state)

	}  // while(true)

}
