################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ChibiOS_2.4.0/os/kernel/src/chcond.c \
../src/ChibiOS_2.4.0/os/kernel/src/chdebug.c \
../src/ChibiOS_2.4.0/os/kernel/src/chdynamic.c \
../src/ChibiOS_2.4.0/os/kernel/src/chevents.c \
../src/ChibiOS_2.4.0/os/kernel/src/chheap.c \
../src/ChibiOS_2.4.0/os/kernel/src/chlists.c \
../src/ChibiOS_2.4.0/os/kernel/src/chmboxes.c \
../src/ChibiOS_2.4.0/os/kernel/src/chmemcore.c \
../src/ChibiOS_2.4.0/os/kernel/src/chmempools.c \
../src/ChibiOS_2.4.0/os/kernel/src/chmsg.c \
../src/ChibiOS_2.4.0/os/kernel/src/chmtx.c \
../src/ChibiOS_2.4.0/os/kernel/src/chqueues.c \
../src/ChibiOS_2.4.0/os/kernel/src/chregistry.c \
../src/ChibiOS_2.4.0/os/kernel/src/chschd.c \
../src/ChibiOS_2.4.0/os/kernel/src/chsem.c \
../src/ChibiOS_2.4.0/os/kernel/src/chsys.c \
../src/ChibiOS_2.4.0/os/kernel/src/chthreads.c \
../src/ChibiOS_2.4.0/os/kernel/src/chvt.c 

OBJS += \
./src/ChibiOS_2.4.0/os/kernel/src/chcond.o \
./src/ChibiOS_2.4.0/os/kernel/src/chdebug.o \
./src/ChibiOS_2.4.0/os/kernel/src/chdynamic.o \
./src/ChibiOS_2.4.0/os/kernel/src/chevents.o \
./src/ChibiOS_2.4.0/os/kernel/src/chheap.o \
./src/ChibiOS_2.4.0/os/kernel/src/chlists.o \
./src/ChibiOS_2.4.0/os/kernel/src/chmboxes.o \
./src/ChibiOS_2.4.0/os/kernel/src/chmemcore.o \
./src/ChibiOS_2.4.0/os/kernel/src/chmempools.o \
./src/ChibiOS_2.4.0/os/kernel/src/chmsg.o \
./src/ChibiOS_2.4.0/os/kernel/src/chmtx.o \
./src/ChibiOS_2.4.0/os/kernel/src/chqueues.o \
./src/ChibiOS_2.4.0/os/kernel/src/chregistry.o \
./src/ChibiOS_2.4.0/os/kernel/src/chschd.o \
./src/ChibiOS_2.4.0/os/kernel/src/chsem.o \
./src/ChibiOS_2.4.0/os/kernel/src/chsys.o \
./src/ChibiOS_2.4.0/os/kernel/src/chthreads.o \
./src/ChibiOS_2.4.0/os/kernel/src/chvt.o 

C_DEPS += \
./src/ChibiOS_2.4.0/os/kernel/src/chcond.d \
./src/ChibiOS_2.4.0/os/kernel/src/chdebug.d \
./src/ChibiOS_2.4.0/os/kernel/src/chdynamic.d \
./src/ChibiOS_2.4.0/os/kernel/src/chevents.d \
./src/ChibiOS_2.4.0/os/kernel/src/chheap.d \
./src/ChibiOS_2.4.0/os/kernel/src/chlists.d \
./src/ChibiOS_2.4.0/os/kernel/src/chmboxes.d \
./src/ChibiOS_2.4.0/os/kernel/src/chmemcore.d \
./src/ChibiOS_2.4.0/os/kernel/src/chmempools.d \
./src/ChibiOS_2.4.0/os/kernel/src/chmsg.d \
./src/ChibiOS_2.4.0/os/kernel/src/chmtx.d \
./src/ChibiOS_2.4.0/os/kernel/src/chqueues.d \
./src/ChibiOS_2.4.0/os/kernel/src/chregistry.d \
./src/ChibiOS_2.4.0/os/kernel/src/chschd.d \
./src/ChibiOS_2.4.0/os/kernel/src/chsem.d \
./src/ChibiOS_2.4.0/os/kernel/src/chsys.d \
./src/ChibiOS_2.4.0/os/kernel/src/chthreads.d \
./src/ChibiOS_2.4.0/os/kernel/src/chvt.d 


# Each subdirectory must supply rules for building sources it contributes
src/ChibiOS_2.4.0/os/kernel/src/%.o: ../src/ChibiOS_2.4.0/os/kernel/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/kernel/include" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/STM32F4xx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/boards/ST_STM32F4_DISCOVERY" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx/CMSIS/include" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32F4xx" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32/GPIOv2" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/various" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/ext/fatfs/src" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/codec" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/mp3dec/real" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/ChibiOS_2.4.0/os/hal/include" -I"/home/zxc2694/workspace/chibios_mp3/MP3/src/mp3dec/pub" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


