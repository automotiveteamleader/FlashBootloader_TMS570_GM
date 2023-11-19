/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Hardware-specific module for Texas Instruments TMS570
 *                 Provides the CAN-Interface and timer operations
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Phil Lapczynski               Pal           Vector CANtech, Inc.
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Heath Smith                   Hsm           Vector CANtech, Inc.
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Johannes Krimmel              KJs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2009-09-24  MVi                      First release for TMS570PSFC66
 *  01.01.00   2010-03-25  Pal     ESCAN00041779    Add stay in boot support
 *                                 ESCAN00041778    Add support for MSF542
 *                                 ESCAN00041101    FblInterruptDisable() doesn't
 *                                                  disable all interrupts
 *  01.02.00   2010-05-07  MVi     ESCAN00042943    No changes
 *  01.03.00   2010-07-21  QPs     ESCAN00044207    farfunc identifier undefined for
 *                                                  flexray bootloader
 *  01.04.00   2011-02-09  AWh     ESCAN00048549    Additions for GM hw-layer requirements
 *  01.05.00   2011-08-03  MVi     ESCAN00052595    Support Vector CANdrv
 *  01.06.00   2011-12-16  Hsm     ESCAN00054688    Change MEMCPY macro to new design
 *  01.07.00   2011-12-20  TBe     ESCAN00055661    Added support for extended identifier
 *                                 ESCAN00055662    Added support for normal fixed addressing
 *  01.08.00   2012-02-08  TBe     ESCAN00056478    FblInterruptDisable macro does not disable all interrupts
 *  01.09.00   2012-10-26  Ach     ESCAN00061454    No changes
 *                         TBe     ESCAN00062446    Reworked CAN ID handling
 *  01.10.00   2013-07-04  MVi     ESCAN00065948    Added support for RX ranges
 *                         ASe     ESCAN00068150    Adapted comments to use doxygen
 *                         MVi     ESCAN00068450    Added support for TMS320_DRA65X
 *  01.11.00   2013-08-19  ASe     ESCAN00069580    Added support for hardware CAN FIFOs
 *  01.12.00   2015-02-12  KJs     ESCAN00079643    Compiler error: ApplIntJmpTable type mismatch
 *                         KJs     ESCAN00079645    Jump to start label not correct for TMS470M derivatives
 *                         KJs     ESCAN00081057    No changes
 *                         KJs     ESCAN00081138    No changes
 **********************************************************************************************************************/

#ifndef __FBL_HW_TMS470_DCAN_H__
#define __FBL_HW_TMS470_DCAN_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#if defined( FBL_USE_OWN_MEMCPY )
#else
# include <string.h>
#endif

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDrvCan_Tms470DcanCrx CQComponent : Implementation */
#define FBLDRVCAN_TMS470DCANCRX_VERSION            0x0112u
#define FBLDRVCAN_TMS470DCANCRX_RELEASE_VERSION    0x00u

/* RI1.0: Reference implementation version */
#define FBLDRVCAN_REFERENCE_IMPLEMENTATION         0x0100u

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef void (* farfunc)(void);

typedef vuint16 tCanId;

/* Data types for driver function parameters                                 */
typedef struct
{
   vuint16 BIT;                            /**< CAN bus Bit Timing Register    */
   vuint16 BRP;                            /**< CAN bus BRP Extension Register */
#   if ( FBL_CAN_NUMBER_OF_RX_ID > 0 )
   tCanId CanRxId[FBL_CAN_NUMBER_OF_RX_ID];
#   endif /* FBL_CAN_NUMBER_OF_RX_ID > 0 */
} tFblCanIdTable;

typedef vuint8 CanTransmitHandle;
typedef volatile vuint8 *pChipDataPtr;

/* Mailbox register */
typedef struct
{
   tCanId Id;                   /**< CAN message identifier                    */
   vuint16  CanMID_low_unused;  /**< Not used for Std IDs. Just a placeholder. */
   vuint16  CanMCF_high_unused; /**< Message control flags                     */
   vuint8   CanMCF_mid_unused;  /**< Message control flags                     */
   vuint8   DLC;                /**< Message control flags: DLC                */
   vuint8   DataFld[8];         /**< indexed access to mailbox data            */
} tInternalMsgObject;

typedef struct
{
   tInternalMsgObject msgObject;
   void (*ConfirmationFct)(vuint8 handle);
} tMsgObject;

/* Constants concerning TMS570-CAN-registers */

