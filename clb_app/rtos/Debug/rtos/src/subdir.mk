################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rtos/src/clock.c \
../rtos/src/components.c \
../rtos/src/cpu.c \
../rtos/src/device.c \
../rtos/src/idle.c \
../rtos/src/ipc.c \
../rtos/src/irq.c \
../rtos/src/kservice.c \
../rtos/src/mem.c \
../rtos/src/memheap.c \
../rtos/src/mempool.c \
../rtos/src/object.c \
../rtos/src/scheduler.c \
../rtos/src/signal.c \
../rtos/src/slab.c \
../rtos/src/thread.c \
../rtos/src/timer.c 

OBJS += \
./rtos/src/clock.o \
./rtos/src/components.o \
./rtos/src/cpu.o \
./rtos/src/device.o \
./rtos/src/idle.o \
./rtos/src/ipc.o \
./rtos/src/irq.o \
./rtos/src/kservice.o \
./rtos/src/mem.o \
./rtos/src/memheap.o \
./rtos/src/mempool.o \
./rtos/src/object.o \
./rtos/src/scheduler.o \
./rtos/src/signal.o \
./rtos/src/slab.o \
./rtos/src/thread.o \
./rtos/src/timer.o 

C_DEPS += \
./rtos/src/clock.d \
./rtos/src/components.d \
./rtos/src/cpu.d \
./rtos/src/device.d \
./rtos/src/idle.d \
./rtos/src/ipc.d \
./rtos/src/irq.d \
./rtos/src/kservice.d \
./rtos/src/mem.d \
./rtos/src/memheap.d \
./rtos/src/mempool.d \
./rtos/src/object.d \
./rtos/src/scheduler.d \
./rtos/src/signal.d \
./rtos/src/slab.d \
./rtos/src/thread.d \
./rtos/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
rtos/src/%.o rtos/src/%.su rtos/src/%.cyclo: ../rtos/src/%.c rtos/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -D__FPU_PRESENT -DARM_MATH_CM4 -DHAVE_CCONFIG_H -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_cominform_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_ad7606_data" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_singlecast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_board_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_multicast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_send_record" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_protocol_file" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_utility" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/dsp/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/algorithm/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/itc" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/net/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/ini" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/module/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Program/STM32CubeIDE/clb_app/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/cortex-m4" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/finsh" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/newlib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/api/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/drivers/spi" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/dfs/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/devfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/elmfat" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/romfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/sdcard" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/netif" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/compat/posix" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netdev/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/drv_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/tfdsn/io" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/ntp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/tftp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/algorithm_lib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_diagnosis" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_tftp_up_raw" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_channel_switch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_ad7606" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_self_test" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_iwdg" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-rtos-2f-src

clean-rtos-2f-src:
	-$(RM) ./rtos/src/clock.cyclo ./rtos/src/clock.d ./rtos/src/clock.o ./rtos/src/clock.su ./rtos/src/components.cyclo ./rtos/src/components.d ./rtos/src/components.o ./rtos/src/components.su ./rtos/src/cpu.cyclo ./rtos/src/cpu.d ./rtos/src/cpu.o ./rtos/src/cpu.su ./rtos/src/device.cyclo ./rtos/src/device.d ./rtos/src/device.o ./rtos/src/device.su ./rtos/src/idle.cyclo ./rtos/src/idle.d ./rtos/src/idle.o ./rtos/src/idle.su ./rtos/src/ipc.cyclo ./rtos/src/ipc.d ./rtos/src/ipc.o ./rtos/src/ipc.su ./rtos/src/irq.cyclo ./rtos/src/irq.d ./rtos/src/irq.o ./rtos/src/irq.su ./rtos/src/kservice.cyclo ./rtos/src/kservice.d ./rtos/src/kservice.o ./rtos/src/kservice.su ./rtos/src/mem.cyclo ./rtos/src/mem.d ./rtos/src/mem.o ./rtos/src/mem.su ./rtos/src/memheap.cyclo ./rtos/src/memheap.d ./rtos/src/memheap.o ./rtos/src/memheap.su ./rtos/src/mempool.cyclo ./rtos/src/mempool.d ./rtos/src/mempool.o ./rtos/src/mempool.su ./rtos/src/object.cyclo ./rtos/src/object.d ./rtos/src/object.o ./rtos/src/object.su ./rtos/src/scheduler.cyclo ./rtos/src/scheduler.d ./rtos/src/scheduler.o ./rtos/src/scheduler.su ./rtos/src/signal.cyclo ./rtos/src/signal.d ./rtos/src/signal.o ./rtos/src/signal.su ./rtos/src/slab.cyclo ./rtos/src/slab.d ./rtos/src/slab.o ./rtos/src/slab.su ./rtos/src/thread.cyclo ./rtos/src/thread.d ./rtos/src/thread.o ./rtos/src/thread.su ./rtos/src/timer.cyclo ./rtos/src/timer.d ./rtos/src/timer.o ./rtos/src/timer.su

.PHONY: clean-rtos-2f-src

