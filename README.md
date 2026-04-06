# STM32 Bare-Metal Timer Interrupt Demo

Minimal bare-metal STM32 project using direct register access (no HAL) to generate two output frequencies from a single hardware timer interrupt.

## What it does

- TIM2 generates an interrupt every 10 ms
- PA1 toggles every interrupt → 50 Hz
- PA0 toggles every 5 interrupts → 10 Hz

Signals are verified on an oscilloscope.

## Key ideas

- No HAL
- No blocking delay loop
- One timer as a time base
- Interrupt-driven execution
- Simple multi-rate scheduling

## Hardware

- STM32F411 (or compatible Cortex-M4)
- Two external LEDs on:
  - PA0
  - PA1
- Oscilloscope connected to both outputs

## Build

./build.sh

## Rebuild

./build.sh rebuild

## Flash

./flash.sh

## Run (build + flash)

./run.sh

## Requirements

- arm-none-eabi-gcc
- cmake
- openocd
- ST-Link debugger

## Project structure

.
└── 01_timer_interrupt_blinky
    ├── build.sh
    ├── CMakeLists.txt
    ├── CMSIS/
    ├── Core/
    ├── flash.sh
    ├── .gitignore
    ├── Linker/
    ├── README.md
    ├── run.sh
    └── Startup/

## Example interrupt handler

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;

        GPIOA->ODR ^= (1U << 1U);

        cnt++;
        if (cnt % 5 == 0)
        {
            GPIOA->ODR ^= (1U << 0U);
            cnt = 0;
        }
    }
}

## Why this project matters

This project demonstrates:

- timer configuration using registers
- interrupt handling via NVIC
- hardware-based timing instead of software delays
- simple embedded scheduling without an RTOS
