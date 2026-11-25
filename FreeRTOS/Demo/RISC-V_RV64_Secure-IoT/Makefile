XLEN ?= 64
APP  ?= hello

CROSS   = riscv$(XLEN)-unknown-elf-
CC      = $(CROSS)gcc
CPP     = $(CROSS)cpp
OBJCOPY = $(CROSS)objcopy
ARCH    = $(CROSS)ar
OBJDUMP = $(CROSS)objdump
NM      = $(CROSS)gcc-nm
ELF2HEX = $(CROSS)elf2hex
ELF2BIN = $(CROSS)elf2bin
DEBUG ?= 0
BASE_ADDRESS ?= 0x80000000
MARCH = rv64imafdc_zicsr_zifencei
MABI = lp64d
LIB = libmgs2401.a
CRYPTO_LIB = cryptolibmgs2401.a

APP_DIR         = apps/$(APP)
BUILD_DIR       = build/$(APP)
RTOS_SOURCE_DIR = $(abspath ../../Source)
DEMO_SOURCE_DIR = $(abspath ../Common/Minimal)
LIB_DIR         = $(CURDIR)/lib

APP_SRCS := $(wildcard $(APP_DIR)/*.c)
ifeq ($(filter clean cleanall,$(MAKECMDGOALS)),)
ifeq ($(APP_SRCS),)
    $(error No .c files found in $(APP_DIR). Usage: make APP=<app_name>)
endif
endif

CPPFLAGS = \
	-DportasmHANDLE_INTERRUPT=handle_trap \
	-I $(APP_DIR) \
	-I . \
	-I ../Common/include \
	-I $(RTOS_SOURCE_DIR)/include \
	-I $(RTOS_SOURCE_DIR)/portable/GCC/RISC-V \
	-I $(RTOS_SOURCE_DIR)/portable/GCC/RISC-V/chip_specific_extensions/RV32I_CLINT_no_extensions \
	-I $(CURDIR)/include \
	-I $(CURDIR)/lib \
	-I $(CURDIR)/scripts

CFLAGS  = -march=$(MARCH) -mabi=$(MABI) -mcmodel=medany \
	-Wall \
	-fmessage-length=0 \
	-ffunction-sections \
	-fdata-sections \
	-fno-builtin-printf \
	-ffreestanding \
	--specs=nano.specs \
	-std=gnu99 -fno-common

LDFLAGS = -nostartfiles \
	-march=$(MARCH) -mabi=$(MABI) -mcmodel=medany \
	-Xlinker --gc-sections \
	-static -lm \
	-lc -lgcc

ifeq ($(DEBUG), 1)
    CFLAGS += -Og -ggdb3
else
    CFLAGS += -Os
endif

SRCS = main.c $(APP_SRCS) \
	$(DEMO_SOURCE_DIR)/EventGroupsDemo.c \
	$(DEMO_SOURCE_DIR)/TaskNotify.c \
	$(DEMO_SOURCE_DIR)/TimerDemo.c \
	$(DEMO_SOURCE_DIR)/blocktim.c \
	$(DEMO_SOURCE_DIR)/dynamic.c \
	$(DEMO_SOURCE_DIR)/recmutex.c \
	$(RTOS_SOURCE_DIR)/event_groups.c \
	$(RTOS_SOURCE_DIR)/list.c \
	$(RTOS_SOURCE_DIR)/queue.c \
	$(RTOS_SOURCE_DIR)/stream_buffer.c \
	$(RTOS_SOURCE_DIR)/tasks.c \
	$(RTOS_SOURCE_DIR)/timers.c \
	$(RTOS_SOURCE_DIR)/portable/MemMang/heap_4.c \
	$(RTOS_SOURCE_DIR)/portable/GCC/RISC-V/port.c

ASMS = $(RTOS_SOURCE_DIR)/portable/GCC/RISC-V/portASM.S

vpath %.c . $(APP_DIR) $(DEMO_SOURCE_DIR) $(RTOS_SOURCE_DIR) \
          $(RTOS_SOURCE_DIR)/portable/MemMang \
          $(RTOS_SOURCE_DIR)/portable/GCC/RISC-V
vpath %.S $(RTOS_SOURCE_DIR)/portable/GCC/RISC-V

OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=$(XLEN).o) $(ASMS:.S=$(XLEN).o)))

TARGET = $(BUILD_DIR)/$(APP).elf

.PHONY: all clean cleanall

all: $(TARGET)

$(TARGET): $(OBJS) $(BUILD_DIR)/link.ld Makefile
	@echo "Linking $(APP)..."
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJS) -T$(BUILD_DIR)/link.ld \
		-Wl,--start-group $(LIB_DIR)/$(LIB) $(LIB_DIR)/$(CRYPTO_LIB) -Wl,--end-group -lm -o $@
	@echo "Generating dump and symbol table..."
	$(OBJDUMP) -D -S $@ > $(BUILD_DIR)/$(APP).disassembly
	$(NM) -aABCglS --size-sort --special-syms $@ > $(BUILD_DIR)/$(APP).sym
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/code.bin

$(BUILD_DIR)/%$(XLEN).o: %.c Makefile
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%$(XLEN).o: %.S Makefile
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/link.ld: link.ld Makefile
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -DBASE_ADDRESS=$(BASE_ADDRESS) $< | grep -v '^#' > $@

clean:
	rm -rf $(BUILD_DIR)

cleanall:
	rm -rf build