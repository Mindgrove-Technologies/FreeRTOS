# Emulating FreeRTOS on Mindgrove Silicon's Secure-IoT SoC

## Requirements

1. GNU RISC-V toolchain
2. OpenOCD from https://github.com/riscv/riscv-openocd

## How to build

To build, simply run `make`. If you want a debug build, pass `DEBUG=1`.

The resulting executable file is ./build/SecureIoTDemo.shakti.

## How to debug with gdb

Start GTKTERM in one terminal:
```
$ sudo gtkterm -p /dev/ttyUSB2 -s 19200
```

Start OpenOCD in another terminal:
```
$ openocd -f seciot_<board>.cfg # <board> can be 100t or 200t, depending on the fpga.
```

Start gdb in yet another:
```
$ riscv64-unknown-elf-gdb
...
(gdb) set remotetimeout unlimited
(gdb) target extended-remote localhost:3333
(gdb) file ./build/SecureIoTDemo.shakti
(gdb) load
(gdb) c
```

## Description

This demo prints Hello Worlld, and displays the current HART id.
