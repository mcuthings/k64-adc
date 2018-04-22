################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc16.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_flash.c \
../drivers/fsl_gpio.c \
../drivers/fsl_smc.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_adc16.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_flash.o \
./drivers/fsl_gpio.o \
./drivers/fsl_smc.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_adc16.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_flash.d \
./drivers/fsl_gpio.d \
./drivers/fsl_smc.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DFRDM_K64F -DFREEDOM -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -DSDK_DEBUGCONSOLE=0 -DARM_MATH_CM4 -D__FPU_PRESENT=1 -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/source" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/drivers" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/utilities" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/board" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/CMSIS" -I"/Users/Hiro-MBP/Freescale/workspaces/k64/frdmk64f_driver_examples_adc16_polling_adc16_polling/startup" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


