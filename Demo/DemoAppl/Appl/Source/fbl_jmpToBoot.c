/* This include is not required in your application. */
#include "fbl_inc.h"

/* these includes from the bootloaders' header files are required in your application. When CANdesc is available this is in appdesc.c */
#include "fbl_def.h"
#include "fbl_cfg.h"

void JumpToBootDemonstration (tFblLength pUncompressedMemSize, vuint8 pFormatId)
{
   tCanInitTable  CanInitTable;

   /* If SecurityAccess is configured you may check here if  Security Access has previously been enabled.
      Send neg. response code 0x22 if not. Also check for active Programming Mode    */
   if ( ( !GetSecurityUnlock()) || (!GetEnablePrgMode()) )
   {
      /* Send neg. response code 0x22 */
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
   }
   else
   {
      /* -- Initialize the diagnostic CAN-identifiers to be passed to the FBL */
      CanInitTable.TpRxIdHigh    =  (vuint8)(( MK_ID( 0x249u )>>8u)&0xFFu);
      CanInitTable.TpRxIdLow     =  (vuint8)(MK_ID( 0x249u )&0xFFu);

      CanInitTable.TpTxIdHigh    =  (vuint8)((MK_ID( 0x649u )>>8u)&0xFFu);
      CanInitTable.TpTxIdLow     =  (vuint8)(MK_ID( 0x649u )&0xFFu);

# if defined ( FBL_ENABLE_HIGHSPEED_MODE )
      if (GetHighspeedMode())
      {
         /* Reset CAN controller to low-speed mode */
         CanInitTable.CanInitCtrl0 = CAN_CTRL0_HIGHSPEED;
         CanInitTable.CanInitCtrl1 = CAN_CTRL1_HIGHSPEED;
         CanInitTable.CanInitCtrl2 = CAN_CTRL2_HIGHSPEED;
         CanInitTable.CanInitPrDiv = CAN_PRESDIV_HIGHSPEED;

         /* Which service and mode the boot is entered with */
         CanInitTable.RequestProgrammingMode = REQUEST_DOWNLOAD_MODE_HIGHSPEED;
      }
      else
# endif
      {
         /* Reset CAN controller to low-speed mode */
         /*
		 CanInitTable.CanInitCtrl0 = CAN_CTRL0;
         CanInitTable.CanInitCtrl1 = CAN_CTRL1;
         CanInitTable.CanInitCtrl2 = CAN_CTRL2;
         CanInitTable.CanInitPrDiv = CAN_PRESDIV;
         */
		 CanInitTable.CanInitBT0 = CAN_BITTIMING;
		 CanInitTable.CanInitBT1 = CAN_BRP_REG;
         /* Which service and mode the boot is entered with */
         CanInitTable.RequestProgrammingMode = REQUEST_DOWNLOAD_MODE_LOWSPEED;
      }

      /* ----------------------------------------------------------------*/
      /* -- controller independent section                            -- */
      /* -- Initialize values passed to the FBL                       -- */
      /* ----------------------------------------------------------------*/
      CanInitTable.requestDownloadMemorySize = pUncompressedMemSize;

      /* ProgrammedState: is fully programmed in application             */
      CanInitTable.ProgrammedState           = 0x00;

      /* Set error to 0x00 for "no error-condition"                      */
      CanInitTable.ErrorCode                 = 0x00;

      /* Uncompressed download format                                    */
      CanInitTable.requestDownloadFormatID   = pFormatId;

      /* ----------------------------------------------------------------*/
      /* -- Start flash boot loader.                                   --*/
      /* -- Trigger watchdog before if necessary.           --*/
      /* ----------------------------------------------------------------*/

      DiagExRCRResponsePending(kForceSendResponsePending);

      while (FblCanMsgTransmitted() == kCanTxInProcess)
      {
         ;
      }

      CallFblStart(&CanInitTable);

      /* -----------------------------------------------------------------*/
      /* -- !! NOTE: Code should never reach here  !!!   --*/
      /* ----------------------------------------------------------------*/

   } /* if (( !GetSecurityUnlock()) || (!GetEnablePrgMode())) */
}

#if 0

/* This is the corresponding example that would be used in CANdesc */


#include "fbl_def.h"
#include "fbl_cfg.h"

void DESC_API_CALLBACK_TYPE ApplDescRequestDownloadDownload(DescMsgContext *pMsgContext)
{
    tCanInitTable  CanInitTable;

   /* If SecurityAccess is configured you may check here if  Security Access has previously been enabled.
      Send neg. response code 0x22 if not.    */

   /* check if Enter Programming mode has completed */
   if( DescGetProgMode() != kDescProgModeActive )
   {
   /* Send neg. response code 0x22 */
      DescSetNegResponse(pMsgContext->iContext, kDescNrcConditionsNotCorrect);
      /* Handle the response */
      pMsgContext->resDataLen              = 0;
      DescProcessingDone(pMsgContext->iContext);
   }
   else
   {
     /* -- Initialize the diagnostic CAN-identifiers to be passed to the FBL*/
     tCanRxId0  fblRxId = MK_STDID_HI( 0x249 );
     tCanTxId0  fblTxId  = MK_STDID_HI( 0x649 );

     /* CAN Rx identifier */
     CanInitTable.TpRxIdHigh    =  (canuint8)((fblRxId>>8)&0xFF);
     CanInitTable.TpRxIdLow     =  (canuint8)(fblRxId&0xFF);

      /* CAN Tx identifer */
     CanInitTable.TpTxIdHigh    =  (canuint8)((fblTxId>>8)&0xFF);
     CanInitTable.TpTxIdLow     =  (canuint8)(fblTxId&0xFF);

      /* --------------------------------------------------*/
      /* --  Controller specific section           -- */
      /* -- Initialize CanInitTable structure   -- */
      /* ------------------------------------------------- */
# if defined ( DESC_ENABLE_REQ_HISPEED_PROG )
      if (DescGetHiSpeedMode() == kDescHiSpeedModeActive)
      {
         CanInitTable.CanInitCtrl0 = /* value generated for Mpc, check can_par.c */;
         CanInitTable.CanInitCtrl1 = /* value generated for Mpc, check can_par.c */;
         CanInitTable.CanInitCtrl2 = /* value generated for Mpc, check can_par.c */;
         CanInitTable.CanInitPrDiv = /* value generated for Mpc, check can_par.c */;

         /* Which service and mode the boot is entered with */
         CanInitTable.RequestProgrammingMode = REQUEST_DOWNLOAD_MODE_HIGHSPEED;
      }
      else
# endif
      {
         CanInitTable.CanInitCtrl0 = /* value generated for Mpc, check can_par.c */;
         CanInitTable.CanInitCtrl1 = /* value generated for Mpc, check can_par.c */;
         CanInitTable.CanInitCtrl2 = /* value generated for Mpc, check can_par.c */;
         CanInitTable.CanInitPrDiv = /* value generated for Mpc, check can_par.c */;

         CanInitTable.RequestProgrammingMode = REQUEST_DOWNLOAD_MODE_LOWSPEED;
      }

     /* ----------------------------------------------------------------*/
     /* -- controller independent section                      -- */
     /* -- Initialize values passed to the FBL               -- */
     /* ----------------------------------------------------------------*/
     CanInitTable.requestDownloadMemorySize = ((vuint32)pMsgContext->reqData[3] << 8) |
     ((vuint32)pMsgContext->reqData[4]);

     /* ProgrammedState: is fully programmed in application    */
     CanInitTable.ProgrammedState                = 0x00;

     /* Set error to 0x00 for "no error-condition"                     */
     CanInitTable.ErrorCode                             = 0x00;

     /* Uncompressed download format                                    */
     CanInitTable.requestDownloadFormatID   = pMsgContext->reqData[0];

     /* ----------------------------------------------------------------*/
     /* -- Start flash boot loader.                                   --*/
     /* -- Trigger watchdog before if necessary.           --*/
     /* ----------------------------------------------------------------*/

     CallFblStart(&CanInitTable);

     /* -----------------------------------------------------------------*/
     /* -- !! NOTE: Code should never reach here  !!!   --*/
     /* ----------------------------------------------------------------*/

   } /* if (DescGetProgMode() != kDescProgModeActive) */
}
#endif

