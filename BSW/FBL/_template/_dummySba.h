/****************************************************************
*
*  Filename:      D:\usr\dev_test\GM\SLP5\CANfbl\Geny_Fw14\_Misc\SbaTicket_Demo\dummySba.h
*  Project:       Exported definition of C-Array Flash-Driver
*  File created:  Thu Jul 17 09:14:49 2014

*
****************************************************************/

#ifndef __SBA__
#define __SBA__
#define SBA_GEN_RAND 26725

#define SBA_NUMBLOCKS 1

#define SBA_DECRYPTDATA(a)   (unsigned char)a
#define SBA_BLOCK0_ADDRESS	0x0
#define SBA_BLOCK0_LENGTH	0x336
#define SBA_BLOCK0_CHECKSUM	0x82C1u

V_MEMROM0 extern V_MEMROM1 unsigned char V_MEMROM2 sbaBlk0[SBA_BLOCK0_LENGTH];


#endif    /* #ifdef __SBA__ */

