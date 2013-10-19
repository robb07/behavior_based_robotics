#pragma config(Sensor, dgtl6,  rightLimit,     sensorTouch)
#pragma config(Sensor, dgtl7,  leftLimit,      sensorTouch)
#pragma config(Sensor, dgtl8,  headEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port4,           headMotor,     tmotorVex269, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//implements the head tracking behavior
// behavior functions:
//    headHalt
//		headCalibrate
//		getHeadAngle
// behavior dependencies:
#include "motorCalibrate.h"
#include "lookBackAndForth.h"
#include "trackHeadAngle.h"

////============ constants =========================
////from trackHeadAngle
//const float HEAD_TICKS_PER_REV = 360.0;
//const float W_HEAD_CALIBRATE = 5.0;//rad/s

////+++++++++++++++| trackHeadAngle behavior |++++++++++++++++++++++++++++++++
//void headHalt(){
//	motor[headMotor] = 0;
//}

//float headCalibrate(){
//	bool hitMin = false;
//	int headMax = 0;

//	headRotate(-W_HEAD_CALIBRATE);//rotate to the minimum
//	while(1){
//		if(SensorValue[rightLimit]==1){
//			//zero out encoder at minimum
//			SensorValue[headEncoder]=0;
//			hitMin = true;
//			headRotate(W_HEAD_CALIBRATE);
//		}
//		if(hitMin && SensorValue[leftLimit]==1){
//			//record highest value
//			headHalt();
//			headMax = SensorValue[headEncoder];
//			break;
//		}
//	}
//	float halfViewingAngle = 0.5*headMax*2.0*PI/HEAD_TICKS_PER_REV;
//	return halfViewingAngle;
//}

//float getHeadAngle(){
//	//radians referenced to zero point from calibration
//	return SensorValue[headEncoder]*2.0*PI/HEAD_TICKS_PER_REV;
//}

task main()
{
	// speeds =========================================================
	float w_look = 0.0;
	float w_head = 0.0;

	// head tracking ==================================================
	float halfViewingAngle = headCalibrate();
	float headAngle = 0;

	while(1){

		headAngle = getHeadAngle();
		if(halfViewingAngle-0.02<headAngle && headAngle<halfViewingAngle+0.02){
			headHalt();
			wait1Msec(1000);
	  }
		//if(w_look!=w_head){
		//	headAngle = getHeadAngle();
		//	headHalt();
		//	wait1Msec(100);
	  //}
		w_look = lookBackAndForth(w_look);
		w_head = w_look;
		headRotate(w_head);
	}


}
