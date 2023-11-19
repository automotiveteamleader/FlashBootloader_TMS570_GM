/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Flash algorithm for the TMS470
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Moe Charara                   Mch           Vector CANtech, Inc.
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Luis Gonzalez                 LGO           Vector CANtech, Inc.
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Johannes Krimmel              KJs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id          Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2011-05-05  Mch                        Creation of the TMS470 F021 flash driver algorithm to support
 *                                                    TMS570LS30336
 *  01.01.00   2011-08-11  Mch     ESCAN00052712      Added support for automatic device information retrieval
 *                                                    (GetFlashDeviceInfo)
 *  01.02.00   2011-10-24  MVi     ESCAN00054376      No changes
 *  01.03.00   2012-03-30  LGO     ESCAN00058048      No changes
 *  01.04.00   2013-07-03  ASe     ESCAN00067934      Adapted comments to use doxygen
 *  01.05.00   2013-08-19  ASe     ESCAN00069457      Adaptations for Flash API 2.00.00
 *  01.06.00   2013-08-21  ASe     ESCAN00069932      No changes
 *  01.07.00   2015-02-12  KJs     ESCAN00080585      No changes
 **********************************************************************************************************************/

#ifndef _FBLDRVFLASH_TMS470F02XHIS_H_
#define _FBLDRVFLASH_TMS470F02XHIS_H_

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "Types.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDrvFlash_Tms470F02xHis CQComponent : Implementation */
#define FBLDRVFLASH_TMS470F02XHIS_VERSION           0x0107u
#define FBLDRVFLASH_TMS470F02XHIS_RELEASE_VERSION   0x00u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* External interface compatibility */
#if defined( _debug_ )
# define _DEBUG_
#endif

#ifndef V_CALLBACK_NEAR
# define V_CALLBACK_NEAR
#endif

#ifndef V_API_NEAR
# define V_API_NEAR
#endif

/* Flash Driver hardware information */
#define FLASH_DRIVER_VERSION_MCUTYPE         0x47u     /* TMS470 */

#define FLASH_DRIVER_VERSION_MASKTYPE        0x05u     /* F021 */

#define FLASH_DRIVER_VERSION_INTERFACE       0x00u

/* Flash Driver software information */
#define FLASH_DRIVER_VERSION_MAJOR     (FBLDRVFLASH_TMS470F02XHIS_VERSION >> 8)
#define FLASH_DRIVER_VERSION_MINOR     (FBLDRVFLASH_TMS470F02XHIS_VERSION & 0xFFu)
#define FLASH_DRIVER_VERSION_PATCH     FBLDRVFLASH_TMS470F02XHIS_RELEASE_VERSION

/* Defines to access version and type information */
#define FLASH_DRIVER_MCUTYPE(ramBuffer)      (*(unsigned char *)(ramBuffer + 3u))
#define FLASH_DRIVER_MASKTYPE(ramBuffer)     (*(unsigned char *)(ramBuffer + 2u))
#define FLASH_DRIVER_INTERFACE(ramBuffer)    (*(unsigned char *)(ramBuffer + 0u))

#define FLASH_DRIVER_NOT_RELOCATABLE

/* Function call table offsets */
#define FLASH_DRIVER_HEADER_OFFSET     0x04u
#define FLASH_DRIVER_INIT_OFFSET       (FLASH_DRIVER_HEADER_OFFSET + 0x00u)
#define FLASH_DRIVER_DEINIT_OFFSET     (FLASH_DRIVER_HEADER_OFFSET + 0x04u)
#define FLASH_DRIVER_ERASE_OFFSET      (FLASH_DRIVER_HEADER_OFFSET + 0x08u)
#define FLASH_DRIVER_WRITE_OFFSET      (FLASH_DRIVER_HEADER_OFFSET + 0x0Cu)

/* Minimum number of bytes that has to be programmed at a time */
#define FLASH_SEGMENT_SIZE    0x08u

/* The contents of a deleted flash byte */
#define FBL_FLASH_DELETED     0xFFu

