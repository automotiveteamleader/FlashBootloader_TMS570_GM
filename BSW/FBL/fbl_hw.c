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
 *  01.02.00   2010-05-07  MVi     ESCAN00042943    Fixed JSR_APPL for TI compiler
 *  01.03.00   2010-07-21  QPs     ESCAN00044207    No changes
 *  01.04.00   2011-02-09  AWh     ESCAN00048549    Additions for GM hw-layer requirements
 *  01.05.00   2011-08-03  MVi     ESCAN00052595    Support Vector CANdrv
 *  01.06.00   2011-12-16  Hsm     ESCAN00054688    Change MEMCPY macro to new design
 *  01.07.00   2011-12-20  TBe     ESCAN00055661    Added support for extended identifier
 *                                 ESCAN00055662    Added support for normal fixed addressing
 *  01.08.00   2012-02-08  TBe     ESCAN00056478    No changes
 *  01.09.00   2012-10-26  Ach     ESCAN00061454    Support TMS470MF0420
 *                         TBe     ESCAN00062446    Reworked CAN ID handling
 *  01.10.00   2013-07-04  MVi     ESCAN00065948    Added support for RX ranges
 *                         ASe     ESCAN00068150    Adapted comments to use doxygen
 *                         MVi     ESCAN00068450    Added support for TMS320_DRA65X
 *  01.11.00   2013-08-19  ASe     ESCAN00069580    Added support for hardware CAN FIFOs
 *  01.12.00   2015-02-12  KJs     ESCAN00079643    No changes
 *                         KJs     ESCAN00079645    No changes
 *                         KJs     ESCAN00081057    Last Rx Message object is not initialized correctly.
 *                         KJs     ESCAN00081138    Stay in Boot is not working
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( FBLDRVCAN_TMS470DCANCRX_VERSION != 0x0112u ) || \
    ( FBLDRVCAN_TMS470DCANCRX_RELEASE_VERSION != 0x00u )
# error "Error in fbl_hw.c: Source and header file are inconsistent!"
#endif
#if ( FBLDRVCAN_TMS470DCANCRX_VERSION != _FBLDRVCAN_VERSION ) || \
    ( FBLDRVCAN_TMS470DCANCRX_RELEASE_VERSION != _FBLDRVCAN_RELEASE_VERSION )
# error "Error in fbl_hw.c: Source and v_ver.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* If number of FIFO entries is undefined fall back to normal behavior (no FIFO) */
#  if ! defined( FBL_CAN_NUMBER_OF_FIFO_ENTRIES )
#   define FBL_CAN_NUMBER_OF_FIFO_ENTRIES   1u
#  endif /* ! FBL_CAN_NUMBER_OF_FIFO_ENTRIES */

#  if ( (FBL_CAN_FIRST_RX_MSGOBJ + ((FBL_CAN_NUMBER_OF_ALL_RX_ID * FBL_CAN_NUMBER_OF_FIFO_ENTRIES) - 1u)) > 16u )
#   error "FBL_HW.C: Number of FIFO entries exceeds HW objects"
#  endif

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/* CanObjectHandle has to be unsigned! */
typedef vuint16 CanObjectHandle;

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/* Flag for confirmation function */
static vuint8 msgObjectActive;
#   if defined( FBL_ENABLE_MULTIPLE_NODES )
V_MEMROM0 V_MEMROM1 tCanId V_MEMROM2 CanTxId[] = FBL_MULTIPLE_TXID;
V_MEMROM0 V_MEMROM1 tCanId V_MEMROM2 CanRxId[] = FBL_MULTIPLE_RXID;
#   endif /* FBL_ENABLE_MULTIPLE_NODES */

/* Global Data ***************************************************************/
tCanId   CanRxActualId;
vuint8 CanRxActualDLC; /* is set in FblCanProcessRxMessage() */

/* Initialization values for CAN-Controller */
extern tFblCanIdTable   fblCanIdTable;

