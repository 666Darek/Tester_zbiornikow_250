################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ds_GFX/subdir.mk
-include DS1Wire/subdir.mk
-include ADC/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Tester_ZB_V2.1_Test_HTR.lss \

FLASH_IMAGE += \
Tester_ZB_V2.1_Test_HTR.hex \

EEPROM_IMAGE += \
Tester_ZB_V2.1_Test_HTR.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Tester_ZB_V2.1_Test_HTR.elf secondary-outputs

# Tool invocations
Tester_ZB_V2.1_Test_HTR.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Tester_ZB_V2.1_Test_HTR.map -mmcu=atmega128 -o "Tester_ZB_V2.1_Test_HTR.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Tester_ZB_V2.1_Test_HTR.lss: Tester_ZB_V2.1_Test_HTR.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Tester_ZB_V2.1_Test_HTR.elf  >"Tester_ZB_V2.1_Test_HTR.lss"
	@echo 'Finished building: $@'
	@echo ' '

Tester_ZB_V2.1_Test_HTR.hex: Tester_ZB_V2.1_Test_HTR.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Tester_ZB_V2.1_Test_HTR.elf  "Tester_ZB_V2.1_Test_HTR.hex"
	@echo 'Finished building: $@'
	@echo ' '

Tester_ZB_V2.1_Test_HTR.eep: Tester_ZB_V2.1_Test_HTR.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Tester_ZB_V2.1_Test_HTR.elf  "Tester_ZB_V2.1_Test_HTR.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Tester_ZB_V2.1_Test_HTR.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128 Tester_ZB_V2.1_Test_HTR.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Tester_ZB_V2.1_Test_HTR.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
