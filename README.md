# ESP32 Embedded Labs

Self-contained ESP32 firmware sketches written for an embedded systems course — from a single
photoresistor to a BLE sensor station that logs measurements to flash memory.

Along the way: ADC/DAC, I²C, SPI, 1-Wire, GPIO interrupts, hardware timers, PWM,
digital filtering, a filesystem, and a BLE GATT server.

**Stack:** C++ · Arduino core 3.x · ESP32 (ESP32-WROOM-32 DevKit) · Arduino IDE 2.x

## Skills demonstrated

| Area | What the code shows |
|---|---|
| **Peripherals** | 12-bit ADC input, DAC output, LEDC/PWM dimming, digital GPIO |
| **Buses** | I²C with two devices on one bus, SPI with per-device chip select, multi-drop 1-Wire |
| **Concurrency** | `IRAM_ATTR` ISRs, debouncing inside an ISR, `volatile` flags, critical sections, hardware timer alarms, non-blocking `millis()` scheduling |
| **Algorithms** | Quicksort, binary search tree construction, 1-D median filter, Kalman filter |
| **Systems** | Heap accounting via `ESP.getFreeHeap()`, CPU frequency scaling, SPIFFS CSV logging and playback |
| **Connectivity** | BLE GATT server exposing four notify characteristics |

## Labs at a glance

### Lab 2 — Sensors and peripherals

| Sketch | What it does | Interface |
|---|---|---|
| `01_ldr_to_dac_led.cpp` | Inverts the photoresistor reading and writes it to the DAC to dim an LED | ADC → DAC |
| `02_two_bmp280_i2c.cpp` | Reads two BMP280 barometers on one I²C bus (`0x76` / `0x77`), pressure in mmHg | I²C |
| `03_two_bmp280_spi.cpp` | Same two sensors over SPI, each on its own chip-select line | SPI |
| `04_ds18b20_three_sensors.cpp` | Enumerates and reads three DS18B20 sensors on a single 1-Wire bus | 1-Wire |
| `05_environment_monitor.cpp` | Combines a BMP280, a DS18B20 and a photoresistor into one telemetry loop | I²C + 1-Wire + ADC |

### Lab 3 — Interrupts, timers, PWM

| Sketch | What it does | Concept |
|---|---|---|
| `01_button_interrupts_pwm.cpp` | Button ISRs drive an LED blink and cycle PWM brightness with a press counter | External interrupts, LEDC |
| `02_dual_hardware_timers.cpp` | Two hardware timer alarms sample the ADC at 2 s and 5 s and hand off to the main loop | Timer ISR + critical section |
| `03_timer_bmp280_button_duration.cpp` | A 3 s timer polls the barometer; the LED mirrors the measured button hold time | Timer vs. GPIO interrupt |

### Lab 4 — Algorithms, filtering, storage

| Sketch | What it does | Concept |
|---|---|---|
| `01_quicksort_vs_bst_benchmark.cpp` | Times quicksort and BST construction for `int`/`float`/`double`/`char` and reports heap used | Complexity vs. memory trade-off |
| `02_median_filter.cpp` | Collects 100 temperature samples and applies a 5-point median filter | 1-D median filtering |
| `03_kalman_filter.cpp` | Runs a scalar Kalman filter over 10 DS18B20 readings and prints the residual | State estimation |
| `04_spiffs_csv_logger.cpp` | Logs four sensors to `/data.csv` every 5 s; `read` on Serial dumps the file | SPIFFS, CSV |

### Lab 5 — BLE IoT station

| Sketch | What it does | Concept |
|---|---|---|
| `01_ble_environment_station.cpp` | Publishes temperature, pressure, altitude and light over BLE notifications, persisted in `/data.csv` | BLE GATT server |

## Repository layout

```
labs/
├── lab2-sensors/               # reading sensors over every bus the board has
├── lab3-interrupts-timers/     # reacting to events instead of polling
├── lab4-algorithms-filters/    # algorithms, noise reduction, persistent storage
└── lab5-ble-iot/               # exposing the station over Bluetooth LE
```

Each lab folder has its own `README.md` with the task list, pinout and notes.

## Hardware

- ESP32 DevKit (ESP32-WROOM-32)
- BMP280 barometer — I²C (`0x76`/`0x77`) or SPI
- DS18B20 temperature sensor — 1-Wire, 4.7 kΩ pull-up to 3.3 V
- Photoresistor (LDR) in a voltage divider
- LEDs, push buttons, resistors, breadboard

## Libraries

Install through the Arduino Library Manager:

- **Adafruit BMP280** + **Adafruit Unified Sensor**
- **OneWire**
- **DallasTemperature**

`SPIFFS`, `Wire`, `SPI` and BLE ship with the ESP32 Arduino core.

## Build and flash

1. Install **Arduino IDE 2.x** and the ESP32 board package (*Boards Manager → esp32 by Espressif*, v3.x — the sketches use the 3.x APIs `ledcAttach()` and `timerBegin(freq)`).
2. Install the libraries listed above.
3. Each `.cpp` file is a complete sketch (`setup()` + `loop()`). To open one in the IDE, put it in a folder of the same name and rename the extension:

   ```
   labs/lab2-sensors/01_ldr_to_dac_led.cpp
   → 01_ldr_to_dac_led/01_ldr_to_dac_led.ino
   ```

4. Select **ESP32 Dev Module**, upload, and open Serial Monitor at **115200 baud**.

## Notes and limitations

- No automated tests or CI: every sketch needs real hardware and specific sensors attached.
- Lab 4 benchmark numbers are printed to Serial and depend on CPU frequency (pinned to 240 MHz) and heap fragmentation.
- Kalman filter constants (`Q = 0.022`, `R = 0.617`) were tuned empirically for the DS18B20.

## Usage

Coursework, shared for reference. Please don't submit it as your own.
