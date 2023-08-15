################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/stm32/drivers/drv_adc.c \
../bsp/stm32/drivers/drv_can.c \
../bsp/stm32/drivers/drv_clk.c \
../bsp/stm32/drivers/drv_common.c \
../bsp/stm32/drivers/drv_crypto.c \
../bsp/stm32/drivers/drv_dac.c \
../bsp/stm32/drivers/drv_eth.c \
../bsp/stm32/drivers/drv_gpio.c \
../bsp/stm32/drivers/drv_hwtimer.c \
../bsp/stm32/drivers/drv_lcd.c \
../bsp/stm32/drivers/drv_lcd_mipi.c \
../bsp/stm32/drivers/drv_lptim.c \
../bsp/stm32/drivers/drv_pm.c \
../bsp/stm32/drivers/drv_pulse_encoder.c \
../bsp/stm32/drivers/drv_pwm.c \
../bsp/stm32/drivers/drv_qspi.c \
../bsp/stm32/drivers/drv_rtc.c \
../bsp/stm32/drivers/drv_sdio.c \
../bsp/stm32/drivers/drv_sdram.c \
../bsp/stm32/drivers/drv_soft_i2c.c \
../bsp/stm32/drivers/drv_spi.c \
../bsp/stm32/drivers/drv_usart.c \
../bsp/stm32/drivers/drv_usbd.c \
../bsp/stm32/drivers/drv_usbh.c \
../bsp/stm32/drivers/drv_wdt.c 

OBJS += \
./bsp/stm32/drivers/drv_adc.o \
./bsp/stm32/drivers/drv_can.o \
./bsp/stm32/drivers/drv_clk.o \
./bsp/stm32/drivers/drv_common.o \
./bsp/stm32/drivers/drv_crypto.o \
./bsp/stm32/drivers/drv_dac.o \
./bsp/stm32/drivers/drv_eth.o \
./bsp/stm32/drivers/drv_gpio.o \
./bsp/stm32/drivers/drv_hwtimer.o \
./bsp/stm32/drivers/drv_lcd.o \
./bsp/stm32/drivers/drv_lcd_mipi.o \
./bsp/stm32/drivers/drv_lptim.o \
./bsp/stm32/drivers/drv_pm.o \
./bsp/stm32/drivers/drv_pulse_encoder.o \
./bsp/stm32/drivers/drv_pwm.o \
./bsp/stm32/drivers/drv_qspi.o \
./bsp/stm32/drivers/drv_rtc.o \
./bsp/stm32/drivers/drv_sdio.o \
./bsp/stm32/drivers/drv_sdram.o \
./bsp/stm32/drivers/drv_soft_i2c.o \
./bsp/stm32/drivers/drv_spi.o \
./bsp/stm32/drivers/drv_usart.o \
./bsp/stm32/drivers/drv_usbd.o \
./bsp/stm32/drivers/drv_usbh.o \
./bsp/stm32/drivers/drv_wdt.o 

