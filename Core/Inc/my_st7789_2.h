/** 
 * @name         : my_st7789_2.h
 * @author       : 729DHS   guo_114@outlook.com
 * @date         : 2026-02-20 19:59:31
 * @brief        : ST7789 240x240 屏幕驱动函数头文件,定义了需要的指令宏,以及简单的绘图函数
 * @version      : V1.0
 */

// TODO 添加对每个指令的说明,以及其所在具体位置

// PLAN 后续可以加入字符显示功能

#ifndef __ST7789_H__
#define __ST7789_H__

/*Include Files*/
#include "main.h"


#define ST7789_SPI_PORT hspi1

/*External Variables*/
extern SPI_HandleTypeDef ST7789_SPI_PORT;

// ***********************************
// 			在此修改引脚定义
// ***********************************

#define ST7789_BLK_PIN  GPIO_PIN_2
#define ST7789_DC_PIN   GPIO_PIN_3
#define ST7789_RST_PIN  GPIO_PIN_4

#define ST7789_BLK_PORT GPIOA
#define ST7789_DC_PORT  GPIOA
#define ST7789_RST_PORT GPIOA

#endif

#define USING_240X240

/* 选择要使用的显示旋转方向：(0-3) */ 
// #define ST7789_ROTATION 0
// #define ST7789_ROTATION 1
#define ST7789_ROTATION 2
// #define ST7789_ROTATION 3

// GOOD 使用ifdef来选择分辨率

#ifdef USING_240X240

#define ST7789_WIDTH 240
#define ST7789_HEIGHT 240

#if ST7789_ROTATION == 0
#define X_SHIFT 0
#define Y_SHIFT 80
#elif ST7789_ROTATION == 1
#define X_SHIFT 80
#define Y_SHIFT 0
#elif ST7789_ROTATION == 2
#define X_SHIFT 0
#define Y_SHIFT 0
#elif ST7789_ROTATION == 3
#define X_SHIFT 0
#define Y_SHIFT 0
#endif

#endif

/**
 * 画笔颜色
 * 如果你想使用其他颜色，可以选择RGB565格式的颜色。
 */
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define GRAY        0X8430
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458

#define LIGHTGREEN  0X841F
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12

/* Control Registers and constant codes */
#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36


/**
 * 内存数据访问控制寄存器 (0x36H)
 * 位映射:     D7  D6  D5  D4  D3  D2  D1  D0
 * 参数:      MY  MX  MV  ML  RGB MH  -   -
 *
 */

/* 页面地址顺序 ('0': 从上到下, '1': 相反) */
#define ST7789_MADCTL_MY  0x80
/* 列地址顺序 ('0': 从左到右, '1': 相反) */
#define ST7789_MADCTL_MX  0x40
/* 页面/列顺序 ('0' = 正常模式, '1' = 反转模式) */
#define ST7789_MADCTL_MV  0x20
/* 行地址顺序 ('0' = LCD 从上到下刷新, '1' = 相反) */
#define ST7789_MADCTL_ML  0x10
/* RGB/BGR 顺序 ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB 0x00

/* Advanced options */
#define ST7789_COLOR_MODE_16bit 0x55    //  RGB565 (16bit)
#define ST7789_COLOR_MODE_18bit 0x66    //  RGB666 (18bit)

/* Basic operations */
#define ST7789_BLK_Clr() HAL_GPIO_WritePin(ST7789_BLK_PORT, ST7789_BLK_PIN, GPIO_PIN_RESET)
#define ST7789_BLK_Set() HAL_GPIO_WritePin(ST7789_BLK_PORT, ST7789_BLK_PIN, GPIO_PIN_SET)

#define ST7789_DC_Clr() HAL_GPIO_WritePin(ST7789_DC_PORT, ST7789_DC_PIN, GPIO_PIN_RESET)
#define ST7789_DC_Set() HAL_GPIO_WritePin(ST7789_DC_PORT, ST7789_DC_PIN, GPIO_PIN_SET)

#define ST7789_RST_Clr() HAL_GPIO_WritePin(ST7789_RST_PORT, ST7789_RST_PIN, GPIO_PIN_RESET)
#define ST7789_RST_Set() HAL_GPIO_WritePin(ST7789_RST_PORT, ST7789_RST_PIN, GPIO_PIN_SET)


/* Basic functions. */
void ST7789_Init(void);
void ST7789_Fill_Color(uint16_t color);
void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7789_Fill(uint16_t xSta, uint16_t ySta, uint16_t xEnd, uint16_t yEnd, uint16_t color);
void ST7789_DrawPixel_4px(uint16_t x, uint16_t y, uint16_t color);

/* Graphical functions. */
void ST7789_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ST7789_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ST7789_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
void ST7789_InvertColors(uint8_t invert);


void ST7789_TearEffect(uint8_t tear);