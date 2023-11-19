/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Get CAN-ID Parameters and make settings for the HW-File
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
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Thomas Sommer                 Ths           Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Alexandre C. Plombin          ACP           Vector CANtech, Inc.
 *  Ahmad Nasser                  An            Vector CANtech, Inc.
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Florian Hees                  FHE           Vector Informatik GmbH
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Slawomir Wilk                 SWk           Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2001-07-12  Ls                       Creation
 *  01.10.00   2001-08-23  Ls                       M16C added
 *  01.11.00   2001-10-10  CB                       Functional receive identifier configurable
 *  01.12.00   2001-11-23  Ls                       ST10 added
 *  01.13.00   2001-11-28  CB                       Symbolic constants for transmit object added
 *  01.14.00   2001-11-29  Ls                       startmessage and addresslength added for ST10
 *  01.15.00   2001-12-06  Ls                       TMS470 added, CAN_RX_SLOT_STARTMSG, kTpTxObj
 *                                                  put global
 *  01.16.00   2001-01-22  Ls                       CLIENT_FORD support added, START_MSG_ID added
 *                         CB                       Minor changes
 *  01.17.00   2002-02-15  CB                       Range enabled if NM is supported
 *  01.18.00   2002-03-03  Hp                       Adding support for CLIENT_VAG
 *  01.19.00   2002-04-05  Ls                       Support for FJ16LX CLIENT_GM
 *                         Ths                      Support for HC08 and VWTP1.6
 *  01.20.00   2002-04-24  Fz                       Support for V850 added
 *             2002-04-06  Th                       Changed ST10 compiler define into C16x
 *             2002-05-08  WM                       Added support for Hiware/IAR HC12 compiler
 *             2002-05-10  Ls                       Minor changes, ping for CLIENT_VECTOR comes through slot 0
 *             2002-05-15  CB      ESCAN00002840    Range mask for NM messages changed
 *  01.21.00   2002-05-16  Fz      ESCAN00002834    Change of stay in boot time
 *  01.22.00   2002-05-16  Th                       Added new CLIENT_CLAAS
 *             2002-05-23            WM      Macros FblFlashAddressGetFblDownloadLengthGet
 *                                                  removed
 *  01.23.00   2002-06-28  WM                       Added support for Metrowerks/Diab Data PPC
 *                         Th                       Added support for extended CAN identifier
 *  01.24.00   2002-08-14  Ls      ESCAN00003496    FBL_CAN_RX_RANGE_MASK for C16X
 *                                                  changed
 *  01.25.00   2002-08-14  ACP                      Added ST9 support+START_DEBUG_DELAY
 *  01.26.00   2002-10-01  Ls                       Minor changes regarding debugging
 *  01.26.00   2002-10-02  Fz                       Add support for SH7047
 *  01.27.00   2002-11-02  WM                       Added multiple nodes support for CLIENT_DC and
 *                                                  CLIENT_FORD
 *  01.28.00   2003-02-04  An                       Added C_COMP_TASKING_XC16X
 *  01.28.00   2003-02-21  CB                       Added C_COMP_HEW_SH705X
 *                         WM      ESCAN00004821    Removed CAN_CIDAC define for
 *                                                  HC08/HC12
 *  01.29.00   2003-03-27  CB                       Range configuration for XC16X changed
 *  01.30.00   2003-04-01  Rr                       Range configuration for V850SC3 added
 *  01.31.00   2003-04-07  Rr                       Removed C_COMP_ANSI_TMS_470
 *  01.32.00   2003-04-12  Fz                       Some changes for CLIENT_VAG (range defines)
 *                         WM                       Minor changes for PPC
 *                         Fz                       Minor changes for CLIENT_VAG
 *  01.33.00   2003-05-16  CB      ESCAN00005681    Support for ST10-CCAN
 *  01.34.00   2003-05-23  Ls      ESCAN00005003    Support for SH7047
 *  01.35.00   2003-06-25  Ls                       Minor changes for CLIENT_VECTOR
 *  01.36.00   2003-08-11  CB                       Minor changes
 *                         WM      ESCAN00006319    Multiple node support for
 *                                                  CLIENT_CLAAS
 *                                 ESCAN00006294    C_ENABLE_EXTENDED_ID
 *  01.37.00   2003-08-28  Fz      ESCAN00006460    Confirmation disabled for
 *                                                  CLIENT_VAG
 *                         CB      ESCAN00005872    Support for 78K0
 *  01.38.00   2003-09-16  Ls      ESCAN00006611    Support for M32R, minor changes
 *  01.39.00   2004-01-19  Ls      ESCAN00007291    New OEM
 *             2004-02-25  Rr                       Added support for M32C
 *  01.40.00   2004-02-27  Rr                       Added new OEM
 *  01.41.00   2004-03-24  ACP                      Modified Ford & GGDS
 *                         WM                       Changed kCanNumberOfTxObj define for CLIENT_DC
 *  01.42.00   2004-05-14  Ls      ESCAN00008376    New OEM
 *  01.43.00   2004-07-16  CB      ESCAN00008927    New OEM
 *  01.44.00   2004-08-20  WM      ESCAN00009234    Extended ECU PIN handling for
 *                                                  CLIENT_CLAAS
 *                                                  Added extended addressing for CLIENT_CLAAS
 *                         Ls                       Added multiple node support for CLIENT_PAG
 *  01.45.00   2004-11-11  Rr                       Minor changes for CLIENT_RENAULT
 *  01.46.00   2004-12-10  Hp                       Adding range-support for TLCS-900.
 *                         AMN                      Resolved conflict with CAN_TP_TXID multiply
 *                                                  defined
 *                         AMN                      Split FBL_CLIENT_FORD_GGDS_COMMENT and
 *                                                  FBL_CLIENT_FORD_COMMENT
 *                         AMN                      FBL_CLIENT_GGDS changed to FBL_CLIENT_FORD_GGDS
 *                         MHz                      Changed define START_MSG_ID for CLIENT_GM
 *  01.47.00   2005-01-13  CB                       Changes for SLP8
 *  01.48.00   2005-02-21  AMN                      Resolved conflict with CAN_TP_TXID multiply
 *                                                  defined for Ford_SLP3
 *  01.49.00   2005-03-30  Hp                       Provide additional define for 2nd StartMsg-ID
 *                                                  (CLIENT_FORD_GGDS only)
 *  01.50.00   2005-05-06  Hp                       Add support for V85x/IAR
 *                         Rr                       Add new OEM
 *                         WM                       Support for C_COMP_MITSUBISHI_M16C/M32C added
 *  01.51.00   2005-06-08  FHE                      Added support for DSP56F83XX and MAC71XX
 *  01.52.00   2005-09-07  MFR                      Changes for GM-FBL v2.0
 *                         WM                       Support for C_COMP_DIABDATA_MPC55XX added
 *  01.53.00   2005-11-14  MFR                      Match GM-FBL SLP2 definitions to GENy.
 *                                                  Make FBL_START_DELAY definition conditional for GM
 *                                                  Minor changes after implementation review
 *                         FHE                      Added support for C_COMP_TASKING_TRICORE_MULTICAN
 *  01.54.00   2005-11-28  SWk                      Added support for XC16X
 *                                 ESCAN00014462    Rename CanMsgObj to fblCanTxObj
 *  11.54.01   2006-01-19  Hp                       Remove configuration switch C_COMP_IAR_78k0
 *                                                  from source code
 *  01.55.00   2006-01-27  WM                       Added range support for C_COMP_FUJITSU_FR60_CCAN
 *  01.56.00   2006-04-21  Hp                       Add support for PAG_SLP3
 *                         Ls                       Minor changes for PAG_SLP3
 *                         AMN                      Corrected the slot numbers for FORD_GGDS
 *                         WM                       Changes for CLIENT_FIAT
 *                         AMN                      Changed slot numbers definitions after review
 *  01.57.00   2006-07-06  FHE                      Added multiple nodes support for PAG
 *                                                  Added support for VECTOR SLP3 and VAG SLP2
 *  01.58.00   2006-08-09  Hp      ESCAN00018815    Add support for GM-S2F SLP3
 *  01.59.00   2006-12-21  Hp      ESCAN00018814    Add support for HMC
 *                         Cb      ESCAN00018080    New OEM
 *                         Hp                       Add optional support for CCP
 *  01.60.00   2007-02-12  Hp                       Add common defines for all CAN-hardware systems.
 *                         CB                       Minor changes
 *                         SWk                      Added support for DC SLP6 for MCS08
 *                         Hp                       Add CAN_CONFIRMATION for HMC.
 *                         FHE                      Added support for C_COMP_VDSP_BF5XX
 *                         JHg                      Moved CAN_CONFIRMATION for HMC
 *  01.61.00   2007-04-23  MFR     ESCAN00019405    Use kFblDiagNodeAddr[]
 *                                                  instead of fbl_hw.c::NmEcuNr[]
 *                                                  to store ECU Diagnostic Address for GM
 *                                                  Multiple-Identity-Modules (MIM).
 *                         SWk                      Support for C_COMP_METROWERKS_MPC55XX_FLEXCAN
 *                         FHe     ESCAN00020931    VAG SLP2 renamed to SLP3
 *                                                  Added support for FUJITSU_16FX (FBL_CAN_RX_RANGE)
 *  01.62.00   2007-08-01  Swk                      Support for C_COMP_HEW_SH2_RCAN
 *  01.63.00   2007-08-06  CB                       Multiple nodes support for Fiat UDS
 *  01.64.00   2007-08-10  Swk                      Added support for C_COMP_MICROCHIP_PIC18
 *  01.65.00   2007-10-17  FHe                      Added cast for VW TP20
 *  01.66.00   2007-11-22  AWh                      Added range support for C_COMP_GHS_470
 *                         JHg                      Added range support for C_COMP_TASKING_XC2000_MULTICAN
 *                         FHe                      Added support for C_COMP_IAR_R32C_COMMENT
 *                                                  Corrected RX_RANGE_MASK for M32C
 *  01.67.00   2008-01-14  Hp                       Define TpHandle when using the generated ftp_cfg.h
 *                         FHe                      Set FBL_ENABLE_CAN_CONFIRMATION for VW TP20
 *  01.68.00   2008-08-15  Rr                       Add multiple node support for Renault/Nissan
 *  01.69.00   2008-09-09  Rr                       New OEM
 *                         Ls                       C_COMP_GHS_MPC55XX added
 *  01.70.00   2009-03-03  FHe                      Added support for C_COMP_MTRWRKS_MPC5X00_COMMENT
 *  01.71.00   2009-05-19  SWk                      Added support for C_COMP_MICROCHIP_DSPIC33
 *                         QPs                      kTpTxHandle renamed to kFblTpTxHandle for compatibility
 *                                                  with FBLTP > 0303
 *  01.72.00   2009-06-28  Ach                      Add new OEM GAC (SLP1)
 *                         AWh                      Added support for OEM Fisker
 *                         Rr                       Added support for OEM Saic
 *  01.73.00   2009-07-01  Ach                      Additional change for OEM GAC (SLP1)
 *  01.74.00   2009-10-28  AWh                      Adopt Claas CAN_RX_SLOT_STARTMSG - handling to changed
 *                                                  file structure.
 *  01.75.00   2010-04-22  CB      ESCAN00042462    New OEM
 *  01.76.00   2010-10-22  Rr                       Completely removed functional slot for Renault SLP3
 *  01.77.00   2011-02-03  Ach     ESCAN00048156    Support normal fixed addressing with
 *                                                  bootloader TP/CAN driver
 *  01.78.00   2011-04-19  SWk     ESCAN00050241    Add support for C_COMP_IAR_78K0R and CAN_RX_RANGE
 *  01.79.00   2011-05-27  Rr                       Reworked OEM Renault
 *  01.80.00   2011-08-24  TBe     ESCAN00053015    Reworked normal fixed addressing
 *                         Ci                       Ford: Removed CAN identifier mapping
 *  01.81.00   2011-08-30  SWk                      Adapt FBL_RX_RANGE_MASK for VW TP2.0
 *  01.82.00   2012-01-13  CB      ESCAN00055503    Support for multiple platforms
 *                         AWh                      Added OEM Toyota
 *                         Ach                      Allow usage of new style TP handle macros with old TP
 *                         Ci                       Added support for XCP
 *  01.83.00   2012-02-23  Hp                       Remove unused CAN-objects when using XCP-FBL
 *  01.84.00   2012-04-20  AWh                      Added Gm SLP4
 *                         Ci                       Added OEM VolvoCar
 *  01.85.00   2012-06-22  Ci                       VolvoCar: Removed Rx range, added 3rd receive
 *                                                  object for functionally addressed messages,
 *                                                  enabled stay-in-boot for 2nd receive object
 *  01.86.00   2012-07-23  Ach     ESCAN00060297    Reworked CAN identifier handling
 *  01.87.00   2012-08-10  CB                       Adaptations for multiple platforms
 *  01.88.00   2012-10-23  Ach                      Add Stay-in-Boot for reworked CAN IDs
 *  01.89.00   2013-02-04  ASe     ESCAN00064581    Adapted comments to use Doxygen
 *  01.90.00   2013-02-18  CB                       Support for Fiat SLP4
 *  01.91.00   2013-04-30  Ach     ESCAN00066298    Support Normal Fixed Addressing with reworked ID handling
 *  01.92.00   2013-05-08  Ach     ESCAN00067186    Support XCP Bootloader addon with ComWrapper
 *  01.93.00   2013-11-12  HRf     ESCAN00071761    Support changed CAN ID handling
 *  01.94.00   2014-02-13  AWh                      Support for Gm SLP6
 *                         Ci                       Support for Ford SLP7 (cease support for SLP3 and earlier)
 *                         MVi                      Enabled new CAN ID handling for HMC
 *  01.95.00   2014-04-30  HRf     ESCAN00075329    Add support for OEM PATAC
 *  01.96.00   2014-05-05  HRf     ESCAN00075366    Correct slot number for Force Boot Mode in case of OEM PATAC
 *  01.97.00   2014-07-25  Ci      ESCAN00077030    Fixed start message ID handling for single receive buffer
 *                                                  configurations
 *                         AWh     ESCAN00077137    Updated: Support for Gm SLP6
 *                         Ach     ESCAN00077367    Remove identifier creation macros from XCP RX IDs
 *  01.98.00   2014-10-14  AWh     ESCAN00078924    Gm SLP6: Allow Stay in Boot configuration
 *  01.99.00   2015-01-09  AWh     ESCAN00080238    Gm SLP6: Allow Multiple node feature on second Rx handle
 **********************************************************************************************************************/