C_DEPS += \
./bsp/stm32/drivers/drv_adc.d \
./bsp/stm32/drivers/drv_can.d \
./bsp/stm32/drivers/drv_clk.d \
./bsp/stm32/drivers/drv_common.d \
./bsp/stm32/drivers/drv_crypto.d \
./bsp/stm32/drivers/drv_dac.d \
./bsp/stm32/drivers/drv_eth.d \
./bsp/stm32/drivers/drv_gpio.d \
./bsp/stm32/drivers/drv_hwtimer.d \
./bsp/stm32/drivers/drv_lcd.d \
./bsp/stm32/drivers/drv_lcd_mipi.d \
./bsp/stm32/drivers/drv_lptim.d \
./bsp/stm32/drivers/drv_pm.d \
./bsp/stm32/drivers/drv_pulse_encoder.d \
./bsp/stm32/drivers/drv_pwm.d \
./bsp/stm32/drivers/drv_qspi.d \
./bsp/stm32/drivers/drv_rtc.d \
./bsp/stm32/drivers/drv_sdio.d \
./bsp/stm32/drivers/drv_sdram.d \
./bsp/stm32/drivers/drv_soft_i2c.d \
./bsp/stm32/drivers/drv_spi.d \
./bsp/stm32/drivers/drv_usart.d \
./bsp/stm32/drivers/drv_usbd.d \
./bsp/stm32/drivers/drv_usbh.d \
./bsp/stm32/drivers/drv_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/stm32/drivers/%.o bsp/stm32/drivers/%.su bsp/stm32/drivers/%.cyclo: ../bsp/stm32/drivers/%.c bsp/stm32/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -D__FPU_PRESENT -DARM_MATH_CM4 -DHAVE_CCONFIG_H -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_cominform_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_ad7606_data" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_singlecast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_board_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_multicast_thread" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_send_record" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_protocol_file" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_utility" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_paras_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/dsp/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/algorithm/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/lh/itc" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/net/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/ini" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/module/include" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Program/STM32CubeIDE/clb_app/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/rtos/libcpu/arm/cortex-m4" -I"E:/Program/STM32CubeIDE/clb_app/rtos/usr/finsh" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/common" -I"E:/Program/STM32CubeIDE/clb_app/rtos/lib/newlib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/api/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/api/drivers/spi" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/dfs/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/devfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/elmfat" -I"E:/Program/STM32CubeIDE/clb_app/rtos/fs/romfs" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/device/sdcard" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/arch/include/arch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/netif" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/lwip-2.1.2/src/include/compat/posix" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netdev/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/include" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/drv_flash" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/stm32/drivers/config" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/project/tfdsn/io" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/ntp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/net/netutils/tftp" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/include" -I"/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/algorithm_lib" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_diagnosis" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_tftp_up_raw" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_timer" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_data_save" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_channel_switch" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_ad7606" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_self_test" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_iwdg" -I"E:/Program/STM32CubeIDE/clb_app/rtos/bsp/internal_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_led_control" -I"E:/Program/STM32CubeIDE/clb_app/rtos/board/app/app_pwm" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bsp-2f-stm32-2f-drivers

