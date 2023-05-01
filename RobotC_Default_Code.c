#pragma config(Motor,  port2,           rightMotor,    tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port5,           myServo,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           myMotor,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           leftMotor,     tmotorVex393_MC29, openLoop, driveLeft)


// Drive function controls drive motors
void drive() {
	int rightSpeed = 0;
	int leftSpeed = 0;

	// Driving forward and backward
	rightSpeed += vexRT[Ch2];
	leftSpeed += vexRT[Ch2];

	// Turning left and right
	rightSpeed += vexRT[Ch1];
	leftSpeed -= vexRT[Ch1];

	// Run motor
	motor(rightMotor) = rightSpeed;
	motor(leftMotor) = leftSpeed;
}


// Servo function controls servo motor
void servo() {
	if (vexRT[Btn5U] == 1) {
		motor[myServo] = -127;
  }
	else if (vexRT[Btn5D] == 1){
		motor[myServo] = 127;
	}
	else {
		motor[myServo] = 0;
	}
}


// Small Motor function controls small motor
void smallMotor() {
	if (vexRT[Btn6U] == 1) {
		motor[myMotor] = 127;
  }
	else if (vexRT[Btn6D] == 1) {
	  motor[myMotor] = -127;
	}
	else {
		motor[myMotor] = 0;
	}
}


// Main function
task main() {
	// Arcade Mode active
	bool arcadeMode = true;

	while (arcadeMode) {
		// Call motor functions
		drive();
		servo();
		smallMotor();
	}
}
