#Output files
PROJECT=MP3Application
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin

#===================================================
#Cross Compiler
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb

#===================================================
#Flags
CFLAGS=-g -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS+=-ffreestanding -nostdlib -Wall 
CFLAGS+=-Wl,-T,stm32_flash.ld

CFLAGS+= \
        -D STM32F40XX \
        -D USE_STDPERIPH_DRIVER \
        -D __FPU_PRESENT=1 \
        -D ARM_MATH_CM4

LDFLAGS+=-lm -lc -lgcc
ARCH=CM4F

#====================================================
CMSIS=./lib/CMSIS
ST=./lib/STM32F4xx_StdPeriph_Driver
CHIBIOS=./lib/FreeRTOS
FATFS=./lib/fatfs
COMMON=./lib/common
DRIVER=./src/driver

#====================================================
vpath %.a lib
vpath %.c src

# Library paths
LIBPATHS += -Llib/helix

# Libraries to link
LIBS = -lm -Llib/helix
#====================================================
CFLAGS+=-I./

CFLAGS+=-I$(CMSIS)
SRC+=$(CMSIS)/system_stm32f4xx.c \
	$(CMSIS)/FastMathFunctions/arm_cos_f32.c \
	$(CMSIS)/FastMathFunctions/arm_sin_f32.c

STARTUP=./lib/CMSIS/startup_stm32f4xx.s

CFLAGS+=-I$(ST)/inc
SRC+=./lib/CMSIS/system_stm32f4xx.c \
        $(ST)/src/misc.c \
        $(ST)/src/stm32f4xx_rcc.c \
        $(ST)/src/stm32f4xx_dma.c \
        $(ST)/src/stm32f4xx_flash.c \
        $(ST)/src/stm32f4xx_gpio.c \
        $(ST)/src/stm32f4xx_usart.c \
        $(ST)/src/stm32f4xx_tim.c \
        $(ST)/src/stm32f4xx_spi.c \
        $(ST)/src/stm32f4xx_i2c.c \
        $(ST)/src/stm32f4xx_sdio.c \
        $(ST)/src/stm32f4xx_spi.c \
        $(ST)/src/tm_stm32f4_spi.c\
        $(ST)/src/tm_stm32f4_gpio.c\
        $(ST)/src/tm_stm32f4_ili9341.c\
        $(ST)/src/tm_stm32f4_fonts.c

CFLAGS+=-I$(COMMON)
SRC+=$(COMMON)/stdlib.c \
	$(COMMON)/string.c

CFLAGS+=-I$(DRIVER)
SRC+=$(DRIVER)/delay.c \
	$(DRIVER)/flash.c \
	$(DRIVER)/i2c.c \
	$(DRIVER)/sdio.c \
	$(DRIVER)/spi.c \
	$(DRIVER)/mpu9150.c \
	$(DRIVER)/ms5611.c \
	$(DRIVER)/sensor.c \
	$(DRIVER)/serial.c \
	$(DRIVER)/led.c 

SRC+=./src/main.c \
	./src/Audio.c 

CFLAGS+=-I$(FREERTOS)/Source/include \
	-I$(FREERTOS)/Source/portable/GCC/ARM_$(ARCH)
SRC+=$(FREERTOS)/Source/croutine.c \
	$(FREERTOS)/Source/list.c \
	$(FREERTOS)/Source/queue.c \
	$(FREERTOS)/Source/tasks.c \
	$(FREERTOS)/Source/timers.c \
	$(FREERTOS)/Source/portable/MemMang/heap_4.c \
	$(FREERTOS)/Source/portable/GCC/ARM_$(ARCH)/port.c \

CFLAGS+=-I$(FATFS)
SRC+=$(FATFS)/diskio.c \
	$(FATFS)/ff.c \
	$(FATFS)/syscall.c \
	$(FATFS)/unicode.c \

CFLAGS+=-I$(DRIVER)
CFLAGS += -Isrc
CFLAGS += -Isrc/config
CFLAGS += -Ilib/helix/pub

OBJS = $(SRCS:.c=.o)
#===================================================
#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo '    OBJCOPY $(BIN_IMAGE)'

STARTUP_OBJ = startup_stm32f4xx.o

$(STARTUP_OBJ): $(STARTUP)
	@$(CC) $(CFLAGS) $^ -c $(STARTUP)
	@echo '    CC $(STARTUP_OBJ)'

$(EXECUTABLE):$(SRC) $(STARTUP_OBJ)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
	@echo '    CC $(EXECUTABLE)'

#Make clean
clean:
	rm -rf $(STARTUP_OBJ)
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

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
