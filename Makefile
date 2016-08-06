#========== Output files =============================
PROJECT=MP3Application
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin

#========= Cross Compiler ============================
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb

#========== Flags ====================================
CFLAGS=-g -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS+=-ffreestanding  -Wall 
CFLAGS+=-Wl,-T,STM32F407xG.ld

CFLAGS+= \
        -D STM32F40XX \
        -D USE_STDPERIPH_DRIVER \
        -D __FPU_PRESENT=1 \
        -D ARM_MATH_CM4

LDFLAGS+=-lm -lc -lgcc

#========== Path =====================================
BOARD=./ChibiOS_2.4.0/boards/ST_STM32F4_DISCOVERY
FATFS=./ChibiOS_2.4.0/ext/fatfs/src
STLIB=./STM32F4xx_StdPeriph_Driver
ChibiOS_HAL=./ChibiOS_2.4.0/os/hal
ChibiOS_HAL_STM32=./ChibiOS_2.4.0/os/hal/platforms/STM32
ChibiOS_HAL_STM32F4XX=./ChibiOS_2.4.0/os/hal/platforms/STM32F4xx
ChibiOS_KERNEL=./ChibiOS_2.4.0/os/kernel
ChibiOS_PORT=./ChibiOS_2.4.0/os/ports
CODEC = ./codec
MP3DEC = ./mp3dec

#========== Include ==================================
CFLAGS+=-I./
CFLAGS+=-I$(BOARD)
CFLAGS+=-I$(FATFS)
CFLAGS+=-I$(ChibiOS_HAL)/include
CFLAGS+=-I$(ChibiOS_HAL_STM32)
CFLAGS+=-I$(ChibiOS_HAL_STM32F4XX)
CFLAGS+=-I$(ChibiOS_KERNEL)/include
CFLAGS+=-I$(ChibiOS_PORT)
CFLAGS+=-I$(CODEC)
CFLAGS+=-I$(MP3DEC)

