#ifndef __CM3_CORE_H__
#define __CM3_CORE_H__


#ifdef __cplusplus
 extern "C" {
#endif

#define __CM3_CMSIS_VERSION_MAIN  (0x01)
#define __CM3_CMSIS_VERSION_SUB   (0x30)
#define __CM3_CMSIS_VERSION       ((__CM3_CMSIS_VERSION_MAIN << 16) | __CM3_CMSIS_VERSION_SUB)

#define __CORTEX_M                (0x03)

#include <stdint.h>

#if defined (__ICCARM__)
  #include <intrinsics.h>
#endif


#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    4
#endif


#ifdef __cplusplus
  #define     __I     volatile
#else
  #define     __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile


typedef struct
{
  __IO uint32_t ISER[8];
       uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];
       uint32_t RSERVED1[24];
  __IO uint32_t ISPR[8];
       uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];
       uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];
       uint32_t RESERVED4[56];
  __IO uint8_t  IP[240];
       uint32_t RESERVED5[644];
  __O  uint32_t STIR;
}  NVIC_Type;


typedef struct
{
  __I  uint32_t CPUID;
  __IO uint32_t ICSR;
  __IO uint32_t VTOR;
  __IO uint32_t AIRCR;
  __IO uint32_t SCR;
  __IO uint32_t CCR;
  __IO uint8_t  SHP[12];
  __IO uint32_t SHCSR;
  __IO uint32_t CFSR;
  __IO uint32_t HFSR;
  __IO uint32_t DFSR;
  __IO uint32_t MMFAR;
  __IO uint32_t BFAR;
  __IO uint32_t AFSR;
  __I  uint32_t PFR[2];
  __I  uint32_t DFR;
  __I  uint32_t ADR;
  __I  uint32_t MMFR[4];
  __I  uint32_t ISAR[5];
} SCB_Type;


#define SCB_CPUID_IMPLEMENTER_Pos          24
#define SCB_CPUID_IMPLEMENTER_Msk          (0xFFul << SCB_CPUID_IMPLEMENTER_Pos)

#define SCB_CPUID_VARIANT_Pos              20
#define SCB_CPUID_VARIANT_Msk              (0xFul << SCB_CPUID_VARIANT_Pos)

#define SCB_CPUID_PARTNO_Pos                4
#define SCB_CPUID_PARTNO_Msk               (0xFFFul << SCB_CPUID_PARTNO_Pos)

#define SCB_CPUID_REVISION_Pos              0
#define SCB_CPUID_REVISION_Msk             (0xFul << SCB_CPUID_REVISION_Pos)


#define SCB_ICSR_NMIPENDSET_Pos            31
#define SCB_ICSR_NMIPENDSET_Msk            (1ul << SCB_ICSR_NMIPENDSET_Pos)

#define SCB_ICSR_PENDSVSET_Pos             28
#define SCB_ICSR_PENDSVSET_Msk             (1ul << SCB_ICSR_PENDSVSET_Pos)

#define SCB_ICSR_PENDSVCLR_Pos             27
#define SCB_ICSR_PENDSVCLR_Msk             (1ul << SCB_ICSR_PENDSVCLR_Pos)

#define SCB_ICSR_PENDSTSET_Pos             26
#define SCB_ICSR_PENDSTSET_Msk             (1ul << SCB_ICSR_PENDSTSET_Pos)

#define SCB_ICSR_PENDSTCLR_Pos             25
#define SCB_ICSR_PENDSTCLR_Msk             (1ul << SCB_ICSR_PENDSTCLR_Pos)

#define SCB_ICSR_ISRPREEMPT_Pos            23
#define SCB_ICSR_ISRPREEMPT_Msk            (1ul << SCB_ICSR_ISRPREEMPT_Pos)

#define SCB_ICSR_ISRPENDING_Pos            22
#define SCB_ICSR_ISRPENDING_Msk            (1ul << SCB_ICSR_ISRPENDING_Pos)

#define SCB_ICSR_VECTPENDING_Pos           12
#define SCB_ICSR_VECTPENDING_Msk           (0x1FFul << SCB_ICSR_VECTPENDING_Pos)

#define SCB_ICSR_RETTOBASE_Pos             11
#define SCB_ICSR_RETTOBASE_Msk             (1ul << SCB_ICSR_RETTOBASE_Pos)

#define SCB_ICSR_VECTACTIVE_Pos             0
#define SCB_ICSR_VECTACTIVE_Msk            (0x1FFul << SCB_ICSR_VECTACTIVE_Pos)


#define SCB_VTOR_TBLBASE_Pos               29
#define SCB_VTOR_TBLBASE_Msk               (0x1FFul << SCB_VTOR_TBLBASE_Pos)

#define SCB_VTOR_TBLOFF_Pos                 7
#define SCB_VTOR_TBLOFF_Msk                (0x3FFFFFul << SCB_VTOR_TBLOFF_Pos)


#define SCB_AIRCR_VECTKEY_Pos              16
#define SCB_AIRCR_VECTKEY_Msk              (0xFFFFul << SCB_AIRCR_VECTKEY_Pos)

