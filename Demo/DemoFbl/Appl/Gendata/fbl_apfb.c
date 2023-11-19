/* -----------------------------------------------------------------------------
  Filename:    fbl_apfb.c
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

/* Flash Block Table for Flashbootloader */
#include "fbl_inc.h"
V_MEMROM0 V_MEMROM1_FAR tFlashBlock V_MEMROM2_FAR FlashBlock[] = 
{
  
  {
    0xC000u /* BANK0 / SECTOR 3 / 16K Bytes */, 
    0xFFFFu
  }, 
  
  {
    0x010000u /* BANK0 / SECTOR 4 / 16K Bytes */, 
    0x013FFFu
  }, 
  
  {
    0x014000u /* BANK0 / SECTOR 5 / 16K Bytes */, 
    0x017FFFu
  }, 
  
  {
    0x018000u /* BANK0 / SECTOR 6 / 32K Bytes */, 
    0x01FFFFu
  }, 
  
  {
    0x020000u /* BANK0 / SECTOR 7 / 128K Bytes */, 
    0x03FFFFu
  }, 
  
  {
    0x040000u /* BANK0 / SECTOR 8 / 128K Bytes */, 
    0x05FFFFu
  }, 
  
  {
    0x060000u /* BANK0 / SECTOR 9 / 128K Bytes */, 
    0x07FFFFu
  }
};
V_MEMROM0 V_MEMROM1_FAR vuint8 V_MEMROM2_FAR kNrOfFlashBlock = sizeof(FlashBlock) / sizeof(FlashBlock[0]);

/* Try to check consistency between memory device driver */
/* header files (e.g. flashdrv.h) and GENy configuration */
#ifdef FLASH_SEGMENT_SIZE
#if (FLASH_SEGMENT_SIZE != 8)
# error "Consistency check failed! Adjust GENy configuration!"
#endif

#endif

#ifdef FBL_FLASH_DELETED
#if (FBL_FLASH_DELETED != 0xFFu)
# error "Consistency check failed! Adjust GENy configuration!"
#endif

#endif



/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\fbl_apfb.c> is different. Please check time and date of generated files!"
  #endif
#else
  #error "The magic number is not defined in the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\fbl_apfb.c> "

#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

