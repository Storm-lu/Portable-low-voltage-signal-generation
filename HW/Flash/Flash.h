#ifndef _FLASH_H_
#define _FLASH_H_


#include "DataType.h"


/*
 * Base address of STM32 internal flash memory.
 * F103C8T6: 64KB flash from 0x08000000 to 0x0800FFFF.
 */
#define STM32_FLASH_BASE 0x08000000


/*
 * Initialize Flash driver (currently empty, reserved for future use).
 */
void  InitFlash(void);


/*
 * Write 32-bit words to flash with automatic page erase.
 *
 * Flash pages are 1KB (0x400 bytes = 256 words). This function handles
 * page erase-and-rewrite automatically: if the target area is not empty
 * (not all 0xFF), it backs up the page, erases, merges new data, and
 * writes the full page back.
 *
 * Parameters:
 *   startAddr  - absolute flash address (must be >= STM32_FLASH_BASE)
 *   pBuf       - pointer to data buffer (array of u32)
 *   numToWrite - number of 32-bit words to write
 *
 * Used by Settings.c to save parameters at 0x0803F800 (last page).
 */
void  STM32FlashWriteWord(const u32 startAddr, u32* pBuf, u16 numToWrite);


/*
 * Read 32-bit words from flash.
 *
 * Parameters:
 *   startAddr - absolute flash address
 *   pBuf      - output buffer (caller-allocated)
 *   numToRead - number of 32-bit words to read
 *
 * Simple sequential read via pointer dereference.
 * Used by Settings.c to load parameters at boot.
 */
void  STM32FlashReadWord(const u32 startAddr, u32* pBuf, u16 numToRead);

#endif