/* Define chip hardware                    */
/* Constants concerning 570 DCAN-registers */
/* Control registers for msg objects       */
/* CAN control register */
typedef union
{
   vuint8   ucData[8];                /* Address data as  8 bit     */
   vuint16  usData[4];                /* Address data as 16 bit     */
   vuint32  ulData[2];                /* Address data as 32 bit     */

} tFblCanData;

typedef volatile struct
{
   vuint16      ComMask;              /* Command Mask register      */
   vuint16      ComRequest;           /* Command Request register   */
   vuint16      Mask2;                /* Mask register 2            */
   vuint16      Mask1;                /* Mask register 1            */
   vuint16      ArbitrFld2;           /* Arbitration 2              */
   vuint16      ArbitrFld1;           /* Arbitration 1              */
   vuint16      notused10;            /* not used                   */
   vuint16      MessageCntrl;         /* Message Control Register   */
   tFblCanData  Data;                 /* CAN Data Register          */
   vuint16      notused11[4];         /* not used                   */

} tFblCanMsgObj;

typedef volatile struct
{
   vuint32       CntrlReg;            /* CAN Control Register            base address + 0x00  */

   vuint16       notused1;            /*                                 base address + 0x04  */
   vuint16       StatReg;             /* CAN Status Register             base address + 0x06  */

   vuint16       notused2;            /*                                 base address + 0x08  */
   vuint16       ErrorCount;          /* CAN Error Counter Register      base address + 0x0A  */

   vuint16       BRP_Reg;             /* Baud Rate Prescaler Register    base address + 0x0C  */
   vuint16       BitTiming;           /* CAN Bit Timing Register         base address + 0x0E  */

   vuint16       InterruptReg1;       /* CAN Interrupt Register          base address + 0x10  */
   vuint16       InterruptReg;        /* CAN Interrupt Register1         base address + 0x12  */

   vuint16       notused3;            /*                                 base address + 0x14  */
   vuint16       TestReg;             /* Test Register                   base address + 0x16  */

   vuint16       notused4[3];         /*                                 base address + 0x18  */
   vuint16       ParityErrorCode;     /* ParityErrorCode                 base address + 0x1E  */

   vuint16       notused5[48];        /*                                 base address + 0x20  */

   vuint16       AutoBusOnTime2;      /* BusOnTime Regsiter2             base address + 0x80  */
   vuint16       AutoBusOnTime1;      /* BusOnTime Regsiter1             base address + 0x82  */

   vuint16       notused6;            /*                                 base address + 0x84  */
   vuint16       TransRequestX;       /* Transmission request Register   base address + 0x86  */

   vuint32       TransRequest[4];     /* Transmission request Register   base address + 0x88  */

   vuint16       notused7;            /* New Data Register               base address + 0x98  */
   vuint16       NewDataX;            /* New Data Register               base address + 0x9A  */

   vuint32       NewData[4];          /* New Data Register               base address + 0x9C  */

   vuint16       notused8;            /*                                 base address + 0xAC  */
   vuint16       InterrPendingX;      /* InterrPending Register          base address + 0xAE  */

   vuint32       InterrPending[4];    /* InterrPending Register          base address + 0xB0  */

   vuint16       notused9;            /*                                 base address + 0xC0  */
   vuint16       MessageValidX;       /* MessageValid Register           base address + 0xC2  */

   vuint32       MessageValid[4];     /* MessageValid Register           base address + 0xC4  */

   vuint32       notused10;           /*                                 base address + 0xD4  */

   vuint16       IntPndMux[8];        /* NIntPndMux Register             base address + 0xD8  */
   vuint16       notused11[12];       /*                                 base address + 0xE8  */

   tFblCanMsgObj sMsgObj[3];          /* Message Interface Register      base address + 0x100 */
   vuint16       notused12[64];       /*                                 base address + 0x160 */

   vuint32       can_IO_ctrlTx;       /* can_IO_ctrlTx Register          base address + 0x1E0 */
   vuint32       can_IO_ctrlRx;       /* can_IO_ctrlRx Register          base address + 0x1E4 */

} tFblCanCellAddrMap;

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#   define FBL_CAN_NUMBER_OF_ALL_RX_ID      (FBL_CAN_NUMBER_OF_RX_ID)

/* Definitions for access to registers */
#  define pFblCanNode            (*(tFblCanCellAddrMap *)CAN_CTRL_ADDR)

