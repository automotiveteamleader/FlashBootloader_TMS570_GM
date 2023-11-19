/****************************************************************************
| Project Name: Flash Bootloader
|    File Name: memmap.h
|
|  Description: Section definition to locate data or code in memory
|               This file contains compiler specific section definition
|               for every segment that can be mapped within the bootloader
|               to a specific location.
|
|-----------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------
| Copyright (c) 2012 by Vector Informatik GmbH, all rights reserved
|
| Please note, that this file contains a collection of definitions
| to be used with the Flash Bootloader. These functions may
| influence the behaviour of the bootloader in principle. Therefore,
| great care must be taken to verify the correctness of the
| implementation.
|
| The contents of the originally delivered files are only examples resp.
| implementation proposals. With regard to the fact that these definitions
| are meant for demonstration purposes only, Vector Informatik´s
| liability shall be expressly excluded in cases of ordinary negligence,
| to the extent admissible by law or statute.
|-----------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------
| Initials      Name                   Company
| --------      --------------------   ---------------------------
|  AWh          Andreas Wenckebach     Vector Informatik GmbH
|-----------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------
| Date        Ver      Author   Description
| ---------   -------  ------   ------------------------------------
| 2012-12-11  1.00.00  AWh      First release
| 2014-07-17  1.01.00  AWh      Remove COMMENT-switches
|*****************************************************************************/


/* Decid whether this need to be enabled or not */
#define FBL_ENABLE_COPY_CODE_TO_RAM_ON_STARTUP

/**********************************************************/
/*---- Locate Shared Ram Fbl/Application              ----*/
/*---- NOTE: This must be located in uninitialized RAM ---*/
/**********************************************************/

/* ------------------------------------------------------ */
/* Info:     The CanInitTable can be mapped to the same   */
/*           location in application and Boot. Thus, data */
/*           don't need to be copied when transit from    */
/*           application to the boot.                     */
/*           This also avoids bootloader code executed    */
/*           aplication stack.                            */
/* ------------------------------------------------------ */
#if defined (CANPARAM_START_SEC_DATA) || \
    defined (CANPARAM_START_SEC_DATA_EXPORT)
#undef CANPARAM_START_SEC_DATA
#undef CANPARAM_START_SEC_DATA_EXPORT
/* Insert memory section definition here   */
# if defined (C_COMP_IAR_12)
#  pragma memory = no_init
# elif defined (C_COMP_IAR_CR16)
#  pragma dataseg=CANINITTABLE
__no_init
# elif defined (C_COMP_COSMIC_MCS12X_MSCAN12)
#  pragma section [CANINITTABLE]
# elif defined (C_COMP_FUJITSU_FR60_CCAN)
#  pragma  segment DATA = CANINITTABLE
# elif defined (C_COMP_MTRWRKS_MPC5X00)
#  pragma push
#  pragma section data_type "CANINITTABLE" "CANINITTABLE"
__declspec(section "CANINITTABLE")
# elif defined (C_COMP_HIWARE_12)
#  pragma DATA_SEG CANINITTABLE
# elif defined (C_COMP_NEC_V85X)
#  pragma section data "CANINITTABLE" begin
# elif defined (C_COMP_GHS_V85X) || \
       defined (C_COMP_GHS_470)
#  pragma ghs section bss=".CANINITTABLE"
#  pragma ghs startdata
# elif defined (C_COMP_HEW_SH70XX_HCAN2)   || \
       defined (C_COMP_HEW_SH705X)         || \
       defined (C_COMP_HEW_H8S)            || \
       defined (C_COMP_GNU_TRICORE_MULTICAN) ||\
       defined (C_COMP_HEW_SH2_RCAN)
#  pragma section CANINITTABLE
# endif /* C_COMP_IAR_12 */
#endif /* CANPARAM_START_SEC_DATA */

#if defined (CANPARAM_STOP_SEC_DATA) || \
    defined (CANPARAM_STOP_SEC_DATA_EXPORT)
#undef CANPARAM_STOP_SEC_DATA
#undef CANPARAM_STOP_SEC_DATA_EXPORT
/* Insert memory section definition here   */

