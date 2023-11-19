/* This file is generated automatically by MakeSupport */
#ifndef __BRSVINFO_H
#define __BRSVINFO_H

#ifndef _BRS_V_INFO_VALID
  #error Include of BrsVInfo.h is only allowed in context of BrsCtrl.c
#endif

#ifndef _BRS_ENABLE_FLAG_ACCESS_
  #define _BRS_ENABLE_FLAG_ACCESS_
#endif

#if defined ( C_COMP_IAR_M16C )
  #if defined ( _NEAR_ )
  /* Set const data to const segment, don\'t initialize */
  #pragma memory=constseg(CONST_DATA) :far
  #endif
#endif

#define BRS_ENABLE_COMPILER_VERSION
#define BRS_ENABLE_ASSEMBLER_VERSION
#define BRS_ENABLE_LINKER_VERSION
#define BRS_ENABLE_LIBRARIAN_VERSION

#if defined ( BRS_ENABLE_TOOL_VERSIONINFO )

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerVersion[] = "TMS470 C/C++ Compiler v4.9.5";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerOptimization[] = "-mv=7R4";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerWarning[] = "";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerMemoryModel[] = "";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerAdditional[] = "-fr obj -fb lst -ff lst -fs lst -dC_DERIVATIVE_LS30336 -dC_FAMILY_CORTEX_R4F -dEVA_BOARD_VEBN00573 -dBRS_TIMEBASE_CLOCK=80 -mv=7R4 -mt -k -g --abi=eabi -o2 --opt_for_speed=3 -fr obj -fb lst -ff lst -fs lst -mv=7R4 -k -g --abi=eabi --float_support=VFPv3D16 --symdebug:dwarf_version=3 --fp_mode=strict --verbose --issue_remarks";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsAssemblerVersion[] = "TMS470 C/C++ Compiler v4.9.5";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsAssemblerOptimization[] = "-mv=7R4";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsAssemblerWarning[] = "";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsAssemblerMemoryModel[] = "";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsAssemblerAdditional[] = "-fr obj -fb lst -ff lst -fs lst -dC_DERIVATIVE_LS30336 -dC_FAMILY_CORTEX_R4F -dEVA_BOARD_VEBN00573 -dBRS_TIMEBASE_CLOCK=80 -mv=7R4 -mt -k -g --abi=eabi -o2 --opt_for_speed=3 -fr obj -fb lst -ff lst -fs lst -mv=7R4 -k -g --abi=eabi --float_support=VFPv3D16 --symdebug:dwarf_version=3 --fp_mode=strict --verbose --issue_remarks";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLinkerVersion[] = "TMS470 Linker v4.9.5";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLinkerOptimization[] = "";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLinkerWarning[] = "-w ";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLinkerMemoryModel[] = "";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLinkerAdditional[] = "-i D:\\uti\\Texas_Instruments\\4.9.5\\lib -l ..\\..\\..\\BSW\\SecMod\\libSYSSERVICE_SECMODHIS_LIB.lib -m DemoFbl.map -o DemoFbl.out -e_c_int00 --be32 -a -c -x --verbose_diagnostics --zero_init=off";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLibrarianVersion[] = "TMS470 Archiver v4.9.5";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLibrarianParameter[] = "-rq GenData/BrsVInfo.h";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

#endif /* defined ( BRS_ENABLE_TOOL_VERSIONINFO ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerDef[]   = "TI";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsControllerDef[] = "TMS470 DCAN";

#if defined ( C_COMP_IAR_M16C )
  #if defined ( _NEAR_ )
  /* Normal segment usage */
  #pragma memory=default
  #endif
#endif

#endif /* __BRSVINFO_H */
