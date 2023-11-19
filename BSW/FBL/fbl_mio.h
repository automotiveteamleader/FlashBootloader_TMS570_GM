/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Memory Driver wrapper functions
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Markus Holzer                 MHz           Vector Informatik GmbH
 *  Andreas Pick                  AP            Vector Informatik GmbH
 *  Ralf Fritz                    Fz            Vector Informatik GmbH
 *  Alexandre C. Plombin          ACP           Vector CANtech, Inc.
 *  Florian Hees                  FHe           Vector Informatik GmbH
 *  Ahmad Nasser                  AMN           Vector CANtech, Inc.
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Slawomir Wilk                 SWk           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Jens Christ                   Cjs           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2003-12-15  WM                       Implementation
 *  01.01.00   2004-02-10  WM      ESCAN00007465    Adaptations for C_COMP_IAR_12
 *  01.02.00   2004-03-05  Fz                       Add of verification function for 78k0 and V850
 *  01.03.00   2004-04-23  WM                       Removed IO types
 *  01.04.00   2004-07-06  Mhz                      Minor changes
 *  01.05.00   2004-07-06  Ls      ESCAN00008880    MemDriver_SegmentSize added
 *  01.06.00   2004-07-28  WM                       Minor changes
 *  01.07.00   2004-08-04  WM                       Added FBL_MEMDRV_MAX_SEGMENT_SIZE define
 *  01.08.00   2004-08-10  Ap                       Changed memory verify routine parameter type to (void *)
 *  01.09.00   2004-12-23  ACP                      Version check reworked
 *  01.10.00   2005-02-16  AMN                      Added MemDriver_REraseAllSync for Ford SLP3
 *  01.11.00   2005-03-24  FHe                      Added MemDriver_REraseAllSync for GM
 *                         WM                       Minor changes
 *  01.12.00   2005-06-08  Hp                       Added support for V85x/IAR
 *                         AMN                      Removed MemDriver_REraseAllSync for Ford SLP3
 *  01.13.00   2005-08-10  AMN                      No changes
 *  01.14.00   2005-09-27  MFR                      Define FBL_MAX_SEGMENT_SIZE for all configurations
 *                                                  Add macros (xxxMemDriverReady) to determine if
 *                                                  individual drivers are ready
 *                         WM      ESCAN00013673    Support for C_COMP_HIWARE_12 added
 *  01.15.00   2005-11-10  MFR                      No changes (See C-File)
 *  01.16.00   2005-11-30  WM      ESCAN00014481    Unbalanced #endif
 *  01.17.00   2005-12-29  FHE                      Changed #elif statement (Organi problems)
 *  01.18.00   2006-02-15  WM                       No changes
 *  01.19.00   2006-07-17  JHg                      Moved V_API_NEAR in InitSync and DeinitSync
 *  01.20.00   2006-08-12  Hp                       Add interface functions for direct device function access.
 *  01.21.00   2006-10-11  Ci                       Added section pragmas for Metrowerks MCS12(X) compiler
 *                         SWk     ESCAN00019219    Adapt for NEC Compiler
 *  01.22.00   2008-11-10  CB      ESCAN00015630    No changes
 *  01.23.00   2009-03-17  JHg     ESCAN00033924    Enabled MemDriverReady for OEM Volvo AB
 *  01.24.00   2009-08-20  Hp                       Status variable to indicate if erase or write is active.
 *                         SWk     ESCAN00036931    CW-layer introduced (optionally).
 *                                                  Add FBL_ENABLE_MEM_DRIVER_DEVICE_COMMENT, used for:
 *                                                  Volvo, GM SLP2/SLP3 only.
 *  01.25.00   2009-08-24  SWk                      Added a dummy statement to avoid the compiler warning
 *  01.26.00   2009-12-09  TBe     ESCAN00039376    No changes
 *  01.27.00   2009-12-11  SWk     ESCAN00039695    Rework this file for compatibility with old interface
 *  01.28.00   2010-01-14  QPs     ESCAN00040144    No changes
 *  01.29.00   2010-02-16  SWk     ESCAN00040355    Rework of this file
 *                         Ci      ESCAN00039458    Enabled MemDriverReady for OEM Ford (SLP6)
 *  01.30.00   2010-03-22  SWk     ESCAN00041499    Reprogramming sequence is interrupted on initializing
 *                                                  the flash driver. [COM Wrapper & Flash only]
 *  01.31.00   2010-04-28  SWk     ESCAN00042370    Compiler error with multiple memory devices configuration
 *                         SWk     ESCAN00042595    Corrected double define of MemDriver_RInitSync [if CW && MMD]
 *  01.32.00   2010-07-01  SWk     ESCAN00043920    Introduction of FBL_ENABLE_FUNC_VERIFYSYNC for V850 and 78k0(R)
 *  01.33.00   2011-03-02  SWk     ESCAN00048161    No changes
 *  01.34.00   2011-03-07  SWk     ESCAN00048791    FlashDriver_VerifySync is not always called for the internal flash driver
 *  01.35.00   2011-04-19  SWk     ESCAN00049841    No changes
 *                         SWk     ESCAN00049851    Optimize inside of FBL_ENABLE_FUNC_VERIFYSYNC
 *  01.36.00   2011-08-12  Rr      ESCAN00052726    Enabled MemDriverReady for Renault
 *  01.37.00   2012-04-05  QPs     ESCAN00058147    Added status variable to indicate which function is active
 *  01.38.00   2012-09-06  AWh     ESCAN00061245    Added FBL_CLIENT_GM_SLP4
 *  01.39.00   2012-11-29  Ach     ESCAN00062923    Adapted comments to use Doxygen
 *  01.40.00   2013-12-13  JHg     ESCAN00064823    Moved function prototypes
 *                         ASe     -                General refactoring
 *                         JHg     ESCAN00072598    No changes
 *  01.41.00   2014-04-30  HRf     ESCAN00075330    Add support for OEM PATAC
 *  01.42.00   2014-05-28  JHg     ESCAN00075942    No changes
 *  01.42.01   2014-07-29  Cjs     ESCAN00077469    No changes
 *  01.42.02   2015-01-14  AWh     ESCAN00080412    Allow overwriting of FBL_MAX_SEGMENT_SIZE define
 **********************************************************************************************************************/