#ifndef __FBL_CAN_H__
#define __FBL_CAN_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblCan CQComponent : Implementation */
#define FBLCAN_VERSION                             0x0199u
#define FBLCAN_RELEASE_VERSION                     0x00u

/***********************************************************************************************************************
 *  COMMON DEFINES FOR CAN HARDWARE (SEE FBL_HW.C)
 **********************************************************************************************************************/

/* FblCanTransmit() and FblCanMsgTransmitted() return values */
#define kFblCanTxOk                 0   /* FblCanTransmit()   : Transmission request accepted. */
                                        /* FblCanTransmitted(): Transmission now completed.    */
#define kFblCanTxFailed             1   /* FblCanTransmit()   : Transmission request failed.   */
                                        /* FblCanTransmitted(): No transmission in progress    */
#define kFblCanTxInProgress         2   /* FblCanTransmit()   : Not allowed                    */
                                        /* FblCanTransmitted(): Transmission still in progress */

/* FblCanSleep() return values */
#define kFblCanSleepOk              0
#define kFblCanSleepFailed          1
#define kFblCanSleepNotSupported    2

/* FblCanStart/FblCanStop (may not be supported by all hardware platforms) */
#define kFblCanHaltOk               0
#define kFblCanHaltFailed           1

#if defined( OSEKTP_CFG_VERSION )
/* Backward compatibility to old-style, non-generated ftp_cfg.h */
#else
/* For TP, to communicate with hw_xx file */
#  if ( _FBLTP_VERSION < 0x0303 )
#   define kTpTxHandle              (&(fblCanTxObj[kTpTxObj]))
#   define kFblTpTxHandle           kTpTxHandle
#  else
#   define kFblTpTxHandle           (&(fblCanTxObj[kTpTxObj]))
#  endif
#   define tpCanTxData              fblCanTxObj[kTpTxObj].msgObject.DataFld
#endif /* OSEKTP_CFG_VERSION */

