# Submarine Monitoring System

A three-part monitoring system for a submarine, built as a final project
combining embedded systems (C) and object-oriented programming (C++).

## Architecture

The system has three programs, each with a distinct role:

| Component | Language | Runs on | Role |
|---|---|---|---|
| **Local Node Controller (LNC)** | C (ANSI C89 style) | STM32 Nucleo-L476RG | Reads onboard sensors, detects objects, manages local alarms and logging |
| **Central Computer** | C++ | PC | Manages the LNC end unit(s), owned as an object by each combat submarine |
| **Ground Station** | C++ | PC | Requests stored data and events from the Central Computer over time ranges |

The LNC talks to the Central Computer over **UART**, using a framed TLV
(Tag-Length-Value) protocol with a start-of-frame marker and checksum for
resync and error detection. The Central Computer talks to the Ground
Station over **TCP**, using the same TLV message vocabulary without the
UART-specific framing, since TCP already guarantees reliable, ordered
delivery.

## Repository layout

```
submarine/
├── lnc/                   Embedded C firmware (STM32, FreeRTOS)
│   ├── Core/ Drivers/ Middlewares/   CubeMX-generated, not hand-edited
│   └── App/
│       ├── inc/           Headers for all drivers and modules
│       └── src/
│           ├── drivers/   Hardware drivers (LED, buzzer, DHT11, ADC, IR, button)
│           ├── modules/   Application logic (Monitor, Event, Config, ...)
│           └── test/      Test bench + menu-driven per-driver self-tests
├── central_computer/       C++ (in progress)
├── ground_station/         C++ (in progress)
└── docs/                   Design log, protocol spec (not tracked in git)
```

## Hardware

- STM32 Nucleo-L476RG
- A generic Arduino-style sensor shield with: DHT11 (temperature +
  humidity), potentiometer (simulated battery voltage), photoresistor
  (light level), IR receiver (object detection — point a remote at it),
  RGB LED, buzzer, and pushbuttons
- Data-logging shield (SD card + RTC) — present but not yet in use

## Building and flashing the LNC

Requires the `arm-none-eabi` toolchain, CMake, Ninja, and `st-flash`
(stlink-tools).

```bash
cd lnc/build
cmake .. -G Ninja
ninja
arm-none-eabi-objcopy -O binary lnc.elf lnc.bin
st-flash write lnc.bin 0x08000000
st-flash reset
```

Or, with the `stflash` shell function set up (see repo owner's `.bashrc`):

```bash
stflash lnc
```

## Testing

The LNC boots into an interactive test menu over its USART2 UART
(115200 8N1, via the ST-Link virtual COM port). Each driver has its own
self-test, selectable by number:

```
==== LNC Test Menu ====
1) RGB LED test
2) Buzzer test
3) DHT11 test
4) ADC test
5) IR receiver test
6) Alarm-stop button test
```

## Status

- **Stage 1 (drivers) — complete.** All six drivers built and verified
  on physical hardware.
- **Stage 2 (Monitor module)** — next up.
- Central Computer and Ground Station (C++) — not yet started.

## Design decisions

Reasoning behind the protocol format, pin assignments, and various
implementation choices is kept in a running decision log, not tracked in
this repository.