#========== Source ===================================
SRC+=$(BOARD)/board.c \
	$(FATFS)/diskio.c \
	$(FATFS)/ff.c \
	$(FATFS)/syscall.c \
	$(ChibiOS_HAL_STM32)/can_lld.c \
	$(ChibiOS_HAL_STM32)/ext_lld.c \
	$(ChibiOS_HAL_STM32)/gpt_lld.c \
	$(ChibiOS_HAL_STM32)/pwm_lld.c \
	$(ChibiOS_HAL_STM32)/pal_lld.c \
	$(ChibiOS_HAL_STM32)/sdc_lld.c \
	$(ChibiOS_HAL_STM32)/serial_lld.c \
	$(ChibiOS_HAL_STM32)/spi_lld.c \
	$(ChibiOS_HAL_STM32)/uart_lld.c \
	$(ChibiOS_HAL_STM32)/i2c_lld.c \
	$(ChibiOS_HAL_STM32F4XX)/adc_lld.c \
	$(ChibiOS_HAL_STM32F4XX)/hal_lld.c \
	$(ChibiOS_HAL_STM32F4XX)/stm32_dma.c \
	$(ChibiOS_HAL)/src/adc.c \
	$(ChibiOS_HAL)/src/can.c \
	$(ChibiOS_HAL)/src/ext.c \
	$(ChibiOS_HAL)/src/gpt.c \
	$(ChibiOS_HAL)/src/hal.c \
	$(ChibiOS_HAL)/src/i2c.c \
	$(ChibiOS_HAL)/src/icu.c \
	$(ChibiOS_HAL)/src/mac.c \
	$(ChibiOS_HAL)/src/mmc_spi.c \
	$(ChibiOS_HAL)/src/pal.c \
	$(ChibiOS_HAL)/src/pwm.c \
	$(ChibiOS_HAL)/src/rtc.c \
	$(ChibiOS_HAL)/src/sdc.c \
	$(ChibiOS_HAL)/src/serial.c \
	$(ChibiOS_HAL)/src/serial_usb.c \
	$(ChibiOS_HAL)/src/spi.c \
	$(ChibiOS_HAL)/src/tm.c \
	$(ChibiOS_HAL)/src/uart.c \
	$(ChibiOS_HAL)/src/usb.c \
	$(ChibiOS_KERNEL)/src/chcond.c \
	$(ChibiOS_KERNEL)/src/chdebug.c \
	$(ChibiOS_KERNEL)/src/chdynamic.c \
	$(ChibiOS_KERNEL)/src/chevents.c \
	$(ChibiOS_KERNEL)/src/chheap.c \
	$(ChibiOS_KERNEL)/src/chlists.c \
	$(ChibiOS_KERNEL)/src/chmboxes.c \
	$(ChibiOS_KERNEL)/src/chmemcore.c \
	$(ChibiOS_KERNEL)/src/chmempools.c \
	$(ChibiOS_KERNEL)/src/chmsg.c \
	$(ChibiOS_KERNEL)/src/chmtx.c \
	$(ChibiOS_KERNEL)/src/chqueues.c \
	$(ChibiOS_KERNEL)/src/chregistry.c \
	$(ChibiOS_KERNEL)/src/chschd.c \
	$(ChibiOS_KERNEL)/src/chsem.c \
	$(ChibiOS_KERNEL)/src/chsys.c \
	$(ChibiOS_KERNEL)/src/chthreads.c \
	$(ChibiOS_KERNEL)/src/chvt.c \
	$(ChibiOS_PORT)/vectors.c \
	$(ChibiOS_PORT)/nvic.c \
	$(ChibiOS_PORT)/chcore.c \
	$(ChibiOS_PORT)/chcore_v7m.c \
	$(ChibiOS_PORT)/crt0.c \
	$(ChibiOS_PORT)/chprintf.c \
	$(CODEC)/codec.c \
	$(CODEC)/st_stm32f4xx_dma.c \
	$(CODEC)/st_stm32f4xx_gpio.c \
	$(CODEC)/st_stm32f4xx_i2c.c \
	$(CODEC)/st_stm32f4xx_rcc.c \
	$(CODEC)/st_stm32f4xx_spi.c \
	$(MP3DEC)/bitstream.c \
	$(MP3DEC)/buffers.c \
	$(MP3DEC)/dct32.c \
	$(MP3DEC)/dequant.c \
	$(MP3DEC)/dqchan.c \
	$(MP3DEC)/huffman.c \
	$(MP3DEC)/hufftabs.c \
	$(MP3DEC)/imdct.c \
	$(MP3DEC)/polyphase.c \
	$(MP3DEC)/scalfact.c \
	$(MP3DEC)/stproc.c \
	$(MP3DEC)/subband.c \
	$(MP3DEC)/trigtabs_fixpt.c \
	$(MP3DEC)/mp3dec.c \
	$(MP3DEC)/mp3tabs.c 

SRC+=./main.c

OBJS = $(SRCS:.c=.o)

#========== Target ===================================
# Make all
all: $(BIN_IMAGE)

# Tool invocations
$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo '    OBJCOPY $(BIN_IMAGE)'

$(EXECUTABLE): $(SRC) 
	@$(CC) $(CFLAGS) $^ -o $@
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Linux GCC C Linker'
	arm-none-eabi-gcc -T"./STM32F407xG.ld" -nostartfiles -Wl,-Map,$(PROJECT).map 
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
	@echo '    CC $(EXECUTABLE)'

#Make clean
clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -rf $(PROJECT).map 
	rm -rf a.out

#Make flash
flash:
	st-flash write $(BIN_IMAGE) 0x8000000

#Make screen
screen:
	echo sudo screen /dev/ttyUSB0 38400 

#Make openocd
openocd: flash
	openocd -s /opt/openocd/share/openocd/scripts/ -f ./debug/openocd.cfg

#Make cgdb
cgdb:
	cgdb -d $(GDB) -x ./debug/openocd_gdb.gdb

#Make gdbtui
gdbtui:
	$(GDB) -tui -x ./debug/openocd_gdb.gdb

#Make gdbauto
gdbauto: cgdb

#automatically formate
astyle: 
	astyle -r --exclude=lib  *.c *.h

.PHONY:all clean flash openocd gdbauto gdbtui cgdb astyle