/* Initialization data for registers of CAN controller */
V_MEMROM0 V_MEMROM1 tFblCanIdTable V_MEMROM2 kFblCanIdTable =
{
   CAN_BITTIMING,
   CAN_BRP_REG,
#  if ( FBL_CAN_NUMBER_OF_RX_ID > 0 )
   {
    FBL_CAN_RX_ID0
#   if ( FBL_CAN_NUMBER_OF_RX_ID > 1 )
    ,FBL_CAN_RX_ID1
#   endif
#   if ( FBL_CAN_NUMBER_OF_RX_ID > 2 )
    ,FBL_CAN_RX_ID2
#   endif
#   if ( FBL_CAN_NUMBER_OF_RX_ID > 3 )
    ,FBL_CAN_RX_ID3
#   endif
#   if ( FBL_CAN_NUMBER_OF_RX_ID > 4 )
    ,FBL_CAN_RX_ID4
#   endif
   },
#  endif /* FBL_CAN_NUMBER_OF_RX_ID > 0 */
};

/* Receive buffer for FblCanProcessRxMsg and FblCanStartMsg */
vuint8 tmpMsgData[8];

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  JSR_APPL
 **********************************************************************************************************************/
/*! \brief       Perform jump to application
 **********************************************************************************************************************/
void JSR_APPL( void )
{
   ((ApplFct)ApplIntJmpTable)();
   /* Code below should never be reached */
}

/***********************************************************************************************************************
 *  FblTimerInit
 **********************************************************************************************************************/
/*! \brief       Initialize the timer for a 1ms-cycle
 **********************************************************************************************************************/
void FblTimerInit( void )
{
   /* The RTI module is initialized as follows. The first UpCounter and CompareUpCounter */
   /* are initialized to pass half of RTICLK to FreeRunningCounter. Compare0 and         */
   /* UpdateCompare0 are used to generate cyclic interrupt INT0                          */
   FBL_RTIGCTRL    = 0x00000000;   /* stop Timers for configuration              */
   FBL_RTICOMPCTRL = 0;            /* all compare registers are linked with RTI1 */
   FBL_RTICPUC0    = 1;            /* f(free running counter) = RTICLK/2         */
   FBL_RTIUC0      = 0;            /* start up counter at zero                   */
   FBL_RTIFRC0     = 0;            /* start free running counter at zero         */
   FBL_RTICOMP0    = FBL_TIMER_RELOAD_VALUE;
   FBL_RTIUDCP0    = FBL_TIMER_RELOAD_VALUE;
   FBL_RTIINTFLAG |= 0x01;         /* clear possible interrupt indication        */
   FBL_RTIGCTRL    = 0x00000001;   /* start the RTI1                             */
}

/***********************************************************************************************************************
 *  FblGetTimerValue
 **********************************************************************************************************************/
/*! \brief       Returns current timer value
 *  \pre         Timer must be running
 *  \return      Current timer value
 **********************************************************************************************************************/
vuint16 FblGetTimerValue( void )
{
   return ((vuint16)FBL_RTIFRC0);
}

/***********************************************************************************************************************
 *  FblCanInit
 **********************************************************************************************************************/
/*! \brief       Initialization of the CAN registers
 **********************************************************************************************************************/
