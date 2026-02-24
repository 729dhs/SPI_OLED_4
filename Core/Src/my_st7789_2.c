/**
 * @name         : my_st7789_2.c
 * @author       : 729DHS   guo_114@outlook.com
 * @date         : 2026-02-20 19:59:19
 * @brief        :
 * ST7789显示屏驱动程序,参考Github开源,减少了一些不必要的代码,只保留核心初始化以及绘制函数
 * @version      : V1.2
 * V1.1 2026-02-24 09:23:06 补全Init
 * V1.2 2026-02-24 18:11:55 修复了一些代码
 */


// DONE -TEST 测试初始化函数

// NEXT 完成其他函数

#include "my_st7789_2.h"
#include "stm32f1xx_hal.h"

// GOOD -arch static
// 写驱动的时候,为保证最底层的函数调用是安全的,用static可以确保只在底层文件中调用,避免接口暴露

// 静态函数部分

/**
 * @brief   向ST7789显示屏写入命令
 * @param   cmd - 要发送的命令字节
 * @note    此函数会先将DC引脚置低（命令模式），然后通过SPI发送命令
 */
static void ST7789_WriteCmd(uint8_t cmd) {
  ST7789_DC_Clr(); // 清除DC引脚，设置为命令模式
  HAL_SPI_Transmit(&ST7789_SPI_PORT, &cmd, 1, 1000); // 通过SPI发送命令
}


/**
 * @brief 向ST7789显示屏写入一个数据字节
 * @param data 要写入的数据字节
 * @note 此函数会先将DC引脚设置为数据模式，然后通过SPI接口发送数据
 */
static void ST7789_WriteData(uint8_t data) {
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
  ST7789_DC_Set(); // 设置DC引脚，切换到数据模式
  HAL_SPI_Transmit(&hspi1, (uint8_t *)data, len,HAL_MAX_DELAY); // 通过SPI发送数据缓冲区
}


/**
 * @brief 设置 ST7789 显示屏的地址窗口
 * @param x0 窗口起始 X 坐标
 * @param y0 窗口起始 Y 坐标
 * @param x1 窗口结束 X 坐标
 * @param y1 窗口结束 Y 坐标
 * @note 该函数用于定义后续写入操作的像素区域，设置完成后可通过 ST7789_WriteData
 * 写入像素数据
 */
static void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1) {
  // 计算实际显示坐标（加上偏移量）
  uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
  uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

  /* 设置列地址范围 */
  ST7789_WriteCmd(ST7789_CASET); // 发送列地址设置命令
  {
    // 准备列地址数据：起始地址高8位、起始地址低8位、结束地址高8位、结束地址低8位
    uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
    st7789_write_data_buf(data, sizeof(data)); // 写入地址数据
  }

  /* 设置行地址范围 */
  ST7789_WriteCmd(ST7789_RASET); // 发送行地址设置命令
  {
    // 准备行地址数据：起始地址高8位、起始地址低8位、结束地址高8位、结束地址低8位
    uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
    st7789_write_data_buf(data, sizeof(data)); // 写入地址数据
  }

  /* 进入RAM写入模式 */
  ST7789_WriteCmd(ST7789_RAMWR); // 发送RAM写入命令，后续数据将直接写入显示RAM
}


/**
 * @brief 设置ST7789显示屏的显示旋转方向
 * @param m 旋转模式，取值范围0-3
 *          0: 正常显示方向（0度）
 *          1: 顺时针旋转90度
 *          2: 顺时针旋转180度
 *          3: 顺时针旋转270度
 * @note 通过配置MADCTL寄存器来控制显示方向，
 *       不同的旋转模式对应不同的内存访问控制参数组合
 */
