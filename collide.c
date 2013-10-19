#pragma config(Sensor, dgtl1,  ultrasonic,     sensorSONAR_cm)
#pragma config(Sensor, dgtl6,  rightLimit,     sensorTouch)
#pragma config(Sensor, dgtl7,  leftLimit,      sensorTouch)
#pragma config(Sensor, dgtl8,  headEncoder,    sensorQuadEncoder)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  leftEncoder,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  rightEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port2,           leftMotor,    tmotorVex269, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port3,           rightMotor,     tmotorVex269, openLoop, reversed, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port4,           headMotor,     tmotorVex269, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//implements the collide behavior
// behavior functions:
//    checkForCollisions
// behavior dependencies:
#include "motorCalibrate.h"
#include "motors.h"
#include "trackPosition.h"
#include "lookBackAndForth.h"
#include "trackHeadAngle.h"
#include "logObstacles.h"
#include "collide.h"

////============ constants =========================
////from collide
//const float COLLIDE_DISTANCE = 0.25;//m
//const float FRONT_ANGLE = PI/8.0;//rad (22.5 deg)

////+++++++++++++++| collide behavior |++++++++++++++++++++++++++++++++
//bool checkForCollisions(float* obs_log, float* pose){
//	float u_x_o = 0.0, u_y_o = 0.0, u_mag = 0.0, u_phi = 0.0;

//	for(int i=0; i<N_ANGLES; i++){
//		u_x_o = obs_log[2*i+0] - pose[0];
//		u_y_o = obs_log[2*i+1] - pose[1];
//		u_mag = sqrt(u_x_o*u_x_o+u_y_o*u_y_o);
//		u_phi = atan2(u_y_o,u_x_o);
//		if((u_mag < COLLIDE_DISTANCE) && (abs(pose[2]-u_phi) < FRONT_ANGLE)){
//			return true;
//		}
//	}

//	return false;
//}

task main()
{
	// pose ===========================================================
	float pose[3];
	float* pose_ptr = &pose;
	float x=0,y=0,phi=0;
	pose[0]=x;pose[1]=y;pose[2]=phi;

	// speeds =========================================================
	float v=CRUISE, w=0;

	float w_look = 0.0;
	float w_head = 0.0;

	// logObstacle behavior ===========================================
	// obstacle log
	float obstacles_xy[N_ANGLES][2];
	for(int i=0; i<N_ANGLES; i++){
		obstacles_xy[i][0]=10.0;//x
		obstacles_xy[i][1]=10.0;//y
	}
	float* obstacle_ptr;
	float temp_o_x,temp_o_y;

	//ranging
	float range;

	// head tracking
	float halfViewingAngle = headCalibrate();
	float headAngle = 0;

	float angleDelta = 2.0*halfViewingAngle/(float)N_ANGLES;

	float headAngleWorld = 0.0;
	float prevHeadAngleWorld = headAngleWorld-2.0*angleDelta;

	resetAngleCounter(N_ANGLES-1,-1);

	// collision =====================================================
	bool collision_flag = false;

	//more state variables for pose ===================================
	float ticks[2];
	float* ticks_ptr;
	float prevTicks[2];
	prevTicks[0]=0;prevTicks[1]=0;

	resetWheelEncoders();

	while(1){
		// update state ================================================================================================
		ticks_ptr = readWheelEncoders();
		ticks[0]=ticks_ptr[0];ticks[1]=ticks_ptr[1];

		pose_ptr = update_odometry(pose,ticks,prevTicks);
		pose[0]=pose_ptr[0];pose[1]=pose_ptr[1];pose[2]=pose_ptr[2];
		x=pose_ptr[0];y=pose_ptr[1];phi=pose_ptr[2];

		//store for next pass
		prevTicks[0] = ticks[0];
		prevTicks[1] = ticks[1];
		// end update state ============================================================================================

		// logObstacle behavior ========================================================================================
		headAngle = getHeadAngle();
		range = getRange();

		headAngleWorld = headAngle-halfViewingAngle+phi;
		if(abs(headAngleWorld - prevHeadAngleWorld) >= angleDelta){
			prevHeadAngleWorld = headAngleWorld;

			obstacle_ptr = getObstaclePosition(headAngleWorld,range,pose);
			temp_o_x = obstacle_ptr[0];temp_o_y = obstacle_ptr[1];

			obstacles_xy[AngleCounter][0] = temp_o_x;
			obstacles_xy[AngleCounter][1] = temp_o_y;

			incrementAngleCounter();
		}
		// end logObstacle behavior ====================================================================================

		// collide behavior ============================================================================================

		collision_flag = checkForCollisions(obstacles_xy,pose);

		// end collide behavior ========================================================================================

		// send actuator commands ======================================================================================
		v = CRUISE;
		if(collision_flag && v>0.0){
			v = 0.0;
		}
		set_motor_speeds(v,0.0);

		//worked before the angle was limited
		//if(collision_flag)v
		//	//halt();
		//	//set_motor_speeds(0.0,0.0);
		//	set_motor_speeds(0.0,5.0);
		//}else{
		//	set_motor_speeds(v,0.0);
		//}

		w_look = lookBackAndForth(w_look);
		w_head = w_look;
		headRotate(w_head);
		// end send actuator commands ==================================================================================
	}
}