//#pragma config(Sensor, dgtl1,  ultrasonic,     sensorSONAR_cm)
//#pragma config(Sensor, dgtl6,  rightLimit,     sensorTouch)
//#pragma config(Sensor, dgtl7,  leftLimit,      sensorTouch)
//#pragma config(Sensor, dgtl8,  headEncoder,    sensorQuadEncoder)
//#pragma config(I2C_Usage, I2C1, i2cSensors)
//#pragma config(Sensor, I2C_1,  leftEncoder,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
//#pragma config(Sensor, I2C_2,  rightEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
//#pragma config(Motor,  port2,           leftMotor,    tmotorVex269, openLoop, encoder, encoderPort, I2C_1, 1000)
//#pragma config(Motor,  port3,           rightMotor,     tmotorVex269, openLoop, reversed, encoder, encoderPort, I2C_2, 1000)
//#pragma config(Motor,  port4,           headMotor,     tmotorVex269, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//implements the runaway behavior
// behavior functions:
//		calculateRunAway
//    runAwayError
// behavior dependencies:
//		motors
//    trackPosition
//		lookBackAndForth
//    trackHeadAngle
//    logObstacles
//		feelForce

//============ constants =========================
//from runAway
const float F_RUNAWAY = 1.0;
const int RUN_AWAY_TIME = 1000;//cycles for the runAway to finish

//============ state timers =========================
//from runAway
int RunAwayTimer = 0;

//+++++++++++++++| runAway behavior |+++++++++++++++++++++++++++++++++++++
float calculateRunAway(float f_x, float f_y, float phi_run){
	float phi_r;

	//calculate the runaway
	if(sqrt(f_x*f_x+f_y*f_y) > F_RUNAWAY){
		phi_r = atan2(f_y,f_x);
		RunAwayTimer = RUN_AWAY_TIME;//reset the runAway timer
	}else{
		phi_r = phi_run;
	}
	return phi_r;
}

float* runAwayError(float phi_runAway,float phi){
	float err_v, err_w;

	//Check the runAway timer
	if(RunAwayTimer>0){
		//calculate the translational velocity error signal to send to the PID
		//scale it based on how inline the robot is with the runAway direction
		err_v = CRUISE*cos(phi_runAway - phi);

		//calculate the rotational velocity error signal to send to the PID
		//scale it based on how much rotation left is needed
		err_w = phi_runAway - phi;
		err_w = atan2(sin(err_w),cos(err_w));

		//decrement runAway timer
		RunAwayTimer -= 1;
	}else{
		//ensure zero state
		RunAwayTimer = 0;
		err_v = 0.0;
		err_w = 0.0;
	}

	float err_sig[2];
	err_sig[0] = err_w;
	err_sig[1] = err_v;
	return err_sig;
}