static void ST7789_SetRotation(uint8_t m) {
  ST7789_WriteCmd(ST7789_MADCTL);
  switch (m) {
  case 0:
    // 设置正常显示方向：水平翻转+垂直翻转+RGB模式
    ST7789_WriteData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
    break;
  case 1:
    // 设置90度旋转：垂直翻转+行列交换+RGB模式
    ST7789_WriteData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
    break;
  case 2:
    // 设置180度旋转：仅RGB模式（无翻转）
    ST7789_WriteData(ST7789_MADCTL_RGB);
    break;
  case 3:
    // 设置270度旋转：水平翻转+行列交换+RGB模式
    ST7789_WriteData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
    break;
  default:
    break;
  }
}


/**
 * @brief ST7789显示屏初始化函数
 * 该函数用于初始化ST7789显示屏，通过控制复位和背光引脚来完成硬件初始化
 */
void ST7789_Init(void) {
  ST7789_BLK_Set(); // 打开显示屏背光
  HAL_Delay(20);    // 等待20ms，确保背光稳定
  ST7789_RST_Clr(); // 复位引脚拉低，开始复位过程
  HAL_Delay(120);   // 等待120ms，保持复位状态
  ST7789_RST_Set(); // 复位引脚拉高，结束复位过程
  HAL_Delay(120);

  ST7789_WriteCmd(ST7789_COLMOD);
  ST7789_WriteData(ST7789_COLOR_MODE_16bit);
  ST7789_WriteCmd(0xB2);
  {
    // GOOD 大括号在这里用于限制作用域,确保data生命周期,尽可能减少内存占用
    uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
    st7789_write_data_buf(data, sizeof(data));
  }
  ST7789_SetRotation(ST7789_ROTATION);

  ST7789_WriteCmd(0xB7);
  ST7789_WriteData(0x35);
  ST7789_WriteCmd(0xBB);
  ST7789_WriteData(0x19);
  ST7789_WriteCmd(0xC0);
  ST7789_WriteData(0x2C);
  ST7789_WriteCmd(0xC2);
  ST7789_WriteData(0x01);
  ST7789_WriteCmd(0xC3);
  ST7789_WriteData(0x12);
  ST7789_WriteCmd(0xC4);
  ST7789_WriteData(0x20);
  ST7789_WriteCmd(0xC6);
  ST7789_WriteData(0x0F);
  ST7789_WriteCmd(0xD0);
  ST7789_WriteData(0xA4);
  ST7789_WriteData(0xA1);

  ST7789_WriteCmd(0xE0);
  {
    uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F,
                      0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
    st7789_write_data_buf(data, sizeof(data));
  }

  ST7789_WriteCmd(0xE1);
  {
    uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F,
                      0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
    st7789_write_data_buf(data, sizeof(data));
  }

  ST7789_WriteCmd(ST7789_INVON);
  ST7789_WriteCmd(ST7789_SLPOUT);
  ST7789_WriteCmd(ST7789_NORON);
  ST7789_WriteCmd(ST7789_DISPON);

  HAL_Delay(50);
  ST7789_Fill_Color(WHITE);
}


/**
 * @brief 使用指定颜色填充ST7789显示屏全屏
 * @details 该函数通过设置全屏窗口地址，然后将指定颜色数据批量写入整个显示区域，
 *          实现全屏颜色填充功能。颜色数据会自动转换为适合显示的字节序列。
 * @param color 要填充的颜色值，16位RGB565格式
 * @note 函数内部会自动处理\t、\r、\n等特殊字符
 * @see ST7789_SetAddressWindow()
 *      st7789_write_data_buf()
 */
void ST7789_Fill_Color(uint16_t color) {
  // 设置全屏窗口
  ST7789_SetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);

  // 预转换颜色为字节序列
  const uint8_t color_bytes[2] = {
      (uint8_t)(color >> 8),  // 高字节
      (uint8_t)(color & 0xFF) // 低字节
  };

  // 计算总像素数
  const uint32_t total_pixels = (uint32_t)ST7789_WIDTH * ST7789_HEIGHT;

  // 批量发送颜色数据
  for (uint32_t i = 0; i < total_pixels; i++) {
    st7789_write_data_buf(color_bytes, sizeof(color_bytes));
  }
}
