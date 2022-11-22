################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32l4a6zgtx.S 

OBJS += \
./startup/startup_stm32l4a6zgtx.o 

S_UPPER_DEPS += \
./startup/startup_stm32l4a6zgtx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I../startup -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_stm32l4a6zgtx.d ./startup/startup_stm32l4a6zgtx.o

.PHONY: clean-startup

