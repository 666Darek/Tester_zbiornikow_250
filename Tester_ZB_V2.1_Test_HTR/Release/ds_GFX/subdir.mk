################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ds_GFX/ds_GFX.c \
../ds_GFX/font1.c 

OBJS += \
./ds_GFX/ds_GFX.o \
./ds_GFX/font1.o 

C_DEPS += \
./ds_GFX/ds_GFX.d \
./ds_GFX/font1.d 


# Each subdirectory must supply rules for building sources it contributes
ds_GFX/%.o: ../ds_GFX/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


