#ifndef _FLASH_H_
#define _FLASH_H_


#include "DataType.h"


/*
 * STM32 内部 Flash 的基地址。
 * F103C8T6：64KB Flash，地址范围 0x08000000 ~ 0x0800FFFF。
 */
#define STM32_FLASH_BASE 0x08000000


/*
 * 初始化 Flash 驱动（当前为空，保留以备将来使用）。
 */
void  InitFlash(void);


/*
 * 向 Flash 写入32位字，自动处理页擦除。
 *
 * Flash 页大小为1KB（0x400字节 = 256个字）。此函数自动处理
 * 页擦除与重写：若目标区域非空（非全 0xFF），则备份该页、擦除、
 * 合并新数据后整页写回。
 *
 * 参数：
 *   startAddr  - Flash 绝对地址（必须 >= STM32_FLASH_BASE）
 *   pBuf       - 数据缓冲区指针（u32 数组）
 *   numToWrite - 要写入的32位字数量
 *
 * 由 Settings.c 调用，用于在 0x0803F800（最后一页）保存参数。
 */
void  STM32FlashWriteWord(const u32 startAddr, u32* pBuf, u16 numToWrite);


/*
 * 从 Flash 读取32位字。
 *
 * 参数：
 *   startAddr - Flash 绝对地址
 *   pBuf      - 输出缓冲区（由调用者分配）
 *   numToRead - 要读取的32位字数量
 *
 * 通过指针解引用实现简单的顺序读取。
 * 由 Settings.c 调用，用于开机时加载参数。
 */
void  STM32FlashReadWord(const u32 startAddr, u32* pBuf, u16 numToRead);

#endif