# if defined (C_COMP_IAR_12)
# pragma memory = default
# elif defined (C_COMP_IAR_CR16)
#  pragma dataseg=default
# elif defined (C_COMP_COSMIC_MCS12X_MSCAN12)
#  pragma section []
# elif defined (C_COMP_FUJITSU_FR60_CCAN)
#  pragma  segment DATA
# elif defined (C_COMP_MTRWRKS_MPC5X00)
#   pragma pop
# elif defined (C_COMP_HIWARE_12)
#  pragma DATA_SEG DEFAULT
# elif defined (C_COMP_NEC_V85X)
#  pragma section data "CANINITTABLE" end
# elif defined (C_COMP_GHS_V85X) || \
       defined (C_COMP_GHS_TMS470)
#  pragma ghs enddata
#  pragma ghs section bss=default
# elif defined (C_COMP_HEW_SH70XX_HCAN2)   || \
       defined (C_COMP_HEW_SH705X)         || \
       defined (C_COMP_HEW_H8S)            || \
       defined (C_COMP_GNU_TRICORE_MULTICAN) ||\
       defined (C_COMP_HEW_SH2_RCAN)
#  pragma section
# endif /* C_COMP_IAR_12 */
#endif /* CANPARAM_STOP_SEC_DATA */

#if defined (MAGICFLAG_START_SEC_DATA) || \
    defined (MAGICFLAG_START_SEC_DATA_EXPORT)
#undef MAGICFLAG_START_SEC_DATA
#undef MAGICFLAG_START_SEC_DATA_EXPORT
/* Insert memory section definition here   */

# if defined (C_COMP_IAR_12)
#  pragma memory = no_init
# elif defined (C_COMP_IAR_CR16)
#  pragma dataseg=MAGICFLAG
__no_init
# elif defined (C_COMP_COSMIC_MCS12X_MSCAN12)
#  pragma section [MAGICFLAG]
# elif defined (C_COMP_FUJITSU_FR60_CCAN)
#  pragma  segment DATA = MAGICFLAG
# elif defined (C_COMP_MTRWRKS_MPC5X00)
#  pragma push
#  pragma section data_type "MAGICFLAG" "MAGICFLAG"
__declspec(section "MAGICFLAG")
# elif defined (C_COMP_HIWARE_12)
#  pragma DATA_SEG MAGICFLAG
# elif defined (C_COMP_NEC_V85X)
#  pragma section data "MAGICFLAG" begin
# elif defined (C_COMP_GHS_V85X) || \
       defined (C_COMP_GHS_470)
#  pragma ghs section bss=".MAGICFLAG"
#  pragma ghs startdata
# elif defined (C_COMP_HEW_SH70XX_HCAN2)   || \
       defined (C_COMP_HEW_SH705X)         || \
       defined (C_COMP_HEW_H8S)            || \
       defined (C_COMP_GNU_TRICORE_MULTICAN) ||\
       defined (C_COMP_HEW_SH2_RCAN)
#   pragma section MAGICFLAG
# endif   /* C_COMP_IAR_12 */
#endif /* MAGICFLAG_START_SEC_DATA */

#if defined (MAGICFLAG_STOP_SEC_DATA) || \
    defined (MAGICFLAG_STOP_SEC_DATA_EXPORT)
#undef MAGICFLAG_STOP_SEC_DATA
#undef MAGICFLAG_STOP_SEC_DATA_EXPORT
/* Insert memory section definition here   */
# if   defined (C_COMP_IAR_12)
#  pragma memory = default
# elif defined (C_COMP_IAR_CR16)
#  pragma dataseg=default
# elif defined (C_COMP_COSMIC_MCS12X_MSCAN12)
#  pragma section []
# elif defined (C_COMP_FUJITSU_FR60_CCAN)
#  pragma  segment DATA
# elif defined (C_COMP_MTRWRKS_MPC5X00)
#  pragma pop
# elif defined (C_COMP_HIWARE_12)
#  pragma DATA_SEG DEFAULT
# elif defined (C_COMP_NEC_V85X)
#  pragma section data "MAGICFLAG" end
# elif defined (C_COMP_GHS_V85X) || \
       defined (C_COMP_GHS_470)
#  pragma ghs enddata
#  pragma ghs section bss=default
# elif defined (C_COMP_HEW_SH70XX_HCAN2)   || \
       defined (C_COMP_HEW_SH705X)         || \
       defined (C_COMP_HEW_H8S)            || \
       defined (C_COMP_GNU_TRICORE_MULTICAN) ||\
       defined (C_COMP_HEW_SH2_RCAN)
#  pragma section
# endif /* C_COMP_IAR_12 */
#endif /* MAGICFLAG_STOP_SEC_DATA */

