#include "OLED.h"
#include "stm32f10x_conf.h"
#include "OLEDFont.h"
#include "SysTick.h"


#define OLED_CMD    0
#define OLED_DATA   1


#define CLR_OLED_CS()   GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SET_OLED_CS()   GPIO_SetBits  (GPIOB, GPIO_Pin_12)

#define CLR_OLED_RES()  GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define SET_OLED_RES()  GPIO_SetBits  (GPIOB, GPIO_Pin_14)

#define CLR_OLED_DC()   GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define SET_OLED_DC()   GPIO_SetBits  (GPIOC, GPIO_Pin_3)

#define CLR_OLED_SCK() GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define SET_OLED_SCK() GPIO_SetBits  (GPIOB, GPIO_Pin_13)

#define CLR_OLED_DIN() GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define SET_OLED_DIN() GPIO_SetBits  (GPIOB, GPIO_Pin_15)


/* GRAM(显存)缓冲区：128列 x 8页（每页 = 8个像素高） */
static  u8  s_arrOLEDGRAM[128][8];


static  void  ConfigOLEDGPIO(void);
static  void  ConfigOLEDReg(void);

static  void  OLEDWriteByte(u8 dat, u8 cmd);
static  void  OLEDDrawPoint(u8 x, u8 y, u8 t);

static  u32   CalcPow(u8 m, u8 n);


/*
 * 底层：OLED 引脚的 GPIO 配置。
 *
 * 引脚：PB12=CS, PB13=SCK, PB14=RES, PB15=DIN, PC3=DC
 * 全部配置为推挽输出，50MHz。
 */
static  void  ConfigOLEDGPIO(void)
{
  /* 待实现：将 PB12、PB13、PB14、PB15 和 PC3 配置为推挽输出。
   *       使能 GPIOB 和 GPIOC 的时钟。
   *       初始将所有引脚置高电平。
   *       提示：使用 RCC_APB2PeriphClockCmd + GPIO_Init。 */
}


/*
 * 底层：发送 SSD1306 初始化命令序列。
 *
 * 关键命令：
 *   0xAE = 关闭显示，0xAF = 开启显示
 *   0xD5 0x50 = 设置显示时钟分频
 *   0xA8 0x3F = 设置多路复用比（64行）
 *   0x20 0x02 = 设置页寻址模式
 *   0xA1 = 段重映射，0xC0 = 正常 COM 扫描
 *   0x8D 0x14 = 使能电荷泵
 *   0xAF = 开启显示
 */
static  void  ConfigOLEDReg( void )
{
  /* 待实现：通过 OLEDWriteByte() 发送 SSD1306 初始化命令序列。
   *       命令的第二个参数使用 OLED_CMD。
   *       最后以 0xAF（开启显示）结束。 */
}


/*
 * 底层：通过软件 SPI 向 OLED 发送一个字节。
 *
 * 参数：
 *   dat - 要发送的字节
 *   cmd - OLED_CMD(0) 表示命令，OLED_DATA(1) 表示数据
 *
 * 步骤：
 *   1. 设置 DC 引脚(PC3)：命令时拉低，数据时拉高
 *   2. 拉低 CS（开始传输）
 *   3. 对8个比特逐位处理（MSB先发）：
 *      - 拉低 SCK
 *      - 根据比特值设置 DIN
 *      - 拉高 SCK（时钟边沿）
 *   4. 拉高 CS（结束传输），DC 拉高（默认状态）
 */
static  void  OLEDWriteByte(u8 dat, u8 cmd)
{
  /* 待实现：实现软件 SPI 按位发送。
   *       MSB优先，8位，使用 CLR/SET 宏控制引脚。 */
}


/*
 * 底层：在 GRAM(显存)中设置或清除单个像素点。
 *
 * 参数：
 *   x - 列号（0~127）
 *   y - 行号（0~63）
 *   t - 1=点亮像素，0=熄灭像素
 *
 * GRAM布局：s_arrOLEDGRAM[x][page]，其中 page = 7 - y/8
 *           字节内的比特位置 = 1 << (7 - y%8)
 *
 * 用 OR 置位，用 AND ~ 清除。
 */
