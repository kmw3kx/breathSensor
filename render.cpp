/*
 ____  _____ _        _
| __ )| ____| |      / \
|  _ \|  _| | |     / _ \
| |_) | |___| |___ / ___ \
|____/|_____|_____/_/   \_\
http://bela.io
*/
/**
 * 
 * Bob Kammauff (kmw3kx)
 * 3/11/26
 * Parent render.ccp file to send data to (hopefully soon from) puredata
 * Built off of:
\example Sensors/MPR121/render.cpp

Capacitive touch sensing with MPR121
------------------------------------

This sketch allows you to hook up an MPR121 capactive touch sensing device
to Bela, for example the SparkFun Capacitive Touch Sensor Breakout - MPR121.
The breakout board gives you 12 electrode connections.

To get this working with Bela you need to connect the breakout board to the I2C
terminal on the Bela board. See the Pin guide for details of which pin is which.

The sensor data will then be available for you to use in the array
`sensorValue[NUM_TOUCH_PINS]`.
*/

#include <Bela.h>
#include <cmath>
#include "I2C_MPR121.h"
#include <libraries/Encoder/Encoder.h>
#include <libraries/libpd/libpd.h>

// How many pins there are
#define NUM_TOUCH_PINS 12

// Define this to print data to terminal
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

// ---- internal stuff -- do not change -----

I2C_MPR121 mpr121;			// Object to handle MPR121 sensing
AuxiliaryTask i2cTask;		// Auxiliary task to read I2C
AuxiliaryTask encoderTask;	// Auxiliary task to read encoder

int readCount = 0;			// How long until we read again...
int readIntervalSamples = 0; // How many samples between reads

void readMPR121(void*);
void readEncoder(void*);

bool setup(BelaContext *context, void *userData)
{
    if(!mpr121.begin(1, 0x5A)) {
        rt_printf("Error initialising MPR121\n");
        return false;
    }

    i2cTask = Bela_createAuxiliaryTask(readMPR121, 50, "bela-mpr121", context);
    encoderTask = Bela_createAuxiliaryTask(readEncoder, 50, "bela-encoder", context);
    readIntervalSamples = context->audioSampleRate / readInterval;
    
    // Setup encoder
    gEncoder.setup(kDebouncingSamples, polarity);
    pinMode(context, 0, kEncChA, INPUT);
    pinMode(context, 0, kEncChB, INPUT);
    pinMode(context, 0, kEncChBtn, INPUT);

    return true;
}

void render(BelaContext *context, void *userData)
{
    for(unsigned int n = 0; n < context->audioFrames; n++) {
        // Schedule tasks at regular intervals
        if(++readCount >= readIntervalSamples) {
            readCount = 0;
            Bela_scheduleAuxiliaryTask(i2cTask);
            Bela_scheduleAuxiliaryTask(encoderTask);
        }
    }
}

void cleanup(BelaContext *context, void *userData)
{ }

// Auxiliary task to read the I2C board
void readMPR121(void*)
{
    for(int i = 0; i < NUM_TOUCH_PINS; i++) {
        sensorValue[i] = -(mpr121.filteredData(i) - mpr121.baselineData(i));
        sensorValue[i] -= threshold;
        if(sensorValue[i] < 0)
            sensorValue[i] = 0;
#ifdef DEBUG_MPR121
        rt_printf("%d ", sensorValue[i]);
#endif
    }
#ifdef DEBUG_MPR121
    rt_printf("\n");
#endif
    libpd_float("mpr121", mpr121.touched());
}

// Auxiliary task to read the encoder
void readEncoder(void*)
{
    bool a = digitalRead(context, 0, kEncChA);
    bool b = digitalRead(context, 0, kEncChB);
    Encoder::Rotation rot = gEncoder.process(a, b);
    
    if(Encoder::NONE != rot) {
        int position = gEncoder.get();
        libpd_float("encoder_pos", (float)position);
    }
}
