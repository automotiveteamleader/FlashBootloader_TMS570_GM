/* -----------------------------------------------------------------------------
  Filename:    ftp_cfg.h
  Description: Toolversion: 05.03.00.01.40.05.01.00.00.00
               
               Serial Number: CBD1400501
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP5
                              Micro: TMS5700714PGEQQ1
                              Compiler: TI Code Composer 4.9.5
               
               
               Generator Fwk   : GENy 
               Generator Module: FblTp_Iso
               
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

#if !defined(__FTP_CFG_H__)
#define __FTP_CFG_H__

#define FBL_TP_DISABLE_EXTENDED_ADDRESSING
#define FBL_TP_DISABLE_TX_FRAME_PADDING
#define kFblTpFillPattern                    255
#define FBL_TP_DISABLE_ACCEPT_TOO_LARGE_DATA
#define FBL_TP_ENABLE_OVERRUN_FLAG_IN_FC
#define FBL_TP_ENABLE_ISO_COMPLIANCE
#define FBL_TP_ENABLE_ONLY_FIRST_FC
#define FBL_TP_ENABLE_FLOW_STATE
#define kFblTpWFTmax                         16u
#define FBL_TP_DISABLE_NO_STMIN_AFTER_FC
#define FBL_TP_DISABLE_WAIT_CORRECT_SN
#define FBL_TP_ENABLE_ISO15765_2_2
#define FBL_TP_DISABLE_VARIABLE_TX_DLC
#define FBL_TP_DISABLE_FIX_RX_DLC_CHECK
#define FBL_TP_DISABLE_VARIABLE_RX_DLC
#define FBL_TP_ENABLE_INTERNAL_MEMCPY
#define TpCallCycle                          1
#define FBL_TP_DISABLE_CONFIRMATION_INTERRUPT
#define FBL_TP_DISABLE_QUEUED_REQUESTS
#define kFblTpConfInterruptTimeout           250
#define kFblTpSTMin                          0
#define kFblTpTimeoutFC                      250
#define kFblTpTimeoutCF                      250
#define kFblTpTransmitCF                     10
#define kFblTpBlocksize                      0
#define kFblTpBufferSize                     4095
#define __ApplFblTpErrorIndication(i)
#define __ApplFblTpDataInd(i)                FblDataInd(i)
#define __ApplFblTpDataIndOverrun(actLen, refLen)
#define __ApplFblTpConfirm(i)
#define __ApplFblTpCopyFromCAN(dst, src, len)
#define __ApplFblTpCopyToCAN(dst, src, len)
#define __ApplFblTpNotifyTx(i)
#define __ApplFblTpCanMessageTransmitted()
#define __ApplFblTpCanMessageReceived()
#define __ApplFblTpRxSF()
#define __ApplFblTpRxFF()
#define __ApplFblTpRxCF()
#define __ApplFblTpMsgTooLong(len_lobyte, len_hibyte)
#ifndef C_CPUTYPE_8BIT
#define FBL_TP_ENABLE_OSEK_DATA_LENGTH
typedef vuint16 tTpDataType;
#else
#define FBL_TP_ENABLE_OSEK_DATA_LENGTH
typedef vuint16 tTpDataType;
#endif


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\ftp_cfg.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 182747247
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __FTP_CFG_H__ */
