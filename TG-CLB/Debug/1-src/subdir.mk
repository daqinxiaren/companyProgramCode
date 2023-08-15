################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1-src/ad7606.c \
../1-src/app_paras_config.c \
../1-src/bearing_diagnosis.c \
../1-src/can_config.c \
../1-src/crc_table.c \
../1-src/derail_app.c \
../1-src/dev_config.c \
../1-src/ezini.c \
../1-src/gpio_app.c \
../1-src/hdd_save.c \
../1-src/lh_math.c \
../1-src/pthread_policy.c \
../1-src/sec_app.c \
../1-src/speed.c \
../1-src/spi_app.c \
../1-src/tfdsn_clb.c \
../1-src/udp_client.c \
../1-src/user_data.c \
../1-src/util.c \
../1-src/vibr_diagnosis.c \
../1-src/watchdog.c 

OBJS += \
./1-src/ad7606.o \
./1-src/app_paras_config.o \
./1-src/bearing_diagnosis.o \
./1-src/can_config.o \
./1-src/crc_table.o \
./1-src/derail_app.o \
./1-src/dev_config.o \
./1-src/ezini.o \
./1-src/gpio_app.o \
./1-src/hdd_save.o \
./1-src/lh_math.o \
./1-src/pthread_policy.o \
./1-src/sec_app.o \
./1-src/speed.o \
./1-src/spi_app.o \
./1-src/tfdsn_clb.o \
./1-src/udp_client.o \
./1-src/user_data.o \
./1-src/util.o \
./1-src/vibr_diagnosis.o \
./1-src/watchdog.o 

C_DEPS += \
./1-src/ad7606.d \
./1-src/app_paras_config.d \
./1-src/bearing_diagnosis.d \
./1-src/can_config.d \
./1-src/crc_table.d \
./1-src/derail_app.d \
./1-src/dev_config.d \
./1-src/ezini.d \
./1-src/gpio_app.d \
./1-src/hdd_save.d \
./1-src/lh_math.d \
./1-src/pthread_policy.d \
./1-src/sec_app.d \
./1-src/speed.d \
./1-src/spi_app.d \
./1-src/tfdsn_clb.d \
./1-src/udp_client.d \
./1-src/user_data.d \
./1-src/util.d \
./1-src/vibr_diagnosis.d \
./1-src/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
1-src/%.o: ../1-src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/mnt/hgfs/VMShareFile/TG-CLB/2-inc" -I"/mnt/hgfs/VMShareFile/TG-CLB/1-src/algorithm_lib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


