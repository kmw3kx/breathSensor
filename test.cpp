#ifdef MY_SENSORS
// //MODIFICATION: include the relevant libraries for your sensors here.
#include "I2C_MPR121.h" // for capacitive touch sensor
#include <libraries/Encoder/Encoder.h> // for rotary encoder

// //Include the relevant variables and setup for your sensors here. The below is for the MPR121 capacitive touch sensor and a rotary encoder, but you can modify it to fit your needs.
// How many pins there are
#define NUM_TOUCH_PINS 12

// Define this to print data to terminal (possibly broken)
#undef DEBUG_MPR121

// Change this to change how often the MPR121 & encoder is read (in Hz)
int readInterval = 50;

// Change this threshold to set the minimum amount of touch
int threshold = 40;

// This array holds the continuous sensor values
int sensorValue[NUM_TOUCH_PINS];

// Encoder setup
Encoder gEncoder;
unsigned int kEncChA = 0;
unsigned int kEncChB = 1;
unsigned int kEncChBtn = 2;
unsigned int kDebouncingSamples = 15;
Encoder::Polarity polarity = Encoder::ANY;

// More setup classes

I2C_MPR121 mpr121;			// Object to handle MPR121 sensing
AuxiliaryTask i2cTask;		// Auxiliary task to read I2C
AuxiliaryTask encoderTask;	// Auxiliary task to read encoder

int readCount = 0;			// How long until we read again...
int readIntervalSamples = 0; // How many samples between reads

void readMPR121(void*); // Auxiliary task to read MPR121 sensor values

/* spare stuff from dist sensor i think
enum { kNumSensors = 1 };
PulseIn pulseIn[kNumSensors];
unsigned int gPulseTriggerIntervalMs = 60; // how often to send out a trigger.
unsigned int gPulseTriggerIntervalSamples; // Set in setup() based on the above
int gPulseMinLength = 7; //to avoid spurious readings
float gPulseRescale = 58; // taken from the datasheet
#ifdef MY_SENSORS_DIGITAL_OUT
unsigned int gPulseTrigDigitalOutPin[kNumSensors] = {0, }; //channel to be connected to the module's TRIGGER pin - check the pin diagram in the IDE
#else // MY_SENSORS_DIGITAL_OUT
unsigned int gPulseTrigAnalogOutPin[kNumSensors] = {0, }; //channel to be connected to the module's TRIGGER pin
#endif // MY_SENSORS_DIGITAL_OUT
unsigned int gPulseEchoDigitalInPin[kNumSensors] = {1, }; //channel to be connected to the modules's ECHO pin via resistor divider. Check the pin diagram in the IDE
int gPulseTriggerCount [kNumSensors] = {0};
*/
#endif // MY_SENSORS 