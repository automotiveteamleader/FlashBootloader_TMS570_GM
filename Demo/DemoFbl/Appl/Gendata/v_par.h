/* -----------------------------------------------------------------------------
  Filename:    v_par.h
  Description: Toolversion: 05.03.00.01.40.05.01.00.00.00
               
               Serial Number: CBD1400501
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP5
                              Micro: TMS5700714PGEQQ1
                              Compiler: TI Code Composer 4.9.5
               
               
               Generator Fwk   : GENy 
               Generator Module: GenTool_GenyVcfgNameDecorator
               
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

#if !defined(__V_PAR_H__)
#define __V_PAR_H__

/* -----------------------------------------------------------------------------
    &&&~ BaseEnv_PHF_Includes
 ----------------------------------------------------------------------------- */

#include "v_cfg.h"
#include "v_def.h"


/* -----------------------------------------------------------------------------
    &&&~ GENy Version Information
 ----------------------------------------------------------------------------- */

#define VGEN_DELIVERY_VERSION_BYTE_0         0x05
#define VGEN_DELIVERY_VERSION_BYTE_1         0x03
#define VGEN_DELIVERY_VERSION_BYTE_2         0x00
#define VGEN_DELIVERY_VERSION_BYTE_3         0x01
#define VGEN_DELIVERY_VERSION_BYTE_4         0x40
#define VGEN_DELIVERY_VERSION_BYTE_5         0x05
#define VGEN_DELIVERY_VERSION_BYTE_6         0x01
#define VGEN_DELIVERY_VERSION_BYTE_7         0x00
#define VGEN_DELIVERY_VERSION_BYTE_8         0x00
#define VGEN_DELIVERY_VERSION_BYTE_9         0x00
#define kGENyVersionNumberOfBytes            10
/* ROM CATEGORY 4 START */
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 kGENyVersion[kGENyVersionNumberOfBytes];
/* ROM CATEGORY 4 END */




/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\v_par.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 182747247
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __V_PAR_H__ */
