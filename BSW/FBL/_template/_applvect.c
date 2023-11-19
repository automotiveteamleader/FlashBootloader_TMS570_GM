/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Indirection of vector table for TMS470/TMS570
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
 *  Christian Baeuerle            Cb            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Florian Hees                  FHE           Vector Informatik GmbH
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
 *  01.01.00   2002-09-11  Ls      ESCAN00003749    isrswi added
 *  01.02.00   2002-11-15  Ls                       appl_vect_base -> APPLVECT
 *  01.03.00   2005-09-07  FHE                      No changes, correcting version number
 *  01.04.00   2006-07-06  Ls                       added ".state32" to use with compiler version 2.54
 *  01.05.00   2007-11-05  AWh                      added support for C_COMP_GHS_470
 *  01.06.00   2008-02-29  AWh                      Merged vector tables for TI and GHS, changed reset
 *                                                  vector data word to also work with arm compiler.
 *  01.07.00   2009-02-27  Ci      ESCAN00029214    Added support for TI ARM9 ABI
 *                         JHg     ESCAN00033357    Added support for TI OMAP
 *  01.08.00   2009-09-24  MVi     ESCAN00038014    Added support for TI TMS570
 *  01.09.00   2010-03-31  Pal     ESCAN00042010    Added support for C_PROCESSOR_TMS470_MSF542
 *  01.10.00   2011-08-25  MVi     ESCAN00053026    Added support for MSF542 with TI compiler
 *  01.11.00   2013-07-04  Ach     ESCAN00061453    Support TMS470MF0420
 *                         ASe     ESCAN00068149    Adapted comments to use doxygen
 *  01.12.00   2015-02-12  KJs     ESCAN00079779    Rework of TMS470M Filter rules
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "applvect.h"

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

#if ( FBLVTABAPPL_TMS470_VERSION != 0x0112u ) || \
    ( FBLVTABAPPL_TMS470_RELEASE_VERSION != 0x00u )
#error "Error in APPLVECT.C: Source and header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

asm("   .state32");
asm("   .global _isrirq");
asm("   .global _isrfiq");
asm("   .global _isruii");
asm("   .global _israpi");
asm("   .global _isradi");
asm("   .global _isrswi");

#pragma DATA_SECTION (ApplIntJmpTable, ".APPLVECT")
/* ApplIntJmpTable is located on the RESET INTERRUPT   */
const tIntJmpTable ApplIntJmpTable[1] = {APPLVECT_FROM_BOOT};

/* This are the other Interruptvectors                                     */
asm("   .sect \".APPLVECT\"");

asm("   .if __TI_ARM9ABI_ASSEMBLER");
asm("   .armfunc _isruii");
asm("   .endif");
asm("_isruii:");
asm("             B  _dummy"); /* UNDEFINED INSTRUCTION INTERRUPT          */

asm("   .if __TI_ARM9ABI_ASSEMBLER");
asm("   .armfunc _isrswi");
asm("   .endif");
asm("_isrswi:");
asm("             B  _dummy"); /* SOFTWARE INTERRUPT                       */

asm("   .if __TI_ARM9ABI_ASSEMBLER");
asm("   .armfunc _israpi");
asm("   .endif");
asm("_israpi:");
asm("             B  _dummy"); /* ABORT (PREFETCH) INTERRUPT               */

asm("   .if __TI_ARM9ABI_ASSEMBLER");
asm("   .armfunc _isradi");
asm("   .endif");
asm("_isradi:");
asm("             B  _dummy"); /* ABORT (DATA) INTERRUPT                   */
asm("             B  _dummy"); /* RESERVED                                 */

asm("   .if __TI_ARM9ABI_ASSEMBLER");
asm("   .armfunc _isrirq");
asm("   .endif");
asm("_isrirq:");
asm("             B  _dummy"); /* IRQ INTERRUPT                            */

asm("   .if __TI_ARM9ABI_ASSEMBLER");
asm("   .armfunc _isrfiq");
asm("   .endif");
asm("_isrfiq:");
asm("             B  _dummy"); /* FIQ INTERRUPT                            */

asm("   .if __TI_ARM9ABI_ASSEMBLER");
asm("   .armfunc _dummy");
asm("   .endif");
asm("_dummy:");
asm("             B  _dummy"); /* LOOP                                     */

/***********************************************************************************************************************
 *  END OF FILE: APPLVECT.C
 **********************************************************************************************************************/