void FblCanInit( void )
{
   CanObjectHandle hwObjHandle;
   CanObjectHandle hwObjHandleMax;
   vuint32 fblCanIdTableIdx;
   vuint16 eobState;

   /* Set CAN-cell into init mode, set control register, clear status register */
   FBL_CNTRLREG = kFblSetCCE | kFblSetInit;

   /* Init bit-timing with user-generated data */
   FBL_BITTIMING = fblCanIdTable.BIT;

   /* Init baud rate prescaler extension with user-generated data */
   FBL_BRP_REG = fblCanIdTable.BRP;

   /* Initialize unused message objects **************************************/

   for (hwObjHandle = 1; hwObjHandle <= CAN_NUMBER_OF_HW_OBJECTS; hwObjHandle++)
   {
      /* Write Mask,Arb,Control,DataA,DataB set */
      FBL_IF_INIT.ComMask        = kFblSetWrRd | kFblSetMask | kFblSetArb | kFblSetControl | kFblSetDataA | kFblSetDataB;

      /* Unused MsgObject => MsgVal=0 */
      FBL_IF_INIT.ArbitrFld1     = kFblClearAll;
      FBL_IF_INIT.ArbitrFld2     = kFblClearAll;
      /* Mask and Data=0 */
      FBL_IF_INIT.Mask1          = kFblClearAll;
      FBL_IF_INIT.Mask2          = kFblClearAll;
      FBL_IF_INIT.Data.ulData[0] = kFblClearAll;
      FBL_IF_INIT.Data.ulData[1] = kFblClearAll;

      FBL_IF_INIT.MessageCntrl   = kFblSetEoB;

      /* Start copy data from IFx to ObjectRAM */
      /* First address of Object in CAN-Cell starts with 1 ! */
      FBL_IF_INIT.ComRequest     = hwObjHandle;

      do
      {
         /* Hardware loop timer */
      } while ( (FBL_IF_INIT.ComRequest & kFblIFxBusy) == kFblIFxBusy );
   }

   /* Initialize Tx message object *******************************************/

   /* Write Mask,Arb,Control,DataA,DataB set */
   FBL_IF_INIT.ComMask        = kFblSetWrRd | kFblSetMask | kFblSetArb | kFblSetControl | kFblSetDataA | kFblSetDataB;

   /* ID=0,MsgVal,Dir set */
   FBL_IF_INIT.ArbitrFld2     = kFblSetMsgVal | kFblSetDir;
   FBL_IF_INIT.ArbitrFld1     = kFblClearAll;

   /* EoB set */
   FBL_IF_INIT.MessageCntrl   = kFblSetEoB;

   /* Mask and Data=0, MDir set */
   FBL_IF_INIT.Mask1          = kFblClearAll;
   FBL_IF_INIT.Mask2          = kFblSetMDir;
   FBL_IF_INIT.Data.ulData[0] = kFblClearAll;
   FBL_IF_INIT.Data.ulData[1] = kFblClearAll;

   /* Start copy data from IFx to ObjectRAM */
   /* First address of Object in CAN-Cell starts with 1 ! */
   FBL_IF_INIT.ComRequest     = FBL_CAN_FIRST_TX_MSGOBJ;

   do
   {
      /* Hardware loop timer */
   } while( (FBL_IF_INIT.ComRequest & kFblIFxBusy) == kFblIFxBusy );

#  if ( FBL_CAN_NUMBER_OF_RX_ID > 0 )

   /* Initialize full-CAN Rx message objects *********************************/

   hwObjHandleMax = FBL_CAN_NUMBER_OF_RX_ID * FBL_CAN_NUMBER_OF_FIFO_ENTRIES;

   for (hwObjHandle = 0; hwObjHandle < hwObjHandleMax; hwObjHandle++)
   {
      fblCanIdTableIdx = hwObjHandle / FBL_CAN_NUMBER_OF_FIFO_ENTRIES;
      eobState = (0u == ((hwObjHandle + 1u) % FBL_CAN_NUMBER_OF_FIFO_ENTRIES)) ? kFblSetEoB : 0u;

      /* Write Mask,Arb,Control,DataA,DataB set */
      FBL_IF_INIT.ComMask        = kFblSetWrRd | kFblSetMask | kFblSetArb | kFblSetControl | kFblSetDataA | kFblSetDataB;

      /* ID=0,Dir=0,MsgVal */
      FBL_IF_INIT.ArbitrFld2     = kFblSetMsgVal | (vuint16)(fblCanIdTable.CanRxId[fblCanIdTableIdx]);

      {
         FBL_IF_INIT.Mask2          = kFblSetMXtd | kFblSetMDir;
         FBL_IF_INIT.Mask1          = kFblClearAll;
         /* EoB,DLC set */
         FBL_IF_INIT.MessageCntrl   = eobState;
      }

      /* Data=0 */
      FBL_IF_INIT.Data.ulData[0] = kFblClearAll;
      FBL_IF_INIT.Data.ulData[1] = kFblClearAll;

      /* Start copy data from IFx to ObjectRAM */
      /* First address of Object in CAN-Cell starts with 1 ! */
      FBL_IF_INIT.ComRequest     = FBL_CAN_FIRST_RX_MSGOBJ + hwObjHandle;

      do
      {
         /* Hardware loop timer */
      } while ((FBL_IF_INIT.ComRequest & kFblIFxBusy) == kFblIFxBusy);
   }
#  endif /* FBL_CAN_NUMBER_OF_RX_ID > 0 */

   /* Switch from init mode to normal operation mode */
   FBL_CNTRLREG = kFblSetPMD; /* Disable parity and interrupts */

   /* Initialize Flags */
   /* Flag for confirmation function */
   msgObjectActive = 0;
}

