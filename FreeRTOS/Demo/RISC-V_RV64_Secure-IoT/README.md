# FreeRTOS on Mindgrove S2401 Secure IoT
## Creating a new application

Create a new folder for your application inside `Demo/RISC-V_RV64_Secure-IoT/apps/` (for example, `apps/<app_name>/`). Place your source files (`.c` and `.h`) inside that folder, define the entry function as `int app_main(void)` (or `void app_main(void)`), create your FreeRTOS tasks, and call `vTaskStartScheduler()`.

Hardware trap vector registration (`mtvec`) and FreeRTOS kernel hooks are already handled in `main.c` and do not need to be duplicated in your application folder.

## How to build and run

Run `make` from `Demo/RISC-V_RV64_Secure-IoT` and pass the application folder name via `<app_name>` (defaults to `hello` if omitted):

```
make <app_name>
```

For a debug build (`-Og -ggdb3`), pass `DEBUG=1`:

```bash
make <app_name> DEBUG=1

```


* To compile the specified application for FLASH. Use the below command:
```
make <app_name> FLASH_LINK=y
```

This will prompt you interactively for

**`Custom QSPI Configuration needed? (y/n):`**

By default, the boot configures QSPI in single line mode — answer `n` unless you need a
custom QSPI configuration. Answering `y` prompts you for three 32-bit hex values,
one at a time: `CR` (Configuration Register), `DCR` (Device Configuration Register), and
`CCR` (Command Configuration Register).

To skip this prompt, pass the argument directly via the command line:

- QSPI_CONFIG=y: Prompts for the configuration values directly, skipping the initial (y/n) question.

- QSPI_CONFIG=n: Bypasses the initial prompt and skips asking for custom configuration values entirely.
```
make <app_name> FLASH_LINK=y QSPI_CONFIG=n

```
* To compile the specified application for PSRAM. Use the below command:
```
make <app_name> PSRAM_LINK=y
```

* To compile the specified application and execute the application in RAM. Use the below command:
```
make <app_name> RUN
```

* To compile the application for FLASH and execute the application from FLASH memory. Use the below command:
```
make <app_name> FLASH
```

This will prompt you interactively for:

**`Custom QSPI Configuration needed? (y/n):`**

By default, the boot configures QSPI in single line mode — answer `n` unless you need a
custom QSPI configuration. Answering `y` prompts you for three 32-bit hex values,
one at a time: `CR` (Configuration Register), `DCR` (Device Configuration Register), and
`CCR` (Command Configuration Register).

To skip this prompt, pass the argument directly via the command line:

- QSPI_CONFIG=y: Prompts for the configuration values directly, skipping the initial (y/n) question.

- QSPI_CONFIG=n: Bypasses the initial prompt and skips asking for custom configuration values entirely.
```
make <app_name> FLASH QSPI_CONFIG=n
```

* To compile the application for PSRAM and execute the application from PSRAM memory. Use the below command:
```
make <app_name> PSRAM
```

## Cleaning build artifacts

* Clean a specific application (`./build/<app_name>/`):
```bash
make clean <app_name>

```


* Clean all built applications (`./build/`):
```bash
make cleanall
```

## Sample applications Description

This FreeRTOS port contains three sample applications hello, semaphore_test and queue_test inside the apps folder to demonstrate the task creation, semaphore and queue usage.

- **TEST_HELLO_TASK**: Demonstrates task creation, scheduling, delays, and optional GPIO interrupt handling.

- **TEST_QUEUE**: Demonstrates inter-task communication using a queue with producer and consumer tasks.

- **TEST_SEMAPHORE_MUTEX**: Demonstrates synchronization using binary Semaphore, counting Semaphore and mutex
