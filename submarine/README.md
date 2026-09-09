# Submarine Monitoring System

A three-part monitoring system for a submarine, built as a final project
combining embedded systems (C) and object-oriented programming (C++).

## Architecture

The system has three programs, each with a distinct role:

| Component | Language | Runs on | Role |
|---|---|---|---|
| **Local Node Controller (LNC)** | C (ANSI C89 style) | STM32 Nucleo-L476RG | Reads onboard sensors, detects objects, manages local alarms and logging |
| **Central Computer** | C++ | PC | Manages the fleet (research + combat submarines) and the live LNC link, owned as an object by each combat submarine |
| **Ground Station** | C++ | PC | Requests stored data and events from the Central Computer over time ranges |

The LNC talks to the Central Computer over **UART**, using a framed TLV
(Tag-Length-Value) protocol with a start-of-frame marker and checksum for
resync and error detection. The Central Computer talks to the Ground
Station over **TCP**, using the same TLV tags and value formats but a
simpler frame (length + tag + value, no start-of-frame marker or
checksum), since TCP already guarantees reliable, ordered delivery. Both
framings, and the tag/value definitions shared between them, live in
`shared/tlv/` so neither program duplicates the protocol independently.

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
    ├── central_computer/       C++ fleet manager + live LNC link (built, tested)
    ├── ground_station/         C++ client for historical data/event queries (built, tested)
    ├── shared/tlv/             TLV codec shared by central_computer and ground_station
    └── docs/                   Design log, protocol spec

## Hardware

- STM32 Nucleo-L476RG
- A generic Arduino-style sensor shield with: DHT11 (temperature +
  humidity), potentiometer (simulated battery voltage), photoresistor
  (light level), IR receiver (object detection — point a remote at it),
  RGB LED, buzzer, and pushbuttons
- Data-logging shield (SD card + DS1307 RTC) — in use since Stage 6

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

All mode transitions and object detection events are sent to the Central
Computer as framed TLV messages over UART, in addition to being logged
locally to the SD card. The Central Computer (and, through it, the
Ground Station) can also request historical data or events for a given
time range — the LNC reads the matching rows straight back off the SD
card and streams them over the same link.

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
  (TIM3 CH1) to support non-blocking alarm-on/off.
- **Stage 4 (FreeRTOS task integration) — complete.** Monitor, Event,
  Keep-Alive, and Watchdog running as separate FreeRTOS tasks. Monitor
  posts sensor data to Event and Keep-Alive via queues. Watchdog refreshes
  the IWDG every 500ms.
- **Stage 5 (Communication module + TLV framing) — complete.** Comm task
  owns UART2 exclusively: polls for incoming bytes, drains an outgoing
  message queue, and is the only module that calls the UART HAL directly.
  KEEPALIVE, EVENT, and TIME_SYNC_REQ frames are sent as framed TLV
  (SOF + tag + length + value + checksum) matching the Central Computer's
  parser.
- **Stage 6 (Log module) — complete.** Sensor readings and events are
  written to dated CSV files on an SD card (FatFs over SPI), with 7-day
  retention. Verified writing on hardware in all system modes.
- **Stage 7 (Init module) — complete.** On boot, requests a time sync from
  the Central Computer (TIME_SYNC_REQ / SET_TIME), falls back to the
  battery-backed DS1307 RTC if the CC doesn't respond, and reports a
  startup event to both the SD log and the Central Computer.
- **Historical data/event queries (GET_DATA_RANGE / GET_EVENTS_RANGE) —
  complete.** The LNC reads matching rows straight from its SD-card CSV
  logs for a requested time range and streams them back as DATA_ITEM/
  EVENT_ITEM frames; per-row timestamps are reconstructed as true epoch
  time from each file's date plus its wall-clock column, independent of
  the LNC's own uptime-relative clock. Verified end-to-end on real
  hardware through the full chain (Ground Station → Central Computer →
  LNC → SD card and back).
- **Central Computer (C++) — built and tested.** Fleet management
  (add/find/display submarines, assign/update/end missions, companion
  messaging); a live LNC link over UART (KEEPALIVE/EVENT/TIME_SYNC_REQ
  handling, GET_DATA_RANGE/GET_EVENTS_RANGE requests); and a TCP server
  relaying Ground Station queries to the LNC and back on a background
  thread. 37 tests passing, covering the menu/OOP layer and live
  hardware integration.
- **Ground Station (C++) — built and tested.** Connects to the Central
  Computer over TCP, requests measurement or event data for a
  human-readable time range, and displays the returned records.

## Design decisions

Reasoning behind the protocol format, pin assignments, threshold
directions, and all other implementation choices is kept in a running
decision log at `docs/design_log.md`.