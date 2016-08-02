################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/chcore.c \
../src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/chcore_v7m.c \
../src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/crt0.c 

OBJS += \
./src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/chcore.o \
./src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/chcore_v7m.o \
./src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/crt0.o 

C_DEPS += \
./src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/chcore.d \
./src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/chcore_v7m.d \
./src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/crt0.d 


# Each subdirectory must supply rules for building sources it contributes
src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/%.o: ../src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/kernel/include" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/STM32F4xx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/boards/ST_STM32F4_DISCOVERY" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx/CMSIS/include" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32F4xx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32/GPIOv2" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/various" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/ext/fatfs/src" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/codec" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/mp3dec/real" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/include" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/mp3dec/pub" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


