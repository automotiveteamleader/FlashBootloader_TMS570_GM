/* -----------------------------------------------------------------------------
  Filename:    WrapNv_cfg.h
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

#if !defined(__WRAPNV_CFG_H__)
#define __WRAPNV_CFG_H__

#ifdef FBL_ENABLE_EEPMGR
#else
#define NV_ENABLE_ADDRESS_LOOKUP
#endif


/*
*                                 ADDRESS        HANDLE         DESCRIPTION
*
*   Memory Layout
*   |
*   +-- Fbl_Nvm_Data_Set                                        FBL NVM Set (NBIDs, ECU_ID, Sba)
*       |
*       +-- Key_NBID              0x0801c000     1
*       |
*       +-- App_NBID              0x0801c002     2
*       |
*       +-- ECU_ID                0x0801c004     3
*       |
*       +-- SBA_ticket            0x0801c014     4
*/

/* Size defines ************************************************************** */
#define kEepSizeKey_NBID                     0x02u
#define kEepSizeApp_NBID                     0x02u
#define kEepSizeECU_ID                       0x10u
#define kEepSizeSBA_ticket                   0x0336u
#define kEepSizeFbl_Nvm_Data_Set             (kEepSizeKey_NBID + kEepSizeApp_NBID + kEepSizeECU_ID + kEepSizeSBA_ticket)
/* Address defines *********************************************************** */
#ifdef FBL_ENABLE_EEPMGR
#else
#define kEepFblBaseAddress                   0x0801C000u
#define kEepStartAddress                     kEepFblBaseAddress
#define kEepAddressFbl_Nvm_Data_Set          kEepStartAddress
#define kEepAddressKey_NBID                  kEepAddressFbl_Nvm_Data_Set
#define kEepAddressApp_NBID                  (kEepAddressKey_NBID + kEepSizeKey_NBID)
#define kEepAddressECU_ID                    (kEepAddressApp_NBID + kEepSizeApp_NBID)
#define kEepAddressSBA_ticket                (kEepAddressECU_ID + kEepSizeECU_ID)
#define kEepEndAddress                       (kEepAddressFbl_Nvm_Data_Set + kEepSizeFbl_Nvm_Data_Set - 1)
#define kEepSizeOfEeprom                     (kEepEndAddress - kEepStartAddress + 1)
/* Initialize NvStructSize-Array */
#define kNvNoOfStructs                       0x01u
#define kNvSizeStructs                       {kEepSizeFbl_Nvm_Data_Set}
#endif

/* Handle defines ************************************************************ */
#define kEepMgrFirstHandle                   1
#define kNvHandleStruct0                     0x00u
#define kEepMgrHandle_Fbl_Nvm_Data_Set       NV_MK_STRUCT_ID(kNvHandleStruct0, kEepMgrFirstHandle)
#define kEepMgrHandle_Key_NBID               0
#define kEepMgrHandle_App_NBID               (kEepMgrHandle_Key_NBID + 1)
#define kEepMgrHandle_ECU_ID                 (kEepMgrHandle_App_NBID + 1)
#define kEepMgrHandle_SBA_ticket             (kEepMgrHandle_ECU_ID + 1)
#define kEepMgrNrOfFbl_Nvm_Data_SetHdls      (kEepMgrHandle_SBA_ticket + 1)
#define kEepMgrLastHandle                    (NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrNrOfFbl_Nvm_Data_SetHdls - 1)
#define kEepMgrNumberOfHandles               (kEepMgrLastHandle - kEepMgrFirstHandle + 1)
#ifdef FBL_ENABLE_EEPMGR
/* Initialize NvStructSize-Array */
#define kNvNoOfStructs                       0x01u
#define kNvSizeStructs                       {kEepMgrNrOfFbl_Nvm_Data_SetHdls}
#endif

