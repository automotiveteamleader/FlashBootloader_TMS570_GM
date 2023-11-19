/*****************************************************************************
|
| This file contains the default exception handlers
| and exception table.
|
| Processor core : Cortex-M3
| Compiler       : Texas Instruments
|
|****************************************************************************/

extern int __main(void);
extern void _c_int00(void);

typedef void(* const ExecFuncPtr)(void);

/*
 * Exception Handlers
 */
interrupt void NMIException(void)          /* 02 */
{   while(1); }

interrupt void HardFaultException(void)    /* 03 */
{   while(1); }

interrupt void MemManageException(void)    /* 04 */
{   while(1); }

interrupt void BusFaultException(void)     /* 05 */
{   while(1); }

interrupt void UsageFaultException(void)   /* 06 */
{   while(1); }

interrupt void SVCHandler(void)            /* 11 */
{   while(1); }

interrupt void DebugMonitor(void)          /* 12 */
{   while(1); }

interrupt void PendSVC(void)               /* 14 */
{   while(1); }

interrupt void SysTickHandler(void)        /* 15 */
{   while(1); }

interrupt void InterruptHandler(void)      /* 16 and above */
{   while(1); }


/*
 * Exception Table, in seperate section so it can be correctly placed at 0x0
 */
#pragma DATA_SECTION (vector_table, ".intvecs")
ExecFuncPtr vector_table[] = {
   (ExecFuncPtr)0x08001000,   /* 00 Stack top is loaded from first entry of vector table on reset. */
   (ExecFuncPtr)&_c_int00,    /* 01 Reset */
   NMIException,              /* 02 */
   HardFaultException,        /* 03 */
   MemManageException,        /* 04 */
   BusFaultException,         /* 05 */
   UsageFaultException,       /* 06 */
   0,                         /* 07 Reserved */
   0,                         /* 08 Reserved */
   0,                         /* 09 Reserved */
   0,                         /* 10 Reserved */
   SVCHandler,                /* 11 */
   DebugMonitor,              /* 12 */
   0,                         /* 13 Reserved */
   PendSVC,                   /* 14 */
   SysTickHandler,            /* 15 */
   /* Configurable interrupts start here...*/
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler,
   InterruptHandler
};

