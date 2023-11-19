/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         HIS IO driver types
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2012 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Slawomir Wilk                 swk           Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2003-08-06  WM                       First implementation
 *  01.01.00   2004-09-22  WM                       Added memory qualifier for IO_MemPtrType for C_COMP_TASKING
 *  01.02.00   2005-03-01  AMN                      Added support for TI_TMS320
 *  01.03.00   2005-06-13  Hp                       Added HUGE qualifier for V850/aFCAN/IAR
 *  01.04.00   2005-10-12  WM                       MEMORY_FAR for M16C/M32C added Changed IO_E_OK definition
 *  01.05.00   2006-09-14  SWk                      Added support for KEIL and ST10F27x
 *  01.06.00   2007-11-22  JHg     ESCAN00023241    C_COMP_TASKING_XC2000_MULTICAN added
 *  01.07.00   2008-05-07  WM                       Removed MEMORY_FAR defintion for M16C
 *  01.08.00   2009-05-26  Ach                      Added support for Tasking VX toolchain on XC2000
 *  01.09.00   2009-11-02  QPs                      Added IO_E_NOT_OK define
 *  01.10.00   2011-05-09  ASe     ESCAN00050240    Modified support for TI_TMS320
 *  01.11.00   2011-07-26  CB                       16 bit position- and size types for non-banked 8-bit CPUs
 *  01.12.00   2012-11-13  Ach     ESCAN00062912    Adapted comments to use Doxygen
 *  01.13.00   2013-01-21  Ach     ESCAN00064406    Add return value definition IO_E_ERASED
 **********************************************************************************************************************/

#ifndef __IO_TYPES_H__
#define __IO_TYPES_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "v_cfg.h"
#include "v_def.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define IO_E_OK       ((vuint8)0x00u)           /**< Memory driver action successful */
#define IO_E_NOT_OK   ((vuint8)0x01u)           /**< Memory driver action failed */
#define IO_E_ERASED   ((vuint8)0xE5u)           /**< Read failure (e.g. ECC), but memory is assumed to be erased */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

typedef vuint8    IO_U8;
typedef vuint16   IO_U16;
typedef vuint32   IO_U32;
typedef vsint8    IO_S8;
typedef vsint16   IO_S16;
typedef vsint32   IO_S32;

#if ( !defined( C_CPUTYPE_8BIT ) )
typedef vuint32   IO_SizeType;
typedef vuint32   IO_PositionType;
#else
typedef vuint16   IO_SizeType;
typedef vuint16   IO_PositionType;
#endif

typedef vuint16   IO_ErrorType;
typedef vuint8*   IO_MemPtrType;

#endif /* __IO_TYPES_H__ */

/***********************************************************************************************************************
 *  END OF FILE: IOTYPES.H
 **********************************************************************************************************************/
