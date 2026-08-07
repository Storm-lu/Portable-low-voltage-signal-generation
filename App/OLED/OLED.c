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


/* GRAM buffer: 128 columns x 8 pages (each page = 8 pixels tall) */
static  u8  s_arrOLEDGRAM[128][8];


static  void  ConfigOLEDGPIO(void);
static  void  ConfigOLEDReg(void);

static  void  OLEDWriteByte(u8 dat, u8 cmd);
static  void  OLEDDrawPoint(u8 x, u8 y, u8 t);

static  u32   CalcPow(u8 m, u8 n);


/*
 * Low-level: GPIO configuration for OLED pins.
 *
 * Pins: PB12=CS, PB13=SCK, PB14=RES, PB15=DIN, PC3=DC
 * All configured as push-pull output, 50MHz.
 */
static  void  ConfigOLEDGPIO(void)
{
  /* TODO: Configure PB12, PB13, PB14, PB15 and PC3 as push-pull output.
   *       Enable clocks for GPIOB and GPIOC.
   *       Set all pins high initially.
   *       Hint: Use RCC_APB2PeriphClockCmd + GPIO_Init. */
}


/*
 * Low-level: Send SSD1306 initialization command sequence.
 *
 * Key commands:
 *   0xAE = display off, 0xAF = display on
 *   0xD5 0x50 = set display clock divide
 *   0xA8 0x3F = set multiplex ratio (64 lines)
 *   0x20 0x02 = set page addressing mode
 *   0xA1 = segment remap, 0xC0 = normal COM scan
 *   0x8D 0x14 = enable charge pump
 *   0xAF = display on
 */
static  void  ConfigOLEDReg( void )
{
  /* TODO: Send the SSD1306 init command sequence via OLEDWriteByte().
   *       Use OLED_CMD as the second parameter for commands.
   *       End with 0xAF (display on). */
}


/*
 * Low-level: Send one byte to OLED via software SPI.
 *
 * Parameters:
 *   dat - the byte to send
 *   cmd - OLED_CMD (0) for command, OLED_DATA (1) for data
 *
 * Steps:
 *   1. Set DC pin (PC3) low for command, high for data
 *   2. Pull CS low (start transfer)
 *   3. For each of 8 bits (MSB first):
 *      - Pull SCK low
 *      - Set DIN based on bit value
 *      - Pull SCK high (clock edge)
 *   4. Pull CS high (end transfer), DC high (default)
 */
static  void  OLEDWriteByte(u8 dat, u8 cmd)
{
  /* TODO: Implement software SPI bit-bang transfer.
   *       MSB first, 8 bits, use CLR/SET macros for pin control. */
}


/*
 * Low-level: Set or clear a single pixel in GRAM.
 *
 * Parameters:
 *   x - column (0~127)
 *   y - row (0~63)
 *   t - 1=set pixel, 0=clear pixel
 *
 * GRAM layout: s_arrOLEDGRAM[x][page], where page = 7 - y/8
 *              bit position within byte = 1 << (7 - y%8)
 *
 * Use OR to set, AND ~ to clear.
 */
static  void  OLEDDrawPoint(u8 x, u8 y, u8 t)
{
  /* TODO: Implement point drawing in GRAM buffer.
   *       Bounds check: x<=127, y<=63.
   *       Calculate page = 7 - y/8, bit = 1<<(7 - y%8).
   *       If t: GRAM[x][page] |= bit, else GRAM[x][page] &= ~bit. */
}


/* Math helper: integer power m^n */
static  u32 CalcPow(u8 m, u8 n)
{
  /* TODO: Return m raised to the power n (integer). */
  return 1;
}


/* ============================================================
 *  Public API - implement these
 * ============================================================ */


/*
 * Initialize OLED: configure GPIO, reset, send init commands, clear screen.
 * Called from Main.c InitHardware().
 */
void  InitOLED(void)
{
  /* TODO:
   *   1. Call ConfigOLEDGPIO()
   *   2. Reset sequence: CLR_OLED_RES -> DelayNms(10) -> SET_OLED_RES -> DelayNms(10)
   *   3. Call ConfigOLEDReg()
   *   4. Call OLEDClear() */
}


void  OLEDDisplayOn( void )
{
  /* TODO: Send 0x8D 0x14 (charge pump on) then 0xAF (display on). */
}


void  OLEDDisplayOff( void )
{
  /* TODO: Send 0x8D 0x10 (charge pump off) then 0xAE (display off). */
}


/*
 * Flush the entire GRAM buffer to the OLED screen.
 *
 * The OLED is organized as 8 pages, each 128 columns wide.
 * For each page i (0~7):
 *   1. Send page address command: 0xB0 + i
 *   2. Send column address: low nibble (0x00) + high nibble (0x10)
 *   3. Send 128 data bytes from s_arrOLEDGRAM[0..127][i]
 *
 * Called at the end of every Display function.
 */
void  OLEDRefreshGRAM(void)
{
  /* TODO: Iterate 8 pages, set page/column address, send 128 bytes each. */
}