/***********************************************************************************************************************
 *  FblCanParamInit
 **********************************************************************************************************************/
/*! \brief       Initialize CAN initialization parameters
 **********************************************************************************************************************/
void FblCanParamInit( void )
{
   fblCanIdTable.BIT = CanInitTable.CanInitBT0;
   fblCanIdTable.BRP = CanInitTable.CanInitBT1;

   /* The TxId is set in FblDiagInit() */
   fblCanIdTable.CanRxId[0] = MK_ID16(CanInitTable.TpRxIdHigh, CanInitTable.TpRxIdLow);
}

/***********************************************************************************************************************
 *  FblCanMsgTransmitted
 **********************************************************************************************************************/
/*! \brief       This function returns kCanTxOk if the message was transmitted.
 *  \pre         CAN interface must be initialized before call
 *  \return      CAN message was transmitted or not
 **********************************************************************************************************************/
vuint8 FblCanMsgTransmitted( void )
{
   vuint8 returnCode;

   if (FBL_TRANSREQUEST(0) & ((vuint32)FBL_CAN_CHECK_TX_MASK))
   {
      /* Message transmission currently in progress */
      returnCode = kCanTxInProcess;
   }
   else
   {
      if (msgObjectActive != 0)
      {
         /* Clear the flag */
         msgObjectActive = 0;

         returnCode = kCanTxOk;
      }
      else
      {
         returnCode = kCanTxFailed;
      }
   }

   return returnCode;
}

/***********************************************************************************************************************
 *  FblCanProcessRxMessage
 **********************************************************************************************************************/
/*! \brief       Processing of the Id of the received CAN messages.
 *  \details     After the type of the message is determined, the right RxCanMsg-Handle is passed to FblHandleRxMsg().
 *               Finally the receive buffer is released.
 *  \pre         CAN hardware has to be initialized before call
 **********************************************************************************************************************/
void FblCanProcessRxMsg( void )
{
#  if ( FBL_CAN_NUMBER_OF_ALL_RX_ID > 0 )
   CanObjectHandle rxObjHandle;
   CanObjectHandle hwObjHandleMax;

   hwObjHandleMax = FBL_CAN_NUMBER_OF_ALL_RX_ID * FBL_CAN_NUMBER_OF_FIFO_ENTRIES;

   for (rxObjHandle = 0; rxObjHandle < hwObjHandleMax; rxObjHandle++)
   {
      if ((FBL_NEWDATA(0) & ((vuint32)0x1 << (FBL_CAN_FIRST_RX_MSGOBJ - 1 + rxObjHandle))) != 0)
      {
         /* WrRd,Mask,Arb,Control,ClrIntPnd,TxRqstNDat,DataA,DataB set */
         FBL_IF_RECEIVE.ComMask    = kFblSetMask | kFblSetArb | kFblSetControl | kFblSetClrIntPnd | kFblSetTxRqstNDat | kFblSetDataA | kFblSetDataB;
         FBL_IF_RECEIVE.ComRequest = FBL_CAN_FIRST_RX_MSGOBJ + rxObjHandle;

         do
         {
            /* Hardware loop timer */
         } while( (FBL_IF_RECEIVE.ComRequest & kFblIFxBusy) == kFblIFxBusy );
         CanRxActualId  = GET_ID(FBL_IF_RECEIVE.ArbitrFld2);
         CanRxActualDLC = FBL_IF_RECEIVE.MessageCntrl & 0xF;  /* Only bits 0-3 are interesting for DLC */
         tmpMsgData[3]  = FBL_IF_RECEIVE.Data.ucData[0];
         tmpMsgData[2]  = FBL_IF_RECEIVE.Data.ucData[1];
         tmpMsgData[1]  = FBL_IF_RECEIVE.Data.ucData[2];
         tmpMsgData[0]  = FBL_IF_RECEIVE.Data.ucData[3];
         tmpMsgData[7]  = FBL_IF_RECEIVE.Data.ucData[4];
         tmpMsgData[6]  = FBL_IF_RECEIVE.Data.ucData[5];
         tmpMsgData[5]  = FBL_IF_RECEIVE.Data.ucData[6];
         tmpMsgData[4]  = FBL_IF_RECEIVE.Data.ucData[7];

         /* Now handle the message
          * rxObjHandle is passed to FblHandleRxMsg instead of FblRxCanMsgXHdl defines */
         FblHandleRxMsg((rxObjHandle / FBL_CAN_NUMBER_OF_FIFO_ENTRIES), tmpMsgData);
      }
   }
#  endif
}

