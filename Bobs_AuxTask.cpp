// Auxiliary task to read the I2C board
#ifdef MY_SENSORS
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
#endif // MY_SENSORS