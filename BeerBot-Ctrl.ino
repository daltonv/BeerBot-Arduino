#include "BeerBotCtrl.h"
#include "Encoder.h"

BeerBot robot;

Encoder enc(18,19);

long prevEncoderTime = 0;
long timediff = 0;
long oldEncoder = -999;

int test = 0;

void setup()
{
	robot.setRightMotor(400);
	Serial.begin(9600);
}

void loop()
{
	int k = enc.read();
	if(k != oldEncoder) {
		Serial.println(k);
		oldEncoder = k;
	} 

}

void enableEncoder() {
	attachInterrupt(digitalPinToInterrupt(18),encoder_isr,RISING);
	prevEncoderTime = micros();
}

void encoder_isr() {
	long time = micros();
	timediff = time - prevEncoderTime;
	prevEncoderTime = time;
	test++;
	Serial.println(test);
}