static  void  OLEDDrawPoint(u8 x, u8 y, u8 t)
{
  /* 待实现：在 GRAM(显存)缓冲区中实现画点操作。
   *       边界检查：x<=127, y<=63。
   *       计算 page = 7 - y/8, bit = 1<<(7 - y%8)。
   *       若 t：GRAM[x][page] |= bit，否则 GRAM[x][page] &= ~bit。 */
}


/* 数学辅助函数：整数的幂运算 m^n */
static  u32 CalcPow(u8 m, u8 n)
{
  /* 待实现：返回 m 的 n 次幂（整数）。 */
  return 1;
}


/* ============================================================
 *  公共 API - 实现以下函数
 * ============================================================ */


/*
 * 初始化 OLED：配置 GPIO、复位、发送初始化命令、清屏。
 * 由 Main.c 的 InitHardware() 调用。
 */
void  InitOLED(void)
{
  /* 待实现：
   *   1. 调用 ConfigOLEDGPIO()
   *   2. 复位序列：CLR_OLED_RES -> DelayNms(10) -> SET_OLED_RES -> DelayNms(10)
   *   3. 调用 ConfigOLEDReg()
   *   4. 调用 OLEDClear() */
}


void  OLEDDisplayOn( void )
{
  /* 待实现：发送 0x8D 0x14（开启电荷泵），然后发送 0xAF（开启显示）。 */
}


void  OLEDDisplayOff( void )
{
  /* 待实现：发送 0x8D 0x10（关闭电荷泵），然后发送 0xAE（关闭显示）。 */
}


/*
 * 将整个 GRAM(显存)缓冲区刷新到 OLED 屏幕。
 *
 * OLED 组织为8页，每页128列。
 * 对每一页 i（0~7）：
 *   1. 发送页地址命令：0xB0 + i
 *   2. 发送列地址：低4位(0x00) + 高4位(0x10)
 *   3. 发送 s_arrOLEDGRAM[0..127][i] 的128字节数据
 *
 * 在每个 Display 函数末尾调用。
 */
void  OLEDRefreshGRAM(void)
{
  /* 待实现：遍历8页，设置页/列地址，每页发送128字节。 */
}


/*
 * 清除 GRAM(显存)缓冲区（全部置0）并刷新到屏幕。
 */
void  OLEDClear(void)
{
  /* 待实现：将 s_arrOLEDGRAM[128][8] 全部清零，然后调用 OLEDRefreshGRAM()。 */
}


/*
 * 在 GRAM(显存)的 (x, y) 位置显示单个 ASCII 字符。
 *
 * 参数：
 *   x, y  - 左上角位置（x=列号，y=像素行号）
 *   chr   - ASCII 字符（空格 ' ' = 0x20 是字模表中的第一个字符）
 *   size  - 字体高度像素数（12 或 16）
 *   mode  - 1=正常显示，0=反色显示
 *
 * 字模数据：g_iASCII1206[chr-' '][12] 或 g_iASCII1608[chr-' '][16]
 * 字模中每个字节代表8个垂直像素，MSB优先。
 * y方向每满8个像素，x前进一列，y回到起始行 y0。
 */
void  OLEDShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode)
{
  /* 待实现：
   *   1. 将 chr 转为索引：chr -= ' '
   *   2. 遍历字模数据中的每个字节（共 size 个字节）：
   *      - 根据 size 选择字模表(g_iASCII1206 或 g_iASCII1608)
   *      - 对8个比特逐位处理（MSB优先）：
   *        若比特为1 -> OLEDDrawPoint(x, y, mode)
   *        若比特为0 -> OLEDDrawPoint(x, y, !mode)
   *        y++，若 (y - y0) == size：y=y0, x++（换到下一列） */
}