#  define FBL_IF_INIT            pFblCanNode.sMsgObj[0]
#  define FBL_IF_TRANSMIT        pFblCanNode.sMsgObj[0]
#  define FBL_IF_RECEIVE         pFblCanNode.sMsgObj[1]

#  define FBL_CNTRLREG           pFblCanNode.CntrlReg
#  define FBL_STATREG            pFblCanNode.StatReg
#  define FBL_ERRORCOUNT         pFblCanNode.ErrorCount
#  define FBL_BITTIMING          pFblCanNode.BitTiming
#  define FBL_BRP_REG            pFblCanNode.BRP_Reg
#  define FBL_INTERRUPTREG       pFblCanNode.InterruptReg
#  define FBL_INTERRUPTREG1      pFblCanNode.InterruptReg1
#  define FBL_TESTREG            pFblCanNode.TestReg
#  define FBL_PARITYERRORCODE    pFblCanNode.ParityErrorCode
#  define FBL_AUTOBUSONTIME1     pFblCanNode.AutoBusOnTime1
#  define FBL_AUTOBUSONTIME2     pFblCanNode.AutoBusOnTime2
#  define FBL_TRANSREQUESTX      pFblCanNode.TransRequestX
#  define FBL_TRANSREQUEST(i)    pFblCanNode.TransRequest[i]
#  define FBL_NEWDATAX           pFblCanNode.NewDataX
#  define FBL_NEWDATA(i)         pFblCanNode.NewData[i]
#  define FBL_INTERRPENDINGX     pFblCanNode.InterrPendingX
#  define FBL_INTERRPENDING(i)   pFblCanNode.InterrPending[i]
#  define FBL_MESSAGEVALIDX      pFblCanNode.MessageValidX
#  define FBL_MESSAGEVALID(i)    pFblCanNode.MessageValid[i]
#  define FBL_INTPNDMUX          pFblCanNode.IntPndMux[i]
#  define FBL_CAN_IO_CTRLTX      pFblCanNode.can_IO_ctrlTx
#  define FBL_CAN_IO_CTRLRX      pFblCanNode.can_IO_ctrlRx

/* Define chip hardware                    */
/* Constants concerning can chip registers */
/* control registers for msg objects       */

#  define kFblClearAll          ((vuint16)0x0000) /* use to clear all Bit in a word */
#  define kFblClearAll32        ((vuint32)0x00000000)
#  define kFblErrBusOff         ((vuint16)0x0080) /* Error:  Bus-off limit reached  */
#  define kFblErrWarning        ((vuint16)0x0040) /* Error:  Warning limit reached  */
#  define kFblErrPassive        ((vuint16)0x0020) /* Error:  Passive limit reached  */
#  define kFblWakeupPending     ((vuint16)0x0200) /* Status: CAN was waked up       */

/* Defines to test a bit in a register */
#  define kFblIFxBusy           ((vuint16)0x8000) /* Test IFx Command Request       */
#  define kFblCanStatusInt      ((vuint16)0x8000) /* Status interrupt in CAN_Cell   */

/* Defines to set a bit in the CAN Control Register */
#  define kFblSetWUBA           ((vuint32)0x02000000)
#  define kFblSetPDR            ((vuint32)0x01000000)
#  define kFblSetDE3            ((vuint32)0x00100000)
#  define kFblSetDE2            ((vuint32)0x00080000)
#  define kFblSetDE1            ((vuint32)0x00040000)
#  define kFblSetIE1            ((vuint32)0x00020000)
#  define kFblSetInitDbg        ((vuint32)0x00010000)
#  define kFblSetSWR            ((vuint32)0x00008000)
#  define kFblSetPMD            ((vuint32)0x00001400)
#  define kFblSetABO            ((vuint32)0x00000200)
#  define kFblSetIDS            ((vuint32)0x00000100)

#  define kFblSetTest           ((vuint32)0x00000080)
#  define kFblSetCCE            ((vuint32)0x00000040)
#  define kFblSetDAR            ((vuint32)0x00000020)
#  define kFblSetEIE            ((vuint32)0x00000008)
#  define kFblSetSIE            ((vuint32)0x00000004)
#  define kFblSetIE             ((vuint32)0x00000002)
#  define kFblSetInit           ((vuint32)0x00000001)

/* Defines to set a bit in the Status Register */
#  define kFblPDA               ((vuint16)0x0400)
#  define kFblStatRxOk          ((vuint16)0x0010)
#  define kFblStatTxOk          ((vuint16)0x0008)

