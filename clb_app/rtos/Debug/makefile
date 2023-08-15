################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include usr/ini/subdir.mk
-include usr/finsh/subdir.mk
-include rtos/src/subdir.mk
-include rtos/libcpu/arm/cortex-m4/subdir.mk
-include rtos/libcpu/arm/common/subdir.mk
-include net/netutils/tftp/subdir.mk
-include net/netutils/telnet/subdir.mk
-include net/netutils/tcpdump/subdir.mk
-include net/netutils/ping/subdir.mk
-include net/netutils/ntp/subdir.mk
-include net/netutils/netio/subdir.mk
-include net/netutils/iperf/subdir.mk
-include net/netdev/src/subdir.mk
-include net/lwip-2.1.2/src/netif/ppp/polarssl/subdir.mk
-include net/lwip-2.1.2/src/netif/ppp/subdir.mk
-include net/lwip-2.1.2/src/netif/subdir.mk
-include net/lwip-2.1.2/src/core/ipv6/subdir.mk
-include net/lwip-2.1.2/src/core/ipv4/subdir.mk
-include net/lwip-2.1.2/src/core/subdir.mk
-include net/lwip-2.1.2/src/arch/subdir.mk
-include net/lwip-2.1.2/src/apps/tftp/subdir.mk
-include net/lwip-2.1.2/src/apps/ping/subdir.mk
-include net/lwip-2.1.2/src/api/subdir.mk
-include lib/newlib/subdir.mk
-include lib/dsp/source/TransformFunctions/subdir.mk
-include lib/dsp/source/SupportFunctions/subdir.mk
-include lib/dsp/source/StatisticsFunctions/subdir.mk
-include lib/dsp/source/MatrixFunctions/subdir.mk
-include lib/dsp/source/FilteringFunctions/subdir.mk
-include lib/dsp/source/FastMathFunctions/subdir.mk
-include lib/dsp/source/ControllerFunctions/subdir.mk
-include lib/dsp/source/ComplexMathFunctions/subdir.mk
-include lib/dsp/source/CommonTables/subdir.mk
-include lib/dsp/source/BasicMathFunctions/subdir.mk
-include lib/common/subdir.mk
-include fs/romfs/subdir.mk
-include fs/elmfat/subdir.mk
-include fs/dfs/src/subdir.mk
-include fs/devfs/subdir.mk
-include bsp/stm32/drivers/drv_flash/subdir.mk
-include bsp/stm32/drivers/subdir.mk
-include bsp/internal_timer/subdir.mk
-include bsp/internal_led_control/subdir.mk
-include bsp/internal_flash/subdir.mk
-include bsp/internal_ad7606/subdir.mk
-include board/project/tfdsn/msp/cortex-m4/subdir.mk
-include board/net/subdir.mk
-include board/module/switch/subdir.mk
-include board/module/led/subdir.mk
-include board/module/iap/subdir.mk
-include board/module/diagnosis/subdir.mk
-include board/module/config/subdir.mk
-include board/module/can/subdir.mk
-include board/module/ad7606/subdir.mk
-include board/lh/util/subdir.mk
-include board/lh/itc/subdir.mk
-include board/lh/algorithm/subdir.mk
-include board/fs/subdir.mk
-include board/device/sram/subdir.mk
-include board/device/sdcard/subdir.mk
-include board/device/phy/subdir.mk
-include board/device/flash/subdir.mk
-include board/app/recv_test/net/subdir.mk
-include board/app/recv_test/subdir.mk
-include board/app/app_utility/subdir.mk
-include board/app/app_timer/subdir.mk
-include board/app/app_tftp_up_raw/subdir.mk
-include board/app/app_singlecast_thread/subdir.mk
-include board/app/app_send_record/subdir.mk
-include board/app/app_self_test/subdir.mk
-include board/app/app_pwm/subdir.mk
-include board/app/app_protocol_file/subdir.mk
-include board/app/app_paras_save/subdir.mk
-include board/app/app_paras_config/subdir.mk
-include board/app/app_multicast_thread/subdir.mk
-include board/app/app_led_control/subdir.mk
-include board/app/app_iwdg/subdir.mk
-include board/app/app_diagnosis/subdir.mk
-include board/app/app_data_save/subdir.mk
-include board/app/app_cominform_config/subdir.mk
-include board/app/app_channel_switch/subdir.mk
-include board/app/app_ad7606_data/subdir.mk
-include board/app/algorithm_lib/subdir.mk
-include board/app/subdir.mk
-include api/drivers/watchdog/subdir.mk
-include api/drivers/spi/sfud/src/subdir.mk
-include api/drivers/spi/subdir.mk
-include api/drivers/serial/subdir.mk
-include api/drivers/sdio/subdir.mk
-include api/drivers/rtc/subdir.mk
-include api/drivers/phy/subdir.mk
-include api/drivers/misc/subdir.mk
-include api/drivers/hwtimer/subdir.mk
-include api/drivers/can/subdir.mk
-include api/api/src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := rtos
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
rtos.elf \

MAP_FILES += \
rtos.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
rtos.list \

OBJCOPY_HEX += \
rtos.hex \

OBJCOPY_BIN += \
rtos.bin \


# All Target
all: main-build

# Main-build Target
main-build: rtos.elf secondary-outputs

# Tool invocations
rtos.elf rtos.map: $(OBJS) $(USER_OBJS) E:\Program\STM32CubeIDE\clb_app\rtos\board\base\cortex-m4\linker_scripts\link.lds makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "rtos.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"E:\Program\STM32CubeIDE\clb_app\rtos\board\base\cortex-m4\linker_scripts\link.lds" --specs=nosys.specs -Wl,-Map="rtos.map" -Wl,--gc-sections -static  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

rtos.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "rtos.list"
	@echo 'Finished building: $@'
	@echo ' '

rtos.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "rtos.hex"
	@echo 'Finished building: $@'
	@echo ' '

rtos.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "rtos.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout rtos.bin rtos.elf rtos.hex rtos.list rtos.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