#define SCB_AIRCR_VECTKEYSTAT_Pos          16
#define SCB_AIRCR_VECTKEYSTAT_Msk          (0xFFFFul << SCB_AIRCR_VECTKEYSTAT_Pos)

#define SCB_AIRCR_ENDIANESS_Pos            15
#define SCB_AIRCR_ENDIANESS_Msk            (1ul << SCB_AIRCR_ENDIANESS_Pos)

#define SCB_AIRCR_PRIGROUP_Pos              8
#define SCB_AIRCR_PRIGROUP_Msk             (7ul << SCB_AIRCR_PRIGROUP_Pos)

#define SCB_AIRCR_SYSRESETREQ_Pos           2
#define SCB_AIRCR_SYSRESETREQ_Msk          (1ul << SCB_AIRCR_SYSRESETREQ_Pos)

#define SCB_AIRCR_VECTCLRACTIVE_Pos         1
#define SCB_AIRCR_VECTCLRACTIVE_Msk        (1ul << SCB_AIRCR_VECTCLRACTIVE_Pos)

#define SCB_AIRCR_VECTRESET_Pos             0
#define SCB_AIRCR_VECTRESET_Msk            (1ul << SCB_AIRCR_VECTRESET_Pos)


#define SCB_SCR_SEVONPEND_Pos               4
#define SCB_SCR_SEVONPEND_Msk              (1ul << SCB_SCR_SEVONPEND_Pos)

#define SCB_SCR_SLEEPDEEP_Pos               2
#define SCB_SCR_SLEEPDEEP_Msk              (1ul << SCB_SCR_SLEEPDEEP_Pos)

#define SCB_SCR_SLEEPONEXIT_Pos             1
#define SCB_SCR_SLEEPONEXIT_Msk            (1ul << SCB_SCR_SLEEPONEXIT_Pos)


#define SCB_CCR_STKALIGN_Pos                9
#define SCB_CCR_STKALIGN_Msk               (1ul << SCB_CCR_STKALIGN_Pos)

#define SCB_CCR_BFHFNMIGN_Pos               8
#define SCB_CCR_BFHFNMIGN_Msk              (1ul << SCB_CCR_BFHFNMIGN_Pos)

#define SCB_CCR_DIV_0_TRP_Pos               4
#define SCB_CCR_DIV_0_TRP_Msk              (1ul << SCB_CCR_DIV_0_TRP_Pos)

#define SCB_CCR_UNALIGN_TRP_Pos             3
#define SCB_CCR_UNALIGN_TRP_Msk            (1ul << SCB_CCR_UNALIGN_TRP_Pos)

#define SCB_CCR_USERSETMPEND_Pos            1
#define SCB_CCR_USERSETMPEND_Msk           (1ul << SCB_CCR_USERSETMPEND_Pos)

#define SCB_CCR_NONBASETHRDENA_Pos          0
#define SCB_CCR_NONBASETHRDENA_Msk         (1ul << SCB_CCR_NONBASETHRDENA_Pos)


#define SCB_SHCSR_USGFAULTENA_Pos          18
#define SCB_SHCSR_USGFAULTENA_Msk          (1ul << SCB_SHCSR_USGFAULTENA_Pos)

#define SCB_SHCSR_BUSFAULTENA_Pos          17
#define SCB_SHCSR_BUSFAULTENA_Msk          (1ul << SCB_SHCSR_BUSFAULTENA_Pos)

#define SCB_SHCSR_MEMFAULTENA_Pos          16
#define SCB_SHCSR_MEMFAULTENA_Msk          (1ul << SCB_SHCSR_MEMFAULTENA_Pos)

#define SCB_SHCSR_SVCALLPENDED_Pos         15
#define SCB_SHCSR_SVCALLPENDED_Msk         (1ul << SCB_SHCSR_SVCALLPENDED_Pos)

#define SCB_SHCSR_BUSFAULTPENDED_Pos       14
#define SCB_SHCSR_BUSFAULTPENDED_Msk       (1ul << SCB_SHCSR_BUSFAULTPENDED_Pos)

#define SCB_SHCSR_MEMFAULTPENDED_Pos       13
#define SCB_SHCSR_MEMFAULTPENDED_Msk       (1ul << SCB_SHCSR_MEMFAULTPENDED_Pos)

#define SCB_SHCSR_USGFAULTPENDED_Pos       12
#define SCB_SHCSR_USGFAULTPENDED_Msk       (1ul << SCB_SHCSR_USGFAULTPENDED_Pos)

#define SCB_SHCSR_SYSTICKACT_Pos           11
#define SCB_SHCSR_SYSTICKACT_Msk           (1ul << SCB_SHCSR_SYSTICKACT_Pos)

#define SCB_SHCSR_PENDSVACT_Pos            10
#define SCB_SHCSR_PENDSVACT_Msk            (1ul << SCB_SHCSR_PENDSVACT_Pos)

