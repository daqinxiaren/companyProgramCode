################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1-src/algorithm_lib/CLX_diag.c \
../1-src/algorithm_lib/DJ_diag.c \
../1-src/algorithm_lib/IIR_polygon.c \
../1-src/algorithm_lib/ZXZC_Wheel_diag.c \
../1-src/algorithm_lib/algor_interface.c \
../1-src/algorithm_lib/corrugation_diag.c \
../1-src/algorithm_lib/fft_diag.c \
../1-src/algorithm_lib/fft_max.c \
../1-src/algorithm_lib/gear_diag.c \
../1-src/algorithm_lib/initial_pro.c \
../1-src/algorithm_lib/main.c \
../1-src/algorithm_lib/mean.c \
../1-src/algorithm_lib/min.c \
../1-src/algorithm_lib/new_fft.c \
../1-src/algorithm_lib/polygon_diag.c \
../1-src/algorithm_lib/rms.c \
../1-src/algorithm_lib/sqrt_sum.c \
../1-src/algorithm_lib/status_judge.c \
../1-src/algorithm_lib/substract_mean.c 

OBJS += \
./1-src/algorithm_lib/CLX_diag.o \
./1-src/algorithm_lib/DJ_diag.o \
./1-src/algorithm_lib/IIR_polygon.o \
./1-src/algorithm_lib/ZXZC_Wheel_diag.o \
./1-src/algorithm_lib/algor_interface.o \
./1-src/algorithm_lib/corrugation_diag.o \
./1-src/algorithm_lib/fft_diag.o \
./1-src/algorithm_lib/fft_max.o \
./1-src/algorithm_lib/gear_diag.o \
./1-src/algorithm_lib/initial_pro.o \
./1-src/algorithm_lib/main.o \
./1-src/algorithm_lib/mean.o \
./1-src/algorithm_lib/min.o \
./1-src/algorithm_lib/new_fft.o \
./1-src/algorithm_lib/polygon_diag.o \
./1-src/algorithm_lib/rms.o \
./1-src/algorithm_lib/sqrt_sum.o \
./1-src/algorithm_lib/status_judge.o \
./1-src/algorithm_lib/substract_mean.o 

C_DEPS += \
./1-src/algorithm_lib/CLX_diag.d \
./1-src/algorithm_lib/DJ_diag.d \
./1-src/algorithm_lib/IIR_polygon.d \
./1-src/algorithm_lib/ZXZC_Wheel_diag.d \
./1-src/algorithm_lib/algor_interface.d \
./1-src/algorithm_lib/corrugation_diag.d \
./1-src/algorithm_lib/fft_diag.d \
./1-src/algorithm_lib/fft_max.d \
./1-src/algorithm_lib/gear_diag.d \
./1-src/algorithm_lib/initial_pro.d \
./1-src/algorithm_lib/main.d \
./1-src/algorithm_lib/mean.d \
./1-src/algorithm_lib/min.d \
./1-src/algorithm_lib/new_fft.d \
./1-src/algorithm_lib/polygon_diag.d \
./1-src/algorithm_lib/rms.d \
./1-src/algorithm_lib/sqrt_sum.d \
./1-src/algorithm_lib/status_judge.d \
./1-src/algorithm_lib/substract_mean.d 


# Each subdirectory must supply rules for building sources it contributes
1-src/algorithm_lib/%.o: ../1-src/algorithm_lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/mnt/hgfs/VMShareFile/TG-CLB/2-inc" -I"/mnt/hgfs/VMShareFile/TG-CLB/1-src/algorithm_lib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


