################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c \
../utilities/fsl_io.c \
../utilities/fsl_log.c \
../utilities/fsl_str.c 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o \
./utilities/fsl_io.o \
./utilities/fsl_log.o \
./utilities/fsl_str.o 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d \
./utilities/fsl_io.d \
./utilities/fsl_log.d \
./utilities/fsl_str.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DFRDM_K64F -DFREEDOM -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -DSDK_DEBUGCONSOLE=0 -DARM_MATH_CM4 -D__FPU_PRESENT=1 -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/source" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/drivers" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/utilities" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/board" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/CMSIS" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/startup" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


