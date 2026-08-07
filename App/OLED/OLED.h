#ifndef _OLED_H_
#define _OLED_H_


#include  "DataType.h"


/*
 * 初始化 OLED 显示。
 * 配置 GPIO（PB12/13/14/15, PC3），复位 SSD1306，发送初始化命令。
 * 由 Main.c 的 InitHardware() 调用。
 */
void  InitOLED(void);

/*
 * 开启 OLED 显示（电荷泵 + 开启显示）。
 */
void  OLEDDisplayOn(void);

/*
 * 关闭 OLED 显示（电荷泵 + 关闭显示）。
 */
void  OLEDDisplayOff(void);

/*
 * 将内部 GRAM(显存)缓冲区（128x8字节）刷新到 OLED 物理屏幕。
 * 必须在所有绘制操作之后调用才能使画面可见。
 * 在每个 Display* 函数末尾调用。
 */
void  OLEDRefreshGRAM(void);

/*
 * 清除 GRAM(显存)缓冲区并刷新屏幕（所有像素熄灭）。
 * 在每个 Display* 函数开头调用。
 */
void  OLEDClear(void);

/*
 * 在 (x, y) 位置绘制单个 ASCII 字符。
 *
 * 参数：
 *   x    - 列位置（0~127）
 *   y    - 行位置，像素为单位（0~63）
 *   chr  - 要显示的 ASCII 字符
 *   size - 字体高度：12(6x12) 或 16(8x16)
 *   mode - 1=正常显示，0=反色显示（背景像素点亮，前景像素熄灭）
 */
void  OLEDShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode);

/*
 * 在 (x, y) 位置显示无符号整数，固定位数。
 *
 * 参数：
 *   x, y - 位置
 *   num  - 要显示的数字（32位无符号）
 *   len  - 总显示位数（前导零替换为空格）
 *   size - 字体大小（12 或 16）
 *
 * 示例：num=42, len=4, size=16 -> 显示 "  42"（2个空格 + "42"）
 */
void  OLEDShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);

/*
 * 在 (x, y) 位置显示以 null 结尾的字符串。
 * 使用 8x16 字体。当 x 超过122或 y 超过58时自动换行。
 *
 * 参数：
 *   x, y - 起始位置
 *   p    - 指向以 null 结尾的字符串的指针
 */
void  OLEDShowString(u8 x, u8 y, const u8* p);

/*
 * 绘制点阵网格背景（示波器风格）。
 *
 * 参数：
 *   xStart, yStart - 网格左上角
 *   width, height  - 网格区域尺寸（像素）
 *   divX           - 水平方向分割数（垂直线数量）
 *   divY           - 垂直方向分割数（水平线数量）
 *
 * 线条为虚线（每隔一个像素画点），避免遮挡波形。
 */
void OLEDDrawGrid(u8 xStart, u8 yStart, u8 width, u8 height, u8 divX, u8 divY);

/*
 * 根据采样数据绘制波形，支持自动缩放。
 *
 * 参数：
 *   pData  - ADC 采样数组（u16，0~4095）
 *   count  - 采样点数（通常为128）
 *   xStart - 绘制区域左边界
 *   yStart - 绘制区域上边界
 *   width  - 绘制区域宽度（像素），波形会降采样以适应宽度
 *   yHeight- 绘制区域高度（像素）
 *   maxVal - 固定增益模式下的采样最大值（如4095）
 *   gain   - 0=自动缩放（寻找最小/最大值，填充区域并留25%边距），
 *            >0=固定增益倍数（value*gain，钳位到 maxVal）
 *
 * 相邻点用垂直线连接以实现连续显示。
 * 由 DisplaySignalMeasure() 调用。
 */
void OLEDDrawWaveFormEx(u16 *pData, u8 count, u8 xStart, u8 yStart, u8 width, u8 yHeight, u16 maxVal, u8 gain);

#endif
