# ST7789 STM32 SPI Driver (CubeMX)

[中文](README.zh-CN.md) | [English](README.md)

基于 STM32F103 + HAL SPI 的 ST7789 最小驱动与演示工程。

## 功能
- 240x240 面板初始化序列
- 基本填充与像素绘制
- CubeMX 生成的工程结构

## 硬件
- MCU: STM32F103
- 屏幕: ST7789 240x240 (SPI)

## 构建
这是一个 CubeMX 生成的工程，可使用你常用的工具链构建
(例如 STM32CubeIDE，或已配置的 CMake/Ninja)。

## 使用
1. 在 CubeMX 中配置 SPI 与 GPIO 引脚，确保与接线一致。
2. 编译并烧录工程。
3. 示例会初始化屏幕并填充颜色。

## 致谢
本项目参考了 Floyd-Fish 的 ST7789 STM32 驱动：
https://github.com/Floyd-Fish/ST7789-STM32

## 许可证
为满足上游协议要求，本仓库使用 GPL-3.0。
详情请见 LICENSE 文件。
