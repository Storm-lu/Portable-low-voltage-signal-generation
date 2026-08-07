#include <stdint.h>


#if defined ( __CC_ARM   )
  #define __ASM            __asm
  #define __INLINE         __inline

#elif defined ( __ICCARM__ )
  #define __ASM           __asm
  #define __INLINE        inline

#elif defined   (  __GNUC__  )
  #define __ASM            __asm
  #define __INLINE         inline

#elif defined   (  __TASKING__  )
  #define __ASM            __asm
  #define __INLINE         inline

#endif


#if defined ( __CC_ARM   )


__ASM uint32_t __get_PSP(void)
{
  mrs r0, psp
  bx lr
}


__ASM void __set_PSP(uint32_t topOfProcStack)
{
  msr psp, r0
  bx lr
}


__ASM uint32_t __get_MSP(void)
{
  mrs r0, msp
  bx lr
}


__ASM void __set_MSP(uint32_t mainStackPointer)
{
  msr msp, r0
  bx lr
}


__ASM uint32_t __REV16(uint16_t value)
{
  rev16 r0, r0
  bx lr
}


__ASM int32_t __REVSH(int16_t value)
{
  revsh r0, r0
  bx lr
}


#if (__ARMCC_VERSION < 400000)


__ASM void __CLREX(void)
{
  clrex
}


__ASM uint32_t  __get_BASEPRI(void)
{
  mrs r0, basepri
  bx lr
}


__ASM void __set_BASEPRI(uint32_t basePri)
{
  msr basepri, r0
  bx lr
}


__ASM uint32_t __get_PRIMASK(void)
{
  mrs r0, primask
  bx lr
}


__ASM void __set_PRIMASK(uint32_t priMask)
{
  msr primask, r0
  bx lr
}


__ASM uint32_t  __get_FAULTMASK(void)
{
  mrs r0, faultmask
  bx lr
}


__ASM void __set_FAULTMASK(uint32_t faultMask)
{
  msr faultmask, r0
  bx lr
}


__ASM uint32_t __get_CONTROL(void)
{
  mrs r0, control
  bx lr
}


__ASM void __set_CONTROL(uint32_t control)
{
  msr control, r0
  bx lr
}

#endif


#elif (defined (__ICCARM__))

#pragma diag_suppress=Pe940


uint32_t __get_PSP(void)
{
  __ASM("mrs r0, psp");
  __ASM("bx lr");
}


void __set_PSP(uint32_t topOfProcStack)
{
  __ASM("msr psp, r0");
  __ASM("bx lr");
}


uint32_t __get_MSP(void)
{
  __ASM("mrs r0, msp");
  __ASM("bx lr");
}


void __set_MSP(uint32_t topOfMainStack)
{
  __ASM("msr msp, r0");
  __ASM("bx lr");
}


uint32_t __REV16(uint16_t value)
{
  __ASM("rev16 r0, r0");
  __ASM("bx lr");
}


uint32_t __RBIT(uint32_t value)
{
  __ASM("rbit r0, r0");
  __ASM("bx lr");
}


uint8_t __LDREXB(uint8_t *addr)
{
  __ASM("ldrexb r0, [r0]");
  __ASM("bx lr");
}


uint16_t __LDREXH(uint16_t *addr)
{
  __ASM("ldrexh r0, [r0]");
  __ASM("bx lr");
}


uint32_t __LDREXW(uint32_t *addr)
{
  __ASM("ldrex r0, [r0]");
  __ASM("bx lr");
}


uint32_t __STREXB(uint8_t value, uint8_t *addr)
{
  __ASM("strexb r0, r0, [r1]");
  __ASM("bx lr");
}


uint32_t __STREXH(uint16_t value, uint16_t *addr)
{
  __ASM("strexh r0, r0, [r1]");
  __ASM("bx lr");
}


uint32_t __STREXW(uint32_t value, uint32_t *addr)
{
  __ASM("strex r0, r0, [r1]");
  __ASM("bx lr");
}

#pragma diag_default=Pe940


#elif (defined (__GNUC__))


uint32_t __get_PSP(void) __attribute__( ( naked ) );
uint32_t __get_PSP(void)
{
  uint32_t result=0;

  __ASM volatile ("MRS %0, psp\n\t"
                  "MOV r0, %0 \n\t"
                  "BX  lr     \n\t"  : "=r" (result) );
  return(result);
}