/*
 * Clear the GRAM buffer (set all to 0) and refresh to screen.
 */
void  OLEDClear(void)
{
  /* TODO: Zero out s_arrOLEDGRAM[128][8], then call OLEDRefreshGRAM(). */
}


/*
 * Display a single ASCII character at (x, y) in GRAM.
 *
 * Parameters:
 *   x, y  - top-left position (x=column, y=row in pixels)
 *   chr   - ASCII character (space ' ' = 0x20 is the first in font table)
 *   size  - font height in pixels (12 or 16)
 *   mode  - 1=normal, 0=inverted
 *
 * Font data: g_iASCII1206[chr-' '][12] or g_iASCII1608[chr-' '][16]
 * Each byte in font data represents 8 vertical pixels, MSB first.
 * After every 8 pixels in y, advance x and reset y to y0.
 */
void  OLEDShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode)
{
  /* TODO:
   *   1. Convert chr to index: chr -= ' '
   *   2. For each byte in font data (size bytes):
   *      - Select font table based on size (g_iASCII1206 or g_iASCII1608)
   *      - For each of 8 bits (MSB first):
   *        If bit set -> OLEDDrawPoint(x, y, mode)
   *        Else       -> OLEDDrawPoint(x, y, !mode)
   *        y++, if (y - y0) == size: y=y0, x++ (wrap to next column) */
}


/*
 * Display an unsigned number at (x, y).
 *
 * Parameters:
 *   x, y - position
 *   num  - number to display (unsigned 32-bit)
 *   len  - number of digits to display (padded with spaces)
 *   size - font size (12 or 16)
 *
 * Leading zeros are suppressed (replaced with spaces), except the last digit.
 * Uses CalcPow(10, n) for digit extraction.
 *
 * Example: num=42, len=4 -> displays "  42"
 */
void  OLEDShowNum(u8 x, u8 y, u32 num, u8 len, u8 size)
{
  /* TODO:
   *   For each digit position t (0 to len-1):
   *     temp = (num / 10^(len-t-1)) % 10
   *     If leading zero and not last digit: show space
   *     Else: show digit via OLEDShowChar
   *   Character width = size/2 pixels. */
}


/*
 * Display a null-terminated string at (x, y).
 *
 * Uses 16-pixel font (size=16, width=8 per char).
 * Auto-wraps: if x > 122, move to next line (y += 16).
 * If y > 58, wrap to (0, 0) and clear screen.
 */
void  OLEDShowString(u8 x, u8 y, const u8* p)
{
  /* TODO: Loop through string, call OLEDShowChar(x, y, *p, 16, 1) for each char.
   *       x += 8 per char. Handle wrapping as described above. */
}


/*
 * Draw a dotted grid background (oscilloscope style).
 *
 * Parameters:
 *   xStart, yStart - top-left corner of grid area
 *   width, height  - grid area dimensions
 *   divX, divY     - number of divisions (cells)
 *
 * Vertical dotted lines at every stepX = width/divX, every other pixel.
 * Horizontal dotted lines at every stepY = height/divY, every other pixel.
 */
void OLEDDrawGrid(u8 xStart, u8 yStart, u8 width, u8 height, u8 divX, u8 divY) {
    /* TODO:
     *   stepX = width / divX, stepY = height / divY (guard against 0)
     *   Vertical lines: for x from xStart, step by stepX:
     *     draw dots at every other y (y += 2)
     *   Horizontal lines: for y from yStart, step by stepY:
     *     draw dots at every other x (x += 2) */
}


/*
 * Draw a waveform from ADC sample data.
 *
 * Parameters:
 *   pData  - pointer to sample array (u16, typically 128 points)
 *   count  - number of samples in pData
 *   xStart, yStart - top-left of drawing area
 *   width, yHeight - drawing area dimensions
 *   maxVal - maximum possible sample value (e.g. 4095 for 12-bit ADC)
 *   gain   - 0 = auto-scale, >0 = fixed gain multiplier
 *
 * Auto-scale mode (gain==0):
 *   1. Find min and max in pData
 *   2. range = max - min (guard against 0)
 *   3. Add 25% padding: padRange = range + range/4 + 4
 *   4. dispMin = min - (padRange-range)/2 (clamp to 0 if underflow)
 *   5. Map each sample: y = yStart + (yHeight-1) - (sample-dispMin)*yHeight/padRange
 *
 * Fixed gain mode (gain>0):
 *   value = pData[idx] * gain, clamp to maxVal
 *   y = yStart + (yHeight-1) - value*yHeight/maxVal
 *
 * Down-sampling: if count > width, map x to index: idx = x * count / width
 *
 * Line connection: track prevY, draw vertical line from prevY to y at each x.
 *   This makes sine waves appear continuous instead of scattered dots.
 */
void OLEDDrawWaveFormEx(u16 *pData, u8 count, u8 xStart, u8 yStart, u8 width, u8 yHeight, u16 maxVal, u8 gain) {
    /* TODO: Implement as described above.
     *       Handle both auto-scale and fixed-gain modes.
     *       Connect adjacent points with vertical lines for continuous waveform. */
}
