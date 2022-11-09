# ARM Buffer Overflow Challenge

### Setup on EK-TM4C123GXL Launchpad

(only necessary if the board does not already have the image flashed)

1. [Install the eCTF Bootstrapper using TI CCS](https://github.com/mitre-cyber-academy/2022-ectf-insecure-example/blob/master/getting_started.md#2a-install-the-device-bootstrapper-first-time-setup)

2. Install Docker

3. Pull the Docker image: `docker pull ectf/ectf-qemu:tiva`

4. Build the system binary: `python3 tools/run_saffire.py build-system --physical --sysname saffire-test --oldest-allowed-version 1`

5. Install the system binary: `python3 tools/run_saffire.py load-device --physical --sysname saffire-test`

### Interfacing with board

1. Plug in the board to your computer's USB port. Wait ~20 seconds until the blue light on the board lights up.

2. Connect to the board using a serial-terminal, at 115200 baud. Recommendation is to use [CoolTerm](https://freeware.the-meiers.org/) as it makes it very easy to send hex-encoded payloads to the device.

3. Take a look at the source-code of the challenge in `bootloader/src/bootloader.c` (and other files in `bootloader/src`). These are two phases which should be completed independently, and the goal is to come up with a payload for each phase which causes a buffer-overflow, takes over program control, and uses this to read out the flag from `FLAG_BUFFER` and print it to the serial port.

### Using GDB

To use GDB with the physical board:

1. Install OpenOCD and an appropriate GDB (recommend `arm-none-eabi-gdb`)

2. Connect to the board with OpenOCD: `openocd -f board/ti_ek-tm4c123gxl.cfg`

3. Launch GDB with the provided binary ELF file: `arm-none-eabi-gdb bootloader.elf`

4. Inside of GDB, connect to the OpenOCD server: `target extended-remote localhost:3333`

5. Inside of GDB, add the source directory to make line-by-line debugging easier: `dir bootloader`