#  if defined( FBL_ENABLE_STAY_IN_BOOT ) 
/***********************************************************************************************************************
 *  FblCanStartMsg
 **********************************************************************************************************************/
/*! \brief       Returns kFblOk after a programming request has been received
 *  \pre         CAN hardware has to be initialized before call
 *  \return      Return value indicates whether a programming request was received
 **********************************************************************************************************************/
vuint8 FblCanStartMsg( void )
{
   CanObjectHandle rxStartMsgObjHandle;
#   if defined( CAN_RX_SLOT_STARTMSG2 )
   CanObjectHandle rxStartMsg2ObjHandle;
#   endif

   rxStartMsgObjHandle = FBL_CAN_FIRST_RX_MSGOBJ - 1 + (CAN_RX_SLOT_STARTMSG * FBL_CAN_NUMBER_OF_FIFO_ENTRIES);
#   if defined( CAN_RX_SLOT_STARTMSG2 )
   rxStartMsg2ObjHandle = FBL_CAN_FIRST_RX_MSGOBJ - 1 + (CAN_RX_SLOT_STARTMSG2 * FBL_CAN_NUMBER_OF_FIFO_ENTRIES);
#   endif

   if (  ((FBL_NEWDATA(0) & ((vuint32)0x1 << rxStartMsgObjHandle)) != 0)
#   if defined( CAN_RX_SLOT_STARTMSG2 )
      || ((FBL_NEWDATA(0) & ((vuint32)0x1 << rxStartMsg2ObjHandle)) != 0)
#   endif
      )
   {
      /* WrRd,Mask,Arb,Control,ClrIntPnd,TxRqstNDat,DataA,DataB set */
      FBL_IF_RECEIVE.ComMask = kFblSetMask | kFblSetArb | kFblSetControl | kFblSetClrIntPnd | kFblSetTxRqstNDat | kFblSetDataA | kFblSetDataB;

      if ((FBL_NEWDATA(0) & ((vuint32)0x1 << rxStartMsgObjHandle)) != 0)
      {
         FBL_IF_RECEIVE.ComRequest = rxStartMsgObjHandle + 1;
      }
#   if defined( CAN_RX_SLOT_STARTMSG2 )
      else if ((FBL_NEWDATA(0) & ((vuint32)0x1 << rxStartMsg2ObjHandle)) != 0)
      {
         FBL_IF_RECEIVE.ComRequest = rxStartMsg2ObjHandle + 1;
      }
#   endif

      do
      {
         /* Hardware loop timer */
      } while ((FBL_IF_RECEIVE.ComRequest & kFblIFxBusy) == kFblIFxBusy);

      CanRxActualId = GET_ID(FBL_IF_RECEIVE.ArbitrFld2);

      CanRxActualDLC = FBL_IF_RECEIVE.MessageCntrl & 0xF;  /* Only bits 0-3 are interesting for DLC */
      tmpMsgData[3]  = FBL_IF_RECEIVE.Data.ucData[0];
      tmpMsgData[2]  = FBL_IF_RECEIVE.Data.ucData[1];
      tmpMsgData[1]  = FBL_IF_RECEIVE.Data.ucData[2];
      tmpMsgData[0]  = FBL_IF_RECEIVE.Data.ucData[3];
      tmpMsgData[7]  = FBL_IF_RECEIVE.Data.ucData[4];
      tmpMsgData[6]  = FBL_IF_RECEIVE.Data.ucData[5];
      tmpMsgData[5]  = FBL_IF_RECEIVE.Data.ucData[6];
      tmpMsgData[4]  = FBL_IF_RECEIVE.Data.ucData[7];

      return (FblCheckOwnStartMsg(tmpMsgData));
   }

   return kFblFailed;
}
#  endif

