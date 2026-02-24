# ST7789 STM32 SPI Driver (CubeMX)

[English](README.md) | [中文](README.zh-CN.md)

Minimal ST7789 driver and demo for STM32F103 using HAL SPI.

## Features
- ST7789 init sequence for 240x240 panels
- Basic fill and pixel drawing
- CubeMX-generated project layout

## Hardware
- MCU: STM32F103
- Display: ST7789 240x240 (SPI)

## Build
This is a CubeMX-generated project. Build it with your preferred toolchain
(e.g. STM32CubeIDE or CMake/Ninja if configured).

## Usage
1. Configure SPI and GPIO pins in CubeMX to match your wiring.
2. Build and flash the project.
3. The demo initializes the screen and fills a color.

## Acknowledgements
This project is based on Floyd-Fish's ST7789 STM32 driver:
https://github.com/Floyd-Fish/ST7789-STM32

## License
This repository is licensed under GPL-3.0 to comply with the upstream license.
See the LICENSE file for details.