/**********************************************************/
/*---- Locate FblHeader                               ----*/
/**********************************************************/
#if defined (FBLHEADER_START_SEC_CONST) || \
    defined (FBLHEADER_START_SEC_CONST_EXPORT)
#undef FBLHEADER_START_SEC_CONST
#undef FBLHEADER_START_SEC_CONST_EXPORT
/* Insert memory section definition here   */


# if   defined (C_COMP_IAR_12)
#  pragma memory=constseg(FBLHEADER)
# elif defined (C_COMP_IAR_V85X)
#  pragma constseg=FBLHEADER
# elif defined (C_COMP_IAR_CR16)
#  pragma constseg=FBLHEADER
# elif defined (C_COMP_COSMIC_08)          || \
       defined (C_COMP_COSMIC_12)          || \
       defined (C_COMP_COSMIC_MCS08_MSCAN) || \
       defined (C_COMP_COSMIC_MCS12X_MSCAN12)
#  pragma section const {FBLHEADER}
# elif defined (C_COMP_FUJITSU_FR60_CCAN)
#  pragma segment CONST=FBLHEADER
# elif defined (C_COMP_KEIL_ST10_CCAN)
#  pragma renameclass(FCONST = FBLHEADER)
# elif defined (C_COMP_HIWARE_12)          || \
       defined (C_COMP_HIWARE_08)          || \
       defined (C_COMP_MTRWRKS_MCS08_MSCAN)
#  pragma CONST_SEG FBLHEADER
# elif defined (C_COMP_IAR_M16C)
#  pragma memory=constseg(CONST_DATA):far
# elif defined (C_COMP_TI_470)
#  pragma DATA_SECTION (FblHeader, ".start")
# elif defined (C_COMP_ARM_470)
#  pragma arm section rodata="FBLHEADER"
# elif defined (C_COMP_TI_320)
#  pragma DATA_SECTION (FblHeader, "FBLSTART_SEG")
# elif defined (C_COMP_HEW_SH70XX_HCAN2)   || \
       defined (C_COMP_HEW_SH705X)         || \
       defined (C_COMP_HEW_H8S)            || \
       defined (C_COMP_GNU_TRICORE_MULTICAN) ||\
       defined (C_COMP_HEW_SH2_RCAN)
#  pragma section FBLHEADER
# elif defined (C_COMP_TASKING_XC16X)      || \
       defined (C_COMP_TASKING_C16X)       || \
       defined (C_COMP_TASKING_ST10_CCAN)
#  pragma class nc=FBLSTART_SEG
# elif defined (C_COMP_NEC_V85X)
#  pragma section sconst "FBLHEADER" begin
# elif defined (C_COMP_FUJITSU_16LX)
#  pragma segment FAR_CONST=FBLHEADER
# elif defined (C_COMP_METROWERKS_PPC) || \
       defined (C_COMP_METROWERKS_MPC55XX_FLEXCAN)
#  pragma force_active on
#  pragma push
#  pragma section ".FBLHEADER"
__declspec(section ".FBLHEADER")
# elif defined (C_COMP_DIABDATA_PPC)
#  pragma section FBLHEADER ".FBLHEADER"
#  pragma use_section FBLHEADER FblHeader
# elif defined (C_COMP_MITSUBISHI_M32R)
#  pragma SECTION C FBLSTART
# elif defined (C_COMP_MITSUBISHI_M32C) || \
       defined (C_COMP_MITSUBISHI_M16C)
#  pragma SECTION rom FBLHEADER
# elif defined (C_COMP_IAR_78K0)
#  if (__VER__ < 400)
#   pragma memory=constseg(FBLSTART_SEG)
#  else
#   pragma constseg=FBLSTART_SEG
#  endif
# elif defined (C_COMP_GHS_V85X)
#  pragma ghs section rodata=".FBLHEADER"
# elif defined (C_COMP_TOSHIBA_TLC900)
#  pragma section const FBLHEADER
# elif defined (C_COMP_GHS_MPC55XX)
#  pragma ghs startdata
#  pragma ghs section rodata="FBLHEADER"
# endif /* C_COMP_IAR_12 */
#endif /* FBLHEADER_START_SEC_CONST */

#if defined (FBLHEADER_STOP_SEC_CONST) || \
    defined (FBLHEADER_STOP_SEC_CONST_EXPORT)
#undef FBLHEADER_STOP_SEC_CONST
#undef FBLHEADER_STOP_SEC_CONST_EXPORT
/* Insert memory section definition here   */

