################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usr/finsh/cmd.c \
../usr/finsh/finsh_compiler.c \
../usr/finsh/finsh_error.c \
../usr/finsh/finsh_heap.c \
../usr/finsh/finsh_init.c \
../usr/finsh/finsh_node.c \
../usr/finsh/finsh_ops.c \
../usr/finsh/finsh_parser.c \
../usr/finsh/finsh_token.c \
../usr/finsh/finsh_var.c \
../usr/finsh/finsh_vm.c \
../usr/finsh/msh.c \
../usr/finsh/msh_file.c \
../usr/finsh/shell.c \
../usr/finsh/symbol.c 

OBJS += \
./usr/finsh/cmd.o \
./usr/finsh/finsh_compiler.o \
./usr/finsh/finsh_error.o \
./usr/finsh/finsh_heap.o \
./usr/finsh/finsh_init.o \
./usr/finsh/finsh_node.o \
./usr/finsh/finsh_ops.o \
./usr/finsh/finsh_parser.o \
./usr/finsh/finsh_token.o \
./usr/finsh/finsh_var.o \
./usr/finsh/finsh_vm.o \
./usr/finsh/msh.o \
./usr/finsh/msh_file.o \
./usr/finsh/shell.o \
./usr/finsh/symbol.o 

C_DEPS += \
./usr/finsh/cmd.d \
./usr/finsh/finsh_compiler.d \
./usr/finsh/finsh_error.d \
./usr/finsh/finsh_heap.d \
./usr/finsh/finsh_init.d \
./usr/finsh/finsh_node.d \
./usr/finsh/finsh_ops.d \
./usr/finsh/finsh_parser.d \
./usr/finsh/finsh_token.d \
./usr/finsh/finsh_var.d \
./usr/finsh/finsh_vm.d \
./usr/finsh/msh.d \
./usr/finsh/msh_file.d \
./usr/finsh/shell.d \
./usr/finsh/symbol.d 


# Each subdirectory must supply rules for building sources it contributes
usr/finsh/%.o usr/finsh/%.su usr/finsh/%.cyclo: ../usr/finsh/%.c usr/finsh/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -D__FPU_PRESENT -DARM_MATH_CM4 -DHAVE_CCONFIG_H -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_cominform_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_ad7606_data" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_singlecast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_board_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_multicast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_send_record" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_protocol_file" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_utility" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/dsp/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/algorithm/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/itc" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/net/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/ini" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/module/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Program/STM32CubeIDE/clb_app/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/cortex-m4" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/finsh" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/newlib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/api/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/drivers/spi" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/dfs/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/devfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/elmfat" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/romfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/sdcard" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/netif" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/compat/posix" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netdev/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/drv_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/tfdsn/io" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/ntp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/tftp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/algorithm_lib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_diagnosis" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_tftp_up_raw" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_channel_switch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_ad7606" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_self_test" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_iwdg" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usr-2f-finsh

clean-usr-2f-finsh:
	-$(RM) ./usr/finsh/cmd.cyclo ./usr/finsh/cmd.d ./usr/finsh/cmd.o ./usr/finsh/cmd.su ./usr/finsh/finsh_compiler.cyclo ./usr/finsh/finsh_compiler.d ./usr/finsh/finsh_compiler.o ./usr/finsh/finsh_compiler.su ./usr/finsh/finsh_error.cyclo ./usr/finsh/finsh_error.d ./usr/finsh/finsh_error.o ./usr/finsh/finsh_error.su ./usr/finsh/finsh_heap.cyclo ./usr/finsh/finsh_heap.d ./usr/finsh/finsh_heap.o ./usr/finsh/finsh_heap.su ./usr/finsh/finsh_init.cyclo ./usr/finsh/finsh_init.d ./usr/finsh/finsh_init.o ./usr/finsh/finsh_init.su ./usr/finsh/finsh_node.cyclo ./usr/finsh/finsh_node.d ./usr/finsh/finsh_node.o ./usr/finsh/finsh_node.su ./usr/finsh/finsh_ops.cyclo ./usr/finsh/finsh_ops.d ./usr/finsh/finsh_ops.o ./usr/finsh/finsh_ops.su ./usr/finsh/finsh_parser.cyclo ./usr/finsh/finsh_parser.d ./usr/finsh/finsh_parser.o ./usr/finsh/finsh_parser.su ./usr/finsh/finsh_token.cyclo ./usr/finsh/finsh_token.d ./usr/finsh/finsh_token.o ./usr/finsh/finsh_token.su ./usr/finsh/finsh_var.cyclo ./usr/finsh/finsh_var.d ./usr/finsh/finsh_var.o ./usr/finsh/finsh_var.su ./usr/finsh/finsh_vm.cyclo ./usr/finsh/finsh_vm.d ./usr/finsh/finsh_vm.o ./usr/finsh/finsh_vm.su ./usr/finsh/msh.cyclo ./usr/finsh/msh.d ./usr/finsh/msh.o ./usr/finsh/msh.su ./usr/finsh/msh_file.cyclo ./usr/finsh/msh_file.d ./usr/finsh/msh_file.o ./usr/finsh/msh_file.su ./usr/finsh/shell.cyclo ./usr/finsh/shell.d ./usr/finsh/shell.o ./usr/finsh/shell.su ./usr/finsh/symbol.cyclo ./usr/finsh/symbol.d ./usr/finsh/symbol.o ./usr/finsh/symbol.su

.PHONY: clean-usr-2f-finsh

