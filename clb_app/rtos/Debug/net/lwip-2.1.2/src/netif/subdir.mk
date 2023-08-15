################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../net/lwip-2.1.2/src/netif/bridgeif.c \
../net/lwip-2.1.2/src/netif/bridgeif_fdb.c \
../net/lwip-2.1.2/src/netif/ethernet.c \
../net/lwip-2.1.2/src/netif/ethernetif.c \
../net/lwip-2.1.2/src/netif/lowpan6.c \
../net/lwip-2.1.2/src/netif/lowpan6_ble.c \
../net/lwip-2.1.2/src/netif/lowpan6_common.c \
../net/lwip-2.1.2/src/netif/slipif.c \
../net/lwip-2.1.2/src/netif/zepif.c 

OBJS += \
./net/lwip-2.1.2/src/netif/bridgeif.o \
./net/lwip-2.1.2/src/netif/bridgeif_fdb.o \
./net/lwip-2.1.2/src/netif/ethernet.o \
./net/lwip-2.1.2/src/netif/ethernetif.o \
./net/lwip-2.1.2/src/netif/lowpan6.o \
./net/lwip-2.1.2/src/netif/lowpan6_ble.o \
./net/lwip-2.1.2/src/netif/lowpan6_common.o \
./net/lwip-2.1.2/src/netif/slipif.o \
./net/lwip-2.1.2/src/netif/zepif.o 

C_DEPS += \
./net/lwip-2.1.2/src/netif/bridgeif.d \
./net/lwip-2.1.2/src/netif/bridgeif_fdb.d \
./net/lwip-2.1.2/src/netif/ethernet.d \
./net/lwip-2.1.2/src/netif/ethernetif.d \
./net/lwip-2.1.2/src/netif/lowpan6.d \
./net/lwip-2.1.2/src/netif/lowpan6_ble.d \
./net/lwip-2.1.2/src/netif/lowpan6_common.d \
./net/lwip-2.1.2/src/netif/slipif.d \
./net/lwip-2.1.2/src/netif/zepif.d 


# Each subdirectory must supply rules for building sources it contributes
net/lwip-2.1.2/src/netif/%.o net/lwip-2.1.2/src/netif/%.su net/lwip-2.1.2/src/netif/%.cyclo: ../net/lwip-2.1.2/src/netif/%.c net/lwip-2.1.2/src/netif/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -D__FPU_PRESENT -DARM_MATH_CM4 -DHAVE_CCONFIG_H -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_cominform_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_ad7606_data" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_singlecast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_board_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_multicast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_send_record" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_protocol_file" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_utility" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/dsp/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/algorithm/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/itc" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/net/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/ini" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/module/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Program/STM32CubeIDE/clb_app/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/cortex-m4" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/finsh" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/newlib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/api/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/drivers/spi" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/dfs/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/devfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/elmfat" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/romfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/sdcard" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/netif" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/compat/posix" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netdev/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/drv_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/tfdsn/io" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/ntp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/tftp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/algorithm_lib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_diagnosis" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_tftp_up_raw" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_channel_switch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_ad7606" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_self_test" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_iwdg" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-netif

clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-netif:
	-$(RM) ./net/lwip-2.1.2/src/netif/bridgeif.cyclo ./net/lwip-2.1.2/src/netif/bridgeif.d ./net/lwip-2.1.2/src/netif/bridgeif.o ./net/lwip-2.1.2/src/netif/bridgeif.su ./net/lwip-2.1.2/src/netif/bridgeif_fdb.cyclo ./net/lwip-2.1.2/src/netif/bridgeif_fdb.d ./net/lwip-2.1.2/src/netif/bridgeif_fdb.o ./net/lwip-2.1.2/src/netif/bridgeif_fdb.su ./net/lwip-2.1.2/src/netif/ethernet.cyclo ./net/lwip-2.1.2/src/netif/ethernet.d ./net/lwip-2.1.2/src/netif/ethernet.o ./net/lwip-2.1.2/src/netif/ethernet.su ./net/lwip-2.1.2/src/netif/ethernetif.cyclo ./net/lwip-2.1.2/src/netif/ethernetif.d ./net/lwip-2.1.2/src/netif/ethernetif.o ./net/lwip-2.1.2/src/netif/ethernetif.su ./net/lwip-2.1.2/src/netif/lowpan6.cyclo ./net/lwip-2.1.2/src/netif/lowpan6.d ./net/lwip-2.1.2/src/netif/lowpan6.o ./net/lwip-2.1.2/src/netif/lowpan6.su ./net/lwip-2.1.2/src/netif/lowpan6_ble.cyclo ./net/lwip-2.1.2/src/netif/lowpan6_ble.d ./net/lwip-2.1.2/src/netif/lowpan6_ble.o ./net/lwip-2.1.2/src/netif/lowpan6_ble.su ./net/lwip-2.1.2/src/netif/lowpan6_common.cyclo ./net/lwip-2.1.2/src/netif/lowpan6_common.d ./net/lwip-2.1.2/src/netif/lowpan6_common.o ./net/lwip-2.1.2/src/netif/lowpan6_common.su ./net/lwip-2.1.2/src/netif/slipif.cyclo ./net/lwip-2.1.2/src/netif/slipif.d ./net/lwip-2.1.2/src/netif/slipif.o ./net/lwip-2.1.2/src/netif/slipif.su ./net/lwip-2.1.2/src/netif/zepif.cyclo ./net/lwip-2.1.2/src/netif/zepif.d ./net/lwip-2.1.2/src/netif/zepif.o ./net/lwip-2.1.2/src/netif/zepif.su

.PHONY: clean-net-2f-lwip-2d-2-2e-1-2e-2-2f-src-2f-netif