/*
 * 在 (x, y) 位置显示无符号数字。
 *
 * 参数：
 *   x, y - 位置
 *   num  - 要显示的数字（32位无符号）
 *   len  - 显示的数字位数（不足位用空格填充）
 *   size - 字体大小（12 或 16）
 *
 * 前导零被抑制（替换为空格），最后一位除外。
 * 使用 CalcPow(10, n) 提取各位数字。
 *
 * 示例：num=42, len=4 -> 显示 "  42"
 */
void  OLEDShowNum(u8 x, u8 y, u32 num, u8 len, u8 size)
{
  /* 待实现：
   *   对每个数字位 t（0 到 len-1）：
   *     temp = (num / 10^(len-t-1)) % 10
   *     若为前导零且不是最后一位：显示空格
   *     否则：通过 OLEDShowChar 显示数字
   *   字符宽度 = size/2 像素。 */
}


/*
 * 在 (x, y) 位置显示以 null 结尾的字符串。
 *
 * 使用16像素字体（size=16，每字符宽8像素）。
 * 自动换行：若 x > 122，移到下一行（y += 16）。
 * 若 y > 58，回到 (0, 0) 并清屏。
 */
void  OLEDShowString(u8 x, u8 y, const u8* p)
{
  /* 待实现：遍历字符串，对每个字符调用 OLEDShowChar(x, y, *p, 16, 1)。
   *       每个字符 x += 8。按上述规则处理换行。 */
}


/*
 * 绘制点阵网格背景（示波器风格）。
 *
 * 参数：
 *   xStart, yStart - 网格区域左上角
 *   width, height  - 网格区域尺寸
 *   divX, divY     - 分割数（格子数）
 *
 * 垂直虚线每隔 stepX = width/divX 出现，每隔一个像素画点。
 * 水平虚线每隔 stepY = height/divY 出现，每隔一个像素画点。
 */
void OLEDDrawGrid(u8 xStart, u8 yStart, u8 width, u8 height, u8 divX, u8 divY) {
    /* 待实现：
     *   stepX = width / divX, stepY = height / divY（防止除以0）
     *   垂直线：x 从 xStart 开始，以 stepX 为步长：
     *     每隔一个 y 画点（y += 2）
     *   水平线：y 从 yStart 开始，以 stepY 为步长：
     *     每隔一个 x 画点（x += 2） */
}


/*
 * 根据 ADC 采样数据绘制波形。
 *
 * 参数：
 *   pData  - 采样数组指针（u16，通常128个点）
 *   count  - pData 中的采样点数
 *   xStart, yStart - 绘制区域左上角
 *   width, yHeight - 绘制区域尺寸
 *   maxVal - 采样值最大可能值（如12位ADC为4095）
 *   gain   - 0 = 自动缩放，>0 = 固定增益倍数
 *
 * 自动缩放模式（gain==0）：
 *   1. 找到 pData 中的最小值和最大值
 *   2. range = max - min（防止为0）
 *   3. 增加25%边距：padRange = range + range/4 + 4
 *   4. dispMin = min - (padRange-range)/2（若下溢则钳位到0）
 *   5. 映射每个采样点：y = yStart + (yHeight-1) - (sample-dispMin)*yHeight/padRange
 *
 * 固定增益模式（gain>0）：
 *   value = pData[idx] * gain，钳位到 maxVal
 *   y = yStart + (yHeight-1) - value*yHeight/maxVal
 *
 * 降采样：若 count > width，将 x 映射到索引：idx = x * count / width
 *
 * 连线：跟踪前一个点的 prevY，在每个 x 处画从 prevY 到 y 的垂直线。
 *   这样正弦波看起来是连续的，而不是散点。
 */
void OLEDDrawWaveFormEx(u16 *pData, u8 count, u8 xStart, u8 yStart, u8 width, u8 yHeight, u16 maxVal, u8 gain) {
    /* 待实现：按上述说明实现。
     *       同时处理自动缩放和固定增益两种模式。
     *       用垂直线连接相邻点以获得连续波形。 */
}