#define SCB_SHCSR_MONITORACT_Pos            8
#define SCB_SHCSR_MONITORACT_Msk           (1ul << SCB_SHCSR_MONITORACT_Pos)

#define SCB_SHCSR_SVCALLACT_Pos             7
#define SCB_SHCSR_SVCALLACT_Msk            (1ul << SCB_SHCSR_SVCALLACT_Pos)

#define SCB_SHCSR_USGFAULTACT_Pos           3
#define SCB_SHCSR_USGFAULTACT_Msk          (1ul << SCB_SHCSR_USGFAULTACT_Pos)

#define SCB_SHCSR_BUSFAULTACT_Pos           1
#define SCB_SHCSR_BUSFAULTACT_Msk          (1ul << SCB_SHCSR_BUSFAULTACT_Pos)

#define SCB_SHCSR_MEMFAULTACT_Pos           0
#define SCB_SHCSR_MEMFAULTACT_Msk          (1ul << SCB_SHCSR_MEMFAULTACT_Pos)


#define SCB_CFSR_USGFAULTSR_Pos            16
#define SCB_CFSR_USGFAULTSR_Msk            (0xFFFFul << SCB_CFSR_USGFAULTSR_Pos)

#define SCB_CFSR_BUSFAULTSR_Pos             8
#define SCB_CFSR_BUSFAULTSR_Msk            (0xFFul << SCB_CFSR_BUSFAULTSR_Pos)

#define SCB_CFSR_MEMFAULTSR_Pos             0
#define SCB_CFSR_MEMFAULTSR_Msk            (0xFFul << SCB_CFSR_MEMFAULTSR_Pos)


#define SCB_HFSR_DEBUGEVT_Pos              31
#define SCB_HFSR_DEBUGEVT_Msk              (1ul << SCB_HFSR_DEBUGEVT_Pos)

#define SCB_HFSR_FORCED_Pos                30
#define SCB_HFSR_FORCED_Msk                (1ul << SCB_HFSR_FORCED_Pos)

#define SCB_HFSR_VECTTBL_Pos                1
#define SCB_HFSR_VECTTBL_Msk               (1ul << SCB_HFSR_VECTTBL_Pos)


#define SCB_DFSR_EXTERNAL_Pos               4
#define SCB_DFSR_EXTERNAL_Msk              (1ul << SCB_DFSR_EXTERNAL_Pos)

#define SCB_DFSR_VCATCH_Pos                 3
#define SCB_DFSR_VCATCH_Msk                (1ul << SCB_DFSR_VCATCH_Pos)

#define SCB_DFSR_DWTTRAP_Pos                2
#define SCB_DFSR_DWTTRAP_Msk               (1ul << SCB_DFSR_DWTTRAP_Pos)

#define SCB_DFSR_BKPT_Pos                   1
#define SCB_DFSR_BKPT_Msk                  (1ul << SCB_DFSR_BKPT_Pos)

#define SCB_DFSR_HALTED_Pos                 0
#define SCB_DFSR_HALTED_Msk                (1ul << SCB_DFSR_HALTED_Pos)


typedef struct
{
  __IO uint32_t CTRL;
  __IO uint32_t LOAD;
  __IO uint32_t VAL;
  __I  uint32_t CALIB;
} SysTick_Type;


#define SysTick_CTRL_COUNTFLAG_Pos         16
#define SysTick_CTRL_COUNTFLAG_Msk         (1ul << SysTick_CTRL_COUNTFLAG_Pos)

#define SysTick_CTRL_CLKSOURCE_Pos          2
#define SysTick_CTRL_CLKSOURCE_Msk         (1ul << SysTick_CTRL_CLKSOURCE_Pos)

#define SysTick_CTRL_TICKINT_Pos            1
#define SysTick_CTRL_TICKINT_Msk           (1ul << SysTick_CTRL_TICKINT_Pos)

#define SysTick_CTRL_ENABLE_Pos             0
#define SysTick_CTRL_ENABLE_Msk            (1ul << SysTick_CTRL_ENABLE_Pos)


#define SysTick_LOAD_RELOAD_Pos             0
#define SysTick_LOAD_RELOAD_Msk            (0xFFFFFFul << SysTick_LOAD_RELOAD_Pos)


#define SysTick_VAL_CURRENT_Pos             0
#define SysTick_VAL_CURRENT_Msk            (0xFFFFFFul << SysTick_VAL_CURRENT_Pos)


#define SysTick_CALIB_NOREF_Pos            31
#define SysTick_CALIB_NOREF_Msk            (1ul << SysTick_CALIB_NOREF_Pos)

#define SysTick_CALIB_SKEW_Pos             30
#define SysTick_CALIB_SKEW_Msk             (1ul << SysTick_CALIB_SKEW_Pos)

#define SysTick_CALIB_TENMS_Pos             0
#define SysTick_CALIB_TENMS_Msk            (0xFFFFFFul << SysTick_VAL_CURRENT_Pos)


