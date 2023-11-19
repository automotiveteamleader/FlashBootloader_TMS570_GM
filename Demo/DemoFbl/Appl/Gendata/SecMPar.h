/* -----------------------------------------------------------------------------
  Filename:    SecMPar.h
  Description: Toolversion: 05.03.00.01.40.05.01.00.00.00
               
               Serial Number: CBD1400501
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP5
                              Micro: TMS5700714PGEQQ1
                              Compiler: TI Code Composer 4.9.5
               
               
               Generator Fwk   : GENy 
               Generator Module: SysService_SecModHis
               
               Configuration   : D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Config\DemoFbl.gny
               
               ECU: 
                       TargetSystem: Hw_Tms470/570Cpu (Dcan)
                       Compiler:     TexasInstruments
                       Derivates:    TMS570LS0322
               
               Channel "Channel0":
                       Databasefile: D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Config\demo_sw.dbc
                       Bussystem:    CAN
                       Manufacturer: General Motors
                       Node:         DUT

 ----------------------------------------------------------------------------- */
/* -----------------------------------------------------------------------------
  C O P Y R I G H T
 -------------------------------------------------------------------------------
  Copyright (c) 2001-2015 by Vector Informatik GmbH. All rights reserved.
 
  This software is copyright protected and proprietary to Vector Informatik 
  GmbH.
  
  Vector Informatik GmbH grants to you only those rights as set out in the 
  license conditions.
  
  All other rights remain with Vector Informatik GmbH.
 -------------------------------------------------------------------------------
 ----------------------------------------------------------------------------- */

#if !defined(__SECMPAR_H__)
#define __SECMPAR_H__

#include "SecM_inc.h"
#define SECMPAR_START_SEC_DATA
#include "MemMap.h"
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 SecM_RsaMod[256];
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 SecM_RsaExp[4];
#define SECMPAR_STOP_SEC_DATA
#include "MemMap.h"

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\SecMPar.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 182747247
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __SECMPAR_H__ */
