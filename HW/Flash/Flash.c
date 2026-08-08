#include "Flash.h"
#include "stm32f10x_conf.h"
#include "UART1.h"


#define FLASH_PAGE_SIZE ((u16)0x0800)


static u32 s_arrFlashBuf[FLASH_PAGE_SIZE / 4];


static u32  ReadWord(const u32 addr);
static void WriteWordNoCheck(const u32 startAddr, u32 *pBuf, u16 numToWrite);


/*
 * 底层：从 Flash 地址读取一个32位字。
 *
 * 参数：
 *   addr - Flash 绝对地址（如 0x0803F800）
 *
 * 返回值：该地址处的32位数据。
 *
 * 实现方式：直接将该地址作为 volatile u32 指针解引用。
 */
static u32  ReadWord(const u32 addr)
{
  /* 待实现：return *(vu32*)addr; */
  return *((volatile u32*)addr);
  
}


/*
 * 底层：向 Flash 写入数据，不做擦除检查。
 *
 * 参数：
 *   startAddr   - 起始 Flash 地址
 *   pBuf        - 数据缓冲区指针
 *   numToWrite  - 要写入的32位字数量
 *
 * 使用标准外设库的 FLASH_ProgramWord()。
 * 不检查目标区域是否已擦除（调用者须确保为 0xFF）。
 */
static void WriteWordNoCheck(const u32 startAddr, u32 *pBuf, u16 numToWrite)
{
  /* 待实现：
   *   for i = 0 to numToWrite-1:
   *     FLASH_ProgramWord(startAddr + i*4, pBuf[i]);
   */
  u16 i;
  for(i = 0; i < numToWrite; i++)
  {
    FLASH_ProgramWord(startAddr + (u32)i * 4, pBuf[i]);//写入一个32位字
  }
}


/*
 * 初始化 Flash（当前为空，保留以备将来使用）。
 */
void InitFlash(void)
{
  /* 无需初始化。 */
}


/*
 * 向 Flash 写入数据，自动处理页擦除。
 *
 * 这是关键函数。Flash 只能通过擦除（将所有位置1）实现 0->1，
 * 再编程实现 1->0。不能向未擦除的区域写入数据，否则会损坏数据。
 *
 * 算法流程：
 *   1. 解锁 Flash（FLASH_Unlock）
 *   2. 计算起始地址所在的页：
 *        offAddr   = addr - STM32_FLASH_BASE
 *        pagePos   = offAddr / FLASH_PAGE_SIZE
 *        pageOff   = (offAddr % FLASH_PAGE_SIZE) / 4   （页内字偏移）
 *        pageResidue = FLASH_PAGE_SIZE/4 - pageOff     （页内剩余字数）
 *   3. 将整页数据读入 s_arrFlashBuf（备份）
 *   4. 检查目标字是否全为 0xFFFFFFFF（已擦除）：
 *        若是：直接用 WriteWordNoCheck 写入
 *        若否：擦除该页，将新数据合并到备份中，整页写回
 *   5. 处理跨页写入（数据跨越页边界的情况）
 *   6. 锁定 Flash（FLASH_Lock）
 *
 * 参数：
 *   startAddr  - Flash 绝对地址（必须 >= STM32_FLASH_BASE）
 *   pBuf       - 要写入的数据
 *   numToWrite - 32位字数量
 */
void  STM32FlashWriteWord(const u32 startAddr, u32* pBuf, u16 numToWrite)
{
  /* 待实现：按上述说明实现页感知的写入算法。
   *       关键步骤：解锁、读页、检查是否已擦除、必要时擦除、
   *       合并数据、写回、处理跨页、锁定。
   *       使用 STM32FlashReadWord 读取页面，FLASH_ErasePage 擦除，
   *       WriteWordNoCheck 写入。 */
  u32 currAddr;
  u16 currIndex;
  u16 pageWords;

  //如果起始地址小于 Flash 基地址，或者缓冲区为空，或者写入数量为0，则返回
  if(startAddr < STM32_FLASH_BASE || pBuf == NULL || numToWrite == 0)
  {
    return; //无效参数
  }

  pageWords = (u16)(FLASH_PAGE_SIZE / 4); //每页的字数
  currAddr = startAddr; //当前写入地址
  currIndex = 0; //当前写入数据的索引

  FLASH_Unlock(); //解锁 Flash

  while(numToWrite > 0)
  {
    u32 offAddr;
    u32 pageBase;
    u16 pageOff;
    u16 pageResidue;
    u16 chunk;
    u16 i;
    BOOL needErase = FALSE;

    offAddr = currAddr - STM32_FLASH_BASE; //计算偏移地址
    pageBase = STM32_FLASH_BASE + (offAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE; //计算页基地址
    pageOff = (u16)((offAddr % FLASH_PAGE_SIZE) / 4); //计算页内偏移
    pageResidue = (u16)(pageWords - pageOff); //计算页内剩余字数
    chunk = (numToWrite < pageResidue) ? numToWrite : pageResidue; //本次写入的字数


    for(i = 0; i < chunk; i++)
    {
      if(ReadWord(currAddr + (u32)i * 4) != 0xFFFFFFFFUL)
      {
        needErase = TRUE; //如果目标字不是全1，标记需要擦除
        break;
      }
    }
    if(needErase)
    {
      STM32FlashReadWord(pageBase, s_arrFlashBuf, pageWords); //备份整页数据
      for(i = 0; i < chunk; i++)
      {
        s_arrFlashBuf[pageOff + i] = pBuf[currIndex + i]; //合并新数据到备份中
      }

      FLASH_ErasePage(pageBase); //擦除该页
      WriteWordNoCheck(pageBase, s_arrFlashBuf, pageWords); //写回整页数据
    }
    else
    {
      WriteWordNoCheck(currAddr, &pBuf[currIndex], chunk); //直接写入数据
    }

    currAddr += (u32)chunk * 4; //更新当前地址
    currIndex += chunk; //更新当前索引
    numToWrite -= chunk; //更新剩余写入数量
  }

  FLASH_Lock(); //锁定 Flash
}


/*
 * 从 Flash 读取数据。
 *
 * 参数：
 *   startAddr - Flash 绝对地址
 *   pBuf      - 输出缓冲区
 *   numToRead - 要读取的32位字数量
 *
 * 简单循环：通过 ReadWord() 读取每个字并存入 pBuf。
 */
void  STM32FlashReadWord(const u32 startAddr, u32* pBuf, u16 numToRead)
{
  /* 待实现：for i = 0 to numToRead-1:
   *         pBuf[i] = ReadWord(startAddr + i*4); */

  u16 i;
  //如果起始地址小于 Flash 基地址，或者缓冲区为空，或者读取数量为0，则返回
  if(pBuf == NULL || numToRead == 0)
  {
    return; //无效参数
  }

  for(i = 0; i < numToRead; i++)
  {
    pBuf[i] = ReadWord(startAddr + (u32)i * 4); //读取每个字
  }
}
