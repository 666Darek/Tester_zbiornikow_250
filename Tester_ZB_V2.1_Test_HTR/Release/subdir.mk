################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common.c \
../initTests.c \
../main.c \
../ports.c \
../showTestInfo.c \
../t6963c.c \
../testy.c 

OBJS += \
./common.o \
./initTests.o \
./main.o \
./ports.o \
./showTestInfo.o \
./t6963c.o \
./testy.o 

C_DEPS += \
./common.d \
./initTests.d \
./main.d \
./ports.d \
./showTestInfo.d \
./t6963c.d \
./testy.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


