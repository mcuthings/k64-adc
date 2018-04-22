/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc16.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "arm_math.h"
#include "fsl_smc.h" //For low power mode
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_BASE ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
//#define DEMO_ADC16_USER_CHANNEL 12U
#define DEMO_ADC16_USER_CHANNEL 26U //ch26 is for Temperature sensor output
#define NUMBER_OF_MEASUREMENT 5000

#define INTERRUPT 1 //ADC conversion wait for "INTERRUPT" should be defined to 1
#define POLLING 0   //ADC conversion wait for "POLLING" should be defined to 1

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ADC0_IRQHandler(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Adc16ConversionDoneFlag = false;
volatile uint32_t g_Adc16ConversionValue;
uint16_t cnt=0;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

/* Standard deviation */
float32_t dev(float32_t data[], int n) {
    int i;
    float32_t m;
    float32_t ret;
    arm_mean_f32(data, n, &m);
    float32_t var = 0.0;
    for (i = 0; i < n; i++)
        var += (data[i] - m) * (data[i] - m);
    arm_sqrt_f32(var/n, &ret);
    return ret;
}

void ADC0_IRQHandler(void)
{
    g_Adc16ConversionDoneFlag = true;
    g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

int main(void)
{
    adc16_config_t adc16ConfigStruct;
    adc16_channel_config_t adc16ChannelConfigStruct;



    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nADC16 polling Example.\r\n");

    /* For Very low power stop */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    NVIC_EnableIRQ(ADC0_IRQn);
    

    /*
     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
     * adc16ConfigStruct.enableAsynchronousClock = true;
     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
     * adc16ConfigStruct.enableHighSpeed = false;
     * adc16ConfigStruct.enableLowPower = false;
     * adc16ConfigStruct.enableContinuousConversion = false;
     */
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
#ifdef BOARD_ADC_USE_ALT_VREF
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif    
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
    PRINTF("Press any key to get user channel's ADC value ...\r\n");

    adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
#if POLLING
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#else// for Interrupt
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;
#endif

#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */

    
    
    float32_t avg=0; //Average of Temp sensor measurement 
    float32_t std=0; //standard deviation of measurement result
    float32_t arm_std=0; //CMSIS-DSP standard deviation of measurement result
    float32_t min;
    float32_t max;
    uint32_t min_index;
    uint32_t max_index;

    float32_t data[NUMBER_OF_MEASUREMENT]={0}; //Buffer for measurement data of Temp sensor x 4096 times
    int n = sizeof(data)/sizeof(float32_t);

    while (1)
    {
        if(cnt==NUMBER_OF_MEASUREMENT){
        break;
        }
        

        #if POLLING
        /*ADC triggered by Software*/
        ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
        
        /* Polling to see if ADC conversion has been done.*/
        while (0U == (kADC16_ChannelConversionDoneFlag &
                      ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
        {
        }
        g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
        #endif

        #if INTERRUPT
        g_Adc16ConversionDoneFlag = false;
        /*ADC triggered by Software*/
        ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);

        /* Enter Partial Stop mode and wait for ADC conversion */
        /* Enter Very low power stop */
        SMC_PreEnterStopModes();
        SMC_SetPowerModeStop(SMC, kSMC_PartialStop);
        SMC_PostExitStopModes();

        while(!g_Adc16ConversionDoneFlag);
        #endif
        
        data[cnt]= g_Adc16ConversionValue;
                 
        cnt++;

    }

    arm_max_f32(data,n,&max,&max_index); // CMSIS-DSP library
    arm_min_f32(data,n,&min,&min_index); // CMSIS-DSP library
    PRINTF("Max value Data[%d]=%f\r\n", max_index, max);
    PRINTF("Min value Data[%d]=%f\r\n", min_index, min);

    arm_mean_f32(data,n,&avg); // Average of measured temp sensor.
    std = dev(data,n);
    PRINTF("Average: %f\r\n",avg);
    PRINTF("Standard deviation: %e\r\n",std);
    
    while(1); //end
}
