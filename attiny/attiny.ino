// Bitbanging with softwire
#define SDA_PORT PORTB
#define SDA_PIN 0
#define SCL_PORT PORTB
#define SCL_PIN 2

#include <SoftWire.h>
SoftWire bbWire = SoftWire();

// USIwire for slave
#include <USIWire.h>
#define MY_SLAVE_ADDR			0x10
#define VEML6070_ADDR_H			0x39
#define VEML6070_ADDR_L			0x38
#define TEST_LED				PB3
#define LIGHT_TRIGGER_VALUE		3

volatile uint8_t lightValue = 0;
volatile byte commandReceived = 0;


void setup() {
	// start USI slave
	Wire.begin( MY_SLAVE_ADDR );
	Wire.onRequest( requestEvent );
	Wire.onReceive( receiveEvent );

	pinMode( TEST_LED, OUTPUT ); // Visual feedback
}


void loop() {
	// This is my ultimate goal to switch between master and slave.
	if ( commandReceived == 'L' ) {
		Wire.end();						// Stop being an USI slave
		readFromLightSensor();			// Get data

		// Start USI slave again
		Wire.begin( MY_SLAVE_ADDR );
		Wire.onRequest( requestEvent );
		Wire.onReceive( receiveEvent );
		commandReceived = 0;			// Reset the command - we will be waiting for the next.
	}
}


/* In slave mode: give master data he polls for */
void requestEvent() {
	Wire.write( lightValue );
}


/* In slave mode: got a command from master */
void receiveEvent( int howMany ) {
	commandReceived = Wire.read();
}


/* In bitbang master mode: get data from a very simple VEML6070 UV sensor */
void readFromLightSensor() {
	bbWire.begin();			// Become master

	bbWire.beginTransmission( VEML6070_ADDR_L );
	bbWire.write( 0x04 );		// Sensor setup
	bbWire.endTransmission();

	bbWire.requestFrom( VEML6070_ADDR_L, 1 );
	lightValue = bbWire.read(); // Get LSB of light value - enough for test

							  // Visual feedback. By using a flashlight on the VEML sensor, I can turn off the TEST LED.
	digitalWrite( TEST_LED, lightValue > LIGHT_TRIGGER_VALUE ? LOW : HIGH );
}