/* Defines to set a bit in the IFx Command Mask Register */
#  define kFblSetWrRd           ((vuint16)0x0080)
#  define kFblSetMask           ((vuint16)0x0040)
#  define kFblSetArb            ((vuint16)0x0020)
#  define kFblSetControl        ((vuint16)0x0010)
#  define kFblSetClrIntPnd      ((vuint16)0x0008)
#  define kFblSetTxRqstNDat     ((vuint16)0x0004)
#  define kFblSetDataA          ((vuint16)0x0002)
#  define kFblSetDataB          ((vuint16)0x0001)

/* Defines to set a bit in the IFx Mask Register */
#   define kFblSetMsgVal         ((vuint16)0x8000)
#   define kFblSetXtd            ((vuint16)0x4000)
#   define kFblSetDir            ((vuint16)0x2000)
/* Defines to set a bit in the IFx Message Control Register */
#  define kFblSetNewData        ((vuint16)0x8000)
#  define kFblSetMsgLst         ((vuint16)0x4000)
#  define kFblSetIntPnd         ((vuint16)0x2000)
#  define kFblSetUMask          ((vuint16)0x1000)
#  define kFblSetTxIE           ((vuint16)0x0800)
#  define kFblSetRxIE           ((vuint16)0x0400)
#  define kFblSetRmtEn          ((vuint16)0x0200)
#  define kFblSetTxRqst         ((vuint16)0x0100)
#  define kFblSetEoB            ((vuint16)0x0080)

/* Defines to set a bit in the Mask Register */
#  define kFblSetMDir           ((vuint16)0x4000)
#  define kFblSetMXtd           ((vuint16)0x8000)

/* Mapping of TX and RX messages to message objects */
/* Every derivative has at least 16 message objects */
/* Therefore use only the first 16 message objects */
/* Message object number 0 does not exist */
/* The first message object has number 1 */
#  define FBL_CAN_FIRST_TX_MSGOBJ    1     /* First and only tx message object */
#  define FBL_CAN_FIRST_RX_MSGOBJ    2     /* First of maximal five rx message objects */

/* Create a bitmask to check if the TX message was sent */
#  define FBL_CAN_CHECK_TX_BITS      1     /* 0000 0001 */
#  define FBL_CAN_CHECK_TX_MASK      (FBL_CAN_CHECK_TX_BITS << (FBL_CAN_FIRST_TX_MSGOBJ - 1))

/* Create a bitmask to check if the RX message(s) (objects 2-16) were received */
#  define FBL_CAN_CHECK_RX_BITS      0xEFFFu
#  define FBL_CAN_CHECK_RX_MASK      (FBL_CAN_CHECK_RX_BITS << (FBL_CAN_FIRST_RX_MSGOBJ - 1))

/* General-Purpose Input/Output (GIO) Module */
# define FBL_GIO_BASE_ADDR ((vuint32) 0xFFF7BC00)
# define FBL_GIOGCR0      (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x00))
# define FBL_GIODIRA      (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x34))
# define FBL_GIODINA      (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x38))
# define FBL_GIODOUTA     (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x3C))
# define FBL_GIODIRB      (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x54))
# define FBL_GIODINB      (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x58))
# define FBL_GIODOUTB     (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x5C))
# define FBL_GIODIRC      (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x74))
# define FBL_GIODINC      (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x78))
# define FBL_GIODOUTC     (*(volatile vuint32 *)(FBL_GIO_BASE_ADDR + 0x7C))

/* Real-Time Interrupt (RTI) Module */
#define FBL_RTI_BASE_ADDR ((vuint32) 0xFFFFFC00)
#define FBL_RTIGCTRL     (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x00))
#define FBL_RTITBCTRL    (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x04))
#define FBL_RTICOMPCTRL  (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x0C))
#define FBL_RTIFRC0      (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x10))
#define FBL_RTIUC0       (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x14))
#define FBL_RTICPUC0     (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x18))
#define FBL_RTICOMP0     (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x50))
#define FBL_RTIUDCP0     (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x54))
#define FBL_RTISETINT    (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x80))
#define FBL_RTIINTFLAG   (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x88))
#define FBL_RTIDWDCTRL   (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x90))
#define FBL_RTIDWDPRLD   (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x94))
#define FBL_RTIDWDKEY    (*(volatile vuint32 *)(FBL_RTI_BASE_ADDR + 0x9C))

