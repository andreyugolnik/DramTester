# DramTester
Simple Arduino DRAM tester. It supports up to 9 address lines.
By default it supports 4164.

## Read / Write cycles

### Read Cycle
![Read Cycle](docs/read_cycle.png)

### Write Cycle
![Write Cycle](docs/write_cycle.png)

## Arduino Nano connections

### DRAM 4164 / 41256 pinout
                 -------------------
     (PB0)  5 --| A0  |       |     |
     (PB1)  7 --| A1  | 4164/ |     |
     (PB2)  6 --| A2  | 41256 |     |
     (PB3) 12 --| A3  |       |     |
     (PB4) 11 --| A4  |       |  DO |-- 14 (PD3)
     (PB5) 10 --| A5  |       |     |
     (PC0) 13 --| A6  |       |     |
     (PC1)  9 --| A7  |       |     |
     (PC2)  1 --| A8* |       |     |
                |-----|       |-----|
     (PD2)  2 --| DI  |       |     |
                |-----|       |     |
     (PD5)  4 --o RAS |       | +5V |-- 8 (PC3)
     (PD4) 15 --o CAS |       |     |
     (PD6)  3 --o WE  |       | GND |-- 16
                 -------------------

### LED
    (PC4)  LED Green
    (PC5)  LED Red

### Button
    (PC5) Start Button

## How to use arduino-cli

Arduino [command line interface](https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5).
