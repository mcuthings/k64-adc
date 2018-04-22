# k64-adc

This code is developed based on MCUXpresso SDK sample code from NXP Semiconductors.
The project of sample code was used K64 ADC16 polling. 

ADC reading was done two ways
1) Polling the status registeer of ADC conversion complete flag until ADC conversion is done.
2) CPU core is in sleep right after ADC conversion is started by software trigger. 
Interrupt is generated when ADC conversion is done and CPU core wakes up by this interrupt.

The value of ADC reading is measured 5000 times. And, it is averaged, also calculate max/min value.
Finally, the standard deviation is calculated based on the 5000 measured value.

PRINTF is a terminal console and print out by semi-hosting. 
