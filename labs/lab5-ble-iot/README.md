# Lab 5 — BLE environment station

The full pipeline: four sensors → CSV log in flash → BLE GATT server that notifies a phone
or desktop client every 5 seconds.

## Task

| File | Task | Concepts |
|---|---|---|
| `01_ble_environment_station.cpp` | Reads DS18B20, BMP280 (temperature, pressure, altitude) and a photoresistor, appends to `/data.csv`, then serves the latest row over BLE | BLE GATT server, four notify characteristics, SPIFFS, non-blocking scheduling |

## Pinout

| Signal | GPIO | Notes |
|---|---|---|
| DS18B20 data | 32 | 1-Wire, 4.7 kΩ pull-up |
| LDR | 34 | ADC1_CH6 |
| BMP280 SDA / SCL | 21 / 22 | I²C, address `0x76` |

## BLE interface

| Item | Value |
|---|---|
| Device name | `BLE_Station` |
| Service | `a1ea81f0-0e1b-d4a1-8840-63f88c8da1ea` |
| Temperature | `…a1eb` — °C, notify |
| Pressure | `…a1ec` — Pa, notify |
| Altitude | `…a1ed` — m, notify |
| Light | `…a1ee` — raw ADC, notify |

## Notes

- BLE characteristics are updated from the last row of the CSV file, so a client that connects late still gets a consistent snapshot.
- Sampling uses a `millis()` deadline rather than `delay()`, so the BLE stack keeps running.
- CSV header: `time_ms,temp_C,press_Pa,alt_m,lux`.