/***********************************************************************************************************************
 *  END COMMON DEFINES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_MULTIPLE_NODES )
/* Handle for ID selection */
extern vuint8 fblEcuHandle;
#endif

/* CAN identifiers */
# if !defined( FBL_START_DELAY )
#  define FBL_START_DELAY        (12u / FBL_REPEAT_CALL_CYCLE) /**< Wait 12 ms before jump into application */
# endif

# define kNrOfTxObjects          1                          /**< This FBL uses exactly one transmit object */
# define CAN_RX_SLOT_STARTMSG    CAN_RX_SLOT_1

#   if defined( FBL_ENABLE_MULTIPLE_NODES )

#    define   ComSetCurrentECU(ecu)              (fblEcuHandle = (ecu))
#    define   DIAG_NODE_ADDRESS                  kFblDiagNodeAddr[fblEcuHandle]

#    if defined( FBL_ENABLE_CAN_CONFIGURATION )
#     define  DIAG_NODE_PHYSICAL_REQUEST_ID      CanRxId[fblEcuHandle]              /**< Physical Request */
#     define  DIAG_NODE_PHYSICAL_RESPONSE_ID     CanTxId[fblEcuHandle]              /**< Physical Request */
#    else
#     define  DIAG_NODE_PHYSICAL_REQUEST_ID      DIAG_NODE_ADDRESS                  /**< Physical Request */
#     define  DIAG_NODE_PHYSICAL_RESPONSE_ID     (DIAG_NODE_ADDRESS + 0x300u)       /**< Physical Request */
#    endif/* FBL_ENABLE_CAN_CONFIGURATION */
#   else
#    if defined( FBL_ENABLE_CAN_CONFIGURATION )
#     define  DIAG_NODE_PHYSICAL_REQUEST_ID      CAN_TP_RXID                        /**< Physical Request */
#     define  DIAG_NODE_PHYSICAL_RESPONSE_ID     CAN_TP_TXID                        /**< Physical Request */
#    else
#     define  DIAG_NODE_PHYSICAL_REQUEST_ID      DIAG_NODE_ADDRESS                  /**< Physical Request */
#     define  DIAG_NODE_PHYSICAL_RESPONSE_ID     (DIAG_NODE_ADDRESS + 0x300u)       /**< Physical Request */
#    endif/* FBL_ENABLE_CAN_CONFIGURATION */
#   endif/* FBL_ENABLE_MULTIPLE_NODES */

