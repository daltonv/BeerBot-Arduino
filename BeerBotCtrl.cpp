// 
// 
// 

#include "BeerBotCtrl.h"

BeerBot::BeerBot()
	:	SonarFL(TRIGGER_PIN_FL,ECHO_PIN_FL,400), SonarFML(TRIGGER_PIN_FML,ECHO_PIN_FML,400),
		SonarFMR(TRIGGER_PIN_FMR,ECHO_PIN_FMR,400),SonarFR(TRIGGER_PIN_FR,ECHO_PIN_FR,400)
{
	//Pin map
	//MOTOR PINS
	_INA1 = 2;
	_INB1 = 4;
	_EN1DIAG1 = 6;
	_CS1 = A0;
	_INA2 = 7;
	_INB2 = 8;
	_EN2DIAG2 = 12;
	_CS2 = A1;
}


// Public Methods //////////////////////////////////////////////////////////////
void BeerBot::init()
{
	// Define pinMode for the pins and set the frequency for timer1.

	pinMode(_INA1,OUTPUT);
	pinMode(_INB1,OUTPUT);
	pinMode(_PWM1,OUTPUT);
	pinMode(_EN1DIAG1,INPUT);
	pinMode(_CS1,INPUT);
	pinMode(_INA2,OUTPUT);
	pinMode(_INB2,OUTPUT);
	pinMode(_PWM2,OUTPUT);
	pinMode(_EN2DIAG2,INPUT);
	pinMode(_CS2,INPUT);
	#if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
	// Timer 1 configuration
	// prescaler: clockI/O / 1
	// outputs enabled
	// phase-correct PWM
	// top of 400
	//
	// PWM frequency calculation
	// 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
	TCCR1A = 0b10100000;
	TCCR1B = 0b00010001;
	ICR1 = 400;
	#endif
}
// Set speed for motor 1, speed is a number between -400 and 400
void BeerBot::setLeftMotor(int speed)
{
	unsigned char reverse = 0;
	
	if (speed < 0)
	{
		speed = -speed;  // Make speed a positive quantity
		reverse = 1;  // Preserve the direction
	}
	if (speed > 400)  // Max PWM dutycycle
	speed = 400;
	#if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
	OCR1A = speed;
	#else
	analogWrite(_PWM1,speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
	#endif
	if (speed == 0)
	{
		digitalWrite(_INA1,LOW);   // Make the motor coast no
		digitalWrite(_INB1,LOW);   // matter which direction it is spinning.
	}
	else if (reverse)
	{
		digitalWrite(_INA1,LOW);
		digitalWrite(_INB1,HIGH);
	}
	else
	{
		digitalWrite(_INA1,HIGH);
		digitalWrite(_INB1,LOW);
	}
}

// Set speed for motor 2, speed is a number betwenn -400 and 400
void BeerBot::setRightMotor(int speed)
{
	unsigned char reverse = 0;
	
	if (speed < 0)
	{
		speed = -speed;  // make speed a positive quantity
		reverse = 1;  // preserve the direction
	}
	if (speed > 400)  // Max
	speed = 400;
	#if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
	OCR1B = speed;
	#else
	analogWrite(_PWM2,speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
	#endif
	if (speed == 0)
	{
		digitalWrite(_INA2,LOW);   // Make the motor coast no
		digitalWrite(_INB2,LOW);   // matter which direction it is spinning.
	}
	else if (reverse)
	{
		digitalWrite(_INA2,LOW);
		digitalWrite(_INB2,HIGH);
	}
	else
	{
		digitalWrite(_INA2,HIGH);
		digitalWrite(_INB2,LOW);
	}
}

// Set speed for motor 1 and 2
void BeerBot::setMotors(int leftSpeed, int rightSpeed)
{
	setLeftMotor(leftSpeed);
	setRightMotor(rightSpeed);
}

// Brake motor 1, brake is a number between 0 and 400
void BeerBot::setLeftBrake(int brake)
{
	// normalize brake
	if (brake < 0)
	{
		brake = -brake;
	}
	if (brake > 400)  // Max brake
	brake = 400;
	digitalWrite(_INA1, LOW);
	digitalWrite(_INB1, LOW);
	#if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
	OCR1A = brake;
	#else
	analogWrite(_PWM1,brake * 51 / 80); // default to using analogWrite, mapping 400 to 255
	#endif
}

// Brake motor 2, brake is a number between 0 and 400
void BeerBot::setRightBrake(int brake)
{
	// normalize brake
	if (brake < 0)
	{
		brake = -brake;
	}
	if (brake > 400)  // Max brake
	brake = 400;
	digitalWrite(_INA2, LOW);
	digitalWrite(_INB2, LOW);
	#if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
	OCR1B = brake;
	#else
	analogWrite(_PWM2,brake * 51 / 80); // default to using analogWrite, mapping 400 to 255
	#endif
}

// Brake motor 1 and 2, brake is a number between 0 and 400
void BeerBot::setBrakes(int leftBrake, int rightBrake)
{
	setLeftBrake(leftBrake);
	setRightBrake(rightBrake);
}

// Return motor 1 current value in milliamps.
unsigned int BeerBot::getLMCurrentMilliamps()
{
	// 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
	return analogRead(_CS1) * 34;
}

// Return motor 2 current value in milliamps.
unsigned int BeerBot::getRMCurrentMilliamps()
{
	// 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
	return analogRead(_CS2) * 34;
}

// Return error status for motor 1
unsigned char BeerBot::getLMFault()
{
	return !digitalRead(_EN1DIAG1);
}

// Return error status for motor 2
unsigned char BeerBot::getRMFault()
{
	return !digitalRead(_EN2DIAG2);
}


//		SONAR METHODS		//

void BeerBot::updateSonarFL()
{
	sonar_distance[0] = SonarFL.ping_cm();
}

void BeerBot::updateSonarFML()
{
	sonar_distance[1] = SonarFML.ping_cm();
}

void BeerBot::updateSonarFMR()
{
	sonar_distance[2] = SonarFMR.ping_cm();
}

void BeerBot::updateSonarFR()
{
	sonar_distance[3] = SonarFR.ping_cm();
}

void BeerBot::updateSonars()
{
	sonar_distance[0] = SonarFL.ping_cm();
	sonar_distance[1] = SonarFML.ping_cm();
	sonar_distance[2] = SonarFMR.ping_cm();
	sonar_distance[3] = SonarFR.ping_cm();
}