/***********************************************************************************************************************
 *  FblCanTransmit
 **********************************************************************************************************************/
/*! \brief       This function transmits a CAN message
 *  \pre         CAN interface must be initialized before call
 *  \param[in]   Transmit handle
 *  \return      The function returns kCanTxOk, or kCanTxFailed
 **********************************************************************************************************************/
vuint8 FblCanTransmit( tMsgObject * tmtObject )
{
   if (FblCanMsgTransmitted() == kCanTxInProcess)
   {
      /* no loop if message object is in use */
      return kCanTxFailed;
   }

   do
   {
      /* Hardware loop timer */
   } while( (FBL_IF_TRANSMIT.ComRequest & kFblIFxBusy) == kFblIFxBusy );

   /* WrRd,Mask,Arb,Control,DataA,DataB set */
   FBL_IF_TRANSMIT.ComMask        =  kFblSetWrRd | kFblSetMask | kFblSetArb | kFblSetControl | kFblSetDataA | kFblSetDataB;

   /* copy ID, Dlc and Data from the MsgObjectBuffer in IF1 */
   FBL_IF_TRANSMIT.ArbitrFld2     = tmtObject->msgObject.Id  | kFblSetMsgVal | kFblSetDir;

   /* set Dlc and TxRqst */
   FBL_IF_TRANSMIT.MessageCntrl   = tmtObject->msgObject.DLC | kFblSetTxRqst | kFblSetEoB;

   FBL_IF_TRANSMIT.Data.ucData[0] = tmtObject->msgObject.DataFld[3];
   FBL_IF_TRANSMIT.Data.ucData[1] = tmtObject->msgObject.DataFld[2];
   FBL_IF_TRANSMIT.Data.ucData[2] = tmtObject->msgObject.DataFld[1];
   FBL_IF_TRANSMIT.Data.ucData[3] = tmtObject->msgObject.DataFld[0];
   FBL_IF_TRANSMIT.Data.ucData[4] = tmtObject->msgObject.DataFld[7];
   FBL_IF_TRANSMIT.Data.ucData[5] = tmtObject->msgObject.DataFld[6];
   FBL_IF_TRANSMIT.Data.ucData[6] = tmtObject->msgObject.DataFld[5];
   FBL_IF_TRANSMIT.Data.ucData[7] = tmtObject->msgObject.DataFld[4];

   /* start copy data from IF1 to ObjectRAM */
   FBL_IF_TRANSMIT.ComRequest     = FBL_CAN_FIRST_TX_MSGOBJ;

   /* Flag for confirmation function */
   msgObjectActive = 1;

   return kCanTxOk;
}

/***********************************************************************************************************************
 *  FblCanErrorTask
 **********************************************************************************************************************/
/*! \brief       Check error flags of CAN controller.
 *  \details     If a bus-off is detected, ApplFblCanBusOff is called.
 **********************************************************************************************************************/
void FblCanErrorTask( void )
{
   /* check for status register (bus error) */
   if (((FBL_STATREG & kFblErrBusOff) == kFblErrBusOff) && ((FBL_CNTRLREG & kFblSetInit) == kFblSetInit))
   {
      /*Bus off occurred, DCAN entered init-mode */
      ApplFblCanBusOff(); /* call application specific function */
   }
}

