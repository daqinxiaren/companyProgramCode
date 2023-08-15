################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -D__FPU_PRESENT -DARM_MATH_CM4 -DHAVE_CCONFIG_H -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_cominform_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_ad7606_data" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_singlecast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_board_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_multicast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_send_record" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_protocol_file" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_utility" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/dsp/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/algorithm/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/itc" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/net/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/ini" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/module/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Program/STM32CubeIDE/clb_app/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/cortex-m4" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/finsh" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/newlib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/api/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/drivers/spi" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/dfs/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/devfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/elmfat" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/romfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/sdcard" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/netif" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/compat/posix" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netdev/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/drv_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/tfdsn/io" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/ntp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/tftp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/algorithm_lib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_diagnosis" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_tftp_up_raw" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_channel_switch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_ad7606" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_self_test" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_iwdg" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

