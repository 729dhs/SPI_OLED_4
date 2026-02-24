/** 
 * @name         : my_st7789_2.c
 * @author       : 729DHS   guo_114@outlook.com
 * @date         : 2026-02-20 19:59:19
 * @brief        : ST7789显示屏驱动程序,参考Github开源,减少了一些不必要的代码,只保留核心初始化以及绘制函数
 * @version      : V1.1
 * V1.1 2026-02-24 09:23:06 补全Init
*/

// TODO -TEST 测试初始化函数

// NEXT 完成其他函数

#include "my_st7789_2.h"
#include "stm32f1xx_hal.h"


// GOOD -static 写驱动的时候,为保证最底层的函数调用是安全的,用static可以确保只在底层文件中调用,避免接口暴露


// 静态函数部分

/**
 * @brief   向ST7789显示屏写入命令
 * @param   cmd - 要发送的命令字节
 * @note    此函数会先将DC引脚置低（命令模式），然后通过SPI发送命令
 */
static void ST7789_WriteCmd(uint8_t cmd)
{
    ST7789_DC_Clr();                   // 清除DC引脚，设置为命令模式
    HAL_SPI_Transmit(&ST7789_SPI_PORT, &cmd, 1, 1000);  // 通过SPI发送命令
}

/**
 * @brief 向ST7789显示屏写入一个数据字节
 * @param data 要写入的数据字节
 * @note 此函数会先将DC引脚设置为数据模式，然后通过SPI接口发送数据
 */
static void ST7789_WriteData(uint8_t data)  
{
    ST7789_DC_Set();
    HAL_SPI_Transmit(&ST7789_SPI_PORT, &data, 1, 1000);
}


/**
 * @brief   向ST7789显示屏写入数据缓冲区
 * @param   data - 指向要发送的数据缓冲区的指针
 * @param   len - 数据缓冲区的长度
 * @note    此函数会先将DC引脚置高（数据模式），然后通过SPI发送指定长度的数据
 *          与单个数据写入函数不同，此函数可以一次性发送多个字节的数据
 */
static void st7789_write_data_buf(const uint8_t *data, size_t len) {
    ST7789_DC_Set();                   // 设置DC引脚，切换到数据模式
    HAL_SPI_Transmit(&hspi1, (uint8_t *)data, len, HAL_MAX_DELAY);  // 通过SPI发送数据缓冲区
}

/**
 * @brief ST7789显示屏初始化函数
 * 该函数用于初始化ST7789显示屏，通过控制复位和背光引脚来完成硬件初始化
 */
void ST7789_Init(void)
{
    ST7789_BLK_Set();    // 打开显示屏背光
    HAL_Delay(20);       // 等待20ms，确保背光稳定
    ST7789_RST_Clr();    // 复位引脚拉低，开始复位过程
    HAL_Delay(120);      // 等待120ms，保持复位状态
    ST7789_RST_Set();    // 复位引脚拉高，结束复位过程
    HAL_Delay(120);
    
}