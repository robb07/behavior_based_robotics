#pragma config(Sensor, dgtl1,  ultrasonic,     sensorSONAR_cm)
#pragma config(Sensor, dgtl6,  rightLimit,     sensorTouch)
#pragma config(Sensor, dgtl7,  leftLimit,      sensorTouch)
#pragma config(Sensor, dgtl8,  headEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port4,           headMotor,     tmotorVex269, openLoop)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//implements the feelForce behavior
// behavior functions:
//		feelForce
// behavior dependencies:
#include "motorCalibrate.h"
#include "lookBackAndForth.h"
#include "trackHeadAngle.h"
#include "logObstacles.h"
#include "feelForce.h"

////============ constants =========================
////from feelForce
//const float MAX_FORCE_DISTANCE = 2.0;//m
//const float FORCE_COEFFICIENT = 0.01;

////+++++++++++++++| feelForce behavior |+++++++++++++++++++++++++++++++++++++

//float* feelForce(float* obs_log, float* pose){
//	float u_x_o, u_y_o, u_mag, phi_o;
//	float f_mag, f_x=0.0, f_y=0.0;
//	//f_x = 0.0;
//	//f_y = 0.0;
//	for(int i=0; i< N_ANGLES; i++){
//		u_x_o = pose[0] - obs_log[2*i+0];
//		u_y_o = pose[1] - obs_log[2*i+1];
//		u_mag = sqrt(u_x_o*u_x_o + u_y_o*u_y_o);
//		phi_o = atan2(u_y_o,u_x_o);
//		if(u_mag < MAX_FORCE_DISTANCE){
//			f_mag = 1.0/(u_mag*u_mag)*FORCE_COEFFICIENT;
//			f_x += f_mag*cos(phi_o);
//			f_y += f_mag*sin(phi_o);
//		}
//	}
//	float f_v[2];
//	f_v[0] = f_x;
//	f_v[1] = f_y;
//	return	f_v;
//}

task main()
{
	// pose ===========================================================
	float pose[3];
	float* pose_ptr = &pose;
	float x=0,y=0,phi=0;
	pose[0]=x;pose[1]=y;pose[2]=phi;

	// speeds =========================================================
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

	//feelForce
	//float u_x_o, u_y_o, u_mag, phi_o;
	//float f_mag, f_x, f_y;
	float* f_ptr;
	float f_x, f_y;

	while(1){

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


		// feelForce behavior ==========================================================================================
		//f_x = 0.0;
		//f_y = 0.0;
		//for(int i=0; i< N_ANGLES; i++){
		//	u_x_o = x - obstacles_xy[i][0];
		//	u_y_o = y - obstacles_xy[i][1];
		//	u_mag = sqrt(u_x_o*u_x_o + u_y_o*u_y_o);
		//	phi_o = atan2(u_y_o,u_x_o);
		//	if(u_mag < MAX_FORCE_DISTANCE){
		//		f_mag = 1.0/(u_mag*u_mag)*FORCE_COEFFICIENT;
		//		f_x += f_mag*cos(phi_o);
		//		f_y += f_mag*sin(phi_o);
		//	}
		//}
		f_ptr = feelForce(obstacles_xy,pose);
		f_x = f_ptr[0];f_y = f_ptr[1];
		// end feelForce behavior ======================================================================================

		w_look = lookBackAndForth(w_look);
		w_head = w_look;
		headRotate(w_head);
	}
}