/* Vector Interrupt Manager register (VIM) */
#define FBL_VIM_BASE_ADDR ((vuint32) 0xFFFFFE00)
#define FBL_IRQIVEC      (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x00))
#define FBL_FIRQPR0      (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x10))
#define FBL_FIRQPR1      (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x14))
#define FBL_INTREQ0      (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x20))
#define FBL_INTREQ1      (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x24))
#define FBL_REQMSKSET0   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x30))
#define FBL_REQMSKSET1   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x34))
#define FBL_REQMSKCLR0   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x40))
#define FBL_REQMSKCLR1   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x44))
#define FBL_WAKMSKSET0   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x50))
#define FBL_WAKMSKSET1   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x54))
#define FBL_WAKMSKCLR0   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x60))
#define FBL_WAKMSKCLR1   (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x64))
#define FBL_IRQVECREG    (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x70))
#define FBL_FIQVECREG    (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x74))
#define FBL_CAPEVT       (*(volatile vuint32 *)(FBL_VIM_BASE_ADDR + 0x78))

/* System control register (SYS) */
#define FBL_SYS_BASE_ADDR ((vuint32) 0xFFFFFF00)
#define FBL_GHVSRC       (*(volatile vuint32 *)(FBL_SYS_BASE_ADDR + 0x48))
#define FBL_CSVSTAT      (*(volatile vuint32 *)(FBL_SYS_BASE_ADDR + 0x54))
#define FBL_PLLCTL1      (*(volatile vuint32 *)(FBL_SYS_BASE_ADDR + 0x70))
#define FBL_PLLCTL2      (*(volatile vuint32 *)(FBL_SYS_BASE_ADDR + 0x74))
#define FBL_SYSECR       (*(volatile vuint32 *)(FBL_SYS_BASE_ADDR + 0xE0))

#if defined( FBL_USE_OWN_MEMCPY )
# define MEMCPY(dst,src,len) ownmemcpy((void *)(dst),(const void *)(src),(vuint16)(len))
#else
# define MEMCPY(dst,src,len) memcpy((void *)(dst),(const void *)(src),(size_t)(len))
#endif /* FBL_USE_OWN_MEMCPY */

/* System Exception Register (can be used for software reset) */
#define SYSECR_RESET()        (FBL_SYSECR |= 0x8000)
/* Address of FBL reset vector */
# define RESETVECT            0x00

#define STARTUP_CODE          (farfunc *)(RESETVECT)
/* Macros for jumps */
#define JSR(x)                (*((void(*)(void))x))() /* Jump to SubRoutine */
#define JSR_RESET()           JSR(STARTUP_CODE)

/* Return values for precopy-routines */
#  define kCanNoCopyData        0x00
#  define kCanCopyData          0x01

#  define MULTIPLE_RECEIVE_BUFFER

/* CAN identifier handles */
#  define FblRxCanMsg0Hdl       0  /* Physical request */
#  define FblRxCanMsg1Hdl       1  /* Functional request */
#  define FblRxCanMsg2Hdl       2
#  define FblRxCanMsg3Hdl       3
#  define FblRxCanMsg4Hdl       4
#  define FblRxCanMsgRangeHdl   1

#  define CAN_RX_SLOT_0         FblRxCanMsg0Hdl  /* Physical request */
#  define CAN_RX_SLOT_1         FblRxCanMsg1Hdl  /* Functional request */

/* Std CAN driver macros */
/* Convert ID to hardware format */
#    define MK_ID(id)             (vuint16)(((0x3FFF & id)<<2) & 0x1FFC)
#    define MK_ID16(idH,idL)      (vuint16)(((idH<<8)|(idL)))
#    define GET_ID(id)            (vuint16)(((0xFFFC & id)>>2) & 0x07FF)

#  define FblCanSetTxId(MsgObj,id)   (MsgObj.msgObject.Id=id)
#  define FblCanSetTxDLC(MsgObj,dlc) (MsgObj.msgObject.DLC=dlc)
#  define FblCanSetTxConfirmation(MsgObj,ConfFctPtr) (MsgObj.ConfirmationFct = ConfFctPtr)
/* Check all message bits except for the TX mailbox, limit range of used mailboxes to 16 to be compatible with all
 * processor derivatives */
#  define FblCanReceive()       (0u != (FBL_NEWDATA(0) & FBL_CAN_CHECK_RX_MASK))

#  define FblCanIsAsleep()      ((FBL_STATREG & kFblPDA) == kFblPDA)

# define FblInterruptDisable() {                                \
                                  FBL_REQMSKCLR0 = 0xFFFFFFFFu; \
                                  FBL_REQMSKCLR1 = 0xFFFFFFFFu; \
                               }

