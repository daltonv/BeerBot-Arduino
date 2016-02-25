// BeerBotCtrl.h

#ifndef _BEERBOTCTRL_h
#define _BEERBOTCTRL_h

#include "NewPing.h"
#include "Pins.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class BeerBot
{
 public:
	// CONSTRUCTORS
	BeerBot();
	
	// MOTOR METHODS
	void init();
	void setLeftMotor(int speed);
	void setRightMotor(int speed);
	void setMotors(int leftSpeed, int rightSpeed);
	void setLeftBrake(int brake);
	void setRightBrake(int brake);
	void setBrakes(int leftBrake, int rightBrake);
	unsigned int getLMCurrentMilliamps();
	unsigned int getRMCurrentMilliamps();
	unsigned char getLMFault();
	unsigned char getRMFault();
	
	//SONAR
	void updateSonarFL();
	void updateSonarFML();
	void updateSonarFMR();
	void updateSonarFR();
	void updateSonars();
	
	
 private:
	//MOTOR VARIABLES
	unsigned char _INA1;
	unsigned char _INB1;
	static const unsigned char _PWM1 = 9;
	unsigned char _EN1DIAG1;
	unsigned char _CS1;
	unsigned char _INA2;
	unsigned char _INB2;
	static const unsigned char _PWM2 = 10;
	unsigned char _EN2DIAG2;
	unsigned char _CS2;
	
	//SONAR OBJECTS
	NewPing SonarFL;
	NewPing SonarFR;
	NewPing SonarFML;
	NewPing SonarFMR;
	
	//SONAR Distance Table
	int sonar_distance[4];
};

extern BeerBot BeerBotCtrl;

#endif