# if defined (C_COMP_FUJITSU_16LX)
#  pragma segment FAR_CONST
# elif defined (C_COMP_FUJITSU_FR60_CCAN)
#  pragma segment CONST
# elif defined (C_COMP_NEC_V85X)
#  pragma section sconst "FBLHEADER" end
# elif defined (C_COMP_COSMIC_08)            || \
       defined (C_COMP_COSMIC_12)            || \
       defined (C_COMP_COSMIC_MCS08_MSCAN)    || \
       defined (C_COMP_COSMIC_MCS12X_MSCAN12)
#  pragma section const {}
# elif defined (C_COMP_HIWARE_12)            || \
       defined (C_COMP_HIWARE_08)            || \
       defined (C_COMP_MTRWRKS_MCS08_MSCAN)
#  pragma CONST_SEG DEFAULT
# elif defined (C_COMP_IAR_M16C)
#  pragma memory=default
# elif defined (C_COMP_HEW_SH70XX_HCAN2)     || \
       defined (C_COMP_HEW_SH705X)           || \
       defined (C_COMP_HEW_H8S)              || \
       defined (C_COMP_GNU_TRICORE_MULTICAN) ||\
       defined (C_COMP_HEW_SH2_RCAN)
#  pragma section
# elif defined (C_COMP_TASKING_XC16X)        || \
       defined (C_COMP_TASKING_C16X)         || \
       defined (C_COMP_TASKING_ST10_CCAN)
#  pragma default_attributes
# elif defined (C_COMP_ARM_470)
#  pragma arm section
# elif defined (C_COMP_METROWERKS_PPC) || \
       defined (C_COMP_METROWERKS_MPC55XX_FLEXCAN)
#  pragma pop
#  pragma force_active off
# elif defined (C_COMP_DIABDATA_PPC)
#  pragma use_section CODE
# elif defined (C_COMP_MITSUBISHI_M32R)
#  pragma SECTION C C
# elif defined (C_COMP_IAR_78K0)
#  if (__VER__ < 400)
#   pragma memory=default
#  else
#   pragma constseg=default
#  endif
# elif defined (C_COMP_IAR_12)
#  pragma memory=default
# elif defined (C_COMP_IAR_V85X) || \
       defined (C_COMP_IAR_CR16)
#  pragma constseg=default
# elif defined (C_COMP_GHS_V85X)
#  pragma ghs section rodata=default
# elif defined (C_COMP_TOSHIBA_TLC900)
#  pragma section const
# elif defined (C_COMP_GHS_MPC55XX)
#  pragma ghs section rodata=default
#  pragma ghs enddata
# endif /* C_COMP_FUJITSU_16LX */
#endif /* FBLHEADER_STOP_SEC_CONST */

/**********************************************************/
/*---- Locate watchdog routines                       ----*/
/**********************************************************/
#if defined (WDTRIGGER_START_SEC_CODE) || \
    defined (WDTRIGGER_START_SEC_CODE_EXPORT)
#undef WDTRIGGER_START_SEC_CODE
#undef WDTRIGGER_START_SEC_CODE_EXPORT
# if defined (C_COMP_GHS_MPC55XX)
#  pragma ghs section text=".TRIGText"
# endif
#endif /* WDTRIGGER_START_SEC_CODE */

#if defined (WDTRIGGER_STOP_SEC_CODE) || \
    defined (WDTRIGGER_STOP_SEC_CODE_EXPORT)
#undef WDTRIGGER_STOP_SEC_CODE
#undef WDTRIGGER_STOP_SEC_CODE_EXPORT
# if defined (C_COMP_GHS_MPC55XX)
#  pragma ghs section text=default
# endif
#endif /* WDTRIGGER_STOP_SEC_CODE */

#if defined (LOOKFORWATCHDOG_START_SEC_CODE) || \
    defined (LOOKFORWATCHDOG_START_SEC_CODE_EXPORT)
#undef LOOKFORWATCHDOG_START_SEC_CODE
#undef LOOKFORWATCHDOG_START_SEC_CODE_EXPORT
/* Insert memory section definition here   */
# if defined (C_COMP_GHS_MPC55XX)
#  pragma ghs callmode=far
#  pragma ghs section text=".WDText"
# endif
#endif /* LOOKFORWATCHDOG_START_SEC_CODE */

#if defined (LOOKFORWATCHDOG_STOP_SEC_CODE) || \
    defined (LOOKFORWATCHDOG_STOP_SEC_CODE_EXPORT)
