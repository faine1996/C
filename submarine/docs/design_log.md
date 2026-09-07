# Design Decision Log — Submarine Monitoring System (LNC)

One entry per decision. Format: Decision / Reasoning / Alternatives considered.

---

## [Architecture] Three-program split
- Decision: LNC in C (embedded, STM32), Central Computer and Ground Station
  each as separate C++ programs on the PC.
- Reasoning: lecturer's explicit instruction. LNC needs hardware timing (C);
  CC/GS manage fleet data and objects (C++ fits the OOP deliverable).
- Alternatives considered: single C++ "Central Computer" also acting as
  ground station — rejected once lecturer specified two separate programs.

## [Architecture] Two transports, two framings
- Decision: LNC <-> Central Computer over UART, framed TLV (SOF + checksum).
  Central Computer <-> Ground Station over TCP, length-prefixed TLV, no
  SOF/checksum.
- Reasoning: UART is a raw byte-ribbon with no delivery guarantee, so it
  needs a resync mark and integrity check. TCP already guarantees ordered,
  reliable delivery, so that envelope would be redundant there — only a
  length prefix is needed since TCP is a stream.
- Alternatives considered: same framing on both links — rejected as
  unnecessary overhead on the TCP side.

## [Protocol] Frozen TLV wire spec v1.0
- Decision: SOF(0xAA) | TAG(1) | LEN(1) | VALUE(LEN) | CHECKSUM(1), all
  multi-byte fields little-endian, checksum = sum of TAG+LEN+VALUE bytes.
- Reasoning: minimal envelope that lets a receiver resync after a glitch
  and detect corruption, without ACK/NACK complexity the spec never asked
  for.
- Alternatives considered: no envelope (rejected — unrecoverable on
  desync); ACK/NACK per frame (rejected — not required by spec, adds
  complexity).

## [Protocol] Epoch-second timestamps on the wire
- Decision: all VALUE-block timestamps are uint32 epoch seconds. SET_TIME
  is the exception and carries broken-down Y/M/D/H/M/S because that's what
  the RTC hardware needs.
- Reasoning: epoch makes range queries a simple integer compare; broken-down
  fields would need per-field comparison logic.

## [Protocol] Single SET_CONFIG tag with param_id
- Decision: one tag (0x20) carries a param_id byte plus a variable payload,
  rather than one tag per config parameter.
- Reasoning: keeps the tag space small and the config-handling code in one
  place; LEN naturally tells the parser how many payload bytes follow.

## [Protocol] TIME_SYNC_REQUEST tag added (0x13)
- Decision: LNC sends TIME_SYNC_REQUEST at startup; Central Computer
  responds with SET_TIME.
- Reasoning: resolves a contradiction in the spec doc (Init module "requests"
  time sync from the CC per one section, but "set date/time" is listed as
  a CC-initiated command in another). This split honors both: LNC asks,
  CC still owns the actual SET_TIME push.
- Note: flag this to the instructor as a spec inconsistency, not an
  oversight on our part.

## [Hardware] IR receiver = "sonar" / object detection
- Decision: the shield's IR receiver module fulfills the spec's "sonar,
  object-detection" requirement. Point a remote at it = object detected;
  stop = object cleared.
- Reasoning: the spec lists 4 Monitor readings (temp, humidity, battery,
  light) plus a separate object-detection component. DHT11 covers temp+
  humidity, pot covers battery, LDR covers light — leaving object detection
  unassigned and the IR receiver unused. They fill each other's gap, and
  matches the lecturer's mention of pointing a TV remote at the board.
- Note: confirm with instructor; treated as very likely, not certain.

## [Hardware] Button is polled, not EXTI
- Decision: alarm-stop button (PA10) is a plain polled GPIO input, not an
  external interrupt.
- Reasoning: PA10 and the IR receiver's PB10 both sit on EXTI line 10;
  STM32 allows only one pin per EXTI line number. Polling the button avoids
  the collision and also avoids switch-bounce interrupt storms. The button
  doesn't need instant reaction the way object detection does.
- Alternatives considered: move one signal to a different pin (rejected —
  polling is simpler and the button's response time isn't safety-critical).

## [Hardware] RGB LED reuses the two discrete LEDs' pins (PC6, PC8)
- Decision: RGB LED's red and green channels are jumpered onto PC6/PC8
  (freed from the now-unused discrete LEDs), not left on their native
  shield pins.
- Reasoning: the RGB LED's native pins (D9/D10/D11 = PC7/PB6/PA7) collide
  with the SD card's SPI bus (PB6 = CS, PA7 = MOSI). PC6/PC8 are already
  proven-working GPIO outputs from the earlier logger project and sit
  nowhere near the SD bus, so files can be re-enabled later with zero
  rewiring. Only red+green are wired since 3 colors (green/yellow/red)
  need just 2 channels — no PWM needed.
- Alternatives considered: find two other free pins (rejected — reusing
  known-good, already-jumpered pins is lower risk with 5 days on the
  clock).

