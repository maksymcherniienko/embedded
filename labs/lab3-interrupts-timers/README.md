# Lab 3 — Interrupts, hardware timers and PWM

The same "read a sensor and react" problem, solved three ways: polling a button, letting a
GPIO interrupt wake the logic, and letting a hardware timer do it on a fixed schedule.

## Tasks

| # | File | Task | Concepts |
|---|---|---|---|
| 1 | `01_button_interrupts_pwm.cpp` | Button 1 blinks an LED and reports the light level; button 2 counts presses and cycles PWM brightness | `attachInterrupt()`, `IRAM_ATTR`, debounce in ISR, LEDC |
| 2 | `02_dual_hardware_timers.cpp` | Two timer alarms read the ADC every 2 s and 5 s | `hw_timer_t`, `portMUX` critical sections, `volatile` flags |
| 3 | `03_timer_bmp280_button_duration.cpp` | A 3 s timer polls the barometer; the LED stays on for exactly as long as the button was held | Timer ISR vs. `CHANGE` GPIO ISR |

## Pinout

| Signal | GPIO | Notes |
|---|---|---|
| Button 1 | 18 | `INPUT_PULLUP`, RISING |
| Button 2 | 19 | `INPUT_PULLUP`, FALLING |
| LDR | 34 | ADC1_CH6, input-only pin |
| LED 1 (task 1) | 21 | digital output |
| LED 2 (task 1) | 22 | LEDC, 5 kHz, 8-bit |
| Button (task 3) | 18 | `INPUT_PULLUP`, `CHANGE` |
| LED — timer / button (task 3) | 16 / 17 | digital outputs |
| BMP280 SDA / SCL | 21 / 22 | I²C, address `0x76` |

> GPIO 21/22 are the default I²C pins, so task 3 moves its LEDs to 16/17 to free them. Run one sketch at a time.

## Notes

- The ISRs only set flags; all Serial output happens in `loop()` — no blocking work inside an ISR.
- Debounce is time-based (200 ms window) and lives inside the ISR itself.
- Task 3 measures the hold time by timestamping press and release with `millis()`.
