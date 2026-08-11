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
    GPIO_InitTypeDef  GPIO_InitStructure;
    
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOC的时钟
  
  //配置PB13（OLED_SCK）
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;          //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //设置模式 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置I/O输出速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);                //根据参数初始化GPIO
  GPIO_SetBits(GPIOB, GPIO_Pin_13);                     //设置初始状态为高电平

  //配置PB15（OLED_DIN）
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;          //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //设置模式  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置I/O输出速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);                //根据参数初始化GPIO
  GPIO_SetBits(GPIOB, GPIO_Pin_15);                     //设置初始状态为高电平

  //配置PB14（OLED_RES）
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;          //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //设置模式  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置I/O输出速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);                //根据参数初始化GPIO
  GPIO_SetBits(GPIOB, GPIO_Pin_14);                     //设置初始状态为高电平

  //配置PB12（OLED_CS）
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;          //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //设置模式  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置I/O输出速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);                //根据参数初始化GPIO
  GPIO_SetBits(GPIOB, GPIO_Pin_12);                     //设置初始状态为高电平

  //配置PC3（OLED_DC）
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;           //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //设置模式  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置I/O输出速度
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化GPIO
  GPIO_SetBits(GPIOC, GPIO_Pin_3);                      //设置初始状态为高电平
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
  OLEDWriteByte(0xAE, OLED_CMD);
  OLEDWriteByte(0xD5, OLED_CMD);
  OLEDWriteByte(0x50, OLED_CMD);
  OLEDWriteByte(0xA8, OLED_CMD);
  OLEDWriteByte(0x3F, OLED_CMD);
  OLEDWriteByte(0xD3, OLED_CMD);
  OLEDWriteByte(0x00, OLED_CMD);
  OLEDWriteByte(0x40, OLED_CMD);
  OLEDWriteByte(0xA1, OLED_CMD);
  OLEDWriteByte(0xC0, OLED_CMD);
  OLEDWriteByte(0xDA, OLED_CMD);
  OLEDWriteByte(0x12, OLED_CMD);
  OLEDWriteByte(0x81, OLED_CMD);
  OLEDWriteByte(0xCF, OLED_CMD);
  OLEDWriteByte(0xD9, OLED_CMD);
  OLEDWriteByte(0xF1, OLED_CMD);
  OLEDWriteByte(0xDB, OLED_CMD);
  OLEDWriteByte(0x40, OLED_CMD);
  OLEDWriteByte(0x8D, OLED_CMD);
  OLEDWriteByte(0x14, OLED_CMD);
  OLEDWriteByte(0x20, OLED_CMD);
  OLEDWriteByte(0x02, OLED_CMD);
  OLEDWriteByte(0xAF, OLED_CMD);
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
  i16 i;
                                
  //判断要写入数据还是写入命令    
  if(OLED_CMD == cmd)         //如果标志cmd为传入命令时
  {                             
    CLR_OLED_DC();            //DC输出低电平用来读写命令  
  }                             
  else if(OLED_DATA == cmd)   //如果标志cmd为传入数据时
  {                             
    SET_OLED_DC();            //DC输出高电平用来读写数据  
  }                             
                                
  CLR_OLED_CS();              //CS输出低电平为写入数据或命令作准备
                                
  for(i = 0; i < 8; i++)      //循环8次，从高到低取出要写入的数据或命令的8个bit
  {                             
    CLR_OLED_SCK();           //SCK输出低电平为写入数据作准备
                                
    if(dat & 0x80)            //判断要写入的数据或命令的最高位是1还是0
    {                           
      SET_OLED_DIN();         //要写入的数据或命令的最高位是1，DIN输出高电平表示1
    }                           
    else                        
    {                           
      CLR_OLED_DIN();         //要写入的数据或命令的最高位是0，DIN输出低电平表示0
    }                           
    SET_OLED_SCK();           //SCK输出高电平，DIN的状态不再变化，此时写入数据线的数据
                                
    dat <<= 1;                //左移一位，次高位移到最高位
  }                             
                                
  SET_OLED_CS();              //OLED的CS输出高电平，不再写入数据或命令  
  SET_OLED_DC();              //OLED的DC输出高电平
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
  u8 pos;                           //存放点所在的页数
  u8 bx;                            //存放点所在的屏幕的行号
  u8 temp = 0;                      //用来存放画点位置相对于字节的位
                                    
  if(x > 127 || y > 63)             //如果指定位置超过额定范围
  {                                 
    return;                         //返回空，函数结束
  }                                 
                                    
  pos = 7 - y / 8;                  //求指定位置所在页数
  bx = y % 8;                       //求指定位置在上面求出页数中的行号
  temp = 1 << (7 - bx);             //（7-bx）求出相应SSD1306的行号，并在字节中相应的位置为1
                                    
  if(t)                             //判断填充标志为1还是0
  {                                 
    s_arrOLEDGRAM[x][pos] |= temp;  //如果填充标志为1，指定点填充
  }                                 
  else                              
  {                                 
    s_arrOLEDGRAM[x][pos] &= ~temp; //如果填充标志为0，指定点清空  
  }
}


