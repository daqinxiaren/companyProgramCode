################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../net/lwip-2.1.2/src/core/ipv4/autoip.c \
../net/lwip-2.1.2/src/core/ipv4/dhcp.c \
../net/lwip-2.1.2/src/core/ipv4/etharp.c \
../net/lwip-2.1.2/src/core/ipv4/icmp.c \
../net/lwip-2.1.2/src/core/ipv4/igmp.c \
../net/lwip-2.1.2/src/core/ipv4/ip4.c \
../net/lwip-2.1.2/src/core/ipv4/ip4_addr.c \
../net/lwip-2.1.2/src/core/ipv4/ip4_frag.c 

OBJS += \
./net/lwip-2.1.2/src/core/ipv4/autoip.o \
./net/lwip-2.1.2/src/core/ipv4/dhcp.o \
./net/lwip-2.1.2/src/core/ipv4/etharp.o \
./net/lwip-2.1.2/src/core/ipv4/icmp.o \
./net/lwip-2.1.2/src/core/ipv4/igmp.o \
./net/lwip-2.1.2/src/core/ipv4/ip4.o \
./net/lwip-2.1.2/src/core/ipv4/ip4_addr.o \
./net/lwip-2.1.2/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./net/lwip-2.1.2/src/core/ipv4/autoip.d \
./net/lwip-2.1.2/src/core/ipv4/dhcp.d \
./net/lwip-2.1.2/src/core/ipv4/etharp.d \
./net/lwip-2.1.2/src/core/ipv4/icmp.d \
./net/lwip-2.1.2/src/core/ipv4/igmp.d \
./net/lwip-2.1.2/src/core/ipv4/ip4.d \
./net/lwip-2.1.2/src/core/ipv4/ip4_addr.d \
./net/lwip-2.1.2/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
net/lwip-2.1.2/src/core/ipv4/%.o net/lwip-2.1.2/src/core/ipv4/%.su net/lwip-2.1.2/src/core/ipv4/%.cyclo: ../net/lwip-2.1.2/src/core/ipv4/%.c net/lwip-2.1.2/src/core/ipv4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -D__FPU_PRESENT -DARM_MATH_CM4 -DHAVE_CCONFIG_H -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_cominform_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_ad7606_data" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_singlecast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_board_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_multicast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_send_record" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_protocol_file" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_utility" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/dsp/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/algorithm/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/itc" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/net/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/ini" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/module/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Program/STM32CubeIDE/clb_app/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/cortex-m4" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/finsh" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/newlib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/api/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/drivers/spi" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/dfs/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/devfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/elmfat" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/romfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/sdcard" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/netif" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/compat/posix" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netdev/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/drv_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/tfdsn/io" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/ntp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/tftp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/algorithm_lib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_diagnosis" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_tftp_up_raw" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_channel_switch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_ad7606" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_self_test" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_iwdg" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-core-2f-ipv4

clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-core-2f-ipv4:
	-$(RM) ./net/lwip-2.1.2/src/core/ipv4/autoip.cyclo ./net/lwip-2.1.2/src/core/ipv4/autoip.d ./net/lwip-2.1.2/src/core/ipv4/autoip.o ./net/lwip-2.1.2/src/core/ipv4/autoip.su ./net/lwip-2.1.2/src/core/ipv4/dhcp.cyclo ./net/lwip-2.1.2/src/core/ipv4/dhcp.d ./net/lwip-2.1.2/src/core/ipv4/dhcp.o ./net/lwip-2.1.2/src/core/ipv4/dhcp.su ./net/lwip-2.1.2/src/core/ipv4/etharp.cyclo ./net/lwip-2.1.2/src/core/ipv4/etharp.d ./net/lwip-2.1.2/src/core/ipv4/etharp.o ./net/lwip-2.1.2/src/core/ipv4/etharp.su ./net/lwip-2.1.2/src/core/ipv4/icmp.cyclo ./net/lwip-2.1.2/src/core/ipv4/icmp.d ./net/lwip-2.1.2/src/core/ipv4/icmp.o ./net/lwip-2.1.2/src/core/ipv4/icmp.su ./net/lwip-2.1.2/src/core/ipv4/igmp.cyclo ./net/lwip-2.1.2/src/core/ipv4/igmp.d ./net/lwip-2.1.2/src/core/ipv4/igmp.o ./net/lwip-2.1.2/src/core/ipv4/igmp.su ./net/lwip-2.1.2/src/core/ipv4/ip4.cyclo ./net/lwip-2.1.2/src/core/ipv4/ip4.d ./net/lwip-2.1.2/src/core/ipv4/ip4.o ./net/lwip-2.1.2/src/core/ipv4/ip4.su ./net/lwip-2.1.2/src/core/ipv4/ip4_addr.cyclo ./net/lwip-2.1.2/src/core/ipv4/ip4_addr.d ./net/lwip-2.1.2/src/core/ipv4/ip4_addr.o ./net/lwip-2.1.2/src/core/ipv4/ip4_addr.su ./net/lwip-2.1.2/src/core/ipv4/ip4_frag.cyclo ./net/lwip-2.1.2/src/core/ipv4/ip4_frag.d ./net/lwip-2.1.2/src/core/ipv4/ip4_frag.o ./net/lwip-2.1.2/src/core/ipv4/ip4_frag.su

.PHONY: clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-core-2f-ipv4