/* Access macros ************************************************************* */
#ifdef FBL_ENABLE_EEPMGR
#define ApplFblNvReadKey_NBID(idx, buf)      ((EepMgrRead(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_Key_NBID, buf, kEepSizeKey_NBID) == kEepSizeKey_NBID) ? kFblOk : kFblFailed)
#define ApplFblNvWriteKey_NBID(idx, buf)     ((EepMgrWrite(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_Key_NBID, buf, kEepSizeKey_NBID) == kEepMgrOk) ? kFblOk : kFblFailed)
#else
#define ApplFblNvReadKey_NBID(idx, buf)      ((EepromDriver_RReadSync(buf, kEepSizeKey_NBID, kEepAddressKey_NBID) == IO_E_OK) ? kFblOk : kFblFailed)
#define ApplFblNvWriteKey_NBID(idx, buf)     ((EepromDriver_RWriteSync(buf, kEepSizeKey_NBID, kEepAddressKey_NBID) == IO_E_OK) ? kFblOk : kFblFailed)
#endif

#ifdef FBL_ENABLE_EEPMGR
#define ApplFblNvReadApp_NBID(idx, buf)      ((EepMgrRead(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_App_NBID, buf, kEepSizeApp_NBID) == kEepSizeApp_NBID) ? kFblOk : kFblFailed)
#define ApplFblNvWriteApp_NBID(idx, buf)     ((EepMgrWrite(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_App_NBID, buf, kEepSizeApp_NBID) == kEepMgrOk) ? kFblOk : kFblFailed)
#else
#define ApplFblNvReadApp_NBID(idx, buf)      ((EepromDriver_RReadSync(buf, kEepSizeApp_NBID, kEepAddressApp_NBID) == IO_E_OK) ? kFblOk : kFblFailed)
#define ApplFblNvWriteApp_NBID(idx, buf)     ((EepromDriver_RWriteSync(buf, kEepSizeApp_NBID, kEepAddressApp_NBID) == IO_E_OK) ? kFblOk : kFblFailed)
#endif

#ifdef FBL_ENABLE_EEPMGR
#define ApplFblNvReadECU_ID(idx, buf)        ((EepMgrRead(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_ECU_ID, buf, kEepSizeECU_ID) == kEepSizeECU_ID) ? kFblOk : kFblFailed)
#define ApplFblNvWriteECU_ID(idx, buf)       ((EepMgrWrite(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_ECU_ID, buf, kEepSizeECU_ID) == kEepMgrOk) ? kFblOk : kFblFailed)
#else
#define ApplFblNvReadECU_ID(idx, buf)        ((EepromDriver_RReadSync(buf, kEepSizeECU_ID, kEepAddressECU_ID) == IO_E_OK) ? kFblOk : kFblFailed)
#define ApplFblNvWriteECU_ID(idx, buf)       ((EepromDriver_RWriteSync(buf, kEepSizeECU_ID, kEepAddressECU_ID) == IO_E_OK) ? kFblOk : kFblFailed)
#endif

#ifdef FBL_ENABLE_EEPMGR
#define ApplFblNvReadSBA_ticket(idx, buf)    ((EepMgrRead(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_SBA_ticket, buf, kEepSizeSBA_ticket) == kEepSizeSBA_ticket) ? kFblOk : kFblFailed)
#define ApplFblNvWriteSBA_ticket(idx, buf)   ((EepMgrWrite(NV_GET_STRUCT_ID(kEepMgrHandle_Fbl_Nvm_Data_Set) + kEepMgrHandle_SBA_ticket, buf, kEepSizeSBA_ticket) == kEepMgrOk) ? kFblOk : kFblFailed)
#else
#define ApplFblNvReadSBA_ticket(idx, buf)    ((EepromDriver_RReadSync(buf, kEepSizeSBA_ticket, kEepAddressSBA_ticket) == IO_E_OK) ? kFblOk : kFblFailed)
#define ApplFblNvWriteSBA_ticket(idx, buf)   ((EepromDriver_RWriteSync(buf, kEepSizeSBA_ticket, kEepAddressSBA_ticket) == IO_E_OK) ? kFblOk : kFblFailed)
#endif


#ifdef FBL_ENABLE_EEPMGR
#else
#define ApplFblIsValidEepAddress(i)          ((i >= kEepStartAddress) && (i <= kEepEndAddress))
#endif

#define ApplFblIsValidEepMgrHandle(i)        ((i >= kEepMgrFirstHandle) && (i <= kEepMgrLastHandle))

/* User Config File ********************************************************** */

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\WrapNv_cfg.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 182747247
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __WRAPNV_CFG_H__ */
