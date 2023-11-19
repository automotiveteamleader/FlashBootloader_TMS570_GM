/* -----------------------------------------------------------------------------
  Filename:    SecM_cfg.h
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

#if !defined(__SECM_CFG_H__)
#define __SECM_CFG_H__

#define SEC_CLASS_DDD                        0
#define SEC_CLASS_C                          1
#define SEC_CLASS_CCC                        2
#define SEC_CLASS_VENDOR                     3
#define SEC_DEVELOPMENT                      1
#define SEC_PRODUCTION                       2
#define SEC_ADDRESS                          1
#define SEC_FILE                             2
#define SEC_CRC_SPEED_OPTIMIZED              0
#define SEC_CRC_SIZE_OPTIMIZED               1
#define SEC_SECURITY_CLASS                   SEC_CLASS_CCC
#define SEC_ECU_KEY                          0x00
#define SEC_KEY_TIMEOUT                      100
#define SEC_CALL_CYCLE                       10
#define SEC_MEMORY_ACCESS                    SEC_ADDRESS
#define SEC_MODE                             SEC_DEVELOPMENT
#define SEC_CRC_OPT                          SEC_CRC_SIZE_OPTIMIZED
#define SEC_VER_DISABLE_CRC_TOTAL
#define SEC_RIPEMD160                        0
#define SEC_SHA1                             1
#define SEC_SHA256                           2
#define SEC_HASH_ALGORITHM                   SEC_SHA256
#define SEC_VERIFY_BYTES                     64
#define SEC_DISABLE_DECRYPTION
#define SEC_VER_DISABLE_LENGTH_AND_ADDRESS_INPUT
#define SECM_VER_SIG_OFFSET                  0
#define SECM_VER_CRC_OFFSET                  0

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\SecM_cfg.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 182747247
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __SECM_CFG_H__ */
