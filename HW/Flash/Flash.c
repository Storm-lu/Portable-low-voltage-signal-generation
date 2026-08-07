#include "Flash.h"
#include "stm32f10x_conf.h"
#include "UART1.h"


#define FLASH_PAGE_SIZE ((u16)0x0800)


static u32 s_arrFlashBuf[FLASH_PAGE_SIZE / 4];


static u32  ReadWord(const u32 addr);
static void WriteWordNoCheck(const u32 startAddr, u32 *pBuf, u16 numToWrite);


/*
 * Low-level: Read one 32-bit word from a flash address.
 *
 * Parameters:
 *   addr - absolute flash address (e.g. 0x0803F800)
 *
 * Returns: the 32-bit value at that address.
 *
 * Implementation: simply dereference the address as a volatile u32 pointer.
 */
static u32  ReadWord(const u32 addr)
{
  /* TODO: Return *(vu32*)addr; */
  return 0;
}


/*
 * Low-level: Write words to flash without erase check.
 *
 * Parameters:
 *   startAddr   - starting flash address
 *   pBuf        - pointer to data buffer
 *   numToWrite  - number of 32-bit words to write
 *
 * Uses FLASH_ProgramWord() from the standard peripheral library.
 * Does NOT check if the target area is erased (caller must ensure 0xFF).
 */
static void WriteWordNoCheck(const u32 startAddr, u32 *pBuf, u16 numToWrite)
{
  /* TODO:
   *   For i = 0 to numToWrite-1:
   *     FLASH_ProgramWord(startAddr + i*4, pBuf[i]);
   */
}


/*
 * Initialize Flash (currently empty, reserved for future use).
 */
void InitFlash(void)
{
  /* Nothing to initialize. */
}


/*
 * Write data to flash with automatic page erase.
 *
 * This is the key function. Flash can only write 0->1 by erasing
 * (setting all bits to 1), then programming 1->0. You cannot write
 * to a non-erased location without corrupting data.
 *
 * Algorithm:
 *   1. Unlock flash (FLASH_Unlock)
 *   2. Calculate which page the start address falls in:
 *        offAddr   = addr - STM32_FLASH_BASE
 *        pagePos   = offAddr / FLASH_PAGE_SIZE
 *        pageOff   = (offAddr % FLASH_PAGE_SIZE) / 4   (word offset in page)
 *        pageResidue = FLASH_PAGE_SIZE/4 - pageOff     (words left in page)
 *   3. Read the entire page into s_arrFlashBuf (backup)
 *   4. Check if target words are all 0xFFFFFFFF (already erased):
 *        If yes: write directly with WriteWordNoCheck
 *        If no:  erase page, merge new data into backup, write entire page back
 *   5. Handle multi-page writes (if data spans page boundary)
 *   6. Lock flash (FLASH_Lock)
 *
 * Parameters:
 *   startAddr  - absolute flash address (must be >= STM32_FLASH_BASE)
 *   pBuf       - data to write
 *   numToWrite - number of 32-bit words
 */
void  STM32FlashWriteWord(const u32 startAddr, u32* pBuf, u16 numToWrite)
{
  /* TODO: Implement the page-aware write algorithm as described above.
   *       Key steps: unlock, read page, check erased, erase if needed,
   *       merge data, write back, handle multi-page, lock.
   *       Use STM32FlashReadWord to read the page, FLASH_ErasePage to erase,
   *       WriteWordNoCheck to write. */
}


/*
 * Read words from flash.
 *
 * Parameters:
 *   startAddr - absolute flash address
 *   pBuf      - output buffer
 *   numToRead - number of 32-bit words to read
 *
 * Simple loop: read each word via ReadWord() and store in pBuf.
 */
void  STM32FlashReadWord(const u32 startAddr, u32* pBuf, u16 numToRead)
{
  /* TODO: For i = 0 to numToRead-1:
   *         pBuf[i] = ReadWord(startAddr + i*4); */
}
