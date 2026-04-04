# STM32 Bare-Metal Timer Interrupt Blinky

STM32F411RE bare-metal project using:
- CMSIS (no HAL)
- Timer interrupt (TIM2)
- GPIO toggle

Work in progress.

# Files and their roles

## Core/Src/main.c
Application entry point.
Contains the user logic:
- GPIO configuration
- TIM2 configuration
- main loop
- optional interrupt-driven behavior support

## Core/Src/system_stm32f4xx.c
System-level initialization file.
Typically provides:
- SystemInit()
- SystemCoreClock
- low-level clock/reset related setup expected by CMSIS

## CMSIS/Include/core_cm4.h
CMSIS Cortex-M4 core definitions.
Provides:
- CPU register access
- NVIC definitions
- SysTick definitions
- intrinsic functions

## CMSIS/Device/ST/STM32F4xx/Include/stm32f411xe.h
Device-specific register definitions for STM32F411xE.
Provides:
- peripheral base addresses
- register structures
- bit definitions
- IRQ numbers

## CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h
Family-level wrapper header.
Selects the correct device header depending on the target macro.

## CMSIS/Device/ST/STM32F4xx/Include/system_stm32f4xx.h
Header associated with system_stm32f4xx.c.

## Startup/startup_stm32f411xe.s
Startup code and vector table.
Responsible for:
- initial stack pointer
- reset handler
- default interrupt handlers
- jumping into C runtime initialization

## Linker/STM32F411RETX_FLASH.ld
Memory layout description for the linker.
Defines:
- Flash and RAM regions
- placement of .text, .data, .bss
- stack/heap boundaries

## CMakeLists.txt
Build configuration.
Defines:
- compiler
- flags
- source files
- include paths
- linker script
- post-build outputs


## compilation step 1 - applicative code
 arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -g3 -Wall -Wextra -I./CMSIS/Include -I./CMSIS/Device/ST/STM32F4xx/Include -c Core/Src/main.c -o build/main.o

## compilation step 2 - CMSIS/ST system files
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -g3 -Wall -Wextra -DSTM32F411xE \
-I./CMSIS/Include \
-I./CMSIS/Device/ST/STM32F4xx/Include \
-c Core/Src/system_stm32f4xx.c -o build/system_stm32f4xx.o


## compilation step 3 - startup file
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -x assembler-with-cpp -DSTM32F411xE \
-c Startup/startup_stm32f411xe.s -o build/startup_stm32f411xe.o

## compilation step 4 - create file for libc symbols
touch Core/Src/runtime_stubs.c

code:
void _init(void)
{
}

void _fini(void)
{
}

compile
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -g3 -Wall -Wextra -DSTM32F411xE \
-c Core/Src/runtime_stubs.c -o build/runtime_stubs.o

## compilation step 4 - link - generation of .elf
 arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -nostartfiles \
build/main.o \
build/system_stm32f4xx.o \
build/startup_stm32f411xe.o \
build/runtime_stubs.o \
-TLinker/STM32F411RETX_FLASH.ld \
-Wl,-Map=build/output.map \
--specs=nosys.specs \
-o build/timer_interrupt_blinky.elf

## compilation step 5 - optional generation of .bin for st-flash for instance
arm-none-eabi-objcopy -O binary build/timer_interrupt_blinky.elf build/timer_interrupt_blinky.bin

## compilation step 6 - check .elf size
arm-none-eabi-size build/timer_interrupt_blinky.elf

   text	   data	    bss	    dec	    hex	filename
    888	      4	      0	    892	    37c	build/timer_interrupt_blinky.elf

text = 888
code + constants in Flash.

data = 4
5 octets of initialized data copied in RAM at startup (from SystemCoreClock presumably)

bss = 0
no non initialized global variables

## README.md
Project overview and usage instructions.