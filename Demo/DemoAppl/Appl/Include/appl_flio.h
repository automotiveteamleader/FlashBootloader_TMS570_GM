/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Flash driver wrapper for TMS470
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Vijay Natarajan               VNA           Vector CANtech Inc.
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2003-03-26  Ls                       Release
 *  01.01.00   2003-04-11  Rr                       No changes
 *  01.02.00   2003-04-14  Ls                       Version control added
 *  01.03.00   2003-04-17  Ls                       No changes
 *  01.04.00   2004-01-26  CB                       No changes
 *  01.05.00   2004-03-05  CB                       No changes
 *  01.06.00   2004-03-26  CB                       Some typedefs removed
 *  01.07.00   2004-05-11  CB                       No changes
 *  01.08.00   2005-03-02  CB                       No changes
 *  01.09.00   2006-03-14  Rr                       No changes
 *  01.10.00   2006-04-20  WM      ESCAN00016093    Version scan support added
 *  01.11.00   2008-02-13  CB                       No changes
 *  01.12.00   2008-03-13  AWh     ESCAN00025330    No changes
 *                         AWh     ESCAN00025327    No changes
 *  01.13.00   2008-08-15  Rr      ESCAN00029265    No changes
 *  01.14.00   2009-09-22  Ci      ESCAN00036244    Added flashCode array
 *                                 ESCAN00036249    No changes
 *  01.15.00   2009-09-24  QPs     ESCAN00038012    No changes
 *                         MVi     ESCAN00038014    Added support for TI TMS570
 *  01.16.00   2010-01-11  Pal     ESCAN00042050    Added support for C_COMP_ARM_TMS470_DCAN
 *  01.17.00   2011-10-24  AWh     ESCAN00049641    Adapt to changed flashdrv-design
 *                         MVi     ESCAN00054405    No changes
 *  01.18.00   2012-06-12  CB      ESCAN00059374    No changes
 *                         Ach     ESCAN00061133    No changes
 *  01.19.00   2013-03-26  QPs     ESCAN00066373    No changes
 *                                 ESCAN00066551    Converted macro FlashDriver_GetVersionOfDriver()
 *                                                  into a function
 *  01.20.00   2013-07-02  ASe     ESCAN00067929    Adapted comments to use doxygen
 *  01.21.00   2013-08-19  ASe     ESCAN00069664    No changes
 *  01.22.00   2014-04-30  ASe     ESCAN00074552    Added multi driver support
 *  01.23.00   2014-06-30  VNA     ESCAN00076586    No changes
 **********************************************************************************************************************/

#ifndef _FBLWRAPPERFLASH_TMS470FXHIS_H_
#define _FBLWRAPPERFLASH_TMS470FXHIS_H_

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblWrapperFlash_Tms470FxHis CQComponent : Implementation */
#define FBLWRAPPERFLASH_TMS470FXHIS_VERSION             0x0123u
#define FBLWRAPPERFLASH_TMS470FXHIS_RELEASE_VERSION     0x00u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* May be defined via GENy preconfiguration to override this standard value. */
# if !defined( FLASH_SIZE )
#  define FLASH_SIZE FLASHDRV_BLOCK0_LENGTH
# else
#  if ( FLASH_SIZE < FLASHDRV_BLOCK0_LENGTH )
#   error "Flash code does not fit into reserved buffer, check FLASH_SIZE define"
#  endif
# endif

#define IO_DRIVERMODUS  SYNCRON
#define IO_DEVICETYPE   RANDOM

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/* Location of flash algorithm in RAM */
/* Address of flash algorithms in RAM */
extern IO_U8 flashCode[FLASH_SIZE];

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

IO_ErrorType FlashDriver_InitSync( void * );
IO_ErrorType FlashDriver_DeinitSync( void * );
IO_ErrorType FlashDriver_RReadSync( IO_MemPtrType, IO_SizeType, IO_PositionType );
IO_ErrorType FlashDriver_RWriteSync( IO_MemPtrType, IO_SizeType, IO_PositionType );
IO_ErrorType FlashDriver_REraseSync( IO_SizeType, IO_PositionType );
IO_U32 FlashDriver_GetVersionOfDriver( void );

#endif /* _FBLWRAPPERFLASH_TMS470FXHIS_H_ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_FLIO.H
 **********************************************************************************************************************/