# if defined( FBL_ENABLE_SLEEPMODE )
#  define FBL_CAN_RX_ID2            MK_ID(0x100u)
# endif
# define FBL_DISABLE_CAN_RX_RANGE

/* TX handle for the transport protocol */
# define kTpTxObj 0

/* Map the receive-ID0 to the pre-set customer-specific CAN-ID */
#  if defined( FBL_ENABLE_MULTIPLE_NODES ) 
#   define FBL_CAN_RX_ID0            0
#  else
#     define FBL_CAN_RX_ID0          MK_ID(DIAG_NODE_PHYSICAL_REQUEST_ID)
#  endif/* FBL_ENABLE_MULTIPLE_NODES || FBL_ENABLE_MULTIPLE_PLATFORMS */

/* Functional Request */
#   if defined( CAN_TP_FUNC_RXID )
#     define FBL_CAN_RX_ID1          MK_ID(CAN_TP_FUNC_RXID)
#   endif/* CAN_TP_FUNC_RXID */
#   if defined( CAN_TP_FUNC_RXID2 )
#     define FBL_CAN_RX_ID2          MK_ID(CAN_TP_FUNC_RXID2)
#   endif/* CAM_TP_FUNC_RXID2 */

   /* The "ping" comes through this message slot */
#   ifndef CAN_RX_SLOT_STARTMSG
#     define CAN_RX_SLOT_STARTMSG    CAN_RX_SLOT_1
#   endif

