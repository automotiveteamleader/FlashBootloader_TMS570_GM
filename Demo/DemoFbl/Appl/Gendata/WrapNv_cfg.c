/* -----------------------------------------------------------------------------
  Filename:    WrapNv_cfg.c
  Description: Toolversion: 05.03.00.01.40.05.01.00.00.00
               
               Serial Number: CBD1400501
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP5
                              Micro: TMS5700714PGEQQ1
                              Compiler: TI Code Composer 4.9.5
               
               
               Generator Fwk   : GENy 
               Generator Module: SysService_WrapperNv
               
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

#include "WrapNv_inc.h"
#ifdef FBL_ENABLE_EEPMGR
#else
#ifdef NV_ENABLE_ADDRESS_LOOKUP
V_MEMROM0 V_MEMROM1_FAR tNvRecordTbl V_MEMROM2_FAR kNvRecordTbl[] = 
{
  
  {
    kEepMgrHandle_Fbl_Nvm_Data_Set + kEepMgrHandle_Key_NBID, 
    kNvAccessStructured, 
    kEepAddressKey_NBID, 
    kEepSizeKey_NBID
  }, 
  
  {
    kEepMgrHandle_Fbl_Nvm_Data_Set + kEepMgrHandle_App_NBID, 
    kNvAccessStructured, 
    kEepAddressApp_NBID, 
    kEepSizeApp_NBID
  }, 
  
  {
    kEepMgrHandle_Fbl_Nvm_Data_Set + kEepMgrHandle_ECU_ID, 
    kNvAccessStructured, 
    kEepAddressECU_ID, 
    kEepSizeECU_ID
  }, 
  
  {
    kEepMgrHandle_Fbl_Nvm_Data_Set + kEepMgrHandle_SBA_ticket, 
    kNvAccessStructured, 
    kEepAddressSBA_ticket, 
    kEepSizeSBA_ticket
  }
};
V_MEMROM0 V_MEMROM1_FAR vuint8 V_MEMROM2_FAR kNvNoOfNvRecords = sizeof(kNvRecordTbl) / sizeof(kNvRecordTbl[0]);
#endif

#endif


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\WrapNv_cfg.c> is different. Please check time and date of generated files!"
  #endif
#else
  #error "The magic number is not defined in the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\WrapNv_cfg.c> "

#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

