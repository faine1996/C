# Claude Code — Working Brief: Submarine Monitoring System (LNC)

## What this project is
Embedded firmware for the Local Node Controller (LNC) of a submarine
monitoring system. Runs on an STM32 Nucleo-L476RG under FreeRTOS
(CMSIS-V2). Talks to a PC-side Central Computer over UART using a
framed TLV protocol. This repo is the C / embedded half only.

## Golden rules — do not violate
- ALL my application code is ANSI C89 STYLE: declarations at the top
  of each block, `/* */` comments only (no `//`), Allman brace style
  (every brace on its own line, opening AND closing).
- The PROJECT compiles as C11 (HAL/FreeRTOS require it). Do not change
  CMAKE_C_STANDARD. Write C89-style code that also compiles clean as C11.
- My code lives ONLY under App/. Never edit Core/, Drivers/,
  Middlewares/, or cmake/stm32cubemx/ — those are CubeMX-generated.
- Do NOT auto-create or auto-edit files unless I ask. Propose code in
  plain-text blocks first; I paste it in myself.
- Plan first. Before writing any code, describe the approach and wait
  for my approval.
- Explain code line by line, per file, when you present it.
- Flag any error you see in my instructions or existing code.

## Folder layout
    App/inc/              headers
    App/src/drivers/      led, buzzer, dht11, adc, ir, button
    App/src/modules/      monitor, event, config, init, keepalive, watchdog
    App/src/test/         test bench + per-driver self-tests

## Frozen pin map (STM32L476RG) — labels come from main.h
    USART2_TX      PA2      (to Central Computer)
    USART2_RX      PA3      (from Central Computer)
    DHT_DATA       PB5      GPIO, driver toggles direction (temp + humidity)
    ADC1_IN5       PA0      battery (potentiometer), single-ended
    ADC1_IN6       PA1      light (LDR), single-ended
    IR_OBJECT      PB10     GPIO EXTI10, pull-up, falling edge = object detected
    BTN_ALARM      PA10     GPIO input, pull-up, POLLED (not EXTI), pressed = LOW
    RGB_RED        PC6      GPIO output
    RGB_GREEN      PC8      GPIO output   (red+green -> green/yellow/red; no blue)
    BUZZER         PB4      GPIO output
    (deferred, do not use yet: SD on SPI1 PA5/PA6/PA7 + CS PB6)

## Clock
    HCLK = 80 MHz. SystemCoreClock = 80000000. DHT11 us-delays derive
    from SystemCoreClock via DWT cycle counter.

## Build order (bottom-up, each tested alone before the next)
    0. Test bench: printf-over-UART + TEST_CHECK macro + menu runner
    1. Drivers: RGB LED, buzzer, DHT11, ADC, IR, button
    2. Monitor module (sample -> compare limits -> mode)
    3. Event, Object Detection, Config, Init, Keep-Alive, Watchdog
    4. Integrate as FreeRTOS tasks
    Every driver ships WITH its own self-test in App/src/test/.

## TLV wire protocol (LNC <-> Central Computer, over UART)
    Frame:  SOF(0xAA) | TAG(1) | LEN(1) | VALUE(LEN) | CHECKSUM(1)
    CHECKSUM = (TAG + LEN + all VALUE bytes) & 0xFF
    All multi-byte fields LITTLE-ENDIAN.
    Receiver is a 5-state machine, resets to WAIT_SOF on any error.
    (Full tag list + value blocks are in docs/protocol_spec.txt —
     read that file before writing any comm code.)

## When in doubt
    Ask me before assuming. Prefer small, testable pieces over big drops.