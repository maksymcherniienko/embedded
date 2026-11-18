# Lab 4 — Algorithms, filtering and storage

From measuring code to cleaning up measurements: algorithm cost on a microcontroller,
two noise-reduction filters, and writing results to flash.

## Tasks

| # | File | Task | Concepts |
|---|---|---|---|
| 1 | `01_quicksort_vs_bst_benchmark.cpp` | Builds four arrays (1000 `int`, 100 `double`, 50 `char`, 500 `float`), sorts them, then rebuilds them as BSTs; reports time and heap for each | Quicksort partitioning, BST insert, `micros()`, `ESP.getFreeHeap()`, CPU frequency |
| 2 | `02_median_filter.cpp` | 100 temperature samples at 200 ms, then a 5-point median filter | 1-D median filter, edge handling, sorting inside a sliding window |
| 3 | `03_kalman_filter.cpp` | 10 readings, then a scalar Kalman filter with fixed `Q` and `R` | Predict/update steps, Kalman gain, error covariance |
| 4 | `04_spiffs_csv_logger.cpp` | Logs DS18B20 + BMP280 + LDR to `/data.csv` every 5 s; `read` dumps the file | SPIFFS, CSV formatting, Serial command parsing |

## Pinout

| Signal | GPIO | Notes |
|---|---|---|
| DS18B20 data | 4 | 1-Wire, 4.7 kΩ pull-up |
| LDR | 34 | ADC1_CH6 |
| BMP280 SDA / SCL | 21 / 22 | I²C, address `0x76` |

Task 1 uses no peripherals.

## Notes

- Benchmark output is a CSV-style summary (`Type,Size,Algorithm,Time(us),Memory(bytes)`) that can be pasted straight into a spreadsheet.
- The median filter copies the array edges unchanged, since a 5-point window does not fit there.
- CSV header: `Timestamp(ms),Temp_DS(C),Temp_BMP(C),Press(hPa),Light`.
