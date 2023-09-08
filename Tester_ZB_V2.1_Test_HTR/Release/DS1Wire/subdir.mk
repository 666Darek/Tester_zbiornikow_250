################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DS1Wire/ds_1wire.c 

OBJS += \
./DS1Wire/ds_1wire.o 

C_DEPS += \
./DS1Wire/ds_1wire.d 


# Each subdirectory must supply rules for building sources it contributes
DS1Wire/%.o: ../DS1Wire/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


