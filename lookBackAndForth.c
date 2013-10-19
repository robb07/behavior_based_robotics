#pragma config(Sensor, dgtl6,  rightLimit,     sensorTouch)
#pragma config(Sensor, dgtl7,  leftLimit,      sensorTouch)
#pragma config(Motor,  port4,           headMotor,     tmotorVex269, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//implements the look back and forth behavior
// behavior functions:
//    lookBackAndForth
//    headRotate
// behavior dependencies:
#include "motorCalibrate.h"
#include "lookBackAndForth.h"

////============ constants =========================
////from lookBackAndForth
//const float W_LOOK_MAG = 5.0;//rad/s

////+++++++++++++++| lookBackAndForth behavior |++++++++++++++++++++++++++++++++
//float lookBackAndForth(float w_look_state){
//	float w_look = w_look_state;
//	if(w_look==0.0){
//		w_look = W_LOOK_MAG;
//	}

//	if(SensorValue[rightLimit]==1){
//		w_look = W_LOOK_MAG;
//	}else if(SensorValue[leftLimit]==1){
//		w_look = -1*W_LOOK_MAG;
//	}
//	return w_look;
//}

//void headRotate(float w_head){
//		motor[headMotor] = angular_wheel_speed_to_power(w_head);
//}

task main()
{
	float w_look = 0.0;
	float w_head = 0.0;

	while(1){
		w_look = lookBackAndForth(w_look);
		w_head = w_look;
		headRotate(w_head);
	}
}