#ifndef __FBL_MIO_H__
#define __FBL_MIO_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblMio CQComponent : Implementation */
#define FBLMIO_VERSION              0x0142
#define FBLMIO_RELEASE_VERSION      0x02

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/** Device active state */
#define kMioDeviceIdle              0xFFu

#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )

#  define MemDriver_RWriteSync                     (*memDrvLst[FlashBlock[memSegment].device].write)
#  define MemDriver_REraseSync                     (*memDrvLst[FlashBlock[memSegment].device].erase)

# define MemDriver_RReadSync                       (*memDrvLst[FlashBlock[memSegment].device].read)
# define MemDriver_SegmentSize                     (memDrvLst[FlashBlock[memSegment].device].segmentSize)
/* Remapped for the new MIO interface */
# define MemDriver_InitSync(a)                     MemDriver_RInitSync((a))
# define MemDriver_DeinitSync(a)                   MemDriver_RDeinitSync((a))
# define MEM_TYPE_SEPARATOR    ,
# if defined( FBL_MEMDRV_SEGMENT_SIZE )
#  if ( FLASH_SEGMENT_SIZE > FBL_MEMDRV_SEGMENT_SIZE )
#   define FBL_MAX_SEGMENT_SIZE_MIO FLASH_SEGMENT_SIZE
#  else
#   define FBL_MAX_SEGMENT_SIZE_MIO FBL_MEMDRV_SEGMENT_SIZE
#  endif
# else
#  define FBL_MAX_SEGMENT_SIZE_MIO FLASH_SEGMENT_SIZE
# endif /* FBL_MEMDRV_SEGMENT_SIZE */

#else /* FBL_ENABLE_MULTIPLE_MEM_DEVICES */

/* In case of single memory this index will be used */
/* In case of multiple memory the FlashBlock[memSegment].device, or device index will be used */
# define kMioDeviceInternalFlash     0u

#  define MemDriver_InitSync(a)                    FlashDriver_InitSync((a))
#  define MemDriver_RWriteSync(a, b, c)            FlashDriver_RWriteSync((a), (b), (c))
#  define MemDriver_REraseSync(a, b)               FlashDriver_REraseSync((a), (b))

# define MemDriver_RReadSync(a, b, c)              FlashDriver_RReadSync((a), (b), (c))
# define MemDriver_DeinitSync(a)                   FlashDriver_DeinitSync((a))
# define MemDriver_SegmentSize                     FLASH_SEGMENT_SIZE
# define MEM_TYPE_SEPARATOR
# define FBL_MAX_SEGMENT_SIZE_MIO FLASH_SEGMENT_SIZE

#endif /* FBL_ENABLE_MULTIPLE_MEM_DEVICES */

#if defined( FBL_MAX_SEGMENT_SIZE )
# if ( FBL_MAX_SEGMENT_SIZE_MIO > FBL_MAX_SEGMENT_SIZE )
#  error "Define FBL_MAX_SEGMENT_SIZE from external only with values larger than determined through FblMio."
# endif
#else
# define FBL_MAX_SEGMENT_SIZE FBL_MAX_SEGMENT_SIZE_MIO
#endif /* FBL_MAX_SEGMENT_SIZE */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

typedef V_API_NEAR IO_ErrorType (*tDrvFctInit)( void * );
typedef V_API_NEAR IO_ErrorType (*tDrvFctDeinit)( void * );
typedef V_API_NEAR IO_ErrorType (*tDrvFctRead)( IO_MemPtrType, IO_SizeType, IO_PositionType );
typedef V_API_NEAR IO_ErrorType (*tDrvFctWrite)( IO_MemPtrType, IO_SizeType, IO_PositionType );
typedef V_API_NEAR IO_ErrorType (*tDrvFctErase)( IO_SizeType, IO_PositionType );

typedef struct
{
   tDrvFctInit    init;
   tDrvFctDeinit  deinit;
   tDrvFctErase   erase;
   tDrvFctWrite   write;
   tDrvFctRead    read;
   vuint16        segmentSize;
} tMemDrv;

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
IO_ErrorType V_API_NEAR MemDriver_RInitSync( void * address );
IO_ErrorType V_API_NEAR MemDriver_RDeinitSync( void * address );
#endif

#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
#endif /* FBL_ENABLE_MULTIPLE_MEM_DEVICES */

/***********************************************************************************************************************
 *  GLOBALS
 **********************************************************************************************************************/

#endif /* __FBL_MIO_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_MIO.H
 **********************************************************************************************************************/
