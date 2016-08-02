################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mp3dec/real/bitstream.c \
../src/mp3dec/real/buffers.c \
../src/mp3dec/real/dct32.c \
../src/mp3dec/real/dequant.c \
../src/mp3dec/real/dqchan.c \
../src/mp3dec/real/huffman.c \
../src/mp3dec/real/hufftabs.c \
../src/mp3dec/real/imdct.c \
../src/mp3dec/real/polyphase.c \
../src/mp3dec/real/scalfact.c \
../src/mp3dec/real/stproc.c \
../src/mp3dec/real/subband.c \
../src/mp3dec/real/trigtabs_fixpt.c 

OBJS += \
./src/mp3dec/real/bitstream.o \
./src/mp3dec/real/buffers.o \
./src/mp3dec/real/dct32.o \
./src/mp3dec/real/dequant.o \
./src/mp3dec/real/dqchan.o \
./src/mp3dec/real/huffman.o \
./src/mp3dec/real/hufftabs.o \
./src/mp3dec/real/imdct.o \
./src/mp3dec/real/polyphase.o \
./src/mp3dec/real/scalfact.o \
./src/mp3dec/real/stproc.o \
./src/mp3dec/real/subband.o \
./src/mp3dec/real/trigtabs_fixpt.o 

C_DEPS += \
./src/mp3dec/real/bitstream.d \
./src/mp3dec/real/buffers.d \
./src/mp3dec/real/dct32.d \
./src/mp3dec/real/dequant.d \
./src/mp3dec/real/dqchan.d \
./src/mp3dec/real/huffman.d \
./src/mp3dec/real/hufftabs.d \
./src/mp3dec/real/imdct.d \
./src/mp3dec/real/polyphase.d \
./src/mp3dec/real/scalfact.d \
./src/mp3dec/real/stproc.d \
./src/mp3dec/real/subband.d \
./src/mp3dec/real/trigtabs_fixpt.d 


# Each subdirectory must supply rules for building sources it contributes
src/mp3dec/real/%.o: ../src/mp3dec/real/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/kernel/include" -I"/home/zxc2694/workspace/player/MP3/src" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/GCC/ARMCMx/STM32F4xx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/boards/ST_STM32F4_DISCOVERY" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/ports/common/ARMCMx/CMSIS/include" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32F4xx" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/platforms/STM32/GPIOv2" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/various" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/ext/fatfs/src" -I"/home/zxc2694/workspace/player/MP3/src/codec" -I"/home/zxc2694/workspace/player/MP3/src/mp3dec/real" -I"/home/zxc2694/workspace/player/MP3/src/ChibiOS_2.4.0/os/hal/include" -I"/home/zxc2694/workspace/player/MP3/src/mp3dec/pub" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


