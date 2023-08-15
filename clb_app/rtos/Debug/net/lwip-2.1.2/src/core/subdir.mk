################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../net/lwip-2.1.2/src/core/altcp.c \
../net/lwip-2.1.2/src/core/altcp_alloc.c \
../net/lwip-2.1.2/src/core/altcp_tcp.c \
../net/lwip-2.1.2/src/core/def.c \
../net/lwip-2.1.2/src/core/dns.c \
../net/lwip-2.1.2/src/core/inet_chksum.c \
../net/lwip-2.1.2/src/core/init.c \
../net/lwip-2.1.2/src/core/ip.c \
../net/lwip-2.1.2/src/core/memp.c \
../net/lwip-2.1.2/src/core/netif.c \
../net/lwip-2.1.2/src/core/pbuf.c \
../net/lwip-2.1.2/src/core/raw.c \
../net/lwip-2.1.2/src/core/stats.c \
../net/lwip-2.1.2/src/core/sys.c \
../net/lwip-2.1.2/src/core/tcp.c \
../net/lwip-2.1.2/src/core/tcp_in.c \
../net/lwip-2.1.2/src/core/tcp_out.c \
../net/lwip-2.1.2/src/core/timeouts.c \
../net/lwip-2.1.2/src/core/udp.c 

OBJS += \
./net/lwip-2.1.2/src/core/altcp.o \
./net/lwip-2.1.2/src/core/altcp_alloc.o \
./net/lwip-2.1.2/src/core/altcp_tcp.o \
./net/lwip-2.1.2/src/core/def.o \
./net/lwip-2.1.2/src/core/dns.o \
./net/lwip-2.1.2/src/core/inet_chksum.o \
./net/lwip-2.1.2/src/core/init.o \
./net/lwip-2.1.2/src/core/ip.o \
./net/lwip-2.1.2/src/core/memp.o \
./net/lwip-2.1.2/src/core/netif.o \
./net/lwip-2.1.2/src/core/pbuf.o \
./net/lwip-2.1.2/src/core/raw.o \
./net/lwip-2.1.2/src/core/stats.o \
./net/lwip-2.1.2/src/core/sys.o \
./net/lwip-2.1.2/src/core/tcp.o \
./net/lwip-2.1.2/src/core/tcp_in.o \
./net/lwip-2.1.2/src/core/tcp_out.o \
./net/lwip-2.1.2/src/core/timeouts.o \
./net/lwip-2.1.2/src/core/udp.o 

C_DEPS += \
./net/lwip-2.1.2/src/core/altcp.d \
./net/lwip-2.1.2/src/core/altcp_alloc.d \
./net/lwip-2.1.2/src/core/altcp_tcp.d \
./net/lwip-2.1.2/src/core/def.d \
./net/lwip-2.1.2/src/core/dns.d \
./net/lwip-2.1.2/src/core/inet_chksum.d \
./net/lwip-2.1.2/src/core/init.d \
./net/lwip-2.1.2/src/core/ip.d \
./net/lwip-2.1.2/src/core/memp.d \
./net/lwip-2.1.2/src/core/netif.d \
./net/lwip-2.1.2/src/core/pbuf.d \
./net/lwip-2.1.2/src/core/raw.d \
./net/lwip-2.1.2/src/core/stats.d \
./net/lwip-2.1.2/src/core/sys.d \
./net/lwip-2.1.2/src/core/tcp.d \
./net/lwip-2.1.2/src/core/tcp_in.d \
./net/lwip-2.1.2/src/core/tcp_out.d \
./net/lwip-2.1.2/src/core/timeouts.d \
./net/lwip-2.1.2/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
net/lwip-2.1.2/src/core/%.o net/lwip-2.1.2/src/core/%.su net/lwip-2.1.2/src/core/%.cyclo: ../net/lwip-2.1.2/src/core/%.c net/lwip-2.1.2/src/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -D__FPU_PRESENT -DARM_MATH_CM4 -DHAVE_CCONFIG_H -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_cominform_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_ad7606_data" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_singlecast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_board_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_multicast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_send_record" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_protocol_file" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_utility" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/dsp/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/algorithm/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/itc" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/net/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/ini" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/module/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Program/STM32CubeIDE/clb_app/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/cortex-m4" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/finsh" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/newlib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/api/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/drivers/spi" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/dfs/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/devfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/elmfat" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/romfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/sdcard" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/netif" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/compat/posix" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netdev/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/drv_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/tfdsn/io" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/ntp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/tftp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/algorithm_lib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_diagnosis" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_tftp_up_raw" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_channel_switch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_ad7606" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_self_test" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_iwdg" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-core

clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-core:
	-$(RM) ./net/lwip-2.1.2/src/core/altcp.cyclo ./net/lwip-2.1.2/src/core/altcp.d ./net/lwip-2.1.2/src/core/altcp.o ./net/lwip-2.1.2/src/core/altcp.su ./net/lwip-2.1.2/src/core/altcp_alloc.cyclo ./net/lwip-2.1.2/src/core/altcp_alloc.d ./net/lwip-2.1.2/src/core/altcp_alloc.o ./net/lwip-2.1.2/src/core/altcp_alloc.su ./net/lwip-2.1.2/src/core/altcp_tcp.cyclo ./net/lwip-2.1.2/src/core/altcp_tcp.d ./net/lwip-2.1.2/src/core/altcp_tcp.o ./net/lwip-2.1.2/src/core/altcp_tcp.su ./net/lwip-2.1.2/src/core/def.cyclo ./net/lwip-2.1.2/src/core/def.d ./net/lwip-2.1.2/src/core/def.o ./net/lwip-2.1.2/src/core/def.su ./net/lwip-2.1.2/src/core/dns.cyclo ./net/lwip-2.1.2/src/core/dns.d ./net/lwip-2.1.2/src/core/dns.o ./net/lwip-2.1.2/src/core/dns.su ./net/lwip-2.1.2/src/core/inet_chksum.cyclo ./net/lwip-2.1.2/src/core/inet_chksum.d ./net/lwip-2.1.2/src/core/inet_chksum.o ./net/lwip-2.1.2/src/core/inet_chksum.su ./net/lwip-2.1.2/src/core/init.cyclo ./net/lwip-2.1.2/src/core/init.d ./net/lwip-2.1.2/src/core/init.o ./net/lwip-2.1.2/src/core/init.su ./net/lwip-2.1.2/src/core/ip.cyclo ./net/lwip-2.1.2/src/core/ip.d ./net/lwip-2.1.2/src/core/ip.o ./net/lwip-2.1.2/src/core/ip.su ./net/lwip-2.1.2/src/core/memp.cyclo ./net/lwip-2.1.2/src/core/memp.d ./net/lwip-2.1.2/src/core/memp.o ./net/lwip-2.1.2/src/core/memp.su ./net/lwip-2.1.2/src/core/netif.cyclo ./net/lwip-2.1.2/src/core/netif.d ./net/lwip-2.1.2/src/core/netif.o ./net/lwip-2.1.2/src/core/netif.su ./net/lwip-2.1.2/src/core/pbuf.cyclo ./net/lwip-2.1.2/src/core/pbuf.d ./net/lwip-2.1.2/src/core/pbuf.o ./net/lwip-2.1.2/src/core/pbuf.su ./net/lwip-2.1.2/src/core/raw.cyclo ./net/lwip-2.1.2/src/core/raw.d ./net/lwip-2.1.2/src/core/raw.o ./net/lwip-2.1.2/src/core/raw.su ./net/lwip-2.1.2/src/core/stats.cyclo ./net/lwip-2.1.2/src/core/stats.d ./net/lwip-2.1.2/src/core/stats.o ./net/lwip-2.1.2/src/core/stats.su ./net/lwip-2.1.2/src/core/sys.cyclo ./net/lwip-2.1.2/src/core/sys.d ./net/lwip-2.1.2/src/core/sys.o ./net/lwip-2.1.2/src/core/sys.su ./net/lwip-2.1.2/src/core/tcp.cyclo ./net/lwip-2.1.2/src/core/tcp.d ./net/lwip-2.1.2/src/core/tcp.o ./net/lwip-2.1.2/src/core/tcp.su ./net/lwip-2.1.2/src/core/tcp_in.cyclo ./net/lwip-2.1.2/src/core/tcp_in.d ./net/lwip-2.1.2/src/core/tcp_in.o ./net/lwip-2.1.2/src/core/tcp_in.su ./net/lwip-2.1.2/src/core/tcp_out.cyclo ./net/lwip-2.1.2/src/core/tcp_out.d ./net/lwip-2.1.2/src/core/tcp_out.o ./net/lwip-2.1.2/src/core/tcp_out.su ./net/lwip-2.1.2/src/core/timeouts.cyclo ./net/lwip-2.1.2/src/core/timeouts.d ./net/lwip-2.1.2/src/core/timeouts.o ./net/lwip-2.1.2/src/core/timeouts.su ./net/lwip-2.1.2/src/core/udp.cyclo ./net/lwip-2.1.2/src/core/udp.d ./net/lwip-2.1.2/src/core/udp.o ./net/lwip-2.1.2/src/core/udp.su

.PHONY: clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-core