typedef struct
{
  __O  union
  {
    __O  uint8_t    u8;
    __O  uint16_t   u16;
    __O  uint32_t   u32;
  }  PORT [32];
       uint32_t RESERVED0[864];
  __IO uint32_t TER;
       uint32_t RESERVED1[15];
  __IO uint32_t TPR;
       uint32_t RESERVED2[15];
  __IO uint32_t TCR;
       uint32_t RESERVED3[29];
  __IO uint32_t IWR;
  __IO uint32_t IRR;
  __IO uint32_t IMCR;
       uint32_t RESERVED4[43];
  __IO uint32_t LAR;
  __IO uint32_t LSR;
       uint32_t RESERVED5[6];
  __I  uint32_t PID4;
  __I  uint32_t PID5;
  __I  uint32_t PID6;
  __I  uint32_t PID7;
  __I  uint32_t PID0;
  __I  uint32_t PID1;
  __I  uint32_t PID2;
  __I  uint32_t PID3;
  __I  uint32_t CID0;
  __I  uint32_t CID1;
  __I  uint32_t CID2;
  __I  uint32_t CID3;
} ITM_Type;


#define ITM_TPR_PRIVMASK_Pos                0
#define ITM_TPR_PRIVMASK_Msk               (0xFul << ITM_TPR_PRIVMASK_Pos)


#define ITM_TCR_BUSY_Pos                   23
#define ITM_TCR_BUSY_Msk                   (1ul << ITM_TCR_BUSY_Pos)

#define ITM_TCR_ATBID_Pos                  16
#define ITM_TCR_ATBID_Msk                  (0x7Ful << ITM_TCR_ATBID_Pos)

#define ITM_TCR_TSPrescale_Pos              8
#define ITM_TCR_TSPrescale_Msk             (3ul << ITM_TCR_TSPrescale_Pos)

#define ITM_TCR_SWOENA_Pos                  4
#define ITM_TCR_SWOENA_Msk                 (1ul << ITM_TCR_SWOENA_Pos)

#define ITM_TCR_DWTENA_Pos                  3
#define ITM_TCR_DWTENA_Msk                 (1ul << ITM_TCR_DWTENA_Pos)

#define ITM_TCR_SYNCENA_Pos                 2
#define ITM_TCR_SYNCENA_Msk                (1ul << ITM_TCR_SYNCENA_Pos)

#define ITM_TCR_TSENA_Pos                   1
#define ITM_TCR_TSENA_Msk                  (1ul << ITM_TCR_TSENA_Pos)

#define ITM_TCR_ITMENA_Pos                  0
#define ITM_TCR_ITMENA_Msk                 (1ul << ITM_TCR_ITMENA_Pos)


#define ITM_IWR_ATVALIDM_Pos                0
#define ITM_IWR_ATVALIDM_Msk               (1ul << ITM_IWR_ATVALIDM_Pos)


#define ITM_IRR_ATREADYM_Pos                0
#define ITM_IRR_ATREADYM_Msk               (1ul << ITM_IRR_ATREADYM_Pos)


#define ITM_IMCR_INTEGRATION_Pos            0
#define ITM_IMCR_INTEGRATION_Msk           (1ul << ITM_IMCR_INTEGRATION_Pos)


#define ITM_LSR_ByteAcc_Pos                 2
#define ITM_LSR_ByteAcc_Msk                (1ul << ITM_LSR_ByteAcc_Pos)

#define ITM_LSR_Access_Pos                  1
#define ITM_LSR_Access_Msk                 (1ul << ITM_LSR_Access_Pos)

#define ITM_LSR_Present_Pos                 0
#define ITM_LSR_Present_Msk                (1ul << ITM_LSR_Present_Pos)


typedef struct
{
       uint32_t RESERVED0;
  __I  uint32_t ICTR;
#if ((defined __CM3_REV) && (__CM3_REV >= 0x200))
  __IO uint32_t ACTLR;
#else
       uint32_t RESERVED1;
#endif
} InterruptType_Type;


#define InterruptType_ICTR_INTLINESNUM_Pos  0
#define InterruptType_ICTR_INTLINESNUM_Msk (0x1Ful << InterruptType_ICTR_INTLINESNUM_Pos)


#define InterruptType_ACTLR_DISFOLD_Pos     2
#define InterruptType_ACTLR_DISFOLD_Msk    (1ul << InterruptType_ACTLR_DISFOLD_Pos)

#define InterruptType_ACTLR_DISDEFWBUF_Pos  1
#define InterruptType_ACTLR_DISDEFWBUF_Msk (1ul << InterruptType_ACTLR_DISDEFWBUF_Pos)

#define InterruptType_ACTLR_DISMCYCINT_Pos  0
#define InterruptType_ACTLR_DISMCYCINT_Msk (1ul << InterruptType_ACTLR_DISMCYCINT_Pos)


#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1)


typedef struct
{
  __I  uint32_t TYPE;
  __IO uint32_t CTRL;
  __IO uint32_t RNR;
  __IO uint32_t RBAR;
  __IO uint32_t RASR;
  __IO uint32_t RBAR_A1;
  __IO uint32_t RASR_A1;
  __IO uint32_t RBAR_A2;
  __IO uint32_t RASR_A2;
  __IO uint32_t RBAR_A3;
  __IO uint32_t RASR_A3;
} MPU_Type;