#if defined( C_ENABLE_CCP )
# if !defined( FBL_CAN_RX_ID0 )
#  define FBL_CAN_RX_ID0          MK_ID(CCP_CRO_ID)
# elif !defined( FBL_CAN_RX_ID1 )
#  define FBL_CAN_RX_ID1          MK_ID(CCP_CRO_ID)
# elif !defined( FBL_CAN_RX_ID2 )
#  define FBL_CAN_RX_ID2          MK_ID(CCP_CRO_ID)
# elif !defined( FBL_CAN_RX_ID3 )
#  define FBL_CAN_RX_ID3          MK_ID(CCP_CRO_ID)
# elif !defined( FBL_CAN_RX_ID4 )
#  define FBL_CAN_RX_ID4          MK_ID(CCP_CRO_ID)
# endif
#endif

/* Receive Configuration */
#if defined( FBL_CAN_RX_ID4 )
# define FBL_CAN_NUMBER_OF_RX_ID 5
#else
# if defined( FBL_CAN_RX_ID3 )
#  define FBL_CAN_NUMBER_OF_RX_ID 4
# else
#  if defined( FBL_CAN_RX_ID2 )
#   define FBL_CAN_NUMBER_OF_RX_ID 3
#  else
#   if defined( FBL_CAN_RX_ID1 )
#    define FBL_CAN_NUMBER_OF_RX_ID 2
#   else
#    if defined( FBL_CAN_RX_ID0 )
#     define FBL_CAN_NUMBER_OF_RX_ID 1
#    else
#     define FBL_CAN_NUMBER_OF_RX_ID 0
#    endif
#   endif
#  endif
# endif
#endif

/************************************************************************************/
/***                             Range - Setup                                    ***/
/************************************************************************************/

#endif /* __FBL_CAN_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_CAN.H
 **********************************************************************************************************************/