void __set_PSP(uint32_t topOfProcStack) __attribute__( ( naked ) );
void __set_PSP(uint32_t topOfProcStack)
{
  __ASM volatile ("MSR psp, %0\n\t"
                  "BX  lr     \n\t" : : "r" (topOfProcStack) );
}


uint32_t __get_MSP(void) __attribute__( ( naked ) );
uint32_t __get_MSP(void)
{
  uint32_t result=0;

  __ASM volatile ("MRS %0, msp\n\t"
                  "MOV r0, %0 \n\t"
                  "BX  lr     \n\t"  : "=r" (result) );
  return(result);
}


void __set_MSP(uint32_t topOfMainStack) __attribute__( ( naked ) );
void __set_MSP(uint32_t topOfMainStack)
{
  __ASM volatile ("MSR msp, %0\n\t"
                  "BX  lr     \n\t" : : "r" (topOfMainStack) );
}


uint32_t __get_BASEPRI(void)
{
  uint32_t result=0;

  __ASM volatile ("MRS %0, basepri_max" : "=r" (result) );
  return(result);
}


void __set_BASEPRI(uint32_t value)
{
  __ASM volatile ("MSR basepri, %0" : : "r" (value) );
}


uint32_t __get_PRIMASK(void)
{
  uint32_t result=0;

  __ASM volatile ("MRS %0, primask" : "=r" (result) );
  return(result);
}


void __set_PRIMASK(uint32_t priMask)
{
  __ASM volatile ("MSR primask, %0" : : "r" (priMask) );
}


uint32_t __get_FAULTMASK(void)
{
  uint32_t result=0;

  __ASM volatile ("MRS %0, faultmask" : "=r" (result) );
  return(result);
}


void __set_FAULTMASK(uint32_t faultMask)
{
  __ASM volatile ("MSR faultmask, %0" : : "r" (faultMask) );
}


uint32_t __get_CONTROL(void)
{
  uint32_t result=0;

  __ASM volatile ("MRS %0, control" : "=r" (result) );
  return(result);
}


void __set_CONTROL(uint32_t control)
{
  __ASM volatile ("MSR control, %0" : : "r" (control) );
}


uint32_t __REV(uint32_t value)
{
  uint32_t result=0;

  __ASM volatile ("rev %0, %1" : "=r" (result) : "r" (value) );
  return(result);
}


uint32_t __REV16(uint16_t value)
{
  uint32_t result=0;

  __ASM volatile ("rev16 %0, %1" : "=r" (result) : "r" (value) );
  return(result);
}


int32_t __REVSH(int16_t value)
{
  uint32_t result=0;

  __ASM volatile ("revsh %0, %1" : "=r" (result) : "r" (value) );
  return(result);
}


uint32_t __RBIT(uint32_t value)
{
  uint32_t result=0;

   __ASM volatile ("rbit %0, %1" : "=r" (result) : "r" (value) );
   return(result);
}


uint8_t __LDREXB(uint8_t *addr)
{
    uint8_t result=0;

   __ASM volatile ("ldrexb %0, [%1]" : "=r" (result) : "r" (addr) );
   return(result);
}


uint16_t __LDREXH(uint16_t *addr)
{
    uint16_t result=0;

   __ASM volatile ("ldrexh %0, [%1]" : "=r" (result) : "r" (addr) );
   return(result);
}


uint32_t __LDREXW(uint32_t *addr)
{
    uint32_t result=0;

   __ASM volatile ("ldrex %0, [%1]" : "=r" (result) : "r" (addr) );
   return(result);
}


uint32_t __STREXB(uint8_t value, uint8_t *addr)
{
   uint32_t result=0;

   __ASM volatile ("strexb %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
   return(result);
}


uint32_t __STREXH(uint16_t value, uint16_t *addr)
{
   uint32_t result=0;

   __ASM volatile ("strexh %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
   return(result);
}


uint32_t __STREXW(uint32_t value, uint32_t *addr)
{
   uint32_t result=0;

   __ASM volatile ("strex %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
   return(result);
}


#elif (defined (__TASKING__))


#endif