# define FblTimerGet()         (FBL_RTIINTFLAG & 0x01)
# define FblTimerReset()       {FBL_RTIINTFLAG |= 0x01;}
# define FblTimerStopp()       {FBL_RTIGCTRL = 0x00;}

# define FblCheckBootVectTableIsValid() (ApplIntJmpTable[0].opcode == APPLVECT_FROM_BOOT)

#define FblFlashAddressGet3Bytes(array)   (FBL_ADDR_TYPE)( (FBL_ADDR_TYPE)(((vuint8 *)array)[0]<<16) | (FBL_ADDR_TYPE)(((vuint8 *)array)[1]<<8) | (FBL_ADDR_TYPE)(((vuint8 *)array)[2]) )
#define FblDownloadLengthGet3Bytes(array) (FBL_MEMSIZE_TYPE)( (FBL_MEMSIZE_TYPE)(((vuint8 *)array)[0]<<16) | (FBL_MEMSIZE_TYPE)(((vuint8 *)array)[1]<<8) | (FBL_MEMSIZE_TYPE)(((vuint8 *)array)[2]) )
#define FblFlashAddressGet4Bytes(array)   (FBL_ADDR_TYPE)( (FBL_ADDR_TYPE)(((vuint8 *)array)[0]<<24) | (FBL_ADDR_TYPE)(((vuint8 *)array)[1]<<16) | (FBL_ADDR_TYPE)(((vuint8 *)array)[2]<<8) | (FBL_ADDR_TYPE)(((vuint8 *)array)[3]) )
#define FblDownloadLengthGet4Bytes(array) (FBL_MEMSIZE_TYPE)( (FBL_MEMSIZE_TYPE)(((vuint8 *)array)[0]<<24) | (FBL_MEMSIZE_TYPE)(((vuint8 *)array)[1]<<16) | (FBL_MEMSIZE_TYPE)(((vuint8 *)array)[2]<<8) | (FBL_MEMSIZE_TYPE)(((vuint8 *)array)[3]) )

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

extern tCanId CanRxActualId;
#   if defined( FBL_ENABLE_MULTIPLE_NODES )
V_MEMROM0 extern V_MEMROM1 tCanId V_MEMROM2 CanTxId[];
V_MEMROM0 extern V_MEMROM1 tCanId V_MEMROM2 CanRxId[];
#   endif /* FBL_ENABLE_MULTIPLE_NODES */
extern vuint8      CanRxActualDLC;

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/* Jump to application */
void JSR_APPL( void );
typedef void (*ApplFct)( void );

/* Initialize ms timer */
void FblTimerInit( void );

/* Get timer value */
vuint16 FblGetTimerValue( void );

/* Initialize CAN interface */
void FblCanInit( void );
void FblCanParamInit( void );

/* Check of CAN controller error flags */
void FblCanErrorTask( void );

/* Transmit CAN message */
vuint8 FblCanTransmit( tMsgObject * tmtObject );

#  define FblCanRetransmit()                                         \
   {                                                                 \
      FBL_IF_TRANSMIT.ComMask    = kFblSetWrRd | kFblSetTxRqstNDat;  \
      FBL_IF_TRANSMIT.ComRequest = FBL_CAN_FIRST_TX_MSGOBJ;          \
   }

/* Function to handle CAN receive messages */
void  FblCanProcessRxMsg( void );

/* Polling function for transmitted CAN messages */
vuint8 FblCanMsgTransmitted( void );

#  if defined( FBL_ENABLE_SLEEPMODE )
/* Function to enter sleep mode */
vuint8 FblCanSleep( void );
/* Function to quit sleep mode */
void FblCanWakeUp( void );
#   if defined( FBL_ENABLE_WAKEUP_INT )
void FblCanWakeUpInterrupt( void );
#   endif
#  endif

#if defined( FBL_USE_OWN_MEMCPY )
/* Own memcpy routine (for WD-triggering) */
void * ownmemcpy( void * dest, const void * source, vuint16 size );
#endif

/* Memory access functions */
FBL_MEMSIZE_TYPE FblReadBlock( FBL_ADDR_TYPE address, vuint8 * buffer, FBL_MEMSIZE_TYPE length );
vuint8 FblReadByte( FBL_ADDR_TYPE address );

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
/* For startup delay phase only */
vuint8 FblCanStartMsg( void );
#endif

#endif /* __FBL_HW_TMS470_DCAN_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_HW.H
 **********************************************************************************************************************/
