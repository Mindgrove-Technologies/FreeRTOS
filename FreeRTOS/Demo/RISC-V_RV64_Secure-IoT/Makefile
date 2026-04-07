XLEN ?= 64
FPU ?= 1
LIB ?= libsecureiot.a
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
LIB = libsecureiot.a

BUILD_DIR       = build
RTOS_SOURCE_DIR = $(abspath ../../Source)
DEMO_SOURCE_DIR = $(abspath ../Common/Minimal)
LIB_DIR = $(PWD)/lib

CPPFLAGS = \
	-DportasmHANDLE_INTERRUPT=handle_trap \
	-I . -I ../Common/include \
	-I ./include \
	-I $(RTOS_SOURCE_DIR)/include \
	-I $(RTOS_SOURCE_DIR)/portable/GCC/RISC-V \
	-I $(RTOS_SOURCE_DIR)/portable/GCC/RISC-V/chip_specific_extensions/RV32I_CLINT_no_extensions -lm -std=gnu99 -fno-common \
	-I $(PWD)/include \
    -I $(PWD)/lib \
    -I $(PWD)/scripts
CFLAGS  = -march=$(MARCH) -mabi=$(MABI) -mcmodel=medany \
	-Wall \
	-fmessage-length=0 \
	-ffunction-sections \
	-fdata-sections \
	-fno-builtin-printf\
	-lm \
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

SRCS = main.c main_blinky.c\
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

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%$(XLEN).o) $(ASMS:%.S=$(BUILD_DIR)/%$(XLEN).o)
DEPS = $(SRCS:%.c=$(BUILD_DIR)/%$(XLEN).d) $(ASMS:%.S=$(BUILD_DIR)/%$(XLEN).d)

$(BUILD_DIR)/SecureIoTDemo.elf: $(OBJS) $(BUILD_DIR)/link.ld Makefile
	@echo "Linking..."
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJS) -T$(BUILD_DIR)/link.ld $(LIB_DIR)/$(LIB) -lm -o $@
	@echo "Generating dump and symbol table..."
	$(OBJDUMP) -D -S $@ > $(BUILD_DIR)/SecureIoTDemo.disassembly
	$(NM) -aABCglS --size-sort --special-syms $@ > $(BUILD_DIR)/SecureIoTDemo.sym

$(BUILD_DIR)/%$(XLEN).o: %.c Makefile
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%$(XLEN).o: %.S Makefile
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# Run ld through the C preprocessor, to replace BASE_ADDRESS with the actual
# value. It might be simpler to use sed instead.
$(BUILD_DIR)/link.ld: link.ld Makefile
	$(CPP) $(CPPFLAGS) -DBASE_ADDRESS=$(BASE_ADDRESS) $< | grep -v '^#' > $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
