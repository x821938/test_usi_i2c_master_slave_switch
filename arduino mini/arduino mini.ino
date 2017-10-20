#include <Wire.h>
	
#define I2C_SLAVE_ADDR			0x10


void setup()
{
	Serial.begin( 115200 );
	Wire.begin();
}


void loop()
{
	//  We want the attiny to become a master, fetch data, and become a slave again
	Serial.println( "Sending master a command: L" );
	Wire.beginTransmission( I2C_SLAVE_ADDR );
	Wire.write( 'L' );	
	Wire.endTransmission();

	Serial.println( "Giving him a little time to finish his job" );
	delay( 1000 );

	// Read from the attiny slave
	Wire.requestFrom( I2C_SLAVE_ADDR, 1 );
	uint8_t lightValue = Wire.read(); 
	Serial.print( "We got light value from I2C slave: " );
	Serial.println( lightValue );

	// Just to verify that I2C comm is working, we try reading directly from the same sensor
	readFromLightSensor(); 

	delay( 10000 );
	Serial.println();

}


/* Just for testing to see if we have I2C connection */
#define VEML6070_ADDR_H			0x39
#define VEML6070_ADDR_L			0x38
void readFromLightSensor() {
	Wire.begin();			

	Wire.beginTransmission( VEML6070_ADDR_L );
	Wire.write( 0x04 );		// Sensor setup
	Wire.endTransmission();

	Wire.requestFrom( VEML6070_ADDR_L, 1 );
	uint8_t lightValue = Wire.read(); // Get LSB of light value - enough for test

	Serial.print( "We got light value directly from sensor: " );
	Serial.println( lightValue );
}