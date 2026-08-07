#ifndef _OLED_H_
#define _OLED_H_


#include  "DataType.h"


/*
 * Initialize OLED display.
 * Configures GPIO (PB12/13/14/15, PC3), resets SSD1306, sends init commands.
 * Called from Main.c InitHardware().
 */
void  InitOLED(void);

/*
 * Turn OLED display on (charge pump + display on).
 */
void  OLEDDisplayOn(void);

/*
 * Turn OLED display off (charge pump + display off).
 */
void  OLEDDisplayOff(void);

/*
 * Flush the internal GRAM buffer (128x8 bytes) to the physical OLED screen.
 * Must be called after all drawing operations to make them visible.
 * Called at the end of every Display* function.
 */
void  OLEDRefreshGRAM(void);

/*
 * Clear the GRAM buffer and refresh screen (all pixels off).
 * Called at the beginning of every Display* function.
 */
void  OLEDClear(void);

/*
 * Draw a single ASCII character at (x, y).
 *
 * Parameters:
 *   x    - column position (0~127)
 *   y    - row position in pixels (0~63)
 *   chr  - ASCII character to display
 *   size - font height: 12 (6x12) or 16 (8x16)
 *   mode - 1=normal, 0=inverted (background pixels on, foreground off)
 */
void  OLEDShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode);

/*
 * Display an unsigned integer at (x, y) with fixed digit count.
 *
 * Parameters:
 *   x, y - position
 *   num  - number to display (32-bit unsigned)
 *   len  - total digits to show (leading zeros replaced with spaces)
 *   size - font size (12 or 16)
 *
 * Example: num=42, len=4, size=16 -> displays "  42" (2 spaces + "42")
 */
void  OLEDShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);

/*
 * Display a null-terminated string at (x, y).
 * Uses 8x16 font. Auto-wraps when x exceeds 122 or y exceeds 58.
 *
 * Parameters:
 *   x, y - starting position
 *   p    - pointer to null-terminated string
 */
void  OLEDShowString(u8 x, u8 y, const u8* p);

/*
 * Draw a dotted grid background (oscilloscope style).
 *
 * Parameters:
 *   xStart, yStart - top-left corner of grid
 *   width, height  - grid area size in pixels
 *   divX           - number of horizontal divisions (vertical lines)
 *   divY           - number of vertical divisions (horizontal lines)
 *
 * Lines are dotted (every other pixel) to avoid obscuring waveform.
 */
void OLEDDrawGrid(u8 xStart, u8 yStart, u8 width, u8 height, u8 divX, u8 divY);

/*
 * Draw a waveform from sample data with optional auto-scaling.
 *
 * Parameters:
 *   pData  - array of ADC samples (u16, 0~4095)
 *   count  - number of samples (typically 128)
 *   xStart - drawing area left edge
 *   yStart - drawing area top edge
 *   width  - drawing area width in pixels (waveform is down-sampled to fit)
 *   yHeight- drawing area height in pixels
 *   maxVal - maximum sample value for fixed-gain mode (e.g. 4095)
 *   gain   - 0=auto-scale (find min/max, fill area with 25% margin),
 *            >0=fixed gain multiplier (value*gain, clamped to maxVal)
 *
 * Adjacent points are connected with vertical lines for continuous display.
 * Called from DisplaySignalMeasure().
 */
void OLEDDrawWaveFormEx(u16 *pData, u8 count, u8 xStart, u8 yStart, u8 width, u8 yHeight, u16 maxVal, u8 gain);

#endif