#define MPU_TYPE_IREGION_Pos               16
#define MPU_TYPE_IREGION_Msk               (0xFFul << MPU_TYPE_IREGION_Pos)

#define MPU_TYPE_DREGION_Pos                8
#define MPU_TYPE_DREGION_Msk               (0xFFul << MPU_TYPE_DREGION_Pos)

#define MPU_TYPE_SEPARATE_Pos               0
#define MPU_TYPE_SEPARATE_Msk              (1ul << MPU_TYPE_SEPARATE_Pos)


#define MPU_CTRL_PRIVDEFENA_Pos             2
#define MPU_CTRL_PRIVDEFENA_Msk            (1ul << MPU_CTRL_PRIVDEFENA_Pos)

#define MPU_CTRL_HFNMIENA_Pos               1
#define MPU_CTRL_HFNMIENA_Msk              (1ul << MPU_CTRL_HFNMIENA_Pos)

#define MPU_CTRL_ENABLE_Pos                 0
#define MPU_CTRL_ENABLE_Msk                (1ul << MPU_CTRL_ENABLE_Pos)


#define MPU_RNR_REGION_Pos                  0
#define MPU_RNR_REGION_Msk                 (0xFFul << MPU_RNR_REGION_Pos)


#define MPU_RBAR_ADDR_Pos                   5
#define MPU_RBAR_ADDR_Msk                  (0x7FFFFFFul << MPU_RBAR_ADDR_Pos)

#define MPU_RBAR_VALID_Pos                  4
#define MPU_RBAR_VALID_Msk                 (1ul << MPU_RBAR_VALID_Pos)

#define MPU_RBAR_REGION_Pos                 0
#define MPU_RBAR_REGION_Msk                (0xFul << MPU_RBAR_REGION_Pos)


#define MPU_RASR_XN_Pos                    28
#define MPU_RASR_XN_Msk                    (1ul << MPU_RASR_XN_Pos)

#define MPU_RASR_AP_Pos                    24
#define MPU_RASR_AP_Msk                    (7ul << MPU_RASR_AP_Pos)

#define MPU_RASR_TEX_Pos                   19
#define MPU_RASR_TEX_Msk                   (7ul << MPU_RASR_TEX_Pos)

#define MPU_RASR_S_Pos                     18
#define MPU_RASR_S_Msk                     (1ul << MPU_RASR_S_Pos)

#define MPU_RASR_C_Pos                     17
#define MPU_RASR_C_Msk                     (1ul << MPU_RASR_C_Pos)

#define MPU_RASR_B_Pos                     16
#define MPU_RASR_B_Msk                     (1ul << MPU_RASR_B_Pos)

#define MPU_RASR_SRD_Pos                    8
#define MPU_RASR_SRD_Msk                   (0xFFul << MPU_RASR_SRD_Pos)

#define MPU_RASR_SIZE_Pos                   1
#define MPU_RASR_SIZE_Msk                  (0x1Ful << MPU_RASR_SIZE_Pos)

#define MPU_RASR_ENA_Pos                     0
#define MPU_RASR_ENA_Msk                    (0x1Ful << MPU_RASR_ENA_Pos)


#endif


typedef struct
{
  __IO uint32_t DHCSR;
  __O  uint32_t DCRSR;
  __IO uint32_t DCRDR;
  __IO uint32_t DEMCR;
} CoreDebug_Type;


#define CoreDebug_DHCSR_DBGKEY_Pos         16
#define CoreDebug_DHCSR_DBGKEY_Msk         (0xFFFFul << CoreDebug_DHCSR_DBGKEY_Pos)

#define CoreDebug_DHCSR_S_RESET_ST_Pos     25
#define CoreDebug_DHCSR_S_RESET_ST_Msk     (1ul << CoreDebug_DHCSR_S_RESET_ST_Pos)

#define CoreDebug_DHCSR_S_RETIRE_ST_Pos    24
#define CoreDebug_DHCSR_S_RETIRE_ST_Msk    (1ul << CoreDebug_DHCSR_S_RETIRE_ST_Pos)

#define CoreDebug_DHCSR_S_LOCKUP_Pos       19
#define CoreDebug_DHCSR_S_LOCKUP_Msk       (1ul << CoreDebug_DHCSR_S_LOCKUP_Pos)

#define CoreDebug_DHCSR_S_SLEEP_Pos        18
#define CoreDebug_DHCSR_S_SLEEP_Msk        (1ul << CoreDebug_DHCSR_S_SLEEP_Pos)

#define CoreDebug_DHCSR_S_HALT_Pos         17
#define CoreDebug_DHCSR_S_HALT_Msk         (1ul << CoreDebug_DHCSR_S_HALT_Pos)

#define CoreDebug_DHCSR_S_REGRDY_Pos       16
#define CoreDebug_DHCSR_S_REGRDY_Msk       (1ul << CoreDebug_DHCSR_S_REGRDY_Pos)