/* Mask is 0x7 for 8byte write and 0xF for 16 byte write */
#define kFlashWriteMask       0x07u

#define kFlashWriteNoAlign    0x00u

#define kFlashWriteAlign      0xFFu

#define kFlashSectorNotFound  0xFFu

/* Error codes */

/* Routine specific error groups */
#define kFlashFctInit         0x00u
#define kFlashFctDeinit       0x20u
#define kFlashFctErase        0x40u
#define kFlashFctWrite        0x60u
#define kFlashFctUsrFct       0x80u

/* Common error codes */
#define kFlashOk                 0x00u   /**< Function call successful */
#define kFlashFailed             0x01u   /**< Function call failed */
#define kFlashVerify             0x02u   /**< Verify error */
#define kFlashInvalidParam       0x03u   /**< Invalid parameter */
#define kFlashInvalidAddr        0x04u   /**< Invalid flash address */
#define kFlashInvalidSize        0x05u   /**< Invalid flash size */
#define kFlashInvalidClock       0x06u   /**< Missing/wrong clock supply */
#define kFlashProtect            0x07u   /**< Protection error */
#define kFlashAcc                0x08u   /**< Access error */
#define kFlashCmdBufFull         0x09u   /**< Command buffer not empty */
#define kFlashInvalidVersion     0x0Au   /**< Invalid version */
#define kFlashCompaction         0x0Bu   /**< Failure during Compaction */
#define kFlashModeNotSupported   0x0Cu   /**< Prog. mode not implemented */
#define kFlashModeNotValid       0x0Du   /**< Invalid programming mode */

/* Flash init error codes */
#define kFlashInitFailed          (kFlashFctInit | kFlashFailed)
#define kFlashInitInvalidParam    (kFlashFctInit | kFlashInvalidParam)
#define kFlashInitInvalidAddr     (kFlashFctInit | kFlashInvalidAddr)
#define kFlashInitInvalidSize     (kFlashFctInit | kFlashInvalidSize)
#define kFlashInitInvalidClock    (kFlashFctInit | kFlashInvalidClock)
#define kFlashInitProtect         (kFlashFctInit | kFlashProtect)
#define kFlashInitAcc             (kFlashFctInit | kFlashAcc)
#define kFlashInitInvalidVersion  (kFlashFctInit | kFlashInvalidVersion)
#define kFlashInitCompaction      (kFlashFctInit | kFlashCompaction)

/* Flash deinit error codes */
#define kFlashDeinitFailed        (kFlashFctDeinit | kFlashFailed)
#define kFlashDeinitInvalidParam  (kFlashFctDeinit | kFlashInvalidParam)

/* Flash erase error codes */
#define kFlashEraseFailed         (kFlashFctErase | kFlashFailed)
#define kFlashEraseVerify         (kFlashFctErase | kFlashVerify)
#define kFlashEraseInvalidParam   (kFlashFctErase | kFlashInvalidParam)
#define kFlashEraseInvalidAddr    (kFlashFctErase | kFlashInvalidAddr)
#define kFlashEraseInvalidSize    (kFlashFctErase | kFlashInvalidSize)
#define kFlashEraseInvalidClock   (kFlashFctErase | kFlashInvalidClock)
#define kFlashEraseProtect        (kFlashFctErase | kFlashProtect)
#define kFlashEraseAcc            (kFlashFctErase | kFlashAcc)
#define kFlashEraseCmdBufFull     (kFlashFctErase | kFlashCmdBufFull)