clean-bsp-2f-stm32-2f-drivers:
	-$(RM) ./bsp/stm32/drivers/drv_adc.cyclo ./bsp/stm32/drivers/drv_adc.d ./bsp/stm32/drivers/drv_adc.o ./bsp/stm32/drivers/drv_adc.su ./bsp/stm32/drivers/drv_can.cyclo ./bsp/stm32/drivers/drv_can.d ./bsp/stm32/drivers/drv_can.o ./bsp/stm32/drivers/drv_can.su ./bsp/stm32/drivers/drv_clk.cyclo ./bsp/stm32/drivers/drv_clk.d ./bsp/stm32/drivers/drv_clk.o ./bsp/stm32/drivers/drv_clk.su ./bsp/stm32/drivers/drv_common.cyclo ./bsp/stm32/drivers/drv_common.d ./bsp/stm32/drivers/drv_common.o ./bsp/stm32/drivers/drv_common.su ./bsp/stm32/drivers/drv_crypto.cyclo ./bsp/stm32/drivers/drv_crypto.d ./bsp/stm32/drivers/drv_crypto.o ./bsp/stm32/drivers/drv_crypto.su ./bsp/stm32/drivers/drv_dac.cyclo ./bsp/stm32/drivers/drv_dac.d ./bsp/stm32/drivers/drv_dac.o ./bsp/stm32/drivers/drv_dac.su ./bsp/stm32/drivers/drv_eth.cyclo ./bsp/stm32/drivers/drv_eth.d ./bsp/stm32/drivers/drv_eth.o ./bsp/stm32/drivers/drv_eth.su ./bsp/stm32/drivers/drv_gpio.cyclo ./bsp/stm32/drivers/drv_gpio.d ./bsp/stm32/drivers/drv_gpio.o ./bsp/stm32/drivers/drv_gpio.su ./bsp/stm32/drivers/drv_hwtimer.cyclo ./bsp/stm32/drivers/drv_hwtimer.d ./bsp/stm32/drivers/drv_hwtimer.o ./bsp/stm32/drivers/drv_hwtimer.su ./bsp/stm32/drivers/drv_lcd.cyclo ./bsp/stm32/drivers/drv_lcd.d ./bsp/stm32/drivers/drv_lcd.o ./bsp/stm32/drivers/drv_lcd.su ./bsp/stm32/drivers/drv_lcd_mipi.cyclo ./bsp/stm32/drivers/drv_lcd_mipi.d ./bsp/stm32/drivers/drv_lcd_mipi.o ./bsp/stm32/drivers/drv_lcd_mipi.su ./bsp/stm32/drivers/drv_lptim.cyclo ./bsp/stm32/drivers/drv_lptim.d ./bsp/stm32/drivers/drv_lptim.o ./bsp/stm32/drivers/drv_lptim.su ./bsp/stm32/drivers/drv_pm.cyclo ./bsp/stm32/drivers/drv_pm.d ./bsp/stm32/drivers/drv_pm.o ./bsp/stm32/drivers/drv_pm.su ./bsp/stm32/drivers/drv_pulse_encoder.cyclo ./bsp/stm32/drivers/drv_pulse_encoder.d ./bsp/stm32/drivers/drv_pulse_encoder.o ./bsp/stm32/drivers/drv_pulse_encoder.su ./bsp/stm32/drivers/drv_pwm.cyclo ./bsp/stm32/drivers/drv_pwm.d ./bsp/stm32/drivers/drv_pwm.o ./bsp/stm32/drivers/drv_pwm.su ./bsp/stm32/drivers/drv_qspi.cyclo ./bsp/stm32/drivers/drv_qspi.d ./bsp/stm32/drivers/drv_qspi.o ./bsp/stm32/drivers/drv_qspi.su ./bsp/stm32/drivers/drv_rtc.cyclo ./bsp/stm32/drivers/drv_rtc.d ./bsp/stm32/drivers/drv_rtc.o ./bsp/stm32/drivers/drv_rtc.su ./bsp/stm32/drivers/drv_sdio.cyclo ./bsp/stm32/drivers/drv_sdio.d ./bsp/stm32/drivers/drv_sdio.o ./bsp/stm32/drivers/drv_sdio.su ./bsp/stm32/drivers/drv_sdram.cyclo ./bsp/stm32/drivers/drv_sdram.d ./bsp/stm32/drivers/drv_sdram.o ./bsp/stm32/drivers/drv_sdram.su ./bsp/stm32/drivers/drv_soft_i2c.cyclo ./bsp/stm32/drivers/drv_soft_i2c.d ./bsp/stm32/drivers/drv_soft_i2c.o ./bsp/stm32/drivers/drv_soft_i2c.su ./bsp/stm32/drivers/drv_spi.cyclo ./bsp/stm32/drivers/drv_spi.d ./bsp/stm32/drivers/drv_spi.o ./bsp/stm32/drivers/drv_spi.su ./bsp/stm32/drivers/drv_usart.cyclo ./bsp/stm32/drivers/drv_usart.d ./bsp/stm32/drivers/drv_usart.o ./bsp/stm32/drivers/drv_usart.su ./bsp/stm32/drivers/drv_usbd.cyclo ./bsp/stm32/drivers/drv_usbd.d ./bsp/stm32/drivers/drv_usbd.o ./bsp/stm32/drivers/drv_usbd.su ./bsp/stm32/drivers/drv_usbh.cyclo ./bsp/stm32/drivers/drv_usbh.d ./bsp/stm32/drivers/drv_usbh.o ./bsp/stm32/drivers/drv_usbh.su ./bsp/stm32/drivers/drv_wdt.cyclo ./bsp/stm32/drivers/drv_wdt.d ./bsp/stm32/drivers/drv_wdt.o ./bsp/stm32/drivers/drv_wdt.su

.PHONY: clean-bsp-2f-stm32-2f-drivers

