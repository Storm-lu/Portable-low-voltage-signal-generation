#ifndef _PROC_KEY_ONE_H_
#define _PROC_KEY_ONE_H_


#include "DataType.h"


/*
 * 初始化按键处理逻辑（当前为空，保留以备将来使用）。
 */
void  InitProcKeyOne(void);

/*
 * KEY1 按下回调。
 * 由 KeyOne.c 扫描框架在 KEY1 被按下时调用。
 *
 * 信号输出页：切换输出启动/停止（OutputChange）。
 * 设置页：光标移到下一个设置项（ParamChange）。
 */
void  ProcKeyDownKey1(void);

/*
 * KEY1 释放回调（当前未使用）。
 */
void  ProcKeyUpKey1(void);

/*
 * KEY2 按下回调。
 *
 * 设置页：当前设置值递增（ValueChange）。
 * 其他页：无操作。
 */
void  ProcKeyDownKey2(void);

/*
 * KEY2 释放回调（当前未使用）。
 */
void  ProcKeyUpKey2(void);

/*
 * KEY3 按下回调。
 *
 * 设置页：启动长按计时器（2秒倒计时）。
 * 其他页：短按 -> 切换到下一页（DisplayChange）。
 */
void  ProcKeyDownKey3(void);

/*
 * KEY3 释放回调。
 *
 * 若达到长按（2秒）：保存设置（SaveSettings）或切换页面。
 * 若为短按：无额外操作（切页已在按下时处理）。
 */
void  ProcKeyUpKey3(void);

/*
 * KEY3 长按检测函数，由 Proc2msTask() 每2ms调用一次。
 *
 * KEY3 按住期间递增计数器。当达到1000次（2秒）时，
 * 置 longPress 标志，该标志在释放时触发 SaveSettings。
 */
void ProcKeyCheckKey3(void);

#endif
