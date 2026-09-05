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

    cd lnc/build
    cmake .. -G Ninja
    ninja
    arm-none-eabi-objcopy -O binary lnc.elf lnc.bin
    st-flash write lnc.bin 0x08000000
    st-flash reset

Or, with the `stflash` shell function set up (see repo owner's `.bashrc`):

    stflash lnc

## How the LNC currently behaves

The LNC boots and immediately starts the monitor/event loop. Every
5 seconds it samples all four sensors and compares each reading against
configured thresholds to determine the system mode:

| Mode | Condition |
|---|---|
| **Normal** | All readings within Normal range — LED green |
| **Warning** | At least one reading in Warning range, none in Error — LED yellow |
| **Error** | At least one reading in Error range — LED red, buzzer on |

Default thresholds (all configurable later via the Central Computer):

| Sensor | Normal | Warning | Error |
|---|---|---|---|
| Temperature | 18–28 °C | 10–17 °C or 29–35 °C | < 10 °C or > 35 °C |
| Humidity | 30–60 % | < 30 % or 60–75 % | > 75 % |
| Battery (raw ADC) | >= 3000 | 1500–2999 | < 1500 |
| Light (raw ADC) | >= 700 | 300–699 | < 300 |

Object detection (IR receiver, simulating sonar) runs in parallel:
pointing a remote at the sensor triggers an alarm (LED red, buzzer on)
independently of the monitor mode. Pressing the alarm-stop button clears
the buzzer and restores the LED to the underlying monitor mode.

All mode transitions and object detection events are printed over UART
and stubbed for transmission to the Central Computer (to be wired up
in a later stage).

## Testing

During Stages 1–2, the LNC booted into an interactive test menu over
USART2 (115200 8N1, via the ST-Link virtual COM port):

    ==== LNC Test Menu ====
    1) RGB LED test
    2) Buzzer test
    3) DHT11 test
    4) ADC test
    5) IR receiver test
    6) Alarm-stop button test
    7) Monitor test

From Stage 3 onward the LNC runs the event loop directly on boot.
The menu is preserved in the source and can be restored for driver-level
debugging by swapping the task body in Core/Src/freertos.c.

## Status

- **Stage 1 (drivers) — complete.** All six drivers built and verified
  on physical hardware: RGB LED, buzzer, DHT11, ADC (battery + light),
  IR receiver, alarm-stop button.
- **Stage 2 (Monitor module) — complete.** Samples all four sensors every
  5 seconds, classifies each reading into Normal/Warning/Error zone,
  combines into a single system mode per spec section 2.10.
- **Stage 3 (Event module + Object Detection) — complete.** Reacts to
  monitor mode transitions and IR object detection events: drives LED and
  buzzer, handles alarm-stop button, sets suppression flag, prints event
  and send-to-CC stubs. Buzzer rewritten from bit-bang to hardware PWM
  (TIM3 CH1) to support non-blocking alarm-on/off. Button press restores
  LED to the mode that was active before the alarm started; alarm
  re-triggers on the next sample cycle if the underlying condition
  persists.
- **Stage 4 (FreeRTOS task integration) — next up.**
- Central Computer and Ground Station (C++) — not yet started.

## Design decisions

Reasoning behind the protocol format, pin assignments, threshold
directions, and all other implementation choices is kept in a running
decision log, not tracked in this repository.