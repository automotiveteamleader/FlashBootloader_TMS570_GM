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
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Jens Christ                   Cjs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2003-12-15  WM                       Implementation
 *  01.01.00   2004-02-10  WM      ESCAN00007465    Adaptations for C_COMP_IAR_12
 *  01.02.00   2004-03-05  Fz                       No changes
 *  01.03.00   2004-04-23  WM                       Minor changes
 *  01.04.00   2004-07-06  Mhz                      Minor changes
 *  01.05.00   2004-07-06  Ls                       No changes in this file
 *  01.06.00   2004-07-28  WM                       Minor changes
 *  01.07.00   2004-08-04  WM                       No changes
 *  01.08.00   2004-08-10  Ap                       No changes
 *  01.09.00   2004-12-23  ACP                      Version check reworked
 *  01.10.00   2005-02-16  AMN                      MemDriver_REraseAllSync for FORD_SLP3 added
 *  01.11.00   2005-03-24  FHe                      Added MemDriver_REraseAllSync for GM
 *                         WM                       Minor changes
 *  01.12.00   2005-06-07  Hp                       Added support for V85x/IAR
 *                         AMN                      Moved EraseAllSync to DiagSectorErase for Ford SLP3
 *  01.13.00   2005-08-10  AMN                      Added version check using v_ver.h
 *  01.14.00   2005-09-27  MFR                      Moved EraseAllSync to DiagEraseAll for GM SLP2
 *                         WM      ESCAN00013673    Support for C_COMP_HIWARE_12 added
 *  01.15.00   2005-11-10  MFR                      Minor changes for MISRA
 *  01.16.00   2005-11-30  WM                       No changes
 *  01.17.00   2005-12-29  FHE                      No changes
 *  01.18.00   2006-02-15  WM      ESCAN00015630    Dummy function added
 *  01.19.00   2006-07-17  JHg                      Added V_API_NEAR to functions
 *  01.20.00   2006-08-12  Hp                       Add interface functions for direct device function access.
 *  01.21.00   2006-10-11  Ci                       No changes
 *                         SWk                      No changes
 *  01.22.00   2008-11-10  CB      ESCAN00015630    Added V_API_NEAR for dummy fct.
 *  01.23.00   2009-03-17  JHg     ESCAN00033924    Enabled MemDriverReady for OEM Volvo AB
 *  01.24.00   2009-08-20  Hp                       Status variable to indicate if erase or write is active.
 *                         SWk     ESCAN00036931    CW-layer introduced (optionally).
 *                                                  Add FBL_ENABLE_MEM_DRIVER_DEVICE_COMMENT, used for:
 *                                                  Volvo, GM SLP2/SLP3 only.
 *  01.25.00   2009-08-24  SWk                      Added a dummy statement to avoid the compiler warning
 *  01.26.00   2009-12-09  TBe     ESCAN00039376    Adaptions for +strict option in Cosmic Compiler
 *  01.27.00   2009-12-11  SWk     ESCAN00039695    Rework this file for compatibility with old interface
 *  01.28.00   2010-01-14  QPs     ESCAN00040144    MemDriver_VerifySync with CW-layer and multiple memory
 *                                                  devices reworked
 *  01.29.00   2010-02-16  SWk     ESCAN00040355    Rework of this file
 *                         Ci      ESCAN00039458    Enabled MemDriverReady for OEM Ford (SLP6)
 *  01.30.00   2010-03-22  SWk     ESCAN00041499    No changes
 *  01.31.00   2010-04-21  SWk     ESCAN00042370    Compiler error with multiple memory devices configuration
 *                         SWk     ESCAN00042595    No changes
 *  01.32.00   2010-07-01  SWk     ESCAN00043920    Introduction of FBL_ENABLE_FUNC_VERIFYSYNC for V850 and 78k0(R)
 *  01.33.00   2011-03-02  SWk     ESCAN00048161    Add prototype for dummy() to avoid compiler warning
 *  01.34.00   2011-03-17  SWk     ESCAN00048791    FlashDriver_VerifySync is not always called for the internal flash driver
 *  01.35.00   2011-04-19  SWk     ESCAN00049841    Add prototype of dummy() into NON_BANKED section to avoid compiler warning
 *                         SWk     ESCAN00049851    No changes
 *  01.36.00   2011-08-12  Rr      ESCAN00052726    Enabled MemDriverReady for Renault
 *  01.37.00   2012-04-05  QPs     ESCAN00058147    Added status variable to indicate which function is active
 *  01.38.00   2012-09-06  AWh     ESCAN00061245    Added FBL_CLIENT_GM_SLP4
 *  01.39.00   2012-11-29  Ach     ESCAN00062923    Adapted comments to use Doxygen
 *  01.40.00   2013-12-13  JHg     ESCAN00064823    Moved pragmas for C_COMP_HIWARE_12
 *                         ASe     -                General refactoring
 *                         JHg     ESCAN00072598    Removed explicit dependencies to fbl_diag.h and fbl_wd.h
 *  01.41.00   2014-04-30  HRf     ESCAN00075330    Add support for OEM PATAC
 *  01.42.00   2014-05-28  JHg     ESCAN00075942    Set device active status for all use-cases during initialization
 *  01.42.01   2014-07-29  Cjs     ESCAN00077469    MemDriver_RInitEnd(i); called regardless of result
 *  01.42.02   2015-01-14  AWh     ESCAN00080412    No changes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLMIO_VERSION != 0x0142 ) || \
    ( FBLMIO_RELEASE_VERSION != 0x02 )