#  if defined( FBL_ENABLE_SLEEPMODE ) 
/***********************************************************************************************************************
 *  FblCanSleep
 **********************************************************************************************************************/
/*! \brief       Enter sleep mode
 *  \pre         CAN interface must be initialized before call
 *  \return      kFblOk/kFblFailed
 **********************************************************************************************************************/
vuint8 FblCanSleep( void )
{
   vuint8 i;

   FBL_CNTRLREG |= kFblSetWUBA; /* Enable WakeUp on Bus Activity (WUBA) */
   FBL_CNTRLREG |= kFblSetPDR;  /* Start local PowerDown mode */

   i = 0;
   while (! FblCanIsAsleep()) /* Wait for PowerDown */
   {
      if ((FblLookForWatchdog() & FBL_TM_TRIGGERED) == FBL_TM_TRIGGERED)
      {
         if (i++ > 10) /* Return if it takes too long */
         {
            FBL_CNTRLREG &= FblInvert32Bit(kFblSetPDR); /* Clear PowerDown request */
            return kFblFailed;
         }
      }
   }
   return kFblOk;
}

/***********************************************************************************************************************
 *  FblCanWakeUp
 **********************************************************************************************************************/
/*! \brief       Nothing to do when waking up.
 **********************************************************************************************************************/
void FblCanWakeUp( void )
{
}

#   if defined( FBL_ENABLE_WAKEUP_INT )
/***********************************************************************************************************************
 *  CanWakeUpInterrupt
 **********************************************************************************************************************/
/*! \brief       Wakeup interrupt service function. Can be configured to inside vector table for sleep/wakeup handling.
 *  \pre         Wakeup interrupt has to be enabled before entering sleepmode
 **********************************************************************************************************************/
void FblCanWakeUpInterrupt( void )
{
   /* Inform application */
   ApplFblCanWakeUp();
}
#   endif
#  endif /* FBL_ENABLE_SLEEPMODE */

/***********************************************************************************************************************
 *  FblReadByte
 **********************************************************************************************************************/
/*! \brief       This function can be used to read one byte from the given address.
 *  \param[in]   Memory address to read out
 *  \return      Byte at memory location 'address'
 **********************************************************************************************************************/
vuint8 FblReadByte(FBL_ADDR_TYPE address)
{
   return *(vuint8 MEMORY_FAR *)address;
}

/***********************************************************************************************************************
 *  FblReadBlock
 **********************************************************************************************************************/
/*! \brief       This function copies a given amount of bytes from the memory address 'address' into the buffer handed over
 *  \details     by the pointer 'buffer'
 *  \param[in]   address Memory address to read out
 *  \param[out]  buffer The target buffer for the returned data
 *  \param[in]   length The number of bytes to read
 *  \return      Number of actually copied bytes
 **********************************************************************************************************************/
FBL_MEMSIZE_TYPE FblReadBlock( FBL_ADDR_TYPE address, vuint8 * buffer, FBL_MEMSIZE_TYPE length )
{
   (void)MEMCPY (buffer, address, length);

   return length;
}

#if defined( FBL_USE_OWN_MEMCPY )
/***********************************************************************************************************************
 *  ownmemcpy
 **********************************************************************************************************************/
/*! \brief       Bytewise copy of size bytes from source to destination
 *  \param[out]  Dest Pointer to destination buffer
 *  \param[in]   Source Pointer to source buffer
 *  \param[in]   size Number of bytes to be copied
 *  \return      Pointer to destination
 **********************************************************************************************************************/
void * ownmemcpy( void * dest, const void * source, vuint16 size )
{
   vuint16 i;

   for (i = 0; i < size; i++)
   {
      ((vuint8 *)dest)[i] = ((vuint8 *)source)[i];
      if ( ( (i) & ((vuint16) 0x3fu) ) == 0 )
      {
         (void)FblLookForWatchdog();
      }
   }
   return dest;
}
#endif

/***********************************************************************************************************************
 *  END OF FILE: FBL_HW.C
 **********************************************************************************************************************/
