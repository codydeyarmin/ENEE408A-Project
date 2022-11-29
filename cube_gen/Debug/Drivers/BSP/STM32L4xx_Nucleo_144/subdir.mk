################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.c 

OBJS += \
./Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.o 

C_DEPS += \
./Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32L4xx_Nucleo_144/%.o Drivers/BSP/STM32L4xx_Nucleo_144/%.su: ../Drivers/BSP/STM32L4xx_Nucleo_144/%.c Drivers/BSP/STM32L4xx_Nucleo_144/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4A6xx -c -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/BSP/STM32L4xx_Nucleo_144/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -O0 -ffunction-sections -fdata-sections -Wall --specs=nosys.specs -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32L4xx_Nucleo_144

clean-Drivers-2f-BSP-2f-STM32L4xx_Nucleo_144:
	-$(RM) ./Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.d ./Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.o ./Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32L4xx_Nucleo_144

