# HX711 C++ Interface for Jetson Nano

This project provides a high-performance C++ implementation for reading weight data from an HX711 load cell amplifier using `libgpiod` on an NVIDIA Jetson Nano (or any Linux system with GPIO support).

It's designed for accurate weight measurements with tare and calibration capabilities.

---

## Features

- Fast GPIO bit-banging using `libgpiod` (no slow `/sys/class/gpio`)
- Supports **Channel A** with **Gain 128**
- Tare (zero the scale) at startup
- Calibration using known weights
- Persistent calibration stored in `calibration.txt`
- Continuous weight output in grams

---

## Requirements

- NVIDIA Jetson Nano (or other Linux SBC)
- HX711 load cell amplifier + load cell
- `libgpiod` (`sudo apt install libgpiod-dev`)
- C++17-compatible compiler (`g++`, `cmake`)

---

## Build Instructions

1. Clone the repo:

```bash
git clone https://github.com/andrew-dave/hx711_cpp.git
cd hx711_cpp
```
2. Install dependencies
Make sure you have libgpiod, cmake, and g++ installed:
```bash
sudo apt update
sudo apt install libgpiod-dev cmake g++
```

3. Build the project:

```bash
mkdir build && cd build
cmake ..
make
```

4. Run the program:
```bash
sudo ./hx711
```


