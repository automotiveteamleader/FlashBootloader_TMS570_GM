/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Indirection of vectortable for TMS470
 *  \details       This file is usually compiled and linked with the bootloader and is located  O U T S I D E  the
 *                 protected area of the FBL. So it could be compiled, linked and downloaded with the application
 *                 after adapting the interrupt entry points to the application interrupt vectors.
 *                 The application service routines are still interrupts!
 *
 *                 This file must be compiled and linked in the bootloader
 *                 and the application to the SAME MEMORY LOCATION!
 *
 *  \note          Please note, that this file contains example source code used by the Flash Bootloader. These
 *                 functions may influence the behavior of the bootloader in principle. Therefore, great care must
 *                 be taken to verify the correctness of the implementation. The contents of the originally delivered
 *                 files are only examples resp. implementation proposals. With regard to the fact that these
 *                 functions are meant for demonstration purposes only, Vector Informatik's liability shall be
 *                 expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Philip Lapczynski             PAL           Vector CANtech, Inc
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Johannes Krimmel              KJs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2002-01-23  Ls                       Derived from equivalent HC08 file
 *  01.01.00   2002-09-11  Ls                       no changes
 *  01.02.00   2002-11-15  Ls                       no changes
 *  01.03.00   2003-04-30  Rr                       minor changes
 *  01.04.00   2006-07-06  Ls                       no changes
 *  01.05.00   2007-11-05  AWh                      Added C_COMP_GHS_470
 *  01.06.00   2008-05-15  AWh                      Added cast to magic bytes to work with arm compiler
 *  01.07.00   2009-02-27  Ci      ESCAN00029214    Added support for TI ARM9 ABI
 *                         JHg     ESCAN00033357    Added support for TI OMAP
 *  01.08.00   2009-09-24  MVi     ESCAN00038014    No changes
 *  01.09.00   2010-03-31  Pal     ESCAN00042010    Added support for C_PROCESSOR_TMS470_MSF542
 *  01.10.00   2011-08-25  MVi     ESCAN00053026    No changes
 *  01.11.00   2013-07-04  Ach     ESCAN00061453    Support TMS470MF0420
 *                         ASe     ESCAN00068149    Adapted comments to use doxygen
 *  01.12.00   2015-02-12  KJs     ESCAN00079779    Rework of TMS470M Filter rules
 **********************************************************************************************************************/

#ifndef APPL_VECT470_H
#define APPL_VECT470_H

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblVtabAppl_Tms470 CQComponent : Implementation */
#define FBLVTABAPPL_TMS470_VERSION             0x0112u
#define FBLVTABAPPL_TMS470_RELEASE_VERSION     0x00u

/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

#define RSTVECT_INDEX         0x00u

/* Reserved for Bootloader internal value                */
/* It is not allowed to set this magic code              */
/* for the application vector table, just                */
/* for the table linked with the bootloader!             */
#define APPLVECT_FROM_BOOT    (unsigned char)0xA5u

/* Set only this label for the user                      */
/* application vector table 0xEA => Branch-instruction!  */
#define APPLVECT_FROM_APPL    (unsigned char)0xEAu

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/** This structure is placed at the reset vector of the application */
typedef struct
{
  unsigned char opcode;
} tIntJmpTable;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

extern const tIntJmpTable ApplIntJmpTable[1];

#endif /* APPL_VECT470_H */

/***********************************************************************************************************************
 *  END OF FILE: APPLVECT.H
 **********************************************************************************************************************/