#undef LOOKFORWATCHDOG_STOP_SEC_CODE
#undef LOOKFORWATCHDOG_STOP_SEC_CODE_EXPORT
/* Insert memory section definition here   */
# if defined (C_COMP_GHS_MPC55XX)
#  pragma ghs callmode=default
#  pragma ghs section text=default
# endif
#endif /* LOOKFORWATCHDOG_STOP_SEC_CODE */

#if defined (FBL_ENABLE_COPY_CODE_TO_RAM_ON_STARTUP)
#else
#if defined (INITWATCHDOG_START_SEC_CODE) || \
    defined (INITWATCHDOG_START_SEC_CODE_EXPORT)
#undef INITWATCHDOG_START_SEC_CODE
#undef INITWATCHDOG_START_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#endif /* INITWATCHDOG_START_SEC_CODE */

#if defined (INITWATCHDOG_STOP_SEC_CODE) || \
    defined (INITWATCHDOG_STOP_SEC_CODE_EXPORT)
#undef INITWATCHDOG_STOP_SEC_CODE
#undef INITWATCHDOG_STOP_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#endif /* INITWATCHDOG_STOP_SEC_CODE */

#if defined (WDTRIGGEREND_START_SEC_CODE) || \
    defined (WDTRIGGEREND_START_SEC_CODE_EXPORT)
#undef WDTRIGGEREND_START_SEC_CODE
#undef WDTRIGGEREND_START_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#endif /* WDTRIGGEREND_START_SEC_CODE */

#if defined (WDTRIGGEREND_STOP_SEC_CODE) || \
    defined (WDTRIGGEREND_STOP_SEC_CODE_EXPORT)
#undef WDTRIGGEREND_STOP_SEC_CODE
#undef WDTRIGGEREND_STOP_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#endif /* WDTRIGGEREND_STOP_SEC_CODE */

#if defined (LOOKFORWATCHDOGEND_START_SEC_CODE) || \
    defined (LOOKFORWATCHDOGEND_START_SEC_CODE_EXPORT)
#undef LOOKFORWATCHDOGEND_START_SEC_CODE
#undef LOOKFORWATCHDOGEND_START_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#endif /* LOOKFORWATCHDOGEND_START_SEC_CODE */

#if defined (LOOKFORWATCHDOGEND_STOP_SEC_CODE) || \
    defined (LOOKFORWATCHDOGEND_STOP_SEC_CODE_EXPORT)
#undef LOOKFORWATCHDOGEND_STOP_SEC_CODE
#undef LOOKFORWATCHDOGEND_STOP_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#endif /* LOOKFORWATCHDOGEND_STOP_SEC_CODE */
#endif /* FBL_ENABLE_COPY_CODE_TO_RAM_ON_STARTUP */

/**********************************************************/
/*---- Locate flashCode Buffer                        ----*/
/**********************************************************/
#if defined (LBT_START_SEC_FLASHCODE) || \
    defined (LBT_START_SEC_FLASHCODE_EXPORT)
#undef LBT_START_SEC_FLASHCODE
#undef LBT_START_SEC_FLASHCODE_EXPORT
/* Insert memory section definition here   */
# if defined (C_COMP_HEW_SH2_RCAN)
#  pragma section FLASHDRV
# endif /* C_COMP_HEW_SH2_RCAN */
#endif /* LBT_START_SEC_FLASHCODE */

#if defined (LBT_STOP_SEC_FLASHCODE) || \
    defined (LBT_STOP_SEC_FLASHCODE_EXPORT)
#undef LBT_STOP_SEC_FLASHCODE
#undef LBT_STOP_SEC_FLASHCODE_EXPORT
/* Insert memory section definition here   */
# if defined (C_COMP_HEW_SH2_RCAN)
#  pragma section
# endif /* C_COMP_HEW_SH2_RCAN */
#endif /* LBT_STOP_SEC_FLASHCODE */

/**********************************************************/
/*---- Locate Libmem regions                          ----*/
/**********************************************************/
#if defined (FBLLIB_MEM_START_SEC_VAR) || \
    defined (FBLLIB_MEM_START_SEC_VAR_EXPORT)
#undef FBLLIB_MEM_START_SEC_VAR
#undef FBLLIB_MEM_START_SEC_VAR_EXPORT
/* Insert memory section definition here   */
#endif /* FBLLIB_MEM_START_SEC_VAR */