/* 数学辅助函数：整数的幂运算 m^n */
static  u32 CalcPow(u8 m, u8 n)
{
  u32 result = 1;
  u8 i;
  for (i = 0; i < n; i++)
  {
    result *= m;
  }
  return result;
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
  ConfigOLEDGPIO();     //配置OLED的GPIO
  
  CLR_OLED_RES();
  DelayNms(10);  
  SET_OLED_RES();       //RES引脚务必拉高
  DelayNms(10);
  
  ConfigOLEDReg();      //配置OLED的寄存器
  
  OLEDClear();          //清除OLED屏内容
}


void  OLEDDisplayOn( void )
{
  //打开关闭电荷泵，第一个字节为命令字，0x8D，第二个字节设置值，0x10-关闭电荷泵，0x14-打开电荷泵
  OLEDWriteByte(0x8D, OLED_CMD);  //第一个字节0x8D为命令
  OLEDWriteByte(0x14, OLED_CMD);  //0x14-打开电荷泵

  //设置显示开关，0xAE-关闭显示，0xAF-开启显示
  OLEDWriteByte(0xAF, OLED_CMD);  //开启显示
}


void  OLEDDisplayOff( void )
{
  //打开关闭电荷泵，第一个字节为命令字，0x8D，第二个字节设置值，0x10-关闭电荷泵，0x14-打开电荷泵
  OLEDWriteByte(0x8D, OLED_CMD);  //第一个字节为命令字，0x8D
  OLEDWriteByte(0x10, OLED_CMD);  //0x10-关闭电荷泵

  //设置显示开关，0xAE-关闭显示，0xAF-开启显示
  OLEDWriteByte(0xAE, OLED_CMD);  //关闭显示
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
  u8 page;
  u8 x;

  for(page = 0; page < 8; page++)
  {
    OLEDWriteByte((u8)(0xB0 + page), OLED_CMD);  //设置页地址
    OLEDWriteByte(0x00, OLED_CMD);         //设置列地址低4位
    OLEDWriteByte(0x10, OLED_CMD);         //设置列地址高4位

    for(x = 0; x < 128; x++)
    {
      OLEDWriteByte(s_arrOLEDGRAM[x][page], OLED_DATA);  //写入数据
    }
  }
}


/*
 * 清除 GRAM(显存)缓冲区（全部置0）并刷新到屏幕。
 */
