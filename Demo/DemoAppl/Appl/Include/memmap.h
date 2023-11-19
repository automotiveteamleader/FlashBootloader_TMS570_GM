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
|*****************************************************************************/

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
#endif /* CANPARAM_START_SEC_DATA */

#if defined (CANPARAM_STOP_SEC_DATA) || \
    defined (CANPARAM_STOP_SEC_DATA_EXPORT)
#undef CANPARAM_STOP_SEC_DATA
#undef CANPARAM_STOP_SEC_DATA_EXPORT
/* Insert memory section definition here   */
#endif /* CANPARAM_STOP_SEC_DATA */

#if defined (MAGICFLAG_START_SEC_DATA) || \
    defined (MAGICFLAG_START_SEC_DATA_EXPORT)
#undef MAGICFLAG_START_SEC_DATA
#undef MAGICFLAG_START_SEC_DATA_EXPORT
/* Insert memory section definition here   */
#pragma DATA_SECTION (fblStartMagicFlag, ".FBLMAGIC")
#endif /* MAGICFLAG_START_SEC_DATA */

#if defined (MAGICFLAG_STOP_SEC_DATA) || \
    defined (MAGICFLAG_STOP_SEC_DATA_EXPORT)
#undef MAGICFLAG_STOP_SEC_DATA
#undef MAGICFLAG_STOP_SEC_DATA_EXPORT
/* Insert memory section definition here   */
#endif /* MAGICFLAG_STOP_SEC_DATA */

/**********************************************************/
/*---- Locate FblHeader                               ----*/
/**********************************************************/
#if defined (FBLHEADER_START_SEC_CONST)
#undef FBLHEADER_START_SEC_CONST

/* Insert memory section definition here   */
#pragma DATA_SECTION (FblHeader, ".FBLHEADER")
#endif /* FBLHEADER_START_SEC_CONST */

#if defined (FBLHEADER_STOP_SEC_CONST) || \
    defined (FBLHEADER_STOP_SEC_CONST_EXPORT)
#undef FBLHEADER_STOP_SEC_CONST
#undef FBLHEADER_STOP_SEC_CONST_EXPORT
/* Insert memory section definition here   */
#endif /* FBLHEADER_STOP_SEC_CONST */
#if defined (FBLHEADER_START_SEC_CONST_EXPORT)
#undef FBLHEADER_START_SEC_CONST_EXPORT
#endif /* FBLHEADER_START_SEC_CONST */
/**********************************************************/
/*---- Locate watchdog routines                       ----*/
/**********************************************************/

#if defined (WDTRIGGER_START_SEC_CODE) || \
    defined (WDTRIGGER_START_SEC_CODE_EXPORT)
#undef WDTRIGGER_START_SEC_CODE
#undef WDTRIGGER_START_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#pragma CODE_SECTION (ApplFblWDTrigger, ".RAMCODE")
#endif /* WDTRIGGER_START_SEC_CODE */

#if defined (WDTRIGGER_STOP_SEC_CODE) || \
    defined (WDTRIGGER_STOP_SEC_CODE_EXPORT)
#undef WDTRIGGER_STOP_SEC_CODE
#undef WDTRIGGER_STOP_SEC_CODE_EXPORT
#endif /* WDTRIGGER_STOP_SEC_CODE */

#if defined (LOOKFORWATCHDOG_START_SEC_CODE) || \
    defined (LOOKFORWATCHDOG_START_SEC_CODE_EXPORT)
#undef LOOKFORWATCHDOG_START_SEC_CODE
#undef LOOKFORWATCHDOG_START_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#pragma CODE_SECTION (FblLookForWatchdog, ".RAMCODE")
#endif /* LOOKFORWATCHDOG_START_SEC_CODE */

#if defined (LOOKFORWATCHDOG_STOP_SEC_CODE) || \
    defined (LOOKFORWATCHDOG_STOP_SEC_CODE_EXPORT)
#undef LOOKFORWATCHDOG_STOP_SEC_CODE
#undef LOOKFORWATCHDOG_STOP_SEC_CODE_EXPORT
/* Insert memory section definition here   */
#endif /* LOOKFORWATCHDOG_STOP_SEC_CODE */

/**********************************************************/
/*---- Locate flashCode Buffer                        ----*/
/**********************************************************/

#if defined (FBLFLIO_FLASHCODE_START_SEC_VAR) 
#undef FBLFLIO_FLASHCODE_START_SEC_VAR
/* Insert memory section definition here   */
#pragma DATA_SECTION (flashHeader, ".FLASHDRV")
#endif /* LBT_START_SEC_FLASHCODE */

#if defined (FBLFLIO_FLASHCODE_STOP_SEC_VAR)
#undef FBLFLIO_FLASHCODE_STOP_SEC_VAR
/* Insert memory section definition here   */
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

/**********************************************************/
/*---- Locate APPLVECT regions                        ----*/
/**********************************************************/
#if defined (FBL_APPLVECT_START_SEC_CONST) || \
    defined (FBL_APPLVECT_START_SEC_CONST_EXPORT)
#undef FBL_APPLVECT_START_SEC_CONST
#undef FBL_APPLVECT_START_SEC_CONST_EXPORT
#endif

#if defined (FBL_APPLVECT_STOP_SEC_CONST) || \
    defined (FBL_APPLVECT_STOP_SEC_CONST_EXPORT)
#undef FBL_APPLVECT_STOP_SEC_CONST
#undef FBL_APPLVECT_STOP_SEC_CONST_EXPORT
#endif

/**********************************************************/
/*---- Locate SecMod regions                          ----*/
/**********************************************************/
#if defined (SECMPAR_START_SEC_DATA) || \
    defined (SECMPAR_START_SEC_DATA_EXPORT)
#undef SECMPAR_START_SEC_DATA
#undef SECMPAR_START_SEC_DATA_EXPORT
/* Insert memory section definition here   */
#pragma DATA_SECTION (fblStartMagicFlag, ".FBLMAGIC")
#endif /* SECMPAR_START_SEC_DATA */

#if defined (SECMPAR_STOP_SEC_DATA) || \
    defined (SECMPAR_STOP_SEC_DATA_EXPORT)
#undef SECMPAR_STOP_SEC_DATA
#undef SECMPAR_STOP_SEC_DATA_EXPORT
/* Insert memory section definition here   */
#endif /* SECMPAR_STOP_SEC_DATA */

/**********************************************************/
/*---- Locate EepIO regions                           ----*/
/**********************************************************/
#if defined (EEPDUMMY_START_SEC_VAR) || \
    defined (EEPDUMMY_START_SEC_VAR_EXPORT)
#undef EEPDUMMY_START_SEC_VAR
#undef EEPDUMMY_START_SEC_VAR_EXPORT
/* Insert memory section definition here   */
#pragma DATA_SECTION (eepData, ".EEPIO")
#endif /* EEPDUMMY_START_SEC_VAR */