/* Flash write error codes */
#define kFlashWriteFailed         (kFlashFctWrite | kFlashFailed)
#define kFlashWriteVerify         (kFlashFctWrite | kFlashVerify)
#define kFlashWriteInvalidParam   (kFlashFctWrite | kFlashInvalidParam)
#define kFlashWriteInvalidAddr    (kFlashFctWrite | kFlashInvalidAddr)
#define kFlashWriteInvalidSize    (kFlashFctWrite | kFlashInvalidSize)
#define kFlashWriteInvalidClock   (kFlashFctWrite | kFlashInvalidClock)
#define kFlashWriteProtect        (kFlashFctWrite | kFlashProtect)
#define kFlashWriteAcc            (kFlashFctWrite | kFlashAcc)
#define kFlashWriteCmdBufFull     (kFlashFctWrite | kFlashCmdBufFull)
#define kFlashProgrammingModeNotSupported (kFlashFctWrite | kFlashModeNotSupported)
#define kFlashProgrammingModeNotValid     (kFlashFctWrite | kFlashModeNotValid)

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef unsigned char  tFlashData;        /**< Flash data type (MCU specific) */
typedef unsigned int   tFlashAddress;     /**< Flash address type (MCU specific) */
typedef unsigned int   tFlashLength;      /**< Flash length type (MCU specific) */
typedef unsigned short tFlashErrorCode;   /**< Flash error code */

typedef struct
{
   unsigned char     patchLevel;          /**< Patch level */
   unsigned char     minorVersion;        /**< Minor version number */
   unsigned char     majorVersion;        /**< Major version number */
   unsigned char     reserved1;           /**< Reserved for future use */

   /* Return value/error code */
   tFlashErrorCode   errorCode;           /**< Return value/error code */
   unsigned short    reserved2;           /**< Reserved for future use */

   /* Erase/write input parameters */
   tFlashAddress     address;             /**< Logical target address */
   tFlashLength      length;              /**< Length (in bytes) */
   tFlashData *      data;                /**< Pointer to data buffer (read only) */

   /** Pointer to watchdog trigger function */
   unsigned char (*wdTriggerFct)(void);

   /* Additional parameters */
   unsigned int                        hclkFrequency;    /**< CPU frequency for FAPI init */
   Fapi_FlashProgrammingCommandsType   programmingMode;  /**< Programming mode */

   Fapi_StatusType fsmResult;             /**< Contains flash APIs result information */

#if defined( FLASH_DRIVER_DEBUG )
   /* Used for verification */
   /* Erase/write output parameters */
   tFlashData        intendedData;        /**< Intended data at error address */
   tFlashData        actualData;          /**< Actual data at error address */
   tFlashAddress     errorAddress;        /**< Error address */
   Fapi_FlashStatusType fsmStat;
#endif /* FLASH_DRIVER_DEBUG */

} tFlashParam;

/** Pointer to flash functions */
typedef void (*tFlashFct)( tFlashParam * flashParam );

/**********************************************************************************************************************
 *  MACROS
 *********************************************************************************************************************/

/** Flash write function */
#define FLASH_DRIVER_WRITE(ramBuffer, flashParam)\
   ((tFlashFct)(*(tFlashAddress *)&ramBuffer[FLASH_DRIVER_WRITE_OFFSET]))(flashParam)
/** Flash erase function */
#define FLASH_DRIVER_ERASE(ramBuffer, flashParam)\
   ((tFlashFct)(*(tFlashAddress *)&ramBuffer[FLASH_DRIVER_ERASE_OFFSET]))(flashParam)
/** Flash init function */
#define FLASH_DRIVER_INIT(ramBuffer, flashParam)\
   ((tFlashFct)(*(tFlashAddress *)&ramBuffer[FLASH_DRIVER_INIT_OFFSET]))(flashParam)
/** Flash deinitialization function */
#define FLASH_DRIVER_DEINIT(ramBuffer, flashParam)\
   ((tFlashFct)(*(tFlashAddress *)&ramBuffer[FLASH_DRIVER_DEINIT_OFFSET]))(flashParam)

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

void ExpFlashInit( tFlashParam * fp );
void ExpFlashDeinit( tFlashParam * fp );
void ExpFlashErase( tFlashParam * fp );
void ExpFlashWrite( tFlashParam * fp );

#endif /* _FBLDRVFLASH_TMS470F02XHIS_H_ */

/***********************************************************************************************************************
 *  END OF FILE: FLASHDRV.H
 **********************************************************************************************************************/
