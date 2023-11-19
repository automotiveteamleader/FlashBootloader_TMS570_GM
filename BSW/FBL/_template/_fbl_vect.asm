;/***********************************************************************************************************************
; *  FILE DESCRIPTION
; *  ------------------------------------------------------------------------------------------------------------------*/
;/** \file
; *  \brief         Interrupt vector table for TMS470 / OMAP
; *
; *  --------------------------------------------------------------------------------------------------------------------
; *  COPYRIGHT
; *  --------------------------------------------------------------------------------------------------------------------
; *  \par Copyright
; *  \verbatim
; *  Copyright (c) 2013 by Vector Informatik GmbH.                                                  All rights reserved.
; *
; *                This software is copyright protected and proprietary to Vector Informatik GmbH.
; *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
; *                All other rights remain with Vector Informatik GmbH.
; *  \endverbatim
; */
;/**********************************************************************************************************************/
;
;/***********************************************************************************************************************
; *  AUTHOR IDENTITY
; *  --------------------------------------------------------------------------------------------------------------------
; *  Name                          Initials      Company
; *  --------------------------------------------------------------------------------------------------------------------
; *  Robert Schaeffner             Rr            Vector Informatik GmbH
; *  Andreas Wenckebach            AWh           Vector Informatik GmbH
; *  Joern Herwig                  JHg           Vector Informatik GmbH
; *  Marcel Viole                  MVi           Vector Informatik GmbH
; *  Alexander Starke              ASe           Vector Informatik GmbH
; *  --------------------------------------------------------------------------------------------------------------------
; *  REVISION HISTORY
; *  --------------------------------------------------------------------------------------------------------------------
; *  Version    Date        Author  Change Id        Description
; *  --------------------------------------------------------------------------------------------------------------------
; *  01.00.00   2005-11-14  Rr      -                Merged Ti and Arm sources to one file added history
; *  01.01.00   2007-10-21  AWh     -                Added  C_COMP_GHS_470
; *  01.02.00   2009-02-27  JHg     ESCAN00033358    Added support for TI OMAP
; *  01.03.00   2009-09-24  MVi     ESCAN00038014    Added support for TI TMS570
; *  01.04.00   2013-07-04  ASe     ESCAN00068663    Adapted comments to use doxygen
; **********************************************************************************************************************/

   .state32
   .global _c_int00
   .global _isrirq
   .global _isrfiq
   .global _isruii
   .global _israpi
   .global _isradi
   .global _isrswi

   .sect ".intvecs"

  B  _c_int00      ;   RESET INTERRUPT

  B  _isruii       ;   UNDEFINED INSTRUCTION INTERRUPT
  B  _isrswi       ;   SOFTWARE INTERRUPT DISPATCHER
  B  _israpi       ;   ABORT (PREFETCH) INTERRUPT
  B  _isradi       ;   ABORT (DATA) INTERRUPT
  B  _c_int00      ;   RESERVED init with existing jump lable
  B  _isrirq       ;   IRQ INTERRUPT
  B  _isrfiq       ;   FIQ INTERRUPT

   .end

/***********************************************************************************************************************
 *  END OF FILE: FBL_VECT.C
 **********************************************************************************************************************/