# error "Error in FBL_MIO.C: Source and header file are inconsistent!"
#endif

#if ( FBLMIO_VERSION != _FBLMIO_VERSION ) || \
    ( FBLMIO_RELEASE_VERSION != _FBLMIO_RELEASE_VERSION )
# error "Error in FBL_MIO.C: Source and V_VER.H are inconsistent!"
#endif

/***********************************************************************************************************************
 *  GLOBALS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
#else
/** Previous prototype for dummy() to avoid compiler warning */
void V_API_NEAR dummy( void );
#endif

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
/***********************************************************************************************************************
 *  MemDriver_RInitSync
 **********************************************************************************************************************/
/*! \brief      Initialize all memory drivers.
 *  \pre        Memory drivers have to be present (e.g. copied to RAM).
 *  \param[in]  address Parameter to be passed to HIS interface flash driver.
 *  \return     Reports if all memory drivers have been initialized successfully.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_RInitSync( void * address )
{
   IO_U8 i;
   IO_ErrorType result = IO_E_OK;
   IO_ErrorType rc;

   for (i = 0; i < kNrOfMemDrv; i++)
   {
      rc = (*memDrvLst[i].init)(address);
      if (rc != IO_E_OK)
      {
         result = rc;
         break;
      }
   }
   return result;
}

/***********************************************************************************************************************
 *  MemDriver_RDeinitSync
 **********************************************************************************************************************/
/*! \brief      Deinitialize all memory drivers.
 *  \pre        Memory drivers have to be present (e.g. copied to RAM).
 *  \param[in]  address Parameter to be passed to HIS interface flash driver.
 *  \return     The return code shows the success of the deinitialization.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_RDeinitSync( void * address )
{
   IO_U8 i;
   IO_ErrorType result = IO_E_OK;

   for (i = 0; i < kNrOfMemDrv; i++)
   {
      result = (*memDrvLst[i].deinit)(address);
      if (result != IO_E_OK)
      {
         return result;
      }
   }
   return result;
}

#else
void V_API_NEAR dummy( void )
{
   ;
}
#endif

/***********************************************************************************************************************
 *  END OF FILE: FBL_MIO.C
 **********************************************************************************************************************/
