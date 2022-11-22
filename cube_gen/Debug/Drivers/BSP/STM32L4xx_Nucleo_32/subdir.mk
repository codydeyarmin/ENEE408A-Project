################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.c 

OBJS += \
./Drivers/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.o 

C_DEPS += \
./Drivers/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32L4xx_Nucleo_32/%.o Drivers/BSP/STM32L4xx_Nucleo_32/%.su: ../Drivers/BSP/STM32L4xx_Nucleo_32/%.c Drivers/BSP/STM32L4xx_Nucleo_32/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4A6xx -c -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32L4xx_Nucleo_32

clean-Drivers-2f-BSP-2f-STM32L4xx_Nucleo_32:
	-$(RM) ./Drivers/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.d ./Drivers/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.o ./Drivers/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32L4xx_Nucleo_32

