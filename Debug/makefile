################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti5.3/ccsv5/tools/compiler/c6000_7.4.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./srio.obj" \
"./signalProcLC2.obj" \
"./signalProcLC1.obj" \
"./signalProc.obj" \
"./setupCPU6678.obj" \
"./resourcemgr.obj" \
"./reset.obj" \
"./platform_osal.obj" \
"./mycore5.obj" \
"./mycore4.obj" \
"./mycore3.obj" \
"./mycore2.obj" \
"./mathcoord.obj" \
"./interrupt.obj" \
"./gpio.obj" \
"./flasshdata.obj" \
"./flash_programm.obj" \
"./filterLC2.obj" \
"./filterLC1.obj" \
"./filter.obj" \
"./evmc66x_spi.obj" \
"./emif16.obj" \
"./device_srio_osal.obj" \
"./device_srio_loopback.obj" \
"./client.obj" \
"./alg_add2ch.obj" \
"./alg_add1ch.obj" \
"./alg_add.obj" \
"./InitCache.obj" \
"./EthernetJob.obj" \
"./BattleJob.obj" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: P_6CA_Reliz1AddSinxBagVideo.out

# Tool invocations
P_6CA_Reliz1AddSinxBagVideo.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti5.3/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --display_error_number --diag_warning=225 -z -m"P_6CA_Reliz1AddSinxBagVideo.map" -i"C:/ti5.3/ccsv5/tools/compiler/c6000_7.4.1/lib" -i"C:/ti5.3/ccsv5/tools/compiler/c6000_7.4.1/include" --reread_libs --warn_sections --rom_model -o "P_6CA_Reliz1AddSinxBagVideo.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_SRCS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(C6000_EXECUTABLE_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED) "P_6CA_Reliz1AddSinxBagVideo.out"
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "BattleJob.pp" "EthernetJob.pp" "InitCache.pp" "alg_add.pp" "alg_add1ch.pp" "alg_add2ch.pp" "client.pp" "device_srio_loopback.pp" "device_srio_osal.pp" "emif16.pp" "evmc66x_spi.pp" "filter.pp" "filterLC1.pp" "filterLC2.pp" "flash_programm.pp" "flasshdata.pp" "gpio.pp" "interrupt.pp" "mathcoord.pp" "mycore2.pp" "mycore3.pp" "mycore4.pp" "mycore5.pp" "platform_osal.pp" "reset.pp" "resourcemgr.pp" "setupCPU6678.pp" "signalProc.pp" "signalProcLC1.pp" "signalProcLC2.pp" "srio.pp" 
	-$(RM) "BattleJob.obj" "EthernetJob.obj" "InitCache.obj" "alg_add.obj" "alg_add1ch.obj" "alg_add2ch.obj" "client.obj" "device_srio_loopback.obj" "device_srio_osal.obj" "emif16.obj" "evmc66x_spi.obj" "filter.obj" "filterLC1.obj" "filterLC2.obj" "flash_programm.obj" "flasshdata.obj" "gpio.obj" "interrupt.obj" "mathcoord.obj" "mycore2.obj" "mycore3.obj" "mycore4.obj" "mycore5.obj" "platform_osal.obj" "reset.obj" "resourcemgr.obj" "setupCPU6678.obj" "signalProc.obj" "signalProcLC1.obj" "signalProcLC2.obj" "srio.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
