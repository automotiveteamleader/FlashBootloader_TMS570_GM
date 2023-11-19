/* -----------------------------------------------------------------------------
  Filename:    fbl_mtab.h
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

#if !defined(__FBL_MTAB_H__)
#define __FBL_MTAB_H__

/* -----------------------------------------------------------------------------
    &&&~ 
 ----------------------------------------------------------------------------- */

/* Defines ******************************************************************* */

#define FBL_MTAB_APPLICATION_AND_CALIBRATION_AREA1_BLOCK_NUMBER 0
#define FBL_MTAB_APPLICATION_AND_CALIBRATION_AREA1_BLOCK_INDEX 0x01u

/* Formula to compute kNrOfValidationBytes is (((FBL_MTAB_NO_OF_BLOCKS-1)/8)+1) */
#define kNrOfValidationBytes                 1

/* Macro to determine if the block table is valid */
#define FBL_MTAB_MAGIC_FLAG                  0xB5A4C3D2u
#define IsLogicalBlockTableValid()           (FblLogicalBlockTable.magicFlag == FBL_MTAB_MAGIC_FLAG)

/* Typedefs ****************************************************************** */

/* Entry type of logical block table */
typedef struct tBlockDescriptorTag
{
  vuint8 blockNr; /* Number of logical block */
  vuint32 blockIndex;
  tFblAddress blockStartAddress; /* Start address of current block */
  tFblLength blockLength; /* Block length in bytes */
  tFblAddress headerAddress;
  tFblAddress presencePatternAddress;
  vuint16 maxProgAttempts; /* Maximum number of reprogramming attempts */
  tExportFct verifyRAM;
  tExportFct verifyROM;
} tBlockDescriptor;

/* The logical block table describes the memory layout of logical blocks */
typedef struct tLogicalBlockTableTag
{
  vuint32 magicFlag; /* Value of FBL_MTAB_MAGIC_FLAG - Indicates the existence of the table */
  vuint8 assignFlags[kNrOfValidationBytes]; /* Disposability bit field (mandatory/optional) */
  vuint8 noOfBlocks; /* Number of configured logical blocks */
  tBlockDescriptor logicalBlock[FBL_MTAB_NO_OF_BLOCKS];
} tLogicalBlockTable;

/* Global data *************************************************************** */
V_MEMROM0 extern  V_MEMROM1 tLogicalBlockTable V_MEMROM2 FblLogicalBlockTable;


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 182747247
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\Demo\DemoFbl\Appl\gendata\fbl_mtab.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 182747247
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __FBL_MTAB_H__ */