## [Hardware] Files (SD/FatFs) and RTC deferred
- Decision: SPI1/SD card and RTC are not enabled in this phase.
- Reasoning: reduces RAM pressure (FatFs work areas are a real chunk of the
  L476's 128 KB) and removes moving parts while the core system is being
  built and tested. LNC gets time from the Central Computer via
  TIME_SYNC_REQUEST/SET_TIME instead of needing a battery-backed RTC.

## [Firmware] Clock: HCLK = 80 MHz via PLL from MSI
- Decision: system clock configured to 80 MHz (not the CubeMX default of
  4 MHz off raw MSI).
- Reasoning: the DHT11 driver's microsecond delay loops derive their
  timing from SystemCoreClock (via DWT cycle counter). At 4 MHz instead of
  80 MHz, every delay is 20x too short and DHT11 reads fail in a way that
  looks like a broken sensor, not a clock misconfiguration.

## [Firmware] HAL timebase on TIM6, FreeRTOS owns SysTick
- Decision: kept CubeMX's default of running HAL's 1ms tick off TIM6
  instead of SysTick.
- Reasoning: FreeRTOS needs SysTick for its own scheduler tick; sharing it
  with HAL causes timing conflicts. This separation was already proven
  stable in the earlier logger project.

## [Firmware] C89-style application code inside a C11-compiled project
- Decision: all App/ code is written in ANSI C89 style (top-of-block
  declarations, /* */ comments, Allman braces), but the project as a whole
  compiles under C11 (CMAKE_C_STANDARD 11).
- Reasoning: the CubeMX-generated CMakeLists explicitly errors out below
  C11 ("Generated code requires C11 or higher") because HAL/FreeRTOS use
  C11 features. Forcing strict C89 project-wide is not possible without
  breaking the generated code.
- Note: if graded against strict C89, add a per-file compile flag on just
  the App/ sources rather than changing the project standard.

## [Firmware] Prefix increment/decrement (++x) as project convention
- Decision: use ++x / --x rather than x++ / x-- throughout, going forward.
- Reasoning: prefix has no obligation to preserve a pre-increment copy of
  the value, which is a marginally cleaner default in C. Style choice, no
  functional impact expected for the plain counters used so far.

## [Testing] Test bench built before any driver
- Decision: build a printf-over-UART harness (__io_putchar retarget) + a
  TEST_CHECK pass/fail macro + a menu runner, before writing the first
  real driver.
- Reasoning: lecturer's instruction to test each sensor/section "alone."
  The menu runner is a fixed structure that every future driver's self-test
  plugs into with one added case, giving continuous testability as modules
  are added one at a time.

## [Testing] Menu task's blocking UART receive — deferred issue
- Decision: Stage 1's menu task uses a hard-blocking HAL_UART_Receive
  (HAL_MAX_DELAY), not interrupt/DMA-driven receive.
- Reasoning: acceptable now because the menu task is the only task in the
  system — nothing else is waiting for CPU time. NOT acceptable once
  Monitor/Event/Keep-Alive tasks exist, since a hard block never yields to
  the FreeRTOS scheduler and would starve them.
- Action required: revisit before Stage 4 (task integration) — switch RX
  to interrupt or DMA-driven, delivered via a queue, before adding
  concurrent tasks.

## [Documentation] Function header comment style — Doxygen tags
- Decision: header (.h) function declarations get Doxygen-style doc
  comments (@brief, @param, @return) rather than freeform
  Description/Expected Input/Output labels.
- Reasoning: Doxygen tags are recognized by VS Code's IntelliSense and any
  doc generator, surfacing in hover tooltips — freeform labels are not
  machine-readable. Content is otherwise equivalent.

## [Testing] Stage 1 complete — RGB LED driver verified on hardware
- Decision: RGB LED driver (RgbLed_Init/SetMode/Test) confirmed working via
  the menu test bench. Colors correct: green (Normal), yellow (Warning, both
  channels), red (Error), off.
- Reasoning: visual confirmation on the physical board, cross-checked
  against the frozen pin map (PC6 = red, PC8 = green).

## [Build] st-flash --reset flag unreliable; use two separate commands
- Decision: flash and reset as two separate st-flash invocations
  (`st-flash write ... ` then `st-flash reset`), not the combined
  `st-flash --reset write ...` form.
- Reasoning: on this setup (st-flash 1.8.0, STM32L476RG), the combined
  --reset flag did not reliably restart the core after flashing — the
  physical RESET button was still required. Running `st-flash reset` as
  its own step after the write works correctly every time.

## [Build] Never flash a stale .bin — always rebuild the full chain
- Decision: the flash sequence is always: build (ninja/cmake --build) ->
  objcopy ELF to BIN -> st-flash write -> st-flash reset, run as one
  atomic sequence, never as separately-remembered manual steps.
- Reasoning: root-caused a debugging session where a driver fix appeared
  to have no effect on hardware — the .elf was rebuilt correctly each
  time, but `objcopy` (ELF -> BIN) was never re-run, so `st-flash` kept
  flashing an old, unrelated .bin file. Confirmed via matching MD5
  checksums across "different" builds. Lesson: a matching/unchanged MD5
  after a supposed rebuild is a strong signal the .bin was never
  regenerated from the current .elf.
- Action taken: added/updated the `stflash <project>` shell function in
  .bashrc to run the full build -> objcopy -> flash -> reset chain as a
  single command, for this and future STM32 projects, so this class of
  mistake can't recur.

## [Firmware] Menu UART receive: RXNE-flag drain, not a timeout guess
- Decision: after reading the user's menu selection, drain any leftover
  bytes (e.g. trailing \r\n from a terminal) by checking the UART_FLAG_RXNE
  hardware flag directly in a loop, not by guessing a timeout duration.
- Reasoning: an earlier version drained with a fixed ~20ms HAL_UART_Receive
  timeout, which failed intermittently — likely because the ST-Link's
  USB-to-serial bridge introduces variable latency that isn't safely
  bounded by a guessed constant. Checking RXNE directly asks the hardware
  "is a byte actually present" instead of "did enough time pass," which is
  deterministic regardless of USB CDC timing.

## [Firmware] UART overrun (ORE) must be cleared explicitly on read failure
- Decision: any time a UART receive call does not return HAL_OK, explicitly
  clear the overrun error flag (__HAL_UART_CLEAR_OREFLAG) before continuing.
- Reasoning: the menu's UART receive is polling-only with no interrupt/DMA
  buffering, so the peripheral has exactly one hardware byte slot. Sending
  multiple keystrokes in a burst while the CPU is busy elsewhere (e.g.
  during the LED test's HAL_Delay calls) overruns that slot and sets the
  ORE flag, after which the UART silently refuses further bytes until the
  flag is cleared in software.
- Known limitation, accepted for Stage 1: bytes sent while a blocking test
  (like RgbLed_Test) is running will still be lost even with this fix,
  because the CPU is not polling UART during that window. Expected
  workflow for now: send one selection, wait for it to finish, then send
  the next. Proper fix (interrupt/DMA-driven RX via a queue) is deferred
  to Stage 4 task integration, same as the earlier blocking-RX item.

## [Testing] Buzzer driver verified on hardware
- Decision: Buzzer driver (Buzzer_Init/On/Off/Test) confirmed working —
  audible ~2kHz beep for ~0.5s via bit-banged GPIO toggling on PB4.
- Reasoning: shield buzzer is passive (confirmed by symptom: constant
  HIGH via HAL_GPIO_WritePin produced no sound), not active. Passive
  buzzers need a toggled square wave, not a steady voltage, to produce
  tone. Matches earlier coursework reference to "buzzer ADT with melody
  playback," which is only possible on a passive buzzer.
- Known limitation, accepted for now: Buzzer_On() blocks the CPU for its
  fixed duration and self-stops; it cannot yet do "stay on until told to
  stop," which the Event module's alarm behavior needs. Proper fix is
  hardware PWM (PB4 supports TIM3_CH1) instead of bit-banging. Deferred
  until the Event/alarm module is built.

## [Firmware] Self-initializing DWT delay helper (bug fix + new pattern)
- Decision: hardware-timing helper functions (e.g. Buzzer_DelayUs) check
  and perform their own one-time DWT cycle-counter setup internally,
  rather than relying on a separate Init() call being made first.
- Reasoning: root-caused a hang in the buzzer driver — Buzzer_Init() (which
  enables DWT->CYCCNT) was not being called before Buzzer_DelayUs() used
  it, so CYCCNT never incremented and the delay's wait condition was
  permanently true, causing an infinite loop with no sound. Making the
  delay helper self-initializing (a static "already initialized" guard)
  makes this class of bug structurally impossible instead of relying on
  remembering call order.
- Action: apply the same self-init guard to any future driver that reads
  DWT->CYCCNT for timing (the DHT11 driver will need this).

  ## [Testing] ADC driver verified on hardware — battery and light channels
- Decision: ADC driver (Adc_Init/ReadBattery/ReadLight/Test) confirmed
  working. Battery (PA0/ADC1_IN5): smooth monotonic sweep from 0 to 4095
  across the potentiometer's full range. Light (PA1/ADC1_IN6): responsive,
  varying readings once a physical connection issue was resolved (see
  below).
- Reasoning: both channels share one Adc_ReadChannel() implementation
  parameterized by channel number; validating one channel's code path via
  the pot's smooth sweep gave confidence the same code was correct for the
  light channel once its wiring issue was fixed.

## [Hardware] LDR light sensor had a bad/floating connection — resolved
- Decision: none needed in code; issue was physical (loose/incorrect
  connection on the LDR's signal, VCC, or GND leg to PA1).
- Reasoning: light channel initially read a fixed 4095 (full-scale) across
  many reads regardless of ambient light or the potentiometer being swept
  — the signature of a floating ADC input, not sensor noise. Battery
  channel's identical code path proved correct via a clean pot sweep in
  the same test session, ruling out the driver. After re-checking the
  physical connection, light readings began varying normally.
- Lesson for future drivers: a value pinned exactly at 0 or 4095 (the ADC
  rails) with zero variance across repeated reads is a strong signal of a
  floating/disconnected input, not a real reading — worth checking wiring
  before suspecting the code, especially when an identical code path is
  already proven correct on a different channel.

  ## [Testing] IR receiver driver verified on hardware
- Decision: IR driver (Ir_Init/IsObjectDetected/Test) confirmed working via
  HAL_GPIO_EXTI_Callback on PB10, falling edge. A single remote button
  press produces a burst of many falling edges (confirmed: count jumped by
  dozens to hundreds per press), consistent with remotes retransmitting
  their code repeatedly while held.
- Reasoning: interrupt-driven detection (vs. polling) means the CPU is
  never blocked waiting for this event and cannot miss a fast pulse burst.
  Ir_IsObjectDetected() only ever latches to "detected" — it does not
  self-clear back to "not detected." That clearing logic (comparing
  previous vs. current state to decide "object cleared") is Object
  Detection module responsibility, not this driver's, per spec section
  2.2.

## [Testing] Alarm-stop button driver verified on hardware
- Decision: Button driver (Button_Init/IsPressed/Test) confirmed working
  via polled GPIO read on PA10. Six deliberate presses correctly produced
  six clean PRESSED/RELEASED pairs, with no extra bounce-artifact
  transitions in this particular test run.
- Reasoning: polling (not EXTI) was already the frozen decision, to avoid
  the PA10/PB10 shared EXTI-line-10 collision with the IR receiver.
- Known limitation, not yet handled: mechanical switch bounce can still
  produce multiple rapid transitions for a single physical press,
  especially a fast one — this test run didn't exhibit it, but it is a
  real, universal behavior of mechanical buttons and is not guaranteed to
  be absent in future presses. Debouncing (e.g. requiring N consecutive
  stable polls before accepting a state change) is deferred to whichever
  module implements the real alarm-stop behavior — not added to this
  driver, since the debounce strategy is a design decision that belongs
  with that module's timing requirements, not hidden inside the raw pin
  read.

## [Milestone] Stage 1 complete — all six drivers built and verified
- Decision: Stage 1 (drivers layer) is complete. All six drivers — RGB
  LED, buzzer, DHT11, ADC (battery + light), IR receiver, alarm-stop
  button — are implemented, each with its own self-test wired into the
  menu test bench, and each has been verified working on real hardware.
- Next: Stage 2, the Monitor module — samples all four Monitor readings
  (temp, humidity, battery, light) every 5 seconds via these drivers,
  compares each against configured limits, and determines system mode
  (Normal/Warning/Error).

  ## [Config] Stage 2 default Normal/Warning/Error limits
- Decision: default limit values for the Monitor module's four readings,
  to be loaded via the Configuration module (Flash-backed, per spec 2.6),
  with these as initial defaults:

  | Reading | Normal | Warning | Error |
  |---|---|---|---|
  | Temperature | 18-28 C | 10-17 C or 29-35 C | <10 C or >35 C |
  | Humidity | 30-60% | 60-75% or <30% | >75% |
  | Battery (raw ADC) | 3000-4095 | 1500-2999 | <1500 |
  | Light (raw ADC) | >700 (bright/ambient) | 300-700 | <300 (blocked/dark) |

- Reasoning, per reading:
  - Temperature/humidity: reasonable interior comfort/safety bounds for a
    submarine; not derived from spec, treated as sensible defaults,
    changeable via Configuration.
  - Battery: raw ADC value from the potentiometer has no inherent
    real-world meaning (it's a simulated battery, not a real voltage
    sense circuit) — direction was a deliberate choice: HIGH raw value =
    healthy battery, LOW = depleted. Confirmed explicitly rather than
    assumed, since the reverse mapping is equally arguable and would
    require flipping every comparison if picked wrong.
  - Light: also a deliberate direction choice, not derived from spec —
    BRIGHT = Normal, DARK = Warning/Error was chosen because the intended
    use is a desk/indoor demo where the sensor should read Normal under
    ordinary ambient light and only trip to Warning/Error when
    deliberately covered/blocked. The alternative reading (dark = Normal,
    representing "submerged, no external light," with bright = anomaly
    representing a hull breach) is equally defensible from the spec's
    perspective and was explicitly rejected in favor of demo practicality.
  - Light threshold (700/300) was tuned down from an initial guess
    (2000/800) after measured ambient desk-lighting conditions at night
    read ~1000-1100 raw — the initial guess would have put normal desk
    light in the Warning band, which contradicts the intent above.
- Note: both the battery and light directional choices are demo/design
  conventions, not requirements stated in the spec document. If graded
  against a specific expected interpretation, flag this as an assumption
  made and documented, not an oversight.
- Note: threshold values and comparison direction are kept as named
  constants separate from the Monitor's comparison logic specifically so
  either can be changed later (e.g. if light's direction needs reversing)
  without touching the surrounding code.

  ## [Architecture] FreeRTOS chosen over bare-metal for the LNC
- Decision: the LNC runs under FreeRTOS (CMSIS-V2), not a bare-metal
  single-loop design.
- Reasoning: the LNC has several genuinely concurrent, independently-timed
  responsibilities — periodic Monitor sampling (5s), periodic Keep-Alive
  (6s), continuous/instant Object Detection and Event reaction, UART
  command handling at any time, and Watchdog refresh. Hand-interleaving
  all of this in one bare-metal loop is error-prone (a single blocking
  operation, like a HAL_Delay or a slow sensor read, stalls everything
  else behind it — already observed once with the test menu's blocking
  UART receive). FreeRTOS's task/priority model lets each concern be
  written independently and lets high-priority reactions (Event/alarm)
  preempt lower-priority routine work (Monitor sampling), matching the
  task-priority scheme already designed for Stage 4.
- Trade-off acknowledged: FreeRTOS costs RAM (a stack per task) and adds
  scheduling overhead and design complexity versus bare-metal. This is
  part of why task count is being kept deliberately low and why SD/RTC
  remain deferred — to protect the L476's RAM budget.
- Also relevant: FreeRTOS (tasks, queues, semaphores, mutexes) was
  explicit embedded coursework content, so demonstrating it is expected,
  not merely convenient.

## [Architecture] Object Detection (IR) is intentionally separate from Monitor
- Decision: the IR/object-detection module does not feed into or get
  checked by the Monitor module's 5-second sample/limit-comparison cycle.
  It has its own interrupt-driven path straight to the Event module.
- Reasoning: spec section 2.1 (Monitor) explicitly lists only temperature,
  humidity, battery, and light as the four periodic readings it samples
  and mode-compares. Spec section 2.2 (Object Detection) is explicitly
  continuous/always-listening, not periodic, and sends directly to Event
  with no mode/limit logic of its own. Folding IR into Monitor's 5-second
  tick would risk missing brief object-detection events between samples —
  exactly why IR was made interrupt-driven in Stage 1. The alarm-stop
  button is similarly excluded from Monitor for the same reason: it is a
  control input for Event, not a sensed environmental condition Monitor
  evaluates.

  ## [Monitor] MonitorStatus_t reports first error only, not all errors

- Decision: Monitor_Sample() returns the first driver error encountered
  (DHT11 > battery > light, in read order). If multiple drivers fail
  simultaneously, only the first failure is visible in the return value.
- Reasoning: simultaneous multi-driver failure is unlikely in normal
  operation, and the caller (Event module, later) needs to know "something
  failed" more than it needs to know the exact combination. First-error-only
  is the simplest convention that satisfies that requirement.
- Alternatives considered: bitmask return (one bit per driver, all failures
  visible simultaneously) — rejected as unnecessary complexity for the
  expected failure modes of this hardware.
- Note: if a future stage needs to distinguish partial-failure combinations
  (e.g. log which sensors are degraded), replace MonitorStatus_t with a
  uint8_t bitmask and update Monitor_Sample() accordingly.

  ## [Milestone] Stage 2 complete — Monitor module built and verified

- Decision: Monitor module (Monitor_Init / Monitor_Sample / Monitor_Test)
  confirmed working on hardware. All six TEST_CHECK assertions pass across
  all runs.
- Verified behaviours:
  - Temperature (26 C) correctly classified NORMAL throughout.
  - Humidity correctly flips between WARNING (75%) and ERROR (76%) at the
    HUM_WARNING_HIGH boundary — real ambient humidity was near that threshold
    during testing.
  - Battery correctly sweeps NORMAL / WARNING / ERROR as the potentiometer
    is adjusted. Battery raw = 0 correctly reads as ERROR (not a driver
    error — 0 is a valid ADC value, not the 0xFFFF sentinel).
  - Light correctly drops to WARNING (~578) when partially covered and
    recovers to NORMAL when uncovered.
  - system_mode correctly reflects the worst-case reading in every run.
- Next: Stage 3 — Event module (mode-change detection, LED and buzzer
  response, button alarm-stop, notifications to Central Computer).

  ## [Monitor] combine_zones — Error check order is load-bearing

- Decision: in combine_zones(), the ZONE_ERROR check across all four
  readings is performed before the ZONE_WARNING check, not after.
- Reasoning: the spec defines Warning as "at least one reading in Warning
  range AND no reading in Error range." If the Warning check ran first, a
  reading that is Error would satisfy the Warning OR condition and return
  ZONE_WARNING before the Error check was ever reached — a silent
  misclassification. Checking Error first means any Error reading short-
  circuits to ZONE_ERROR immediately, regardless of how many other readings
  are also in Error. The order of the two if-blocks is load-bearing, not
  cosmetic.
- Note: the Error OR condition covers all four readings simultaneously —
  one, two, three, or four simultaneous Error readings are all caught
  correctly by the same branch.

  ## [Event] Object detection and mode-change are parallel alarm paths,
   both suppress non-essential operations

- Decision: both a Monitor Error transition and an Object Detection event
  set s_suppressed = 1. Clearing follows the same rules for both paths:
  transition to Normal or Warning clears suppression and stops the alarm.
- Reasoning: the spec treats object detection and monitor mode-change as
  independent sources of alarm, but the suppression requirement ("suppress
  non-essential operations" on Error/alarm) is a system-wide safety
  behaviour, not specific to one source. Both conditions represent a
  situation serious enough to warrant suppression. Treating them as
  parallel paths that both suppress is the conservative and consistent
  choice.
- Note: s_suppressed is a flag set and checked in this stage; the modules
  that honour it (Log, Keep-Alive, etc.) are not yet built. Consumers must
  check this flag before performing non-essential work.

## [Event] Stage 3 testing — observe behaviour directly, no menu entry

- Decision: Event_Test() is not wired into the menu. Stage 3 is verified
  by running the main loop directly and observing LED colour, buzzer, and
  printf output on the terminal.
- Reasoning: the Event module is a run loop, not a one-shot test. A menu
  entry that blocks inside Event_Run() would make the menu unusable, and
  a one-shot snapshot would not exercise transitions. Direct observation
  is the correct verification method for this module.
- Action: menu stays at 7 items. Event_Init() and the Event_Run() loop
  live in main.c for Stage 3; in Stage 4 they move into a FreeRTOS task.

  ## [Milestone] Buzzer driver rewritten — PWM replaces bit-banging

- Decision: Buzzer_On() and Buzzer_Off() now use HAL_TIM_PWM_Start /
  HAL_TIM_PWM_Stop on TIM3 CH1 (PB4, GPIO_AF2_TIM3). The blocking
  bit-bang toggle loop is removed entirely.
- Reasoning: the Event module needs "alarm stays on until told to stop,"
  which a blocking self-stopping implementation cannot provide. PWM
  hardware handles the square wave autonomously; the CPU starts and stops
  it with a single HAL call and is free to do other work between them.
- Parameters: PSC=0, ARR=39999, CCR=20000 → 2 kHz at 50% duty cycle
  from HCLK=80 MHz. Frequency matches the tone confirmed audible during
  Stage 1 bit-bang testing.
- BUZZER_Pin / BUZZER_GPIO_Port macros are gone from main.h by design —
  CubeMX removes plain GPIO labels when a pin is reassigned to an
  alternate function. The driver no longer references them.
- Verified: audible beep on hardware via menu option 2.
- Note: Buzzer_Test() still blocks for 500 ms via HAL_Delay — acceptable
  for a one-shot menu test. The Event module calls Buzzer_On/Off directly
  and never calls Buzzer_Test().

  ## [IR Driver] Ir_Clear() added — latch reset is caller responsibility

- Decision: added Ir_Clear() to ir_receiver.h/c. Resets s_object_detected
  to 0 so that a subsequent Ir_IsObjectDetected() call returns 0, allowing
  ObjDet_Poll() to fire OBJDET_CLEARED on the next poll.
- Reasoning: the IR driver was deliberately built to latch on detection
  and never self-clear, with clearing deferred to the module that owns
  state transition logic. That module is now the Event module. Without
  Ir_Clear(), OBJDET_CLEARED is permanently dead code — ObjDet_Poll()
  would never see a 1->0 transition because the latch never goes to 0.
- Where it is called: Event module, after acting on OBJDET_DETECTED and
  deciding the detection cycle is complete (alarm stopped or condition
  cleared). Not called from ObjDet_Poll() itself — Object Detection
  reports transitions, it does not decide when a detection is resolved.

  ## [Milestone] Stage 3 complete — Event module, Object Detection, and
   buzzer PWM fix built and verified on hardware

- Verified behaviours:
  - Monitor mode transitions (Normal/Warning/Error) correctly drive LED
    colour and buzzer, with printf stubs for event log and send-to-CC.
  - Object detection (IR remote) correctly fires OBJDET_DETECTED, sets
    LED red, activates buzzer alarm, sets suppression flag.
  - Button press correctly stops alarm on rising edge, restores LED to
    underlying monitor mode, clears IR latch, clears suppression.
  - OBJDET_CLEARED fires correctly on next poll after Ir_Clear() is
    called by button handler — expected ordering.
  - LED yellow->green after button press is correct: yellow reflects the
    real underlying monitor mode (Warning) at the time of button press;
    green follows on the next sample when conditions return to Normal.
- Bugs found and fixed during this stage:
  - Event_Init() was inside the FreeRTOS task loop instead of outside it,
    causing s_prev_mode to reset to NORMAL on every cycle and firing a
    fresh ERROR transition every iteration.
  - Default FreeRTOS task stack (128*4 = 512 B) too small for Event_Run
    call depth. Increased to 512*4 = 2048 B.
  - BUZZER_Pin/BUZZER_GPIO_Port macros removed from main.h by CubeMX
    after PB4 reassigned to TIM3_CH1 alternate function — old bit-bang
    buzzer.c references removed, driver rewritten to HAL_TIM_PWM_Start/
    Stop.
  - prev_pressed in poll_button_if_alarm() was a plain local variable,
    resetting to 0 on every call — debounce had no memory between polls.
    Fixed by making it static. Then simplified to rising-edge detection
    (act on first press, not second) for natural single-press feel.
  - Missing includes in event.c: ir_receiver.h (for Ir_Clear) and
    stm32l4xx_hal.h (for HAL_Delay) caused implicit declaration warnings.
- Next: Stage 4 — FreeRTOS task integration, replacing HAL_Delay with
  osDelay, splitting Event/Monitor/Keep-Alive into separate tasks.

  ## [Event] Button restores LED to pre-alarm mode, not current s_prev_mode

- Decision: added s_pre_alarm_mode static variable to event.c. Captured
  when an alarm starts (both monitor Error transition and object detection).
  Button press restores LED to s_pre_alarm_mode instead of s_prev_mode.
- Reasoning: s_prev_mode is updated to the current mode after every
  transition, so by the time the alarm is active s_prev_mode holds
  ZONE_ERROR — restoring to it left the LED red after the buzzer stopped.
  s_pre_alarm_mode captures what the mode was immediately before the alarm
  started, which is the correct LED state to return to.
- Known behaviour: if the underlying condition that caused the alarm is
  still present when the button is pressed, the next Event_Run cycle will
  re-sample and immediately re-trigger the alarm. This is accepted for
  now — the button stops the current alarm instance, not the underlying
  condition.
- Note: Ir_Clear() is also called from the button handler even when the
  alarm was triggered by the monitor path rather than object detection.
  This is harmless — clearing an already-clear latch is a no-op — but
  could be tightened later by tracking which path triggered the alarm.

  ## [Stage 4] Inter-task communication — FreeRTOS queues, one per consumer

- Decision: Monitor task posts MonitorData_t to two separate FreeRTOS
  queues — one for the Event task, one for the Keep-Alive task. Each
  consumer blocks on osMessageQueueGet until Monitor posts, then acts.
- Reasoning: the spec consistently uses "sends a message to" language for
  module-to-module communication, which maps directly to a queue. Queues
  also decouple producers from consumers — Monitor does not know or care
  who reads its output. Two separate queues solve the two-reader problem
  cleanly: whoever reads first does not starve the other.
- Alternatives considered:
  - Shared global struct with mutex — rejected. Consumers would need to
    poll for new data with no natural wake-up mechanism. Adds coupling
    between modules via shared state.
  - Event flags + shared struct — rejected. More moving parts (struct +
    mutex + flag group) for no benefit at the 5-second sampling rate used
    here. Race condition risk between readers is low but non-zero.
- Note: MonitorData_t is 12 bytes — copied into the queue on each post.
  Acceptable overhead at a 5-second production rate.
- Note: Keep-Alive always wants the latest measurement. At 5-second
  Monitor sampling and 6-second Keep-Alive intervals the queue depth of
  1 is sufficient — Keep-Alive will always find a fresh reading waiting.

  ## [Object Detection] ObjDet_Reset() added — sync with Ir_Clear()

- Decision: added ObjDet_Reset() to object_detection.h/c. Resets
  s_prev_detected to 0 to match the IR latch state after Ir_Clear()
  is called. Must be called alongside every Ir_Clear() call.
- Reasoning: Ir_Clear() resets the hardware latch (ObjectDetected = 0)
  but ObjDet_Poll() has its own s_prev_detected state. If s_prev_detected
  stays at 1 after Ir_Clear(), the next IR detection sees current=1
  vs prev=1 — no transition fires, OBJDET_NONE returned, alarm never
  triggers again. Keeping both in sync is the correct fix.
- Called from: poll_button_if_alarm() and handle_object_event()
  OBJDET_CLEARED branch in events.c, alongside every Ir_Clear() call.

  ## [FreeRTOS] configTOTAL_HEAP_SIZE increased from 3000 to 15360 bytes

- Decision: changed configTOTAL_HEAP_SIZE in FreeRTOSConfig.h from
  3000 to 15360 bytes (15 KB).
- Reasoning: FreeRTOS allocates task stacks and control blocks from its
  own internal heap, not from the linker static RAM region. The default
  CubeMX value of 3000 bytes was exhausted by the first task creation,
  causing osThreadNew() to return NULL silently for any subsequent task.
  The scheduler then had no valid tasks to run and the board appeared
  completely dead with no LED output. Symptom was confirmed by the fact
  that one task ran correctly but adding a second task killed the system.
- The 15360 byte value is the standard CubeMX default for projects with
  multiple tasks. At 21KB total RAM usage (21% of 96KB), there is ample
  headroom for the remaining tasks and future modules.
- Note: task stack sizes are not reflected in the linker RAM report —
  they are allocated from the FreeRTOS heap at runtime. The RAM figure
  in the build output only covers static .bss/.data allocations. Always
  check configTOTAL_HEAP_SIZE when adding tasks to a FreeRTOS project.

  ## [Stage 4] Concurrent printf interleaving — deferred to Stage 5

- Known issue: Event and Keep-Alive tasks both call printf directly,
  causing interleaved UART output when both transmit simultaneously.
  printf is not thread-safe and has no mutex protecting the UART.
- Not fixed in Stage 4 — the printf calls are stubs that will be
  replaced in Stage 5 by osMessageQueuePut calls to a Comm task queue.
  The Comm task will be the sole owner of the UART, eliminating the
  interleaving problem by design rather than by adding a mutex.

  ## Stage 5 — Communication Module

### Decision: Option A polling for UART RX (not interrupt-driven ring buffer)
The Comm task polls for incoming bytes using HAL_UART_Receive with a 10ms
timeout between TX queue drains. This was chosen for simplicity at this
stage. An interrupt-driven ring buffer (Option B) is the recommended
upgrade path if message throughput increases or missed commands become
a problem in integration testing with the Central Computer.

### Decision: vTaskSuspendAll during DHT11 read
The DHT11 driver bit-bangs a timing-critical signal measured in
microseconds using the DWT cycle counter. FreeRTOS task preemption was
corrupting the timing mid-read, causing checksum and timeout errors on
every sample. The fix is to suspend the FreeRTOS scheduler for the
~20ms duration of the read, then resume it.

Known limitation: while the scheduler is suspended, the Comm task
cannot poll for incoming UART bytes. A command sent by the Central
Computer during this 20ms window will be lost. This occurs at most
once every 5 seconds (the Monitor sampling interval) and lasts 0.4%
of the time. The protocol handles dropped frames gracefully by
resetting to WAIT_SOF. The CC-side developer should be aware that
commands may occasionally require a retry.

Proper long-term fix: move DHT11 reading to a dedicated lowest-priority
task, or use a DMA-based UART RX approach that buffers incoming bytes
in hardware regardless of scheduler state.

### Decision: UART transport with thin wrapper for future swap
All HAL UART calls are isolated in two functions — comm_send_bytes()
and comm_recv_byte() — in comm.c. To switch from UART to Ethernet,
only these two functions need to change. No other code in the Comm
module touches the transport layer directly.

### Known issue: data parameter unused in handle_mode_transition
The MonitorData_t *data parameter in handle_mode_transition() in
events.c is not used. It was included anticipating that Comm would
need the full sensor reading to build EVENT frames, but the
implementation only needs the event type, detail, and timestamp.
The parameter and the (void)data suppression remain for now.
Removing it requires changing the call site in Event_Task as well —
deferred to a future tidy-up pass.

Log module — RTC decision:
  DS1307 external RTC with battery selected over tick-based day numbering.
  Rationale: battery-backed DS1307 survives power cycles, enabling real
  calendar date filenames (YYYY-MM-DD.log) as the spec intends.
  DS1307 driver ported from previous project (I2C, BCD conversion verified).
  The SET_TIME command from the Central Computer will also be wired to
  update the DS1307 via Ds1307_SetTime() so the clock stays accurate
  after the C++ integration in Phase 5.

  Log module — 7-day retention policy:
  Spec says "retain up to 7 days of data; delete oldest on 8th day."
  Implementation uses file-count retention (max 7 files per prefix)
  rather than calendar-day comparison.
  Rationale: FatFS FAT timestamps are unreliable on embedded systems
  without guaranteed RTC sync. Filenames are DATE_YYYYMMDD.csv so
  alphabetical order = chronological order. When file count exceeds 7,
  the lexicographically smallest filename (oldest date) is deleted.
  Behaviour difference: if board is offline for >7 days, old files
  are NOT automatically purged on reconnect — they persist until
  the 8-file threshold is reached naturally. This matches the spirit
  of the spec under normal continuous operation.
  Accepted tradeoff given embedded constraints.

  Log module — SD/DHT11 concurrency fix:
  Problem: vTaskSuspendAll() in Monitor_Task (used to protect DHT11
  bit-bang timing) was freezing the Log task mid-SPI-transaction,
  leaving the SD card in an undefined state and causing FR_DISK_ERR
  on subsequent f_open calls.
  Fix: replaced vTaskSuspendAll/xTaskResumeAll with a shared binary
  semaphore (s_dht_sem) that both Monitor_Task and Log_Task acquire
  before touching their respective hardware. Monitor holds the
  semaphore for the ~20ms DHT11 read window; Log waits on it before
  any SD SPI operation. This serialises the two hardware accesses
  without freezing the scheduler.
  Alternative considered: taskENTER_CRITICAL/taskEXIT_CRITICAL —
  rejected because it disables all interrupts including FreeRTOS
  tick, which breaks osDelay and queue operations inside the critical
  window.

  ## [Log Module] SD mount must happen in main.c before osKernelInitialize()

- Decision: f_mount(&USERFatFS, USERPath, 1U) is called in main.c inside
  the USER CODE BEGIN 2 block, after all peripheral inits and before
  osKernelInitialize(). A HAL_Delay(500) precedes the mount to allow SD
  card power-on settling.
- Reasoning: f_mount calls disk_initialize which runs USER_SPI_initialize.
  This function contains a 1-second ACMD41 polling loop that uses
  HAL_GetTick() for its timeout. After osKernelInitialize() is called,
  the FreeRTOS scheduler reconfigures SysTick, and HAL_GetTick() behavior
  becomes unreliable until osKernelStart() completes. Mounting inside
  MX_FREERTOS_Init() (after osKernelInitialize but before osKernelStart)
  caused HAL_GetTick() to not advance, making the ACMD41 loop spin forever.
  Mounting before osKernelInitialize() keeps HAL_GetTick() under pure TIM6
  control — deterministic, no scheduler interference.
- Alternatives considered:
  - Mount inside Log_Task after osDelay(10000): worked intermittently but
    failed whenever vTaskSuspendAll (DHT11 read) happened to coincide with
    an SD SPI transaction. Not reliable.
  - Mount inside MX_FREERTOS_Init with NOP busy-wait: NOP loop at 80MHz
    did not provide enough settling time and also blocked all output.
  - HAL_Delay inside MX_FREERTOS_Init: blocked after osKernelInitialize
    because TIM6 IRQ behavior changes at that point.
- Note: USERFatFS and USERPath are defined in FATFS/App/fatfs.c and
  declared extern in fatfs.h. Log_Task uses these globals directly rather
  than its own private FATFS object.

## [Log Module] FR_INVALID_NAME (err 6) caused by 8.3 filename length violation

- Decision: log filenames use 1-char prefix + 6-char date (YYMMDD) + .csv,
  e.g. D260908.csv and E260908.csv. Prefixes are "D" (data) and "E" (event).
- Reasoning: FatFs with _USE_LFN=0 enforces strict 8.3 filenames (max 8
  chars before the dot, max 3 after). The original design used "data" +
  "20260908" = 12 chars before the dot — a violation that FatFs rejects
  with FR_INVALID_NAME. Shortening to 1-char prefix + 6-char date = 7 chars
  fits within the 8-char limit.
- Note: the year is stored as 2-digit (26 for 2026) so filenames remain
  unambiguous within the project's operational lifetime.

## [Log Module] FR_INVALID_NAME misread as FR_INVALID_DRIVE during debugging

- Decision: none (diagnostic note).
- Reasoning: the FatFs FRESULT enum in this project's ff.h assigns
  FR_INVALID_NAME = 6, not FR_INVALID_DRIVE = 11. Early debugging assumed
  err 6 was a drive path problem and spent significant time adjusting
  path prefixes and drive registration. The actual cause was the filename
  length. Always cross-reference error codes against the project's own ff.h
  enum, not documentation for a different FatFs version.

## [Log Module] USERFatFS/USERPath must be used, not a private FATFS object

- Decision: log.c includes fatfs.h and uses USERFatFS and USERPath for all
  f_mount and f_open calls. The private static FATFS s_fatfs was removed.
- Reasoning: ff_gen_drv maintains an internal driver table (disk.drv[]).
  FATFS_LinkDriver registers the USER_Driver and writes the drive path into
  USERPath. FatFs's FatFs[] array (indexed by drive number) is populated
  only when f_mount is called with a FATFS object. If a private FATFS object
  is used for the mount but USERPath was set up for the globally declared
  USERFatFS, the internal state is consistent. However, using the canonical
  globals (USERFatFS, USERPath) as CubeMX intends avoids any ambiguity about
  which object is registered with which drive slot.

## [Log Module] _FS_REENTRANT must stay 1 with _SYNC_t and _FS_TIMEOUT defined

- Decision: ffconf.h keeps _FS_REENTRANT=1, _FS_TIMEOUT=1000,
  _SYNC_t=osSemaphoreId_t. The syscall.c file in
  Middlewares/Third_Party/FatFs/src/option/ provides ff_cre_syncobj,
  ff_del_syncobj, ff_req_grant, ff_rel_grant using CMSIS-RTOS2 semaphores.
- Reasoning: setting _FS_REENTRANT=0 removed the _SYNC_t type definition
  that ff.h needs when _FS_REENTRANT=1, causing build errors on revert.
  The sync functions exist in syscall.c and are compiled into the project.
  Keeping _FS_REENTRANT=1 is consistent with the FreeRTOS environment even
  though only one task accesses FatFs.
- Note: _USE_MUTEX is not a standard FatFs macro and was removed.

## [Log Module] SPI_TIMEOUT tuning — 100ms per-byte HAL timeout, 500ms ready wait

- Decision: SPI_TIMEOUT in user_diskio_spi.h is set to 100 (ms). All
  SD_WaitReady() calls that gate command readiness use 500U as their
  wall-clock budget directly.
- Reasoning: SPI_TIMEOUT controls the HAL_SPI_TransmitReceive per-byte
  timeout and also the SD_RxDataBlock token wait. At 500ms it caused
  apparent hangs because each byte poll could block for 500ms. At 50ms it
  caused FR_DISK_ERR because the card wasn't getting enough time to signal
  ready after a write. 100ms per-byte is a stable middle ground. The
  SD_WaitReady calls in SD_SendCmd that gate command readiness are the
  critical long waits — these explicitly use 500U to give the card
  sufficient time after write operations.

## [Log Module] SD card must be FAT32 formatted, not FAT16

- Decision: SD card formatted as FAT32 using mkfs.vfat -F 32.
- Reasoning: the card was originally FAT16. While FatFs supports FAT16,
  repeated failed writes and watchdog resets left the filesystem in a
  corrupt state. Reformatting as FAT32 resolved the corruption and
  established a clean baseline. FAT32 is also more appropriate for cards
  >= 32MB and is the standard format for SD cards used with embedded FatFs.

## [Log Module] Log module complete and verified on hardware

- Verified behaviours:
  - SD card mounts successfully on every boot (fr=0) with 500ms power-on
    delay before f_mount.
  - Data log (D260908.csv): sensor readings written every 5 seconds,
    comma-separated: timestamp_s, temp, humidity, battery, light, mode.
  - Event log (E260908.csv): mode transitions and object detection events
    written with timestamp, event_type, detail.
  - Both files confirmed on SD card after power cycle — data persists.
  - Watchdog running (10s timeout) — no watchdog-triggered resets during
    normal logging operation.
  - Writing continues in all modes (Normal, Warning, Error).
- Known limitations:
  - 7-file retention logic is implemented but not yet triggered (requires
    8 days of operation to observe deletion).
  - False IR object detection events occur without remote input — hardware
    sensitivity issue, not a software bug. To be investigated separately.
  - Log filenames are hardcoded to current date at write time via DS1307.
    If DS1307 loses power and is not re-synced via SET_TIME, filenames
    fall back to "700101" (epoch default).

    ## [Log Module] CSV column value meanings

Data file (D{YYMMDD}.csv):
- timestamp_s: seconds since board boot (HAL_GetTick() / 1000)
- temp_c: temperature in degrees Celsius (integer, from DHT11)
- humidity_pct: relative humidity percentage (integer, from DHT11)
- battery_raw: ADC raw value 0-4095 (potentiometer on PA0, 4095 = max voltage)
- light_raw: ADC raw value 0-4095 (LDR on PA1, 4095 = maximum light)
- mode: system mode — 0=Normal, 1=Warning, 2=Error

Event file (E{YYMMDD}.csv):
- timestamp_s: seconds since board boot when event occurred
- event_type: 1=mode change (from Monitor), 2=object detection (from IR/sonar)
- detail for event_type 1: new mode after transition — 0=Normal, 1=Warning, 2=Error
- detail for event_type 2: 0=object detected, 1=object cleared