#define CoreDebug_DHCSR_C_SNAPSTALL_Pos     5
#define CoreDebug_DHCSR_C_SNAPSTALL_Msk    (1ul << CoreDebug_DHCSR_C_SNAPSTALL_Pos)

#define CoreDebug_DHCSR_C_MASKINTS_Pos      3
#define CoreDebug_DHCSR_C_MASKINTS_Msk     (1ul << CoreDebug_DHCSR_C_MASKINTS_Pos)

#define CoreDebug_DHCSR_C_STEP_Pos          2
#define CoreDebug_DHCSR_C_STEP_Msk         (1ul << CoreDebug_DHCSR_C_STEP_Pos)

#define CoreDebug_DHCSR_C_HALT_Pos          1
#define CoreDebug_DHCSR_C_HALT_Msk         (1ul << CoreDebug_DHCSR_C_HALT_Pos)

#define CoreDebug_DHCSR_C_DEBUGEN_Pos       0
#define CoreDebug_DHCSR_C_DEBUGEN_Msk      (1ul << CoreDebug_DHCSR_C_DEBUGEN_Pos)


#define CoreDebug_DCRSR_REGWnR_Pos         16
#define CoreDebug_DCRSR_REGWnR_Msk         (1ul << CoreDebug_DCRSR_REGWnR_Pos)

#define CoreDebug_DCRSR_REGSEL_Pos          0
#define CoreDebug_DCRSR_REGSEL_Msk         (0x1Ful << CoreDebug_DCRSR_REGSEL_Pos)


#define CoreDebug_DEMCR_TRCENA_Pos         24
#define CoreDebug_DEMCR_TRCENA_Msk         (1ul << CoreDebug_DEMCR_TRCENA_Pos)

#define CoreDebug_DEMCR_MON_REQ_Pos        19
#define CoreDebug_DEMCR_MON_REQ_Msk        (1ul << CoreDebug_DEMCR_MON_REQ_Pos)

#define CoreDebug_DEMCR_MON_STEP_Pos       18
#define CoreDebug_DEMCR_MON_STEP_Msk       (1ul << CoreDebug_DEMCR_MON_STEP_Pos)

#define CoreDebug_DEMCR_MON_PEND_Pos       17
#define CoreDebug_DEMCR_MON_PEND_Msk       (1ul << CoreDebug_DEMCR_MON_PEND_Pos)

#define CoreDebug_DEMCR_MON_EN_Pos         16
#define CoreDebug_DEMCR_MON_EN_Msk         (1ul << CoreDebug_DEMCR_MON_EN_Pos)

#define CoreDebug_DEMCR_VC_HARDERR_Pos     10
#define CoreDebug_DEMCR_VC_HARDERR_Msk     (1ul << CoreDebug_DEMCR_VC_HARDERR_Pos)

#define CoreDebug_DEMCR_VC_INTERR_Pos       9
#define CoreDebug_DEMCR_VC_INTERR_Msk      (1ul << CoreDebug_DEMCR_VC_INTERR_Pos)

#define CoreDebug_DEMCR_VC_BUSERR_Pos       8
#define CoreDebug_DEMCR_VC_BUSERR_Msk      (1ul << CoreDebug_DEMCR_VC_BUSERR_Pos)

#define CoreDebug_DEMCR_VC_STATERR_Pos      7
#define CoreDebug_DEMCR_VC_STATERR_Msk     (1ul << CoreDebug_DEMCR_VC_STATERR_Pos)

#define CoreDebug_DEMCR_VC_CHKERR_Pos       6
#define CoreDebug_DEMCR_VC_CHKERR_Msk      (1ul << CoreDebug_DEMCR_VC_CHKERR_Pos)

#define CoreDebug_DEMCR_VC_NOCPERR_Pos      5
#define CoreDebug_DEMCR_VC_NOCPERR_Msk     (1ul << CoreDebug_DEMCR_VC_NOCPERR_Pos)

#define CoreDebug_DEMCR_VC_MMERR_Pos        4
#define CoreDebug_DEMCR_VC_MMERR_Msk       (1ul << CoreDebug_DEMCR_VC_MMERR_Pos)

#define CoreDebug_DEMCR_VC_CORERESET_Pos    0
#define CoreDebug_DEMCR_VC_CORERESET_Msk   (1ul << CoreDebug_DEMCR_VC_CORERESET_Pos)


#define SCS_BASE            (0xE000E000)
#define ITM_BASE            (0xE0000000)
#define CoreDebug_BASE      (0xE000EDF0)
#define SysTick_BASE        (SCS_BASE +  0x0010)
#define NVIC_BASE           (SCS_BASE +  0x0100)
#define SCB_BASE            (SCS_BASE +  0x0D00)

#define InterruptType       ((InterruptType_Type *) SCS_BASE)
#define SCB                 ((SCB_Type *)           SCB_BASE)
#define SysTick             ((SysTick_Type *)       SysTick_BASE)
#define NVIC                ((NVIC_Type *)          NVIC_BASE)
#define ITM                 ((ITM_Type *)           ITM_BASE)
#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)