void  OLEDClear(void)
{
  u8 i;
  u8 n;
                                  
  for(i = 0; i < 8; i++)          //遍历每一页
  {                               
    for(n = 0; n < 128; n++)      //遍历每一列
    {                             
      s_arrOLEDGRAM[n][i] = 0x00;   //将指定点清零
    }                             
  }                               
                                  
  OLEDRefreshGRAM();  //将STM32的GRAM写入到SSD1306的GRAM
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
  const u8 *pFont;  //指向字模数据的指针
  u8 width;   //字模宽度（字节数）
  u8 col;
  u8 bit;

  if(chr < ' ' || chr > '~')  //如果字符不在可显示范围内
  {
    chr = ' ';  //显示空格
  }

  if (size == 12)
  {
    pFont = g_iASCII1206[chr - ' '];  //指向12x6字模数据
  }
  else if (size == 16)
  {
    pFont = g_iASCII1608[chr - ' '];  //指向16x8字模数据
  }
  else
  {
    return;  //不支持的字体大小
  }

  width = (u8)(size / 2);  //字模宽度（字节数）

  for (col = 0; col < width; col++)
  {
      u8 topByte = pFont[col * 2];//获取上半部分字模数据
      u8 bottomByte = pFont[col * 2 + 1];//获取下半部分字模数据

      for (bit = 0; bit < 8; bit++)
      {
        /* code */
        OLEDDrawPoint((u8)(x + col),(u8)(y + bit),(topByte & (0x80 >> bit)) ? mode : !mode);//绘制上半部分
      }
      
      for(bit = 0; bit < 8; bit++)
      {
        OLEDDrawPoint((u8)(x + col),(u8)(y + 8 + bit),(bottomByte & (0x80 >> bit)) ? mode : !mode);//绘制下半部分
      }
  }
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
  u8 t;
  u8 step;

  step = (u8)(size / 2);  //每个数字占用的列数

  for(t = 0; t<len;t++)
  {
    u32 divisor = CalcPow(10, (u8)(len - t - 1));

    u8 temp = (u8)((num / divisor) % 10);  //提取当前位数字

    u8 ch;

    if (temp == 0 && t < (u8)(len - 1))  //前导零处理
    {
      ch = ' ';  //用空格代替前导零
    }
    else
    {
      ch = (u8)(temp + '0');  //转换为字符
    }

    OLEDShowChar((u8)(x + step * t), y, ch, size, 1);  //显示字符
  }
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
  while (*p != '\0')
  {
    //如果当前 x 坐标超过屏幕宽度，换行
    if (x > 122)
    {
      x = 0;
      y=(u8)(y+16);

    }

    if (y > 58)//如果y坐标大于58，回到(0,0)并清屏
    {
      x = 0;
      y = 0;
      OLEDClear();
    }

    OLEDShowChar(x, y, *p, 16, 1);  //显示字符
    x=(u8)(x + 8);  //移动到下一个字符位置
    p++;  //指向下一个字符
    
  }
  
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
    
    /*   stepX = width / divX, stepY = height / divY（防止除以0）
     *   垂直线：x 从 xStart 开始，以 stepX 为步长：
     *     每隔一个 y 画点（y += 2）
     *   水平线：y 从 yStart 开始，以 stepY 为步长：
     *     每隔一个 x 画点（x += 2） */
    u8 stepX;
    u8 stepY;
    u16 x;
    u16 y;
    u16 xEnd;
    u16 yEnd;

    if(width == 0 || height == 0)
    {
      return;  //避免除以0
    }

    stepX = (divX == 0) ? width : (u8)(width / divX);
    stepY = (divY == 0) ? height : (u8)(height / divY);

    if(stepX == 0) stepX = 1;  //防止步长为0
    if(stepY == 0) stepY = 1;  //防止步长为0

    xEnd = (u16)(xStart + width);
    yEnd = (u16)(yStart + height);

    for(x = xStart;x<xEnd;x = (u16)(x + stepX))
    {
        for(y = yStart;y<yEnd;y = (u16)(y + 2))
        {
            OLEDDrawPoint((u8)x, (u8)y, 1);
        }
    }

    for(y = yStart;y<yEnd;y = (u16)(y + stepY))
    {
        for(x = xStart;x<xEnd;x = (u16)(x + 2))
        {
            OLEDDrawPoint((u8)x, (u8)y, 1);
        }
    }

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

    u8 x;
    u16 hasPrev = FALSE;
    u8 prevY = 0;

    if(pData == NULL || count == 0 || width == 0 || yHeight == 0)//检查参数有效性
    {
        return;  //无效参数
    }

    /* [FIX] 预计算自动缩放参数，移到循环外避免 O(n²) 性能问题
     *       原代码在每个像素都遍历整个数组计算 min/max，80x128=10240次迭代 */
    u16 preMinVal = 0;
    u16 preMaxValData = 0;
    u32 preRange = 0;
    u32 prePadRange = 0;
    u32 prePadDelta = 0;
    u32 preDispMin = 0;

    if(gain == 0)
    {
        u8 i;
        preMinVal = pData[0];
        preMaxValData = pData[0];
        for(i = 1; i < count; i++)
        {
            if(pData[i] < preMinVal) preMinVal = pData[i];
            if(pData[i] > preMaxValData) preMaxValData = pData[i];
        }
        preRange = (u32)(preMaxValData - preMinVal);
        if(preRange == 0) preRange = 1;
        prePadRange = preRange + (preRange / 4) + 4;
        if(prePadRange == 0) prePadRange = 1;
        prePadDelta = (prePadRange > preRange) ? ((prePadRange - preRange) / 2) : 0;
        preDispMin = (preMinVal > prePadDelta) ? (u32)(preMinVal - prePadDelta) : 0;
    }

    for(x = 0; x<width; x++)
    {
      u8 idx;
      u16 sample;
      u8 curY;

      if(count > width)
      {
        idx = (u8)((u16)x * count / width);  //降采样索引

        if(idx >= count)
        {
          idx = (u8)(count - 1);  //防止越界
        }
      }
      else
      {
        idx = (x < count) ? x : (u8)(count - 1); //直接映射索引
      }

      sample = pData[idx];//获取采样值

      if(gain == 0)//自动缩放：[FIX] 使用循环外预计算的值，不再重复遍历数组
      {
        i32 delta = (i32)sample - (i32)preDispMin;

        if(delta < 0)
        {
          delta = 0;  //防止下溢
        }

        if((u32)delta > prePadRange)
        {
          delta = (i32)prePadRange;  //防止上溢
        }

        curY = (u8)(yStart + (yHeight - 1) - ((u32)delta * yHeight / prePadRange));//映射到屏幕坐标
      }

      else//固定增益模式
      {
        u32 value = (u32)sample * gain;

        if(value > maxVal)
        {
          value = maxVal;  //钳位到最大值
        }

        if(maxVal == 0)
        {
          curY = yStart;   //防止除以0，直接使用起始y坐标
        }
        else
        {
          curY = (u8)(yStart + (yHeight - 1) - (value * yHeight / maxVal));//映射到屏幕坐标
        }


      }

      if (!hasPrev)
      {
        OLEDDrawPoint((u8)(xStart + x), curY, 1);  //绘制第一个点
        //记录当前点为前一个点
        prevY = curY;
        hasPrev = TRUE;
      }
      else
      { 
        u8 y1 = prevY;
        u8 y2 = curY;
        u8 yy;

        //交换 y1 和 y2 以确保 y1 <= y2
        if(y1 > y2)
        {
          u8 temp = y1;
          y1 = y2;
          y2 = temp;
        }

        for(yy = y1; yy <= y2; yy++)
        {
          OLEDDrawPoint((u8)(xStart + x), yy, 1);  //绘制垂直线
        }

        prevY = curY;  //更新前一个点
      }
    }
}
