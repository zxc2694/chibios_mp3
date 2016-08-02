################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ChibiOS_2.4.0/os/hal/src/adc.c \
../src/ChibiOS_2.4.0/os/hal/src/can.c \
../src/ChibiOS_2.4.0/os/hal/src/ext.c \
../src/ChibiOS_2.4.0/os/hal/src/gpt.c \
../src/ChibiOS_2.4.0/os/hal/src/hal.c \
../src/ChibiOS_2.4.0/os/hal/src/i2c.c \
../src/ChibiOS_2.4.0/os/hal/src/icu.c \
../src/ChibiOS_2.4.0/os/hal/src/mac.c \
../src/ChibiOS_2.4.0/os/hal/src/mmc_spi.c \
../src/ChibiOS_2.4.0/os/hal/src/pal.c \
../src/ChibiOS_2.4.0/os/hal/src/pwm.c \
../src/ChibiOS_2.4.0/os/hal/src/rtc.c \
../src/ChibiOS_2.4.0/os/hal/src/sdc.c \
../src/ChibiOS_2.4.0/os/hal/src/serial.c \
../src/ChibiOS_2.4.0/os/hal/src/serial_usb.c \
../src/ChibiOS_2.4.0/os/hal/src/spi.c \
../src/ChibiOS_2.4.0/os/hal/src/tm.c \
../src/ChibiOS_2.4.0/os/hal/src/uart.c \
../src/ChibiOS_2.4.0/os/hal/src/usb.c 

OBJS += \
./src/ChibiOS_2.4.0/os/hal/src/adc.o \
./src/ChibiOS_2.4.0/os/hal/src/can.o \
./src/ChibiOS_2.4.0/os/hal/src/ext.o \
./src/ChibiOS_2.4.0/os/hal/src/gpt.o \
./src/ChibiOS_2.4.0/os/hal/src/hal.o \
./src/ChibiOS_2.4.0/os/hal/src/i2c.o \
./src/ChibiOS_2.4.0/os/hal/src/icu.o \
./src/ChibiOS_2.4.0/os/hal/src/mac.o \
./src/ChibiOS_2.4.0/os/hal/src/mmc_spi.o \
./src/ChibiOS_2.4.0/os/hal/src/pal.o \
./src/ChibiOS_2.4.0/os/hal/src/pwm.o \
./src/ChibiOS_2.4.0/os/hal/src/rtc.o \
./src/ChibiOS_2.4.0/os/hal/src/sdc.o \
./src/ChibiOS_2.4.0/os/hal/src/serial.o \
./src/ChibiOS_2.4.0/os/hal/src/serial_usb.o \
./src/ChibiOS_2.4.0/os/hal/src/spi.o \
./src/ChibiOS_2.4.0/os/hal/src/tm.o \
./src/ChibiOS_2.4.0/os/hal/src/uart.o \
./src/ChibiOS_2.4.0/os/hal/src/usb.o 

C_DEPS += \
./src/ChibiOS_2.4.0/os/hal/src/adc.d \
./src/ChibiOS_2.4.0/os/hal/src/can.d \
./src/ChibiOS_2.4.0/os/hal/src/ext.d \
./src/ChibiOS_2.4.0/os/hal/src/gpt.d \
./src/ChibiOS_2.4.0/os/hal/src/hal.d \
./src/ChibiOS_2.4.0/os/hal/src/i2c.d \
./src/ChibiOS_2.4.0/os/hal/src/icu.d \
./src/ChibiOS_2.4.0/os/hal/src/mac.d \
./src/ChibiOS_2.4.0/os/hal/src/mmc_spi.d \
./src/ChibiOS_2.4.0/os/hal/src/pal.d \
./src/ChibiOS_2.4.0/os/hal/src/pwm.d \
./src/ChibiOS_2.4.0/os/hal/src/rtc.d \
./src/ChibiOS_2.4.0/os/hal/src/sdc.d \
./src/ChibiOS_2.4.0/os/hal/src/serial.d \
./src/ChibiOS_2.4.0/os/hal/src/serial_usb.d \
./src/ChibiOS_2.4.0/os/hal/src/spi.d \
./src/ChibiOS_2.4.0/os/hal/src/tm.d \
./src/ChibiOS_2.4.0/os/hal/src/uart.d \
./src/ChibiOS_2.4.0/os/hal/src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/ChibiOS_2.4.0/os/hal/src/%.o: ../src/ChibiOS_2.4.0/os/hal/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/kernel/include" -I"/home/zxc2694/workspace/player/MP3/src" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/STM32F4xx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/boards/ST_STM32F4_DISCOVERY" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx/CMSIS/include" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32F4xx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32/GPIOv2" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/various" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/ext/fatfs/src" -I"/home/zxc2694/workspace/player/MP3/src/codec" -I"/home/zxc2694/workspace/player/MP3/src/mp3dec/real" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/include" -I"/home/zxc2694/workspace/player/MP3/src/mp3dec/pub" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