#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1)
  #define MPU_BASE          (SCS_BASE +  0x0D90)
  #define MPU               ((MPU_Type*)            MPU_BASE)
#endif


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


#define __enable_fault_irq                __enable_fiq
#define __disable_fault_irq               __disable_fiq

#define __NOP                             __nop
#define __WFI                             __wfi
#define __WFE                             __wfe
#define __SEV                             __sev
#define __ISB()                           __isb(0)
#define __DSB()                           __dsb(0)
#define __DMB()                           __dmb(0)
#define __REV                             __rev
#define __RBIT                            __rbit
#define __LDREXB(ptr)                     ((unsigned char ) __ldrex(ptr))
#define __LDREXH(ptr)                     ((unsigned short) __ldrex(ptr))
#define __LDREXW(ptr)                     ((unsigned int  ) __ldrex(ptr))
#define __STREXB(value, ptr)              __strex(value, ptr)
#define __STREXH(value, ptr)              __strex(value, ptr)
#define __STREXW(value, ptr)              __strex(value, ptr)


extern uint32_t __get_PSP(void);


extern void __set_PSP(uint32_t topOfProcStack);


extern uint32_t __get_MSP(void);


extern void __set_MSP(uint32_t topOfMainStack);


extern uint32_t __REV16(uint16_t value);


extern int32_t __REVSH(int16_t value);


#if (__ARMCC_VERSION < 400000)


extern void __CLREX(void);


extern uint32_t __get_BASEPRI(void);


extern void __set_BASEPRI(uint32_t basePri);


extern uint32_t __get_PRIMASK(void);


extern void __set_PRIMASK(uint32_t priMask);


extern uint32_t __get_FAULTMASK(void);


extern void __set_FAULTMASK(uint32_t faultMask);


extern uint32_t __get_CONTROL(void);


extern void __set_CONTROL(uint32_t control);

#else


#define __CLREX                           __clrex


static __INLINE uint32_t  __get_BASEPRI(void)
{
  register uint32_t __regBasePri         __ASM("basepri");
  return(__regBasePri);
}


static __INLINE void __set_BASEPRI(uint32_t basePri)
{
  register uint32_t __regBasePri         __ASM("basepri");
  __regBasePri = (basePri & 0xff);
}


static __INLINE uint32_t __get_PRIMASK(void)
{
  register uint32_t __regPriMask         __ASM("primask");
  return(__regPriMask);
}


static __INLINE void __set_PRIMASK(uint32_t priMask)
{
  register uint32_t __regPriMask         __ASM("primask");
  __regPriMask = (priMask);
}


static __INLINE uint32_t __get_FAULTMASK(void)
{
  register uint32_t __regFaultMask       __ASM("faultmask");
  return(__regFaultMask);
}


static __INLINE void __set_FAULTMASK(uint32_t faultMask)
{
  register uint32_t __regFaultMask       __ASM("faultmask");
  __regFaultMask = (faultMask & 1);
}


static __INLINE uint32_t __get_CONTROL(void)
{
  register uint32_t __regControl         __ASM("control");
  return(__regControl);
}


static __INLINE void __set_CONTROL(uint32_t control)
{
  register uint32_t __regControl         __ASM("control");
  __regControl = control;
}

#endif


#elif (defined (__ICCARM__))


#define __enable_irq                              __enable_interrupt
#define __disable_irq                             __disable_interrupt

static __INLINE void __enable_fault_irq()         { __ASM ("cpsie f"); }
static __INLINE void __disable_fault_irq()        { __ASM ("cpsid f"); }

#define __NOP                                     __no_operation
static __INLINE  void __WFI()                     { __ASM ("wfi"); }
static __INLINE  void __WFE()                     { __ASM ("wfe"); }
static __INLINE  void __SEV()                     { __ASM ("sev"); }
static __INLINE  void __CLREX()                   { __ASM ("clrex"); }


extern uint32_t __get_PSP(void);


extern void __set_PSP(uint32_t topOfProcStack);


extern uint32_t __get_MSP(void);


extern void __set_MSP(uint32_t topOfMainStack);


extern uint32_t __REV16(uint16_t value);


extern uint32_t __RBIT(uint32_t value);


extern uint8_t __LDREXB(uint8_t *addr);


extern uint16_t __LDREXH(uint16_t *addr);


extern uint32_t __LDREXW(uint32_t *addr);


extern uint32_t __STREXB(uint8_t value, uint8_t *addr);


extern uint32_t __STREXH(uint16_t value, uint16_t *addr);


extern uint32_t __STREXW(uint32_t value, uint32_t *addr);


#elif (defined (__GNUC__))


static __INLINE void __enable_irq()               { __ASM volatile ("cpsie i"); }
static __INLINE void __disable_irq()              { __ASM volatile ("cpsid i"); }

static __INLINE void __enable_fault_irq()         { __ASM volatile ("cpsie f"); }
static __INLINE void __disable_fault_irq()        { __ASM volatile ("cpsid f"); }

