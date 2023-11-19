/* -----------------------------------------------------------------------------
  Filename:    SecMPar.c
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

#include "SecM_inc.h"
#define SECMPAR_START_SEC_DATA
#include "MemMap.h"
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 SecM_RsaMod[256] = 
{ 
 	0xb1, 0xc5, 0xbe, 0x46, 0x9a, 0x4f, 0x56, 0x09, 0x8b, 0x1b, 0x4e, 0xc0, 0xb5, 0x15, 0x02, 0xb0, 0xe8, 0x9c, 0x14, 0x7a, 0x8f, 0x76, 0x97, 0x08, 0x07, 0x8d, 0x3a, 0xa3, 0x95, 0xec, 0x97, 0xcd, 0x76, 0x84, 0x8c, 0x9d, 0x9a, 0x6e, 0x00, 0xc3, 0xf2, 0xc1, 0xcb, 0xe3, 0x5a, 0xa5, 0xff, 0xd9, 0xc0, 0x6c, 0xaa, 0xfd, 0xe1, 0x6a, 0x61, 0xbf, 0x06, 0xe4, 0x15, 0xbd, 0xa6, 0xd5, 0x2b, 0x57, 0x37, 0x18, 0x8b, 0x01, 0xb6, 0x09, 0x29, 0x2f, 0x08, 0xaf, 0x97, 0x60, 0xed, 0x62, 0x31, 0x6e, 0x00, 0x66, 0x55, 0x63, 0x1a, 0x7e, 0xf9, 0xb2, 0x3d, 0xbd, 0xd4, 0xa5, 0xf4, 0xf6, 0xf6, 0xc8, 0x7b, 0x65, 0x75, 0xdc, 0x40, 0xd0, 0x5e, 0x75, 0x45, 0x1d, 0xd3, 0x94, 0x58, 0x3d, 0x89, 0xc9, 0x23, 0x80, 0xaf, 0xb5, 0x98, 0xc7, 0x75, 0x64, 0xd1, 0x35, 0x28, 0x1c, 0xa6, 0x9b, 0x72, 0x5c, 0xd2, 0x72, 0xda, 0xb4, 0x78, 0xfe, 0x33, 0x0d, 0xf9, 0xd3, 0x13, 0x6d, 0x57, 0x59, 0x7f, 0x7f, 0x40, 0xe9, 0x65, 0xed, 0xf3, 0x27, 0xc7, 0x29, 0x86, 0xd8, 0x02, 0x86, 0x06, 0x3c, 0xc9, 0xaa, 0x16, 0x5b, 0xbd, 0xce, 0x00, 0xdc, 0x0e, 0x67, 0x44, 0x5a, 0x62, 0x55, 0x4d, 0x66, 0xc7, 0x57, 0x75, 0x17, 0x4f, 0x57, 0x5c, 0xa9, 0xe2, 0xf6, 0x48, 0xeb, 0xa9, 0x7f, 0x0b, 0x26, 0x1b, 0xa6, 0xdf, 0x91, 0x8c, 0x53, 0x5c, 0x04, 0x30, 0xaf, 0x04, 0x5b, 0xad, 0x7f, 0x41, 0xbe, 0x01, 0xee, 0x8f, 0x89, 0x18, 0x25, 0xd5, 0x41, 0x43, 0x4d, 0x8f, 0x42, 0x9f, 0xa7, 0x4f, 0xac, 0x7a, 0xf4, 0xa0, 0x0d, 0x95, 0xf0, 0x7d, 0x1c, 0x7f, 0x5d, 0xfa, 0xe9, 0x4a, 0x46, 0x8c, 0xd5, 0x35, 0x1e, 0x1b, 0x0c, 0x15, 0xe9, 0xf5, 0x2f, 0xa8, 0xf1, 0x01, 0xb1, 0x7c, 0xf7, 0x84, 0x63, 0x2d, 0x75
};

V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 SecM_RsaExp[4] = 
{ 
 	0x00, 0x01, 0x00, 0x01
};
#define SECMPAR_STOP_SEC_DATA
#include "MemMap.h"

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\SecMPar.c> is different. Please check time and date of generated files!"
  #endif
#else
  #error "The magic number is not defined in the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\SecMPar.c> "

#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

