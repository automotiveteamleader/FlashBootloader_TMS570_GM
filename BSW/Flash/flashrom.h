/****************************************************************
*
*  Filename:      D:\usr\usage\Delivery\CBD14x\CBD1400501\D00\external\BSW\Flash\Build\..\flashrom.h
*  Project:       Exported definition of C-Array Flash-Driver
*  File created:  Fri Feb 20 18:20:30 2015

*
****************************************************************/

#ifndef __FLASHDRV__
#define __FLASHDRV__
#define FLASHDRV_GEN_RAND 28385

#define FLASHDRV_NUMBLOCKS 1

#define FLASHDRV_DECRYPTVALUE	0xCC
#define FLASHDRV_DECRYPTDATA(a)   (unsigned char)((unsigned char)a ^ (unsigned char)FLASHDRV_DECRYPTVALUE)
#define FLASHDRV_BLOCK0_ADDRESS	0x8001000
#define FLASHDRV_BLOCK0_LENGTH	0x11E8
#define FLASHDRV_BLOCK0_CHECKSUM	0x1C3Du

V_MEMROM0 extern V_MEMROM1 unsigned char V_MEMROM2 flashDrvBlk0[FLASHDRV_BLOCK0_LENGTH];


#endif    /* #ifdef __FLASHDRV__ */

