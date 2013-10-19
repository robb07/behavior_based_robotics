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
