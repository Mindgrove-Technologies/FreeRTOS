XLEN ?= 64

APPS         := $(shell find apps -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
ACTIONS      := COMPILE RUN FLASH PSRAM clean
DETECTED_APP := $(firstword $(filter $(APPS),$(MAKECMDGOALS)))
APP          ?= $(if $(DETECTED_APP),$(DETECTED_APP),hello)

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
LIB        = libmgs2401.a
CRYPTO_LIB = cryptolibmgs2401.a

RAM        ?= y
FLASH_LINK ?=
PSRAM_LINK ?=

APP_DIR         = apps/$(APP)
BUILD_DIR       = build/$(APP)
RTOS_SOURCE_DIR = $(abspath ../../Source)
DEMO_SOURCE_DIR = $(abspath ../Common/Minimal)
LIB_DIR         = $(CURDIR)/lib
SCRIPTS_DIR     = $(CURDIR)/scripts

CFG_FILE    ?= ./scripts/FT2232-JTAG.cfg
HEADER_TOOL := $(SCRIPTS_DIR)/build_firm_script.py

ifeq ($(RAM), y)
    LINKER_SCRIPT = link.ld
endif
ifeq ($(FLASH_LINK), y)
    LINKER_SCRIPT = flash-link.ld
endif
ifeq ($(PSRAM_LINK), y)
    LINKER_SCRIPT = psram-link.ld
endif

LINKER_SRC = $(if $(wildcard $(SCRIPTS_DIR)/$(LINKER_SCRIPT)),$(SCRIPTS_DIR)/$(LINKER_SCRIPT),$(LINKER_SCRIPT))
LINKER_OUT = $(BUILD_DIR)/$(LINKER_SCRIPT)

APP_SRCS := $(wildcard $(APP_DIR)/*.c)
ifeq ($(filter clean cleanall help,$(MAKECMDGOALS)),)
ifeq ($(APP_SRCS),)
    $(error No .c files found in $(APP_DIR). Available apps: $(APPS))
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
	-I $(SCRIPTS_DIR)

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

OBJS   = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=$(XLEN).o) $(ASMS:.S=$(XLEN).o)))
TARGET = $(BUILD_DIR)/$(APP).elf

.PHONY: all $(APPS) COMPILE RUN FLASH PSRAM clean cleanall help FORCE

all: COMPILE

$(APPS): $(if $(filter $(ACTIONS),$(MAKECMDGOALS)),,COMPILE)
	@:

COMPILE: $(TARGET)

$(TARGET): $(OBJS) $(LINKER_OUT) Makefile
	@echo "Linking $(APP) ($(LINKER_SCRIPT))..."
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJS) -T$(LINKER_OUT) \
		-Wl,--start-group $(LIB_DIR)/$(LIB) $(LIB_DIR)/$(CRYPTO_LIB) -Wl,--end-group -lm -o $@
	@echo "Generating dump and symbol table..."
	$(OBJDUMP) -D -S $@ > $(BUILD_DIR)/$(APP).disassembly
	$(NM) -aABCglS --size-sort --special-syms $@ > $(BUILD_DIR)/$(APP).sym
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/code.bin
	@if [ "$(FLASH_LINK)" = "y" ]; then \
		python3 $(HEADER_TOOL) --secure-boot n --app-bin $(BUILD_DIR)/code.bin --output $(BUILD_DIR)/firmware.bin; \
		if [ ! -f "$(BUILD_DIR)/firmware.bin" ]; then \
			echo "Error: header tool did not produce $(BUILD_DIR)/firmware.bin"; exit 1; \
		fi; \
	fi

$(BUILD_DIR)/%$(XLEN).o: %.c Makefile
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%$(XLEN).o: %.S Makefile
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LINKER_OUT): $(LINKER_SRC) Makefile FORCE
	@mkdir -p $(@D)
ifeq ($(LINKER_SCRIPT), link.ld)
	@$(CPP) $(CPPFLAGS) -DBASE_ADDRESS=$(BASE_ADDRESS) $< | grep -v '^#' > $@.tmp; \
	if ! cmp -s $@.tmp $@ 2>/dev/null; then mv $@.tmp $@; else rm -f $@.tmp; fi
else
	@cp -f $< $@.tmp; \
	if ! cmp -s $@.tmp $@ 2>/dev/null; then mv $@.tmp $@; else rm -f $@.tmp; fi
endif

RUN:
	@$(MAKE) --no-print-directory COMPILE APP=$(APP) RAM=y FLASH_LINK= PSRAM_LINK= DEBUG=$(DEBUG)
	@echo "Running $(APP)..."
	@echo "CFG file: $(CFG_FILE)"
	@entrypoint=$$(readelf -h $(TARGET) | awk '/Entry point address:/ {print $$4}'); \
	entry_base=$$(printf "0x%08X" $$(( $$entrypoint & 0xFFFF0000 )) ); \
	if [ "$$entry_base" = "0x80000000" ]; then \
		echo "Compiled for correct RAM address $$entry_base"; \
	else \
		echo "Compiled for wrong address $$entry_base"; \
		exit 1; \
	fi; \
	openocd -f $(CFG_FILE) -c "load_image $(TARGET); reg pc 0x80000000; resume; exit;"

FLASH:
	@$(MAKE) --no-print-directory COMPILE APP=$(APP) FLASH_LINK=y RAM= PSRAM_LINK= DEBUG=$(DEBUG)
	@echo "Flashing $(APP)..."
	@entrypoint=$$(readelf -h $(TARGET) | awk '/Entry point address:/ {print $$4}'); \
	echo "Entry point of $(APP): $$entrypoint"; \
	entrypoint_lc=$$(echo "$$entrypoint" | tr '[:upper:]' '[:lower:]'); \
	if [ "$$entrypoint_lc" != "0x90000200" ]; then \
		echo "Error: Application must start at 0x90000200 (got $$entrypoint)"; \
		exit 1; \
	fi; \
	echo "Compiled for correct FLASH address $$entrypoint"; \
	if [ ! -f "$(BUILD_DIR)/firmware.bin" ]; then \
		echo "Error: $(BUILD_DIR)/firmware.bin not found."; \
		exit 1; \
	fi; \
	openocd -f $(CFG_FILE) \
		-c "s2401 flash_write $(BUILD_DIR)/firmware.bin 0x90000000" \
		-c "s2401 reset" -c "exit"

PSRAM:
	@$(MAKE) --no-print-directory COMPILE APP=$(APP) PSRAM_LINK=y RAM= FLASH_LINK= DEBUG=$(DEBUG)
	@PSRAM_NUMBER=1; \
	echo "Loading application $(APP) in PSRAM $$PSRAM_NUMBER..."; \
	entrypoint=$$(readelf -h $(TARGET) | awk '/Entry point address:/ {print $$4}'); \
	entry_base=$$(printf "0x%08X" $$(( $$entrypoint & 0xFFFF0000 )) ); \
	if [ "$$entry_base" = "0xB0000000" ]; then \
		echo "Compiled for correct PSRAM address $$entry_base"; \
	elif [ "$$entry_base" = "0x90000000" ]; then \
		echo "Compiled for address $$entry_base"; \
		echo "Check once PSRAM is connected with QSPI 0"; \
		printf "Proceed with PSRAM load? [y/N]: "; \
		read ans; \
		if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
			PSRAM_NUMBER=0; \
		else \
			echo "Aborting as per user input."; \
			exit 1; \
		fi; \
	else \
		echo "Compiled for wrong address $$entry_base"; \
		exit 1; \
	fi; \
	openocd -f $(CFG_FILE) \
		-c "s2401 psram_init $$PSRAM_NUMBER" -c "load_image $(TARGET); reg pc $$entrypoint; resume; exit"

clean:
	rm -rf $(BUILD_DIR)

cleanall:
	rm -rf build

help:
	@echo "Usage: make <app_name> [COMPILE|RUN|FLASH|PSRAM|clean] [FLASH_LINK=y|PSRAM_LINK=y] [DEBUG=1]"
	@echo "Available apps: $(APPS)"