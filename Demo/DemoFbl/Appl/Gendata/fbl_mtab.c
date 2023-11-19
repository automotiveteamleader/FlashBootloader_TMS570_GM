/* -----------------------------------------------------------------------------
  Filename:    fbl_mtab.c
  Description: Toolversion: 05.03.00.01.40.05.01.00.00.00
               
               Serial Number: CBD1400501
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP5
                              Micro: TMS5700714PGEQQ1
                              Compiler: TI Code Composer 4.9.5
               
               
               Generator Fwk   : GENy 
               Generator Module: GenTool_GenyFblCanBase
               
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

/* -----------------------------------------------------------------------------
    &&&~ 
 ----------------------------------------------------------------------------- */

#define __FBL_MTAB_C

/* Includes ****************************************************************** */
#include "fbl_inc.h"

/* Global data *************************************************************** */
V_MEMROM0 V_MEMROM1 tLogicalBlockTable V_MEMROM2 FblLogicalBlockTable = 
{
  FBL_MTAB_MAGIC_FLAG, 
  
  {
    0x1
  } /* Disposability bit field (mandatory/optional) */, 
  FBL_MTAB_NO_OF_BLOCKS, 
  
  {
    
    {
      0, 
      0x01u /* Block Index */, 
      0xC000u /* Block Start Address */, 
      0x074000u /* Block Length (End Address = 0x7FFFF) */, 
      0x010000u /* Header Address */, 
      0x01FFF0u /* Presence Pattern Address */, 
      0x00 /* Max Prog Attempts */, 
      (tExportFct)FblHdrPipelinedVerifyIntegrity, 
      (tExportFct)FblHdrVerifyIntegrity
    } /* Name 'Application and Calibration Area1', Mandatory */
  }
};


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\fbl_mtab.c> is different. Please check time and date of generated files!"
  #endif
#else
  #error "The magic number is not defined in the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\fbl_mtab.c> "

#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

