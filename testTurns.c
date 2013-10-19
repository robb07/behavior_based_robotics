#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  leftEncoder,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  rightEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port2,           leftMotor,    tmotorVex269, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port3,           rightMotor,     tmotorVex269, openLoop, reversed, encoder, encoderPort, I2C_2, 1000)
//#pragma config(Sensor, I2C_1,  rightEncoder,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
//#pragma config(Sensor, I2C_2,  leftEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
//#pragma config(Motor,  port2,           rightMotor,    tmotorVex269, openLoop, reversed, encoder, encoderPort, I2C_1, 1000)
//#pragma config(Motor,  port3,           leftMotor,     tmotorVex269, openLoop, encoder, encoderPort, I2C_2, 1000)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

float wheel_radius = 0.065;//m (R)
float wheel_base = 0.265;//m (L)


//+++++++++++++++| functions to map linear speed to motor controls |+++++++++++

int angular_wheel_speed_to_power(float angular_wheel_speed){
  float m = 17.26307692;//from previous measurements (new battery) (w/ more low end meas.)
	float b = -17.28923077;//from previous measurements (new battery)(w/ more low end meas.)


	float angular_wheel_speed_mag = abs(angular_wheel_speed);
	int angular_wheel_speed_sign = 1;
	if(angular_wheel_speed < 0){
		angular_wheel_speed_sign = -1;
	}

	float power_f = pow(10.0,(angular_wheel_speed_mag-b)/m);
	int power = floor(power_f);
	if(power>127)
		power = 127;
	power = angular_wheel_speed_sign * power;
	return power;
}

float* uni_to_diff(float linear_speed, float angular_speed){
	//inputs:
	//linear_speed is the forward speed of the robot in m/s
	//angular_speed is the angluar speed (left & right) of the robot in rad/s
	//outputs:
	//angluar_wheel_speeds is a pointer of speeds for the rotation of each wheel in rad/s
	float angluar_wheel_speeds[2];
	float v_r, v_l;

	v_r = (2*linear_speed + angular_speed*wheel_base)/(2*wheel_radius);
	v_l = (2*linear_speed - angular_speed*wheel_base)/(2*wheel_radius);

	angluar_wheel_speeds[0] = v_r;
	angluar_wheel_speeds[1] = v_l;

	return angluar_wheel_speeds;

}

void set_motor_speeds(float linear_speed, float angular_speed){
	float* wheel_speeds;
	wheel_speeds = uni_to_diff(linear_speed, angular_speed);
	float v_r = wheel_speeds[0];
	float v_l = wheel_speeds[1];

	motor[rightMotor] = angular_wheel_speed_to_power(v_r);
  motor[leftMotor]  = angular_wheel_speed_to_power(v_l);
}

void halt(){
  motor[rightMotor] = 0;
  motor[leftMotor]  = 0;
}


task main()
{
	wait1Msec(500);
	set_motor_speeds(0.0,10.0);
	wait1Msec(500);
	set_motor_speeds(0.0,-10.0);
	wait1Msec(500);
	set_motor_speeds(0.3,0.0);
	wait1Msec(1000);
	set_motor_speeds(0.3,-10.0);

	wait1Msec(5000);
	halt();


}