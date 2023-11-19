/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file          fbl_hdr.h
 *  \brief         Secure Fbl header parsing module
 *                 Declaration of functions, variables, and constants
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
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Vijay Natarajan               Vna           Vector CANtech, Inc.
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id       Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2014-01-31  AWh                     First release
 *  01.01.00   2014-03-04  AWh     ESCAN00073546   No changes
 *                                 ESCAN00073567   Change to LibMem 2.xx interface; Minor Improvements
 *                                 ESCAN00073883   No changes
 *                                 ESCAN00073910   No changes
 *                         Dod     ESCAN00074067   No changes
 *  01.02.00   2014-07-14  Dod     ESCAN00075058   No changes
 *                         AWh     ESCAN00075272   GapFill need to use at least FLASH_SEGMENT_SIZE buffer size
 *                                 ESCAN00075353   No changes
 *                                 ESCAN00075678   No changes
 *                                 ESCAN00075685   No changes
 *                                 ESCAN00076569   SLP6: Download with more than one calibration in partition fails
 *                                 ESCAN00076911   Sba ticket is stored and read inclusive the data type
 *                                 ESCAN00077055   No changes
 *  01.03.00   2014-09-11  HRf     ESCAN00078350   Add cast to second parameter when FblMemGetInteger() is called in
 *                                                 the context of FblHdrInitFblRomCheck()
 *  01.04.00   2014-11-05  AWh     ESCAN00078423   HDR_GAPFILL_BUFF_SIZE is not defined
 *                         Vna     ESCAN00078603   Padding added to tagBlockRegion structure in order for proper byte
 *                                                 alignment. Aligned to 2 bytes for 16-bit and 4 byte for 32-bit micros
 *                         AWh     ESCAN00078852   No changes
 *                                 ESCAN00078855   No changes
 *                                 ESCAN00079183   No changes
 *                                 ESCAN00079328   SLP5: Order of calibration partition members is not checked
 *  02.00.00   2015-01-15  AWh     ESCAN00079553   Add Compression Interface
 *  02.00.01   2015-01-16  AWh     ESCAN00080628   Allow overwriting Data compression type
 *  02.01.00   2015-02-09  AWh     ESCAN00081143   Programming failure on transfer data
 **********************************************************************************************************************/
#ifndef __FBL_HDR_H__
#define __FBL_HDR_H__

/* --- fbl_hdr module version --- */
/* ##V_CFG_MANAGEMENT ##CQProject : FblDiag_SecHdr_Gm CQComponent : Implementation */
#define FBLDIAGHDR_GM_VERSION           0x0201u
#define FBLDIAGHDR_GM_RELEASE_VERSION   0x00u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "fbl_apfb.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES: FblHdr specific Debug status error codes
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* FblHDr module specific IDs (>0x6000)*/
# define kHdrDebugErrInvalidApplHeaderAddress      0x6000u
# define kHdrDebugErrConflictingPSILocation        0x6100u
# define kHdrDebugErrIlegalSwRegion                0x6200u
# define kHdrDebugErrSegmentEndInd                 0x6300u

# define kHdrDebugErrBlockStart                    0x6400u
# define kHdrDebugErrBlockEnd                      0x6500u
# define kHdrDebugErrBlockVerify                   0x6600u
# define kHdrDebugErrSegStart                      0x6700u
# define kHdrDebugErrDataInd                       0x6800u
# define kHdrDebugErrSegEnd                        0x6900u
# define kHdrDebugErrTransferTooSmall              0x6A00u
#endif

/***********************************************************************************************************************
 *  DEFINES: Data Types
 **********************************************************************************************************************/

#define DATA_TYPE_PLAIN_V1 0x0101u
#define DATA_TYPE_PROTECTED 0x0200u
#define DATA_TYPE_SIGNED_V1 0x0301u
#define DATA_TYPE_COMPRESSED_V1 0x0401u
#define DATA_TYPE_ENCRYPTED 0x0500u
#define DATA_TYPE_DIFFERENCE 0x0600u

/* Allows using different data type in case of user compression is to be used (check which data type shall be used) */
#if defined( DATA_TYPE_COMPRESSED_USER )
# define DATA_TYPE_COMPRESSED DATA_TYPE_COMPRESSED_USER
#else
# define DATA_TYPE_COMPRESSED DATA_TYPE_COMPRESSED_V1
#endif

/***********************************************************************************************************************
 *  DEFINES: Header field length
 **********************************************************************************************************************/

#define HDR_DATATYPE_LEN 2u /**< Data type field length */
#define HDR_NBROF_LEN 2u /**< Length for fields depicting the number of entries, regions, partitions, cal modules */
#define HDR_MID_LEN 2u /**< Module Id field length, used in several headers */
#define HDR_DCID_LEN 2u /**< Data Compatibility Id field length, used in several headers */
#define HDR_NBID_LEN 2u /**<  Not Before Id field length, used in several headers (key, application) */
#define HDR_ECUNAME_LEN 8u /**< Ecu name field length */
#define HDR_ECUID_LEN 16u /**< Ecu Id field length */

#define HDR_REGADDR_LEN 4u /**< Region Info address field length */
#define HDR_REGLEN_LEN 4u /**< Region Info length field length */

#define HDR_MSGDIGEST_LEN 32u /**< Message Digest field length */
#define HDR_SIGNATURE_LEN 256u /**< Signature field length (both root/signer signature) */
#define HDR_SUBJNAME_LEN 16u /**< Subject name field length */
#define HDR_CERTID_LEN 8u /**< Certificate Id field length ( the field itself is not used in the bootloader ) */
#define HDR_SIGNPKEY_LEN 256u /**< Public key field length */

#define HDR_CS_LEN 2u /**< Integrity Word == Checksum field length */
#define HDR_CFGOPT_LEN 2u /**< Configuration options field length */
#define HDR_DLS_LEN 2u /**< Design Level Suffix field length */
#define HDR_HEXPN_LEN 4u /**< Hex Part Number field length */
#define HDR_ASCIIPN_LEN 16u /**< Ascii Part Number field length */

/* Total length of data to store one region - address field + length field */
#define HDR_REGINFO_LEN (HDR_REGADDR_LEN + HDR_REGLEN_LEN)
/* Length of data that each calibration requires within the application header */
#define HDR_CALINFO_LEN (HDR_MID_LEN + HDR_DCID_LEN + HDR_REGINFO_LEN)
/* Length of the entire signed header */
#define HDR_SIGINFO_LEN (HDR_SUBJNAME_LEN + HDR_CERTID_LEN + HDR_NBROF_LEN + HDR_SIGNPKEY_LEN + HDR_SIGNATURE_LEN)

/* Length of the entire SBA ticket */
#define HDR_SBA_TICKET_LEN (HDR_DATATYPE_LEN + HDR_MID_LEN + HDR_ECUNAME_LEN + HDR_ECUID_LEN + HDR_SIGINFO_LEN + HDR_SIGNATURE_LEN)
/* Maximum Length of downloaded module (this is always an application module) */
#define HDR_MODULE_MAX_RAW_LEN (HDR_SBA_TICKET_LEN + (SWM_MAX_NOAR * HDR_REGINFO_LEN) + HDR_MSGDIGEST_LEN + HDR_NBROF_LEN)

#if defined ( FBL_ENABLE_COMPRESSION_MODE )
# if !defined( HDR_MODULE_MAX_POSS_COMPR_GROWTH )
/* Default consideration: Maximum possible growth when compression is applied is by 1/5 */
#  define HDR_MODULE_MAX_POSS_COMPR_GROWTH  (((HDR_MODULE_MAX_RAW_LEN - 1u)/5)+1u)
# endif  /* HDR_MODULE_MAX_POSS_COMPR_GROWTH */
# define HDR_MODULE_MAX_LEN (HDR_MODULE_MAX_RAW_LEN + HDR_MODULE_MAX_POSS_COMPR_GROWTH)
#else
# define HDR_MODULE_MAX_LEN HDR_MODULE_MAX_RAW_LEN
#endif /* FBL_ENABLE_COMPRESSION_MODE */

/***********************************************************************************************************************
 *  DEFINES: Header field indices
 **********************************************************************************************************************/

/***** Generic data type index (always at the very start) ******/
#define HDR_DATYPE_IDX 0u /**< Datatype is envelop start field, in front of any header */

/***** Field static indexes in Signed Header Info  V1 ******/
/** Data Type field  */
#define S1_MID_IDX (HDR_DATYPE_IDX + HDR_DATATYPE_LEN) /**< Module-ID field index */
#define S1_DCID_IDX (S1_MID_IDX + HDR_MID_LEN) /**< Data-Compatibility-Identifier field index */
#define S1_ECUNAME_IDX (S1_DCID_IDX + HDR_DCID_LEN) /**< Ecu name App/Cal field index */
#define S1_ECUID_IDX (S1_ECUNAME_IDX + HDR_ECUNAME_LEN) /** Ecu Id field index */
#define S1_APP_NBID_IDX (S1_ECUID_IDX + HDR_ECUID_LEN) /**< App NBID field index */
#define S1_APP_SWLINFO_IDX (S1_APP_NBID_IDX + HDR_NBID_LEN) /**< App Sw Location Info struct index */

/* No static App indexes after Sw Location Info, further indexes are dynamic */
#define S1_CAL_MSGDIGEST_IDX (S1_ECUID_IDX + HDR_ECUID_LEN) /** Cal Message Digest index */
#define S1_CAL_SIGINFO_IDX (S1_CAL_MSGDIGEST_IDX + HDR_MSGDIGEST_LEN) /** Cal Signer Info index */
#define S1_CAL_SIGNATURE_IDX (S1_CAL_SIGINFO_IDX + HDR_SIGINFO_LEN) /** Cal Signature index */

/* For SBA-ticket different Indices have to be used (no starting HDR_DATYPE_IDX ) */
#define S1_SBA_MID_IDX S1_MID_IDX /**< SBA ticket Module-ID field index */
#define S1_SBA_ECUNAME_IDX (S1_SBA_MID_IDX + HDR_MID_LEN) /**< SBA ticket Ecu name index */
#define S1_SBA_SIGNINFO_IDX (S1_SBA_ECUNAME_IDX + (HDR_ECUNAME_LEN+HDR_ECUID_LEN)) /**< SBA ticket Signer Info index */
#define S1_SBA_SIGNATURE_IDX (S1_SBA_SIGNINFO_IDX + HDR_SIGINFO_LEN) /**< SBA ticket Signature index */

/***** Field static indexes in Signer Info ******/
#define SIGNINFO_SUBJNAME_IDX  0u /**< Signer Info Subject Name index */
#define SIGNINFO_CERTID_IDX (SIGNINFO_SUBJNAME_IDX + HDR_SUBJNAME_LEN) /**< Signer Info Certificate ID index */
#define SIGNINFO_KEYNBID_IDX (SIGNINFO_CERTID_IDX + HDR_CERTID_LEN) /**< Signer Info Key-NBID index */
#define SIGNINFO_SIGNPKEY_IDX (SIGNINFO_KEYNBID_IDX + HDR_NBID_LEN) /**< Signer Info Signer Public Key index */
#define SIGNINFO_ROOTSIGN_IDX (SIGNINFO_SIGNPKEY_IDX + HDR_SIGNPKEY_LEN) /**< Signer Info Root Signature index */

/***** Field static indexes in Plain Header V1 (programmed) ******/
#define P1_CS_IDX  0u /**< Integrity word field  */
#define P1_CFGOPT_IDX  (P1_CS_IDX + HDR_CS_LEN) /**< Configuration Options */
#define P1_MID_IDX  (P1_CFGOPT_IDX + HDR_CFGOPT_LEN) /**< Configuration Options */
#define P1_DCID_IDX (P1_MID_IDX + HDR_MID_LEN) /**< Data-Compatibility-Identifier field */
#define P1_APP_NBID_IDX (P1_DCID_IDX + HDR_DCID_LEN) /**< App NBID */
#define P1_APP_DLS_IDX (P1_APP_NBID_IDX + HDR_NBID_LEN) /**< DLS Appl behind App NBID */
#define P1_APP_HEXPN_IDX (P1_APP_DLS_IDX + HDR_DLS_LEN) /**< Hex Part number Appl */
#define P1_APP_ASCIIPN_IDX (P1_APP_HEXPN_IDX + HDR_HEXPN_LEN) /**< Asci Part number Appl */

#define P1_CAL_DLS_IDX (P1_DCID_IDX + HDR_DCID_LEN) /**< DLS Cal behind DCID */
#define P1_CAL_HEXPN_IDX (P1_CAL_DLS_IDX + HDR_DLS_LEN) /**< Hex Part number Cal */
#define P1_CAL_ASCIIPN_IDX (P1_CAL_HEXPN_IDX + HDR_HEXPN_LEN) /**< Asci Part number Cal */
/* No static App indexes after Ascii Pn, further indexes are dynamic */

/***********************************************************************************************************************
 *  DEFINES: Module Type handling
 **********************************************************************************************************************/

#define MOD_TYPE_APPL 0u
#define MOD_TYPE_CAL 1u
#define MOD_TYPE_RESERVED 2u
#define MOD_TYPE_NOTSUPPORTED 3u

#define IsCalDownLd() (MOD_TYPE_CAL == parsedModuleInfo.modType)

/***********************************************************************************************************************
 *  DEFINES: Module Id handling
 **********************************************************************************************************************/

#define MIN_MODULE_ID ((vuint8)1u)
#define MAX_MODULE_ID ((vuint8)99u)

/** Macros for evaluating type of module-id */
#define MODID_FBL_1 ((vuint16) 71u)
#define MODID_FBL_LAST ((vuint16) 80u)
#define MODID_SBA ((vuint16) 91u)

#if defined( FBL_USER_OPSW1_ID )
# define MODID_OPSW_1 ((vuint16) FBL_USER_OPSW1_ID)
#else
# define MODID_OPSW_1 ((vuint16) 1u)
#endif
#define MODID_OPSW_2 ((vuint16) 21u)
#define MODID_OPSW_3 ((vuint16) 31u)
#define MODID_OPSW_4 ((vuint16) 41u)

#define START_CALID_OPSW_1 (MODID_OPSW_1 + 1u)
#define START_CALID_OPSW_2 (MODID_OPSW_2 + 1u)
#define START_CALID_OPSW_3 (MODID_OPSW_3 + 1u)
#define START_CALID_OPSW_4 (MODID_OPSW_4 + 1u)

#if defined( FBL_USER_OPSW1_ID )
# define END_CALID_OPSW_1 (FBL_USER_OPSW1_ID + 9u)
#else
# define END_CALID_OPSW_1 (MODID_OPSW_2 -1)
#endif
#define END_CALID_OPSW_2 (MODID_OPSW_3 -1)
#define END_CALID_OPSW_3 (MODID_OPSW_4 -1)
#define END_CALID_OPSW_4 (50u)

#define IsIdOfOpsw1(id) (MODID_OPSW_1 == (id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define IsIdOfOpsw2(id) (MODID_OPSW_2 == (id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define IsIdOfOpsw3(id) (MODID_OPSW_3 == (id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define IsIdOfOpsw4(id) (MODID_OPSW_4 == (id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */

#if !defined(IsResCalIdOpsw1)
# define IsResCalIdOpsw1(id) 0u
#endif
#if !defined(IsResCalIdOpsw2)
# define IsResCalIdOpsw2(id) 0u
#endif
#if !defined(IsResCalIdOpsw3)
# define IsResCalIdOpsw3(id) 0u
#endif
#if !defined(IsResCalIdOpsw4)
# define IsResCalIdOpsw4(id) 0u
#endif

#define IsIdOfCalForOpsw1(id)  ( (((id) >= START_CALID_OPSW_1) && ((id) <= END_CALID_OPSW_1) ) || IsResCalIdOpsw1(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define IsIdOfCalForOpsw2(id)  ( (((id) >= START_CALID_OPSW_2) && ((id) <= END_CALID_OPSW_2) ) || IsResCalIdOpsw2(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define IsIdOfCalForOpsw3(id)  ( (((id) >= START_CALID_OPSW_3) && ((id) <= END_CALID_OPSW_3) ) || IsResCalIdOpsw3(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define IsIdOfCalForOpsw4(id)  ( (((id) >= START_CALID_OPSW_4) && ((id) <= END_CALID_OPSW_4) ) || IsResCalIdOpsw4(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */

#if defined( FBL_ENABLE_MULTI_PROCESSOR )
# define ModIdOpSw(id)        ( IsIdOfOpsw1(id) || IsIdOfOpsw2(id) || IsIdOfOpsw3(id) || IsIdOfOpsw4(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define ModIdCalibration(id) ( IsIdOfCalForOpsw1(id) || IsIdOfCalForOpsw2(id) || IsIdOfCalForOpsw3(id) || IsIdOfCalForOpsw4(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#else
# define ModIdOpSw(id)        ( IsIdOfOpsw1(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define ModIdCalibration(id) ( IsIdOfCalForOpsw1(id) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif
#define ModIdFbl(id)          ( ((id) >= MODID_FBL_1) && ((id) <= MODID_FBL_LAST) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */

#define ModIdSba(id)          ( (id) == (vuint16)MODID_SBA )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define ModIddInRange(id)     ( (MIN_MODULE_ID <= (id)) && (MAX_MODULE_ID >=(id)) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define ModIdReserved(id)     ( ModIddInRange(id) && !( ModIdOpSw(id) || ModIdCalibration(id) ||  ModIdFbl(id) || ModIdSba(id)) )  /* PRQA S 3453 */ /* MD_CBD_19.7 */

/* Bits 15-7 are reserved, 6-0 contains mid */
#define FilterMid(mid)  ((mid) & 0x007F)  /* PRQA S 3453 */ /* MD_CBD_19.7 */

/***********************************************************************************************************************
 *  DEFINES: Partition handling
 **********************************************************************************************************************/
/* Macros for setting partition ID */
#define PARTID_BASEMOD  ((vuint8) 0x01u)
#define PARTID_MASK_BASE  (0x0Fu)
#define PARTID_MASK_CAL  (0xF0u)

#define PARTID_OPSW_1   (PARTID_BASEMOD + 0x00u)
#define PARTID_OPSW_2   (PARTID_BASEMOD + 0x10u)
#define PARTID_OPSW_3   (PARTID_BASEMOD + 0x20u)
#define PARTID_OPSW_4   (PARTID_BASEMOD + 0x30u)

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/* changing base partition definition in this case */
# define FblHdrGetBasePartId(pid) (((pid) & PARTID_MASK_CAL) + PARTID_BASEMOD) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#else
/* no base partition definitions in this case */
# define FblHdrGetBasePartId(pid) (pid)  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#define FblHdrGetFirstCalPartNbr(opSwPartition) ((opSwPartition)+1u) /* PRQA S 3453 */ /* MD_CBD_19.7 */

/** Function to return partition Id for a partition Number 1..n inside a given Application header (opSwId) */
#if defined( FBL_ENABLE_MULTI_PROCESSOR )
# define PartIdOpSw(pid)        ( ((pid) == PARTID_OPSW_1) || ((pid) == PARTID_OPSW_2) || ((pid) == PARTID_OPSW_3) || ((pid) == PARTID_OPSW_4) ) /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define FblHdrGetCalPartNbrFromId(pid) ((pid) - FblHdrGetBasePartId(pid)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#else
# define PartIdOpSw(pid)        ((pid) == PARTID_OPSW_1 ) /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define FblHdrGetCalPartNbrFromId(pid) ((pid) - PARTID_OPSW_1) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#define PartIdCal(pid)          (((pid) & PARTID_MASK_BASE)> PARTID_BASEMOD) /* PRQA S 3453 */ /* MD_CBD_19.7 */

#define FblHdrGetParsedModPartId() (parsedModuleInfo.partID)

# define IsCalFirstModInPartition() (CALPOS_FIRST == (parsedModuleInfo.calPos & CALPOS_FIRST))  /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define IsCalLastModInPartition() (CALPOS_LAST == (parsedModuleInfo.calPos & CALPOS_LAST)) /* PRQA S 3453 */ /* MD_CBD_19.7 */

/* This is the maximum number of allowed gaps between partitions, it may be increased by user configuration file. */
#if !defined( HDR_MAX_PARTITION_TRACKED_REGIONS )
/* one entry is usually 5 byte and is required per to be large enough to hold (n+1+m)
 * - n appl regions
 * - 1 appl presence pattern
 * - m cal erase regions from all partitions
 */
# define HDR_MAX_PARTITION_TRACKED_REGIONS 15
#endif

#define HDR_GAPFILL_BUFF_SIZE_DEFAULT 0x40u
#if !defined( HDR_GAPFILL_BUFF_SIZE )
# if (FLASH_SEGMENT_SIZE < HDR_GAPFILL_BUFF_SIZE_DEFAULT)
#  define HDR_GAPFILL_BUFF_SIZE HDR_GAPFILL_BUFF_SIZE_DEFAULT
# else
    /* In case of multiple mem devices, this will only work if internal flash has the largest Segment Size.
     * If this is not the case, the following will happen:
     *  - If assertions are enabled: assertion is hit during gap fill, else
     *  - LibMem will not write chunks within Gapfill feature.
     */
#  define HDR_GAPFILL_BUFF_SIZE FLASH_SEGMENT_SIZE
# endif
#endif

/***********************************************************************************************************************
 *  DEFINES: PEC (Programming error code)
 **********************************************************************************************************************/
/* DebugStatus will also be set to PecError.(no overwriting) */
#define FblSetPecErr(errNo)  {\
                                 errPecErrorCode = (errNo);\
                                 FblErrDebugStatus(errNo);\
                             } /* PRQA S 3458 */ /* MD_CBD_19.4 */
/* Also init Debug status */
#define FblClrPecErr() {\
                           errPecErrorCode = 0;\
                           FblErrStatSetError(0);\
                       } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblGetPecErr() (errPecErrorCode)

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
# define  Err_Revoke    (IsCalDownLd()? Err_RevokeCal : Err_RevokeSW)
# define  Err_GetInfo   (IsCalDownLd()? Err_GetCalInfo : Err_GetAppInfo)
# define  Err_Erase     (IsCalDownLd()? Err_EraseCal : Err_EraseSW)
# define  Err_CID       (IsCalDownLd()? Err_CCID : Err_BCID)
#else
# define  Err_Revoke  Err_RevokeSW
# define  Err_GetInfo Err_GetAppInfo
# define  Err_Erase   Err_EraseSW
# define  Err_CID     Err_BCID
#endif

#define Err_NoError          ((vuint16)0x0000u) /* No errors. */
#define Err_PartitionId      ((vuint16)0x0001u) /* Indicates  an  attempt  to  erase/program  a  non-supported partition. */
#define Err_SwNotPresent     ((vuint16)0x0002u) /* This  error  indicates  an  attempt  to  program  a calibration  module  while  the  ECU  is  not  fully programmed with valid application software. */
#define Err_RevokeSW         ((vuint16)0x0003u) /* This  error  indicates  a  failed  attempt  to  revoke  the Application  SW  partition  (i.e.  set  the  PSI  to  a "Revoked" state). */
#define Err_EraseSW          ((vuint16)0x0004u) /* This  error  indicates  a  failed  attempt  to  erase  the Application SW partition. */
#define Err_GetAppInfo       ((vuint16)0x0005u) /* Indicates an error reading the Application SW Info */
#define Err_RevokeCal        ((vuint16)0x0006u) /* This  error  indicates  a  failed  attempt  to  revoke  the calibration  partition  (i.e.  set  the  PSI  to  a  "Revoked" state). */
#define Err_EraseCal         ((vuint16)0x0007u) /* This  error  indicates  a  failed  attempt  to  erase  the calibration partition. */
#define Err_GetCalInfo       ((vuint16)0x0008u) /* Indicates an error reading the Cal Info */
#define Err_UpdatePSI        ((vuint16)0x0009u) /* This error indicates a failed attempt to set the PSI to a "Programmed" State. */
#define Err_DataType         ((vuint16)0x000Au) /* Data Type not supported */
#define Err_Compression      ((vuint16)0x000Bu) /* Error while decompressing the data stream. */
#define Err_LengthExceeded   ((vuint16)0x000Cu) /* The received file length exceeds the expected length. This PEC is set when the bootloader received more data in the  Transfer  Data service than  the  allocated space. */
#define Err_MoreDataExpected ((vuint16)0x000Du) /* Indicates the length of the programmable file is below the expected length. This PEC is set when the client send  a Transfer End service while the bootloader is expecting more data to be written to flash. */
#define Err_FlashWrite       ((vuint16)0x000Eu) /* Error writing a block to the flash memory */
#define Err_ModuleID         ((vuint16)0x000Fu) /* Module ID is not expected or out of sequence */
#define Err_BCID             ((vuint16)0x0010u) /* Application SW is not compatible with bootloader */
#define Err_CCID             ((vuint16)0x0011u) /* Calibration Module is not compatible with Application SW */
#define Err_EcuName          ((vuint16)0x0012u) /* The  ECU  name in  the flash file  does not  match the one stored in the bootloader Info block */
#define Err_EcuId            ((vuint16)0x0013u) /* The flash file contains a non-zero ECU ID that does not match the ECU ID */
#define Err_SwRegion         ((vuint16)0x0014u) /* The Application SW Region Info is out of the reserved region as defined in the Boot Info block */
#define Err_Signature        ((vuint16)0x0015u) /* The signature on the header is invalid */
#define Err_AppNBID          ((vuint16)0x0016u) /* The  Application  SW  has  an  older  security  version than the current programmed version. */
#define Err_SubjectName      ((vuint16)0x0017u) /* The  Subject  Name  in  the  file  does  not  match  the expected subject name stored in the Boot Info block */
#define Err_KeyNBID          ((vuint16)0x0018u) /* The Cert  key  has an  older security version than the previously used one known to the boot loader. */
#define Err_Cert             ((vuint16)0x0019u) /* Invalid Cert signature */
#define Err_CalRegion        ((vuint16)0x001Au) /* The Cal Region Info is out of the reserved region as defined in the Boot Info block */
#define Err_MD               ((vuint16)0x001Bu) /* The  message  digest  calculated  over  the  flashed content  does  not  match  the  Message  Digest  in  the signed header */
#define Err_RootSignature    ((vuint16)0x001Cu) /* The Signer Info certificate has invalid signature */
#define Err_MsgOutOfSequence ((vuint16)0x001Du) /* General  error indicating that  the bootloader receives a message out of sequence during the programming session */

/***********************************************************************************************************************
 *  DEFINES: Boot Initialization Status masks and access macros
 **********************************************************************************************************************/

#define SBILSB_ERR_BOOTROMCHK ((vuint8) 0x01u)
#define SBILSB_ERR_RAMCHK ((vuint8) 0x02u)

#define SBIMSB_ERR_ECUNAME ((vuint8) 0x01u)
#define SBIMSB_ERR_ECUID ((vuint8) 0x02u)
#define SBIMSB_ERR_SIGNATURE ((vuint8) 0x04u)
#define SBIMSB_ERR_CERT ((vuint8) 0x08u)
#define SBIMSB_SBI_VALID ((vuint8) 0x80u)

#define FblClrBisBytes()         {\
                                   FblClrBisLsbByte();FblClrBisMsbByte();\
                                 } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblSetBisLsbByte(nbr)    {\
                                   fblHdrBis.integrity |= (nbr);\
                                 } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblClrBisLsbByte()       {\
                                    fblHdrBis.integrity = 0u;\
                                 } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblGetBisLsbByte()       (fblHdrBis.integrity)
#define FblSetBisMsbByte(nbr)    {\
                                   fblHdrBis.signBypass |= (nbr);\
                                 } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblClrBisMsbByte()       {\
                                   fblHdrBis.signBypass = 0u;\
                                 } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblGetBisMsbByte()       (fblHdrBis.signBypass)

/***********************************************************************************************************************
 *  DEFINES: Data Processing
 **********************************************************************************************************************/

#define FblHdrSetHdrExpectedState()   {\
                                         rcvBuffHandle.transferState = DIAG_TRANSFERDATA_HDR_EXPECTED;\
                                      } /* PRQA S 3458 */ /* MD_CBD_19.4 */

#define FblHdrGetHdrExpectedState()   (DIAG_TRANSFERDATA_HDR_EXPECTED == rcvBuffHandle.transferState)  /* PRQA S 3453 */ /* MD_CBD_19.7 */

/***********************************************************************************************************************
 *  DEFINES: FBL_UTIL
 **********************************************************************************************************************/
#define FblHdrGetAlignedAddr(address, segSize) ((address)+GetBytesToNextAlignedAddress(address, segSize)) /* PRQA S 3453 */ /* MD_CBD_19.7 */

/*
 * Macro for calling FblRealTimeSupport() every n-th loop cycle. This is
 * required for short loops which will be slowed down significantly if the
 * function is called every loop cycle.
 */
#if !defined(FblRealTimeSupportInLoop)
# define FblRealTimeSupportInLoop(loopCtr, loopfrqu) { \
                                             if (((loopCtr) & ((loopfrqu) - 1)) == 0) \
                                             { \
                                                (void)FblRealTimeSupport(); \
                                             } \
                                          } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGE_OVERFLOW_LEN )
# define FBL_UTIL_CHECK_RANGE_OVERFLOW_LEN(rangeStart, rangeLength) \
   (((rangeLength) > 0u) && ((rangeStart) > (((FBL_ADDR_TYPE)(-1)) - ((rangeLength) - 1u))))  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN )
# define FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN(address, rangeStart, rangeLength) \
   (((address) >= (rangeStart)) && (((address) - (rangeStart)) < (rangeLength)))  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGE_CONTAINED_LEN )
# define FBL_UTIL_CHECK_RANGE_CONTAINED_LEN(address, length, rangeStart, rangeLength) \
   (   FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN(address, rangeStart, rangeLength)          \
    && ((length) <= (rangeLength))                                                    \
    && (((address) - (rangeStart)) <= ((rangeLength) - (length))))  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR )
# define FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(address, endAddress, rangeStart, rangeEnd ) \
   (((endAddress) < (rangeStart) ) || ((address) > (rangeEnd)))  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGES_EXCLUSIVE_LEN )
# define FBL_UTIL_CHECK_RANGES_EXCLUSIVE_LEN(address, length, rangeStart, rangeLength ) \
   FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR((address), (address)+((length)-1), (rangeStart), (rangeStart)+((rangeLength)-1))  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#if !defined( FBL_UTIL_GET_RANGE_LEN )
# define FBL_UTIL_GET_RANGE_LEN(startAddress, endAddresss) (((endAddresss)-(startAddress)) + (tFblAddress)1)  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

#if !defined( FBL_UTIL_GET_RANGE_END )
# define FBL_UTIL_GET_RANGE_END(startAddress, rangeLen) ((startAddress)+((rangeLen)-1))  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

/***********************************************************************************************************************
 *  DEFINES: Optional User functions
 **********************************************************************************************************************/

#if !defined( APPL_FBL_READ_SEC_DATA )
/* This function can be implemented in case the controller requires to differentiate between Ram and Rom pointers.
 * The default just passes a Rom pointer to the security module, whereas a Ram pointer is expected, which is
 * no problem for usual machines.
 */
# define APPL_FBL_READ_SEC_DATA(romLocation) (romLocation)  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

/***********************************************************************************************************************
 *  DEFINES: Other
 **********************************************************************************************************************/
#if defined( FBL_ENABLE_DATA_PROCESSING )
# define FblHdrModIsRawData() (MODULE_DF_RAW == currMemLibSegment.dataFormat)
#else
# define FblHdrModIsRawData() 1u
#endif

#define FblHdrGetDataInCurrSegment(byteNumber)  (tTpDataType) ((((tFblLength)(byteNumber) > currDataSegInfo.length))?currDataSegInfo.length : (byteNumber)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define FblHdrConsumeCurrSegmentBytes(bytesToConsume) {\
                                         currDataSegInfo.targetAddress += (bytesToConsume);\
                                         currDataSegInfo.length -= (bytesToConsume);\
                                      } /* PRQA S 3458 */ /* MD_CBD_19.4 */

# define FblHdrGetDataIndBytes(byteNumber) ((FblHdrModIsRawData())? FblHdrGetDataInCurrSegment(byteNumber):(byteNumber)) /* PRQA S 3453 */ /* MD_CBD_19.7 */

#define SWM_MAX_NOAR                         SWM_DATA_MAX_NOAR

#if defined (FBL_ENABLE_VERIFY_INTEGRITY_WORD)
# define FblHdrCheckDldIntegrityWord SecM_VerifyClassVendor
#endif

#if !defined( FBL_HDR_MAX_S1_FILL )
/** According to GM there might be a gap filled with dummy content between S1 and P1 header */
#define FBL_HDR_MAX_S1_FILL FBL_MAX_SEGMENT_SIZE
#endif

#define NO_HEADER_ADDR_DEFINED  ((tFblAddress)0xFFFFFFFFu)
#define NO_PP_ADDRESS_DEFINED ((tFblAddress)0xFFFFFFFFu)
#define NO_PART_ID_DEFINED ((vuint8)0x00u)
#define FIRST_CAL_NBR ((vuint8)0x01u)
#define DOWNLOAD_MODULE_NOT_ACCEPTED ((tTpDataType)0u)
#define UNDEFINED_LOG_BLOCK ((vuint8)0xFFu)
#define PARTITION_REGION_ERROR ((vuintx)0xFFu)
#define PARTITION_REGION_UNDEF ((vuintx)0xFEu)

#define NO_CALPART_ACTIVE ((vuint8)0x00u)
#define CALPART_ACTIVE ((vuint8)0x01u)
#define CALPART_START_SEQU ((vuint8)0x02u)

#define ASCII_PN_PRESENT ((vuint16)0x0001u)
#define ASCII_PN_NOT_PRESENT ((vuint16)0x0000u)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

typedef enum
{
   SEG_OUTSIDE_BLOCK, /**< segment region distinct from flashBlock */
   SEG_COVERS_BLOCK_START, /**< segment starts at start of the flashBlock or earlier */
   SEG_START_INSIDE_BLOCK /**< segment starts somewhere INSIDE flashBlock */
} tSegmentBlockLocation;

typedef enum
{
   CAL_REGION_STATE_ERASED, /**< calibration area region of last downloaded application is erased; state is cleared upon any application download */
   CAL_REGION_STATE_UNKNOWN /**< calibration area region of last downloaded application is unknown, erasure has to take place */
} tCalRegionErasedState;

typedef enum
{
   ERASE_BLOCK, /**< the block shall be erased on module currently downloaded */
   ERASE_AND_FILL, /**< the block shall be erased and filled on module currently downloaded (module owns but does not use block) */
   DO_NOT_ERASE_BLOCK, /**< module currently downloaded shall not erase the current block */
   ILLEGAL_MODULE_SEGMENT, /**< module currently downloaded defines segments that are not allowed to be erased by this module */
   GENERAL_ERROR /**< Some error occurred while analyzing erase blocks for module */
} tEraseCheckResult;

typedef enum
{
   ECUID_DO_ZEROCHECK, /* check against 16 byte zero array */
   ECUID_SKIP_ZEROCHECK /* no check against 16 byte zero array */
} tEcuIdZeroCheck;

typedef enum
{
   MODULE_DF_RAW = 0x00u /**< data format is raw */
#if defined( FBL_ENABLE_DATA_PROCESSING )
   ,MODULE_DF_COMPR = 0x01 /**< data format is compressed */
   ,MODULE_DF_ENCR = 0x02 /**< data format is encrypted */
   ,MODULE_DF_COMPR_HDR = 0x11 /**< data format is compressed, only process header */
   ,MODULE_DF_ENCR_HDR = 0x12 /**< data format is encrypted, only process header */
#endif
} tdataFormat;

typedef enum
{
   CALPOS_IRRELEVANT = 0x00u /* If the position is not relevant */
   ,CALPOS_FIRST = 0x01u
   ,CALPOS_LAST = 0x04u
   ,CALPOS_SINGLE = 0x05u  /** This is (CALPOS_FIRST|CALPOS_LAST) */
} tCalPosType;

#if defined( FBL_ENABLE_GAP_FILL )
typedef enum
{
   BLOCK_USED_BY_PARTITION = 0x01u,
   BLOCK_UNUSED_AND_IN_OTHER_PARTITION = 0x02u,
   BLOCK_UNUSED_AND_IN_BASEPARTITION = 0x03u,
   BLOCK_UNUSED_NOT_IN_BASEPARTITION = 0x04u
} tFblBlockPartUsage;
#endif

typedef enum
{
   DIAG_TRANSFERDATA_IDLE,
   DIAG_TRANSFERDATA_HDR_EXPECTED
} tTransferState;

typedef struct
{
   vuint32 flagA; /**< SBI flag information A */
   vuint32 flagB; /**< SBI flag information B */
} tSBIFlag;

typedef struct
{
   vuint8 integrity; /**< integrity flag */
   vuint8 signBypass; /**< signature bypass status */
} tBootInitStatus;

typedef struct
{
#if defined( FBL_ENABLE_MULTI_PROCESSOR )
   vuint8 MID; /**< Module-ID */
#endif
   vuint16 stored; /**< stored Value */
   vuint16 received; /**< received Value */
} tNBIDInfo;

/* Structure for download module information */
typedef struct
{
   vuint8         modType;         /**< module Type */
   vuint8         MID;             /**< Module-ID */
   vuint8         partID;          /**< Partition-ID */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   tCalPosType    calPos;          /**< Cal Position */
#endif
   FL_SegmentListType segmentList;
} tDownloadModuleInfo;

typedef struct
{
   vuint8 nbrOfRgns; /**< Number of regions */
   tFblAddress startAddr; /**< Module start address */
   tFblAddress swLocInfoAddr; /**< start address of Sw Location Info section in Memory */
   tFblAddress nbPartAddr; /**< start address of Number of partitions field */
} tBaseModInfo;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/* Structure for partition information */
typedef struct
{
   vuint8 partID; /**< Partition-ID */
   vuint8 nbrOfPrts; /**< Number of partitions */
   vuint8 nbrOfRgns; /**< Number of regions */
   vuint8 nbrOfCals; /**< Number of cals */
   tFblAddress partInfoAddr; /**< start address of partition Info section for this Id in Memory */
   vuint16 lengthPart;  /**< length of partition info block */
   tFblAddress psiStart; /**< start address of psi for this partition */
} tCalPartitionInfo;

typedef struct
{
   vuint8 calNbr; /**< Current calibration file number in partition 1..n */
   tFblAddress startAddr; /**< Start address of "Cal Info" structure in memory */
} tCalInfo;
#endif

typedef struct
{
   vsint16 startBlk;
   vsint16 endBlk;
#if defined( FBL_ENABLE_CALIBRATION_MODULES) || defined( FBL_ENABLE_MULTI_PROCESSOR )
   vuint8 partID;
# if defined( C_CPUTYPE_32BIT ) || defined( C_CPUTYPE_16BIT )
   vuint8 padding1; /* 2 byte alignment for structure */
# endif
# if defined( C_CPUTYPE_32BIT )
   vuint16 padding2; /* 4 byte alignment for structure */
# endif
#endif
} tPartitionSegment;

typedef struct
{
   tPartitionSegment segList[HDR_MAX_PARTITION_TRACKED_REGIONS];
} tPartitionBlockList;

typedef struct
{
   tTpDataType buffIdx;
   tTransferState transferState;
   vuint16 progDataOffset; /* Offset to next programming Data to be indicated */
} tHdrRcvBuffHdl;

/***********************************************************************************************************************
 *  GLOBALS
 **********************************************************************************************************************/

V_MEMRAM0 extern V_MEMRAM1 tDownloadModuleInfo V_MEMRAM2 parsedModuleInfo;
V_MEMRAM0 extern V_MEMRAM1 tBootInitStatus V_MEMRAM2 fblHdrBis;
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 calPartActive;
#endif
V_MEMRAM0 extern V_MEMRAM1 tHdrRcvBuffHdl V_MEMRAM2 rcvBuffHandle;
V_MEMRAM0 extern V_MEMRAM1_NEAR FL_SegmentInfoType V_MEMRAM2_NEAR currDataSegInfo;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR errPecErrorCode;

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

/* FblHdr Init Routines */
void FblHdrInitPowerOn(void);
void FblHdrInit(void);
void FblHdrInitRetry(void);

/* Checkers Setters and Getters */
vuint8 FblGetModType( const vuint8 mid );
void FblHdrGetPsiRegion( vuint8 partId, V_MEMRAM1 IO_PositionType V_MEMRAM2 V_MEMRAM3 * const pPsiAddr, V_MEMRAM1 IO_SizeType V_MEMRAM2 V_MEMRAM3 * const pPsiLen );
tFblAddress FblHdrGetBaseModuleHdrAddr( const vuint8 partId );
V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 * FblGetBlockDescriptor( vuint8 partId );
#if defined( FBL_ENABLE_MULTI_PROCESSOR )
vuint8 FblHdrGetBasePartIdFromModuleId( const vuint8 mid );
#else
#define FblHdrGetBasePartIdFromModuleId(x)  PARTID_OPSW_1
#endif
#if (FBL_MTAB_NO_OF_BLOCKS > 1)
vuint8 GetBlockNbrFromPartId( const vuint8 partId );
#else
#define GetBlockNbrFromPartId(partId) 0
#endif
tFblAddress FblHdrCheckModuleValidAndGetAddr( const vuint8 mid );
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
tFblResult FblHdrGetCalPartitionValid( vuint8 opSwPartId );
vuint8 FblHdrGetNoOfCalPartitions( vuint8 opSwPartId );
#endif
# if defined( FBL_HDR_ENABLE_GAP_FILL )
tFblBlockPartUsage FblHdrPartSegmentListGetBlockStatus( vsint16 blockNbr, vuint8 partId );
#endif

/* Fbl Hdr Services */
void FblHdrRealTimeSupportSecMod( void );
#if defined( FBL_ENABLE_GAP_FILL )
tFblResult FblHdrFillGaps( void );
#endif
tFblResult FblHdrTransferDataProcess( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const rcvBuffer, tTpDataType rcvDataSize);
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
tFblResult FblHdrInitCalPartInfoAndCalInfo( vuint8 calMid, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * const pCalPartInfo,
                                            V_MEMRAM1 tCalInfo V_MEMRAM2 V_MEMRAM3 * const pCalInfo );
#endif
#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
void FblHdrInitFblRomCheck( V_MEMRAM1 FL_SegmentListType V_MEMRAM2 V_MEMRAM3 * const segList );
#endif
vuint16 FblCalculateCsum16Bit( const V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * buffer, vuint32 len, vuint16 csum );
const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblHdrCheckEnvelopesExtractSignedHeader( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const rcvBuffer, tTpDataType rcvDataSize);

/* Post Flash Validation (called out of LibMem context) */
tFblResult FblHdrPostFlashValidation( void );
tFblResult FblHdrUpdatePSI( void );
SecM_StatusType FblHdrVerifyIntegrity( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * verifyParam );
SecM_StatusType FblHdrPipelinedVerifyIntegrity( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * verifyParam );

#if ( (!defined( FBL_ENABLE_MULTI_PROCESSOR ) && !defined( FBL_DISABLE_MULTI_PROCESSOR ) ) ||\
      (defined( FBL_ENABLE_MULTI_PROCESSOR ) && defined( FBL_DISABLE_MULTI_PROCESSOR ) ) )
# error "Error in fbl_cfg.h: Missing or incorrect usage of Multi processor feature switch."
#endif

#if ( (!defined( FBL_ENABLE_CALIBRATION_MODULES ) && !defined( FBL_DISABLE_CALIBRATION_MODULES ) ) ||\
      (defined( FBL_ENABLE_CALIBRATION_MODULES ) && defined( FBL_DISABLE_CALIBRATION_MODULES ) ) )
# error "Error in fbl_cfg.h: Missing or incorrect usage of Calibration Modules feature switch."
#endif

#if ( (!defined( FBL_ENABLE_DATA_PROCESSING ) && !defined( FBL_DISABLE_DATA_PROCESSING ) ) ||\
      (defined( FBL_ENABLE_DATA_PROCESSING ) && defined( FBL_DISABLE_DATA_PROCESSING ) ) )
# error "Error in fbl_cfg.h: Missing or incorrect usage of Data processing interface switch."
#endif

#if ( (defined( FBL_ENABLE_COMPRESSION_MODE ) && !defined( FBL_ENABLE_DATA_PROCESSING ) ) ||\
      (defined( FBL_ENABLE_COMPRESSION_MODE ) && defined( FBL_DISABLE_DATA_PROCESSING ) ) )
# error "Error in fbl_cfg.h: Compression requires Data processing switch to be active."
#endif

#if ( FBL_DIAG_BUFFER_LENGTH < (HDR_MODULE_MAX_LEN) )
# error "Error in fbl_cfg.h: FBL_DIAG_BUFFER_LENGTH is not large enough. Check minimum configuration."
#endif

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
# if ( FBL_GMHEADER_NOAR_FBL > SWM_DATA_MAX_NOAR)
#  error "SWM_DATA_MAX_NOAR is defined to small. FBL_GMHEADER_NOAR_FBL does not fit in."
# endif
#endif

#if (!defined( SEC_ENABLE_WORKSPACE_EXTERNAL ) || !defined( SEC_ENABLE_WORKSPACE_EXTERNAL ))
# error "Both workspace external and internal need to be set."
#endif

#endif /* __FBL_HDR_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_HDR.H
 **********************************************************************************************************************/

