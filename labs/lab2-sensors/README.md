# Lab 2 — Sensors and peripherals

Six ways of getting data in and out of the ESP32: analog input, analog output, and the three
sensor buses on the board (I²C, SPI, 1-Wire).

## Tasks

| # | File | Task | Concepts |
|---|---|---|---|
| 1 | `01_ldr_to_dac_led.cpp` | Light level → inverted DAC voltage → LED brightness | 12-bit ADC, 8-bit DAC, `map()`, `constrain()` |
| 2 | `02_two_bmp280_i2c.cpp` | Two BMP280 barometers on one I²C bus | I²C addressing (`0x76`/`0x77`), Pa → mmHg conversion |
| 3 | `03_two_bmp280_spi.cpp` | The same two sensors over SPI | SPI with one CS line per device |
| 4 | `04_ds18b20_three_sensors.cpp` | Three DS18B20 sensors on one wire | 1-Wire enumeration, index-based reads |
| 5 | `05_environment_monitor.cpp` | Barometer + thermometer + light sensor in one loop | Combining three peripherals in a single telemetry cycle |

## Pinout

| Signal | GPIO | Notes |
|---|---|---|
| LDR | 4 | ADC1, 12-bit (0–4095) |
| LED (DAC) | 25 | DAC1 |
| BMP280 SDA / SCL | 21 / 22 | I²C, address `0x76` or `0x77` |
| BMP280 CS #1 / #2 | 5 / 17 | SPI variant only |
| DS18B20 data | 4 (task 4), 5 (task 5) | 4.7 kΩ pull-up to 3.3 V |

Some tasks reuse GPIO 4; they are separate sketches, so only one runs at a time.

## Notes

- Each sketch prints a readable report to Serial at **115200 baud**.
- Pressure is converted from Pa to mmHg for readability (`× 0.00750062`).
