PROJECT = zeptos
SRC = reset init debug

GCC_PREFIX = arm-none-eabi-
CC = $(GCC_PREFIX)gcc
AS = $(GCC_PREFIX)as
OBJCOPY = $(GCC_PREFIX)objcopy
SIZE = $(GCC_PREFIX)size
GDB = $(GCC_PREFIX)gdb

TARGET = build/$(PROJECT)
vpath %.c src
vpath %.s src
OBJ = $(SRC:%=build/%.o)

CPU_FLAGS = -mcpu=cortex-m4 -mthumb
INC = -Isrc

CC_FLAGS = -g -std=gnu99 -O0 $(CPU_FLAGS) $(INC) -Wall -Werror -Wno-unused-function -fstack-usage -funsigned-bitfields
AS_FLAGS = $(CPU_FLAGS) -Wall
LD_FLAGS = -nostartfiles $(CPU_FLAGS) -Wl,-gc-sections -Wl,-Map=$(TARGET).map -T link.ld

all: $(TARGET).bin

$(OBJ): Makefile

$(TARGET).elf: builddir $(OBJ) link.ld
	$(CC) $(LD_FLAGS) $(OBJ) -o $@
	$(SIZE) -B -t --common $(OBJ) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

build/%.o: %.c
	$(CC) -c $(CC_FLAGS) -Wa,-anhls=$(@:%.o=%.s) -o $@ $<

build/%.o: %.s
	$(AS) -c $(AS_FLAGS) -ans=$(@:%.o=%.sym) -o $@ $<

clean:
	rm -rf build

flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x08000000

debug: $(TARGET).elf
	st-util & $(GDB) -ex "target extended-remote localhost:4242" $(TARGET).elf

builddir:
	@mkdir -p build