static __INLINE void __NOP()                      { __ASM volatile ("nop"); }
static __INLINE void __WFI()                      { __ASM volatile ("wfi"); }
static __INLINE void __WFE()                      { __ASM volatile ("wfe"); }
static __INLINE void __SEV()                      { __ASM volatile ("sev"); }
static __INLINE void __ISB()                      { __ASM volatile ("isb"); }
static __INLINE void __DSB()                      { __ASM volatile ("dsb"); }
static __INLINE void __DMB()                      { __ASM volatile ("dmb"); }
static __INLINE void __CLREX()                    { __ASM volatile ("clrex"); }


extern uint32_t __get_PSP(void);


extern void __set_PSP(uint32_t topOfProcStack);


extern uint32_t __get_MSP(void);


extern void __set_MSP(uint32_t topOfMainStack);


extern uint32_t __get_BASEPRI(void);


extern void __set_BASEPRI(uint32_t basePri);


extern uint32_t  __get_PRIMASK(void);


extern void __set_PRIMASK(uint32_t priMask);


extern uint32_t __get_FAULTMASK(void);


extern void __set_FAULTMASK(uint32_t faultMask);


extern uint32_t __get_CONTROL(void);


extern void __set_CONTROL(uint32_t control);


extern uint32_t __REV(uint32_t value);


extern uint32_t __REV16(uint16_t value);


extern int32_t __REVSH(int16_t value);


extern uint32_t __RBIT(uint32_t value);


extern uint8_t __LDREXB(uint8_t *addr);


extern uint16_t __LDREXH(uint16_t *addr);


extern uint32_t __LDREXW(uint32_t *addr);


extern uint32_t __STREXB(uint8_t value, uint8_t *addr);


extern uint32_t __STREXH(uint16_t value, uint16_t *addr);


extern uint32_t __STREXW(uint32_t value, uint32_t *addr);


#elif (defined (__TASKING__))


#endif


static __INLINE void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);

  reg_value  =  SCB->AIRCR;
  reg_value &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);
  reg_value  =  (reg_value                       |
                (0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << 8));
  SCB->AIRCR =  reg_value;
}


static __INLINE uint32_t NVIC_GetPriorityGrouping(void)
{
  return ((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos);
}


static __INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}


static __INLINE void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}


static __INLINE uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((NVIC->ISPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}


static __INLINE void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  NVIC->ISPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}


static __INLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}


static __INLINE uint32_t NVIC_GetActive(IRQn_Type IRQn)
{
  return((uint32_t)((NVIC->IABR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}


static __INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if(IRQn < 0) {
    SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); }
  else {
    NVIC->IP[(uint32_t)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);    }
}


static __INLINE uint32_t NVIC_GetPriority(IRQn_Type IRQn)
{

  if(IRQn < 0) {
    return((uint32_t)(SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] >> (8 - __NVIC_PRIO_BITS)));  }
  else {
    return((uint32_t)(NVIC->IP[(uint32_t)(IRQn)]           >> (8 - __NVIC_PRIO_BITS)));  }
}


static __INLINE uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

  return (
           ((PreemptPriority & ((1 << (PreemptPriorityBits)) - 1)) << SubPriorityBits) |
           ((SubPriority     & ((1 << (SubPriorityBits    )) - 1)))
         );
}


static __INLINE void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

  *pPreemptPriority = (Priority >> SubPriorityBits) & ((1 << (PreemptPriorityBits)) - 1);
  *pSubPriority     = (Priority                   ) & ((1 << (SubPriorityBits    )) - 1);
}


#if (!defined (__Vendor_SysTickConfig)) || (__Vendor_SysTickConfig == 0)


static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);

  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
  SysTick->VAL   = 0;
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;
  return (0);
}

#endif


static __INLINE void NVIC_SystemReset(void)
{
  SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos)      |
                 (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                 SCB_AIRCR_SYSRESETREQ_Msk);
  __DSB();
  while(1);
}


extern volatile int ITM_RxBuffer;
#define             ITM_RXBUFFER_EMPTY    0x5AA55AA5


static __INLINE uint32_t ITM_SendChar (uint32_t ch)
{
  if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)  &&
      (ITM->TCR & ITM_TCR_ITMENA_Msk)                  &&
      (ITM->TER & (1ul << 0)        )                    )
  {
    while (ITM->PORT[0].u32 == 0);
    ITM->PORT[0].u8 = (uint8_t) ch;
  }
  return (ch);
}


static __INLINE int ITM_ReceiveChar (void) {
  int ch = -1;

  if (ITM_RxBuffer != ITM_RXBUFFER_EMPTY) {
    ch = ITM_RxBuffer;
    ITM_RxBuffer = ITM_RXBUFFER_EMPTY;
  }

  return (ch);
}


static __INLINE int ITM_CheckChar (void) {

  if (ITM_RxBuffer == ITM_RXBUFFER_EMPTY) {
    return (0);
  } else {
    return (1);
  }
}


#ifdef __cplusplus
}
#endif


#endif
