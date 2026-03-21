#ifdef MY_SENSORS // // MODIFICATION: add requisite setup code here
	if(!mpr121.begin(1, 0x5A)) 
	{
        rt_printf("Error initialising MPR121\n");
        return false;
    }
	i2cTask = Bela_createAuxiliaryTask(readMPR121, 50, "bela-mpr121");
    
    readIntervalSamples = context->audioSampleRate / readInterval;
    
    // Setup encoder
    gEncoder.setup(kDebouncingSamples, polarity);
    pinMode(context, 0, kEncChA, INPUT);
    pinMode(context, 0, kEncChB, INPUT);
    pinMode(context, 0, kEncChBtn, INPUT);
#endif // MY_SENSORS
