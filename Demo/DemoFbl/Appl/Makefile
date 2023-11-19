###############################################################################
# File Name  : Makefile                                                       #
# Description: local main project makefile                                    #
#              This makefile contains the project settings.                   #
# Application: GNU Make version 3.79.1, by Richard Stallman and Roland McGrath#
#              Built for Windows32                                            #
# FAQ        : This is only a template!                                       #
#                                                                             #
#              Examples can be found under:                                   #
#                [...]\BRS\PlatformSpecific\ECU\OrganiAndMake\can\OEM\COMPILER#
#              or on one of its parents (when it is more generic).            #
#                                                                             #
#              This Makefile includes:                                        #
#                 Makefile.project.part.defines                               #
#                 Makefile.XXX.YYY.ZZZ.make                                   #
#                 Global.Makefile.Target.make (includes <$(PROJECTNAME).dep>  #
#                                                                             #
#-----------------------------------------------------------------------------#
#               C O P Y R I G H T                                             #
#-----------------------------------------------------------------------------#
# Copyright (c) 2003 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# Uf           Ulrich Frank              Vector Informatik GmbH               #
# Arh          Andre Rauh                Vector Informatik GmbH               #
# Ard          Thomas Arnold             Vector Informatik GmbH               #
# Lo           Kai Uwe van de Loo        Vector Informatik GmbH               #
# Fsn          Steffen Frank             Vector Informatik GmbH               #
# oce          Chibueze Obikwelu         Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Ver  Sign Description                                          #
# ----------   ---  ---- -----------------------------------------------------#
# 2002/08/27   3.0  Uf   Initial version                                      #
# 2002/10/07   3.1  Uf   Added new compilers/platforms                        #
# 2002/10/23   3.2  Uf   Updated to subversion 4 (osCAN support)              #
# 2002/10/31   3.3  Uf   Subversion 5 (osCAN: addition derivative support)    #
# 2002/11/21   3.4  Uf   Subversion 6 (performance update, PCLINT_DEF added)  #
# 2002/11/21   3.5  Uf   Subversion 7 (CodeWright/VisualStudio Support)       #
# 2003/02/27   3.6  Uf   Platform specific help added                         #
# 2003/05/27   3.7  Uf   Subversion 8                                         #
# 2003/12/11   3.8  Uf   Resource information generation added                #
# 2004/07/29   3.9  Uf   QAC support added                                    #
# 2004/09/30   3.10 Uf   Check of required Makefile.config variables          #
# 2004/11/05   3.11 Uf   Subversion 9                                         #
#                        Split of XXFLAGS into XXFLAGS_WARNING...             #
#                        QAC and PCLINT modifications                         #
#                        DOXYGen settings                                     #
# 2005/11/29   3.12 Arh  Subversion 10                                        #
#                        Library generation support                           #
#                        Resource Scanner added                               #
# 2005/11/18   3.12 Uf   Subversion 10                                        #
#                        Library generation support                           #
#                        New resource scanner                                 #
# 2006-07-24   3.13 Ard  Support compilation of VStdLib in 32Bis mode         #
# 2006-11-08   3.14 Ard  Support Makefile.config INSTRUCTIONSET functionality #
# 2007-07-24   3.15 Ard  Support processor optimization + Workarounds         #
#                        in makefile.config                                   #
# 2007-07-31   3.16 Ard  Filter -o2 optimization from startup.c               #
#                        (may result in problems due to inline assembler)     #
# 2008/04/29   3.17 Lo   Subversion 11                                        #
# 2009/01/26   3.18 Kbr  added Thumb Mode support and Floating point unit     #
#                         support for TMS570                                  #
# 2013/03/01   3.19 Fsn  added Family to derivative mapping                   #
# 2013/03/13   3.20 oce  Introduced new option groups for assembler,          #
#                        compiler and linker (xFLAGS_VECTOR_MAKESUPPORT,      #
#                        xFLAGS_VECTOR_OPTIONS and xFLAGS_CUSTOMER_OPTIONS).  #
#                        For usage rules please see comments below. For more  #
#                        information please ask visagh / visntr.              #
# 2013/05/17   3.21 Fsn  added generation of RTS_LIB filename                 #
#                        VECTOR_MAKESUPPORT flags added                       #
# 2013/11/22  3.22 kkn   Multiple TMS570/470 derivatives are now supported.   #
#                        Derivative to Family mapping outsourced to BrsHw.c   #
#                        BRS_TIMEBASE_Clock renamed to BRS_TIMEBASE           #
# 2013/12/03   3.23 Fsn  REMOVED HECC/SCC WORKRAROUND defines (nowhere used)  #
#                        Changed linker suffix from lnk to ld (windows link)  #
#                        COMPILER_BASE is now set in Makefile.Config          #
# 2013/12/03   3.24 kkn  Added Makefile.derivative.memorymap                  #
###############################################################################

#MKVERBOSE=1

#------------------------------------------------------------------------------
# Include the project specific Makefile
#------------------------------------------------------------------------------
include Makefile.config
include Makefile.project.part.defines

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Version of MakeSupport used (current version is 3)
#------------------------------------------------------------------------------
VERSION = 3
SUB_VERSION = 11

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Name of this project (dir under which the appl dir resides)
# E.g.: testsuit
#------------------------------------------------------------------------------
PROJECT_NAME = DemoFbl

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# List of supported derivatives
# !!! must be equal to C_PROCESSOR_ define in v_cfg.h !!!
# E.g.: SUPPORTED_CPU = MB90V540G MB90V540
#------------------------------------------------------------------------------

# Why do we need TMS470? DbComGen? old CanGen? 
# I think we rather need TI_TMS470R1 for newer GenTools
SUPPORTED_CPU = TMS470 TI_TMS470R1 TI_TMS470 TI_TMS470P TI_TMS470PLF521 TI_TMS470_MSF542

# Added ? Why?
CPU_TYPE = TMS470

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Define Compiler path
# E.g.: COMPILER_BASE = C:\Uti\HC08\HIWARE
#       COMPILER_BIN  = $(COMPILER_BASE)\prog
#       COMPILER_INC  = $(COMPILER_BASE)\lib\hc08c\include
#       COMPILER_LIB  = $(COMPILER_BASE)\lib\hc08c\lib
#------------------------------------------------------------------------------
COMPILER_BASE = D:\uti\Texas_Instruments\4.9.5
COMPILER_BIN  = $(COMPILER_BASE)\bin
COMPILER_INC  = $(COMPILER_BASE)\include
COMPILER_LIB  = $(COMPILER_BASE)\lib

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Define flash utility 
# E.g.: FLASH = C:\Uti\TMS470\flash\flash470.exe
#------------------------------------------------------------------------------
FLASH = C:\Uti\TMS470\flash\flash470.exe

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Define hex convert utility 
# E.g.: HEX = C:\Uti\TMS470\flash\hex470.exe
#------------------------------------------------------------------------------
HEX = $(COMPILER_BIN)\hex470.exe
HEX_FLAGS = -m3           \
            -o $@         \
            -map $@.map   \
            -datawidth 32 \
            -memwidth 32  \
            -romwidth 32

#------------------------------------------------------------------------------
# Define path to PCLint
# Default if it is not defined: PCLINT = c:\uti\lint\8.0\lint-nt
#------------------------------------------------------------------------------
PCLINT = C:\Uti\Lint\8.0\lint-nt.exe

#------------------------------------------------------------------------------
# Define path to PCLint File
# Default if it is not defined: PCLINT_FILE = .\pclint.lnt
#------------------------------------------------------------------------------
PCLINT_FILE = .\pclint.lnt

#------------------------------------------------------------------------------
# Define files excluded from PCLint processing
# E.g.:
# PCLINT_EXCL  = source/foo.c \
#                $(addprefix $(ROOT), $(call compSources,IL_VECTOR))
#------------------------------------------------------------------------------
PCLINT_EXCL  =

#------------------------------------------------------------------------------
# Set platform specific options for PCLINT
# E.g.:
# PCLINT_OPT_SPECIFIC = \
#                      -si2 \
#                      -sp2
#------------------------------------------------------------------------------
PCLINT_OPT_SPECIFIC = \
                      -e46                            \
                      -fcu                            \
                      -ss2                            \
                      -si4                            \
                      -sl4                            \
                      -spN4                           \
                      -spF4                           \
                      -spF3                           \
                      -A                              \
                      -elib(950)                      \
                      +rw(asm)                        \
                      +rw(interrupt)                  \
                      -d__TMS470__                    \
                      -d__unsigned_chars__            \
                      -d__signed_chars__              \
                      -d__big_endian__                \
                      -d__little_endian__             \
                      -d__16bis__                     \
                      -d__32bis__                     \
                      -d__LINE__                      \
                      -d__FILE__                      \
                      -d__TIME__                      \
                      -d__STDC__                      \
                      -d__DATE__


#------------------------------------------------------------------------------


# Set defines for PCLINT
# E.g.:
# PCLINT_DEF  = BRS_TIMEBASE_CLOCK=$(BRS_TIMEBASE_CLOCK)
#------------------------------------------------------------------------------
PCLINT_DEF  = C_DERIVATIVE_$(DERIVATIVE)            \
              C_FAMILY_$(FAMILY_$(DERIVATIVE))      \
              $(DEF_$(INSTRUCTION_SET))             \
              BRS_TIMEBASE_CLOCK=$(BRS_TIMEBASE_CLOCK) \
							_BIG_ENDIAN                           \
							__TI_COMPILER_VERSION__

DEF_THUMB = __16bis__
DEF_ARM   = __32bis__

#This controls the DERIVATIVE -> FAMILY mapping for BrsHw
FAMILY_R1B31    = SE
FAMILY_R1VF45A  = JUPITER
FAMILY_R1VF336  = VENUS
FAMILY_R1F316   = VENUS
FAMILY_R1VF338  = TITAN
FAMILY_R1VF348  = TITAN
FAMILY_AVF68X   = AVF68X
FAMILY_PVF241PN = PVF241PN
FAMILY_PSF761 = PSF761
FAMILY_PSFC61 = CORTEX_R4F
FAMILY_PSF762 = PSF762
FAMILY_PSFC66 = CORTEX_R4F
FAMILY_MSF542 = MSF542
FAMILY_LS20216 = CORTEX_R4F
FAMILY_LS30336 = CORTEX_R4F

ifeq ($(FAMILY_$(DERIVATIVE)),)
    $(error DERIVATIVE to FAMILY mapping not defined for derivative $(DERIVATIVE))
endif

#------------------------------------------------------------------------------
# Set location of QAC
# E.g.: QAC_PATH = C:\Program Files\PRQA\QAC-6.0
#------------------------------------------------------------------------------
QAC_PATH = C:\Program Files\PRQA\QAC-6.0

#------------------------------------------------------------------------------
# Set defines for QAC Compiler Personality
#------------------------------------------------------------------------------
define QAC_PERSONALITY 
  $(ECHO) "-d \"__TMS470__        =\""                                          >> $@; \
  $(ECHO) "-d \"__unsigned_chars__=\""                                          >> $@; \
  $(ECHO) "-d \"__signed_chars__  =\""                                          >> $@; \
  $(ECHO) "-d \"__big_endian__    =\""                                          >> $@; \
  $(ECHO) "-d \"__little_endian__ =\""                                          >> $@; \
  $(ECHO) "-d \"__LINE__          =\""                                          >> $@; \
  $(ECHO) "-d \"__FILE__          =\""                                          >> $@; \
  $(ECHO) "-d \"__TIME__          =\""                                          >> $@; \
  $(ECHO) "-d \"__STDC__          =\""                                          >> $@; \
  $(ECHO) "-d \"__DATE__=\""                                                    >> $@; \
  $(ECHO) "-d \"C_DERIVATIVE_$(PLANET_$(DERIVATIVE))\""                         >> $@; \
  $(ECHO) "-d \"$(DEF_$(INSTRUCTION_SET))\""                                    >> $@; \
  $(ECHO) "-d \"BRS_TIMEBASE_CLOCK=$(BRS_TIMEBASE_CLOCK)\""                     >> $@; \
  $(ECHO) "-d \"asm=_ignore_paren\""                                            >> $@; \
  $(ECHO) "-d \"interrupt=\""                                                   >> $@; \
  $(ECHO) "-ar-"                                                                >> $@; \
  $(ECHO) "-bits-"                                                              >> $@; \
  $(ECHO) "-u+"                                                                 >> $@; \
  $(ECHO) "-fpc+"                                                               >> $@; \
  $(ECHO) "-s int=32"                                                           >> $@; \
  $(ECHO) "-s longlong=32"                                                      >> $@; \
  $(ECHO) "-s codeptr=32"                                                       >> $@; \
  $(ECHO) "-s dataptr=32"                                                       >> $@; \
  $(ECHO) "-a int=4"                                                            >> $@; \
  $(ECHO) "-a longlong=4"                                                       >> $@; \
  $(ECHO) "-a codeptr=4"                                                        >> $@; \
  $(ECHO) "-a dataptr=4"                                                        >> $@; \
  $(ECHO) "-it \"size_t=unsigned int\""                                         >> $@; \
  $(ECHO) "-it \"ptrdiff_t=signed int\""                                        >> $@; \
  $(ECHO) "-it \"wchar_t=unsigned short\""                                      >> $@; \
  $(ECHO) "-na 31"                                                              >> $@; \
  $(ECHO) "-xn 31"                                                              >> $@; \
  $(ECHO) "-xc-"                                                                >> $@; \
  $(ECHO) "-ex ansipc"                                                          >> $@; \
  $(ECHO) "-ex pc"                                                              >> $@; \
  $(ECHO) "-ex asm"                                                             >> $@; \
  $(ECHO) "-ex c++"                                                             >> $@; \
  $(ECHO) "-fpc+"                                                               >> $@; \
  $(ECHO) "-sl-"                                                                >> $@;
endef
#------------------------------------------------------------------------------
# Define files/directories excluded from Doxygen processing
# E.g.:
# DOXYGEN_EXCL_FILES += 
# DOXYGEN_EXCL_DIRS  += $(ROOT)\drv
#------------------------------------------------------------------------------
DOXYGEN_EXCL_FILES +=
DOXYGEN_EXCL_DIRS  +=

#------------------------------------------------------------------------------
# The following define overrides the rule - how to assemble a file -
# implemented in Global.Makefile.target.make.
#------------------------------------------------------------------------------
override define ASSEMBLE_RULE
        if [ -n "$(MKVERBOSE)" ] ; then                                                       \
                $(ECHO) "    $(AS) $(filter-out $(CF_FILTER_$(call _toUpper,$*)),$(ASFLAGS) $(CFLAGS_$(call _toUpper,$*))) $(INCLUDES) $(subst /,\,$<) $(REDIRECT_OUTPUT)"; \
        fi;                                                                                   \
        $(subst \,/,$(AS) $(filter-out $(CF_FILTER_$(call _toUpper,$*)),$(ASFLAGS) $(CFLAGS_$(call _toUpper,$*)))) $(INCLUDES_UNIX) $< $(REDIRECT_OUTPUT);
endef

#------------------------------------------------------------------------------
# The following define overrides the rule - how to compile a file -
# implemented in Global.Makefile.target.make.
#------------------------------------------------------------------------------
override define COMPILE_RULE
        if [ -n "$(MKVERBOSE)" ] ; then                                                       \
                $(ECHO) "    $(CC) $(filter-out $(CF_FILTER_$(call _toUpper,$*)),$(CFLAGS) $(CFLAGS_$(call _toUpper,$*))) $(INCLUDES) $(subst /,\,$<) $(REDIRECT_OUTPUT)"; \
        fi;                                                                                   \
        $(subst \,/,$(CC) $(filter-out $(CF_FILTER_$(call _toUpper,$*)),$(CFLAGS) $(CFLAGS_$(call _toUpper,$*)))) $(INCLUDES_UNIX) $< $(REDIRECT_OUTPUT);
endef

#------------------------------------------------------------------------------
# The following define overrides the rule - how to link the object files -
# implemented in Global.Makefile.target.make.
#------------------------------------------------------------------------------
override define LINK_RULE
        if [ -n "$(MKVERBOSE)" ] ; then                                                    \
                $(ECHO) "    $(subst \,/,$(LD) $(OBJECTS) $(LDFLAGS)) $(REDIRECT_OUTPUT) $(PROJECT_NAME).$(LNK_SUFFIX)"; \
        fi;                                                                                \
        $(subst \,/,$(LD) $(LDFLAGS) $(OBJECTS) $(PROJECT_NAME).$(LNK_SUFFIX)) $(REDIRECT_OUTPUT)
endef

#------------------------------------------------------------------------------
# The following define overrides the rule - how to build a lib file -
# implemented in Global.Makefile.target.make.
#------------------------------------------------------------------------------
#override define LIB_RULE
#  if [ -n "$(MKVERBOSE)" ] ; then                                             \
#    $(ECHO) '    $(LB_U) $(LBFLAGS) $(subst \,/,$^) $(REDIRECT_OUTPUT)';      \
#  fi;                                                                         \
#  $(LB_ENV_U) $(LB_U) $(subst \,/,$^) $(LBFLAGS) $(REDIRECT_OUTPUT);
#endef


################################################################################
#################### all below this line MUST be defined  ######################
################################################################################

#------------------------------------------------------------------------------
# make builds the whole project by default
# To make another target you have to add it to the prerequisites of default.
# E.g.: default: all $(PROJECT_NAME).opt
#------------------------------------------------------------------------------
default: all $(PROJECT_NAME).hex

#------------------------------------------------------------------------------
# If platform specific rules exist, they can be described here to be shown
# in the usage output of the help target
# E.g.: PLATFORM_HELP = \
#           $(ECHO) "m mytarget ................ -- build mytarget"; \
#           $(ECHO) "                               nextline";
#------------------------------------------------------------------------------
PLATFORM_HELP  = $(ECHO) "m convertflash ........... -- create hex file and flash the code"; \
                 $(ECHO) "m flash .................. -- just flash the code"

#------------------------------------------------------------------------------
# If there is anything that has to be done before dependency generation
# its target can be assigned to $(PREDEP)
# E.g.:
#  PREDEP = $(ROOT)\oseksos\tcb.c
#     where tcb.c is defined as
#
#  $(ROOT)\oseksos\tcb.c: simple.oil
#     C:\OSEK\ST10\OILTool\gen\genc167 -r $(ERROR_OUT) -i INC_PATH -f simple.oil
#
#  This rule generates the osekOS every time the oil configuration has changed
#------------------------------------------------------------------------------

###############################################################################
# Platform/compiler/derivative/emulator are relevant for the include of       #
# linker and symbol preprocessor command file identified by name.             #
# Vector standard names have to be used                                       #
###############################################################################
#------------------------------------------------------------------------------
# Compiler Manufacturer used in this project
# E.g.: ARM, COSMIC, DIABDATA, MICROTEC, TASKING, FUJITSU, GHS, NEC, GNU, GAIO
#       HIWARE, IAR, KEIL, MITSUBISHI, OKI, HEW, TI, GHS, TOSHIBA, DIABDATA
#       ALL is special for "all available compilers are supported in this file"
#------------------------------------------------------------------------------
COMPILER_MANUFACTURER = TI

#------------------------------------------------------------------------------
# Platform used for this project
# E.g.: 78K0, AT91, C16X, C5x5, CANOE, CR16, FJ16LX, H8S, HC08, HC12, M16C
#       M32C, M32R, MC68376, MGT5100, MSM9225, PIC, PPC, SH7055, ST7, TLC900
#       TMS470, V850-DCAN, V850X, XC16X
#------------------------------------------------------------------------------
PLATFORM = TMS470

#------------------------------------------------------------------------------
# Emulator used for this project
# E.g.: HITEX, LAUTERBACH, ISYSTEMS, GHS
#       ALL is special for "all derivatives are supported in this file"
#------------------------------------------------------------------------------
EMULATOR = ALL

#------------------------------------------------------------------------------
# $(AS_ENV) is the environment necessary to run the assembler
# $(AS) defines the path to the assembler
# $(ASFLAGS_ADDITIONAL_PRE) defines all additional assembler switches that will
#                           be placed in front of all other switches
# $(ASFLAGS_WARNING) defines the assembler switches concerning warning output
# $(ASFLAGS_OPTIMIZATION) defines the assembler switches concerning optimization
# $(ASFLAGS_MEMORYMODEL) defines the assembler switches concerning the memory model
# $(ASFLAGS_ADDITIONAL_POST) defines all additional assembler switches that will be
#                            placed at the end of all switches
# E.g.: AS_ENV  = export AS_BIN="$(COMPILER_BIN)"
#       AS      = $(COMPILER_BIN)\Ahc08.exe
#------------------------------------------------------------------------------
AS_ENV   = $(LD_ENV)
AS       = $(CC)

ASFLAGS_DEFAULT         =
ASFLAGS_MSF52           = --abi=eabi
ifeq ($(ASFLAGS_$(DERIVATIVE)),)
ASFLAGS                 = $(ASFLAGS_DEFAULT)
else
ASFLAGS                 = $(ASFLAGS_$(DERIVATIVE))
endif
ASFLAGS_ADDITIONAL_PRE =  $(CFLAGS_ADDITIONAL_PRE)
ASFLAGS_WARNING         = $(CFLAGS_WARNING)
ASFLAGS_OPTIMIZATION    = $(CFLAGS_OPTIMIZATION)
ASFLAGS_MEMORYMODEL     = $(CFLAGS_MEMORYMODEL)
ASFLAGS_ADDITIONAL_POST = $(CFLAGS_ADDITIONAL_POST)

#------------------------------------------------------------------------------
# $(ASVERSION) defines the assembler switch for version report
# E.g.: ASVERSION = -V
#------------------------------------------------------------------------------
ASVERSION = 
AS_LC = 1

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# $(CC_ENV) is the environment necessary to run the compiler
# $(CC) defines the path to the C-Compiler
# $(CFLAGS_ADDITIONAL_PRE) defines all additional compiler switches that will
#                          be placed in front of all other switches
# $(CFLAGS_WARNING) defines the compiler switches concerning warning output
# $(CFLAGS_OPTIMIZATION) defines the compiler switches concerning optimization
# $(CFLAGS_MEMORYMODEL) defines the compiler switches concerning the memory model
# $(CFLAGS_ADDITIONAL_POST) defines all additional compiler switches that will be
#                           placed at the end of all switches
# It is highly recommended to add the compiler switch description in a comment
# E.g.: CC_ENV  = export CC_BIN="$(COMPILER_BIN)"
#       CC       = $(COMPILER_BIN)\cx6808.exe
#------------------------------------------------------------------------------
# Options:
#   -@<fn>  read options from file fn     -q      quiet
#   -b      generate user info file       -qq     super quiet
#   -c      no linking (negates -z)       -s      source interlist
#   -dNAME  pre-define NAME               -ss     C source-code interlist
#   -g      symbolic debugging            -uNAME  undefined NAME
#   -i<dir> #include search path          -z...   link, options follow
#   -n      compile only (no asm)       
# 
# 
#   --small-enum  enums may be char/short, instead of int
#   -abs    produce absolute listing file (only after -z)
#   -k      keep the assembly language (.asm) file
#   -rrx    disallow use of rx=[5,6,9] by compiler
# 
# Parser options:  -p<options...>
#   -pi     ignore inline keyword         -pn     disable intrinsic functions
#   -pk     K&R compatiblity              -pr     relaxed (non-strict ANSI)
#   -pm     program mode compilation      -ps     strict ANSI mode (errors)
#   -pl     output raw listing to .rl file                                       
#   -px     output xref listing to .crl file                                     
# 
# Parser preprocessing options:  -pp<options...>
#   -ppa    continue with compilation     -ppi    file inclusion only
#   -ppc    preprocess only w/comments    -ppl    preprocess only w/line #
#   -ppd    dependencies only             -ppo    preprocess only
# 
# Parser diagnostics options:  -pd<options...>
#   -pdel<n> set error limit to <n>       -pds<n>  suppress diagnostic <n>
#   -pden    emit diagnostics ids         -pdv     verbose diagnostics
#   -pdr     issue remarks                -pdw     suppress warnings
#   -pdf     output diagnostic to .err file                                      
#   -pdse<n> treat diagnostic <n> as error                                       
#   -pdsr<n> treat diagnostic <n> as remark                                      
#   -pdsw<n> treat diagnostic <n> as warning                                     
# 
# Runtime model options:  -m<options...>
#   -ma     aliased variables             -ml     Generate long calls
#   -mc     chars signed by default       -mn     normal opt with debug
#   -md     no dual state support         -mt     16 bit state code
#   -me     little endian code            -mx     enable exception handling
#   -mf     opt for speed               
# 
# Optimizations:  -o<options...>
#   -o0     level 0: register optim       -o      optimize (level 2)
#   -o1     level 1: +local optim         -oi<xx> auto inlining threshold
#   -o2     level 2: +global optim                 
#   -o3     level 3: +interproc optim   
#   -os     optimizer source comment interlisting                                
# 
# File and Directory specifiers:  -f...
#   -fa<file>    this file is an asm file       (default for .asm/.s*)
#   -fc<file>    this file is a C file          (default for .c/no ext)
#   -fo<file>    this file is an obj file       (default for .o*)
#   -fp<file>    this file is a C++ file        (default for .C, .cpp, .cc, .cxx)
# 
# 
# 
#   -fb<dir>  absolute listing directory     (default is object file dir.)
#   -ff<dir>  listing/xref file directory    (default is object file dir.)
#   -fr<dir>  object file directory          (default is .)
#   -fs<dir>  asm file directory             (default is .)
#   -ft<dir>  temporary file directory       (unique filenames are generated)
#------------------------------------------------------------------------------
#
#
# if e.g. --abi=tiabi is specified, make sure the parameter is also passed 
# to the assembler
# e.g. by specifying in ASFLAGS
#


CC_ENV   = $(LD_ENV)
CC        = $(COMPILER_BIN)\cl470.exe

CFLAGS    = 

CFLAGS_ADDITIONAL_PRE  =
CFLAGS_WARNING         =
CFLAGS_OPTIMIZE_DEFAULT   =
CFLAGS_OPTIMIZE_CORTEX_R4 = -mv=7R4
CFLAGS_OPTIMIZE_CORTEX_M3 = -mv=7M3
CFLAGS_OPTIMIZATION    = $(CFLAGS_OPTIMIZE_$(CPU_OPTIMIZATION))

CFLAGS_MEMORYMODEL     =
CFLAGS_ARM             =
CFLAGS_THUMB           = -mt

CFLAGS_ADDITIONAL_POST = $(CFLAGS_VECTOR_MAKESUPPORT) \
                         $(CFLAGS_VECTOR_OPTIONS)     \
                         $(CFLAGS_CUSTOMER_OPTIONS)

#Please add only absolutely mandatory options below (options which are needed by the makesupport)
CFLAGS_VECTOR_MAKESUPPORT =  -fr $(OBJ_PATH)                            \
                             -fb $(LST_PATH)                            \
                             -ff $(LST_PATH)                            \
                             -fs $(LST_PATH)                            \
                             -dC_DERIVATIVE_$(DERIVATIVE)               \
                             -dC_FAMILY_$(FAMILY_$(DERIVATIVE))         \
                             -dEVA_BOARD_$(EVA_BOARD)                   \
                             -dBRS_TIMEBASE_CLOCK=$(BRS_TIMEBASE_CLOCK)

#Please add any other option below (vector default options)
CFLAGS_VECTOR_OPTIONS = $(CFLAGS_OPTIMIZE_$(CPU_OPTIMIZATION)) \
                        $(CFLAGS_$(INSTRUCTION_SET))           \
                        -k                                     \
                        -g                                     

ifeq ($(CPU_OPTIMIZATION),CORTEX_R4)
ifeq ($(INSTRUCTION_SET),THUMB)
CFLAGS_VECTOR_OPTIONS += --abi=eabi
endif
endif

#Please enter all customer options below and remove CFLAGS_VECTOR_OPTIONS from CFLAGS_ADDITIONAL_POST
CFLAGS_CUSTOMER_OPTIONS =    -o2                                        \
                             --opt_for_speed=3                          \
                             -fr obj                                    \
                             -fb lst                                    \
                             -ff lst                                    \
                             -fs lst                                    \
                             -mv=7R4                                    \
                             -k                                         \
                             -g                                         \
                             --abi=eabi                                 \
                             --float_support=VFPv3D16                   \
                             --symdebug:dwarf_version=3                 \
                             --fp_mode=strict                           \
                             --verbose                                  \
                             --issue_remarks

CF_FILTER_FBL_WD =         -mt                                 \
													 --code_state=32 

CF_FILTER_FBL_APWD =       -mt                                 \
													 --code_state=32

#Option Filters for internal files
#--code_state=16: Generates 16-bit Thumb code
#-mt: Compile in Thumb mode
#-pdseXXX: handle warning XXX as error
#-pds179: suppress warning #179-D: variable was declared but never referenced
#-pds239: suppress remark #239-D: selector expression is constant
#-pds552: suppress warning #552-D: variable was set but never used
#-pds225: suppress remark #225-D: function declared implicitly
CF_FILTER_INTERNAL = -pds179 -pds239 -pds552 -pds225

ifeq ($(CPU_OPTIMIZATION),CORTEX_R4)
ifeq ($(INSTRUCTION_SET),THUMB)
CF_FILTER_INTERNAL += -mt --code_state=16
endif
endif

#Startup
CF_FILTER_STARTUP                      = $(CF_FILTER_INTERNAL) -o2
#CF_FILTER_STARTUPPSF761                = $(CF_FILTER_INTERNAL)
#CF_FILTER_STARTUPPSF762                = $(CF_FILTER_INTERNAL)
#CF_FILTER_startupMSF542                = $(CF_FILTER_INTERNAL)
#CF_FILTER_stackMSF542                  = $(CF_FILTER_INTERNAL)
CF_FILTER_SYS_STARTUP_TMS570_CR4F_CORE = $(CF_FILTER_INTERNAL)
CF_FILTER_SYS_STARTUP_TMS570_CR4F      = $(CF_FILTER_INTERNAL)
#CF_FILTER_INTVECT                      = $(CF_FILTER_INTERNAL)
#CF_FILTER_EXCEPTION                    = $(CF_FILTER_INTERNAL)

#Brs
#BrsHw.c has to be compiled in ARM mode, since the used compiler intrinsics _enableIRQ() and _disableIRQ() are not available in Thumb mode
#CF_FILTER_BRSHW                        = $(CF_FILTER_INTERNAL) -o2

#------------------------------------------------------------------------------
# $(CVERSION) defines the compiler switch for version report
# E.g.: CVERSION = -V
#------------------------------------------------------------------------------
CVERSION =
CC_LC = 1

#------------------------------------------------------------------------------
# $(CINC) defines the include switch of the used compiler.
# E.g.: COSMIC Compiler use: CINC = -i
#------------------------------------------------------------------------------
CINC      = -I

#ifeq (,$(RTS_LIB))
#------------------------------------------------------------------------------
# Generation of library filename
# Filename: rtsArchVersion_mode_endian[_n][_vn]_abi[_eh].lib
#
# ArchVersion The version of the ARM architecture that the library was built for. This can be one of the following: 
#   v4, v5, v6, v7A8, v7R4, or v7M3
# mode Indicates compilation mode : 
#        T Thumb mode
#        A ARM mode
# endian Indicates endianness:
#        le Little-endian library
#        be Big-endian library
# n Indicates the library contains NEON support.
# vn Indicates the library has VFP support, where n designates the version. Current values are:
#   2 VFPv2
#   3 VFPv3
#   3D16 VFPv3D16
# abi Indicates the application binary interface (ABI) used: eabi tiarm7 tiarm9
# eh Indicates the library has exception handling support
#------------------------------------------------------------------------------
#CFLAGS_TMP = $(CFLAGS_ADDITIONAL_PRE)  \
#             $(CFLAGS_OPTIMIZATION)    \
#             $(CFLAGS_WARNING)         \
#             $(CFLAGS_MEMORYMODEL)     \
#             $(CFLAGS_ADDITIONAL_POST) \
#	     $(CF_FILTER_INTERNAL)

#If the following code is not tested for every usecase.
#If the code does not work properly for your usecase, inform visfsn or enter RTS_LIB manually in Makefile.Config

# CPU
# Currently, only Cortex R4 and Cortex M3 are supported
#ifneq (,$(findstring -mv=7R4,$(CFLAGS_TMP)))
#  RTS_CPU=v7R4
#endif
#ifneq (,$(findstring -mv7R4,$(CFLAGS_TMP)))
#  RTS_CPU=v7R4
#endif 
#ifneq (,$(findstring -mv=7M3,$(CFLAGS_TMP)))
#  RTS_CPU=v7M3
#endif
#Default
#ifeq (,$(RTS_CPU))
#  RTS_CPU=v4
#endif



# MODE
#ifneq (,$(findstring -mt,$(CFLAGS_TMP)))
#  RTS_MODE=T
#endif
#ifneq (,$(findstring --code_state=16,$(CFLAGS_TMP)))
#  RTS_MODE=T
#endif
#Default
#ifeq (,$(RTS_MODE))
#  RTS_MODE=A
#endif


#ifeq ($(INSTRUCTION_SET),ARM)
#  RTS_MODE=A
#endif

#ENDIANESS
#ifneq (,$(findstring --endian=little,$(CFLAGS_TMP)))
#  RTS_ENDIAN=le
#else
#  RTS_ENDIAN=be
#endif

#FLOATING POINT SUPPORT
#ifneq (,$(findstring --float_support=vfpv3d16,$(CFLAGS_TMP)))
#  RTS_FPS=_3D16
#endif

#APPLICATION BINARY INTERFACE (ABI) 
#ifneq (,$(findstring --abi=eabi,$(CFLAGS_TMP)))
#  RTS_ABI=eabi
#endif
#ifneq (,$(findstring --abi=tiabi,$(CFLAGS_TMP)))
#  RTS_ABI=tiarm7
#endif
#Default
#ifeq (,$(RTS_ABI))
#  RTS_ABI=tiarm9
#endif


#RTS_LIB=rts$(RTS_CPU)_$(RTS_MODE)_$(RTS_ENDIAN)$(RTS_FPS)_$(RTS_ABI).lib

#endif

#------------------------- MUST be filled out ---------------------------------
# $(LD_ENV) is the environment necessary to run the linker
# $(LD) defines the path linker
# $(LDFLAGS_ADDITIONAL_PRE) defines all additional linker switches that will
#                           be placed in front of all other switches
# $(LDFLAGS_WARNING) defines the linker switches concerning warning output
# $(LDFLAGS_OPTIMIZATION) defines the linker switches concerning optimization
# $(LDFLAGS_MEMORYMODEL) defines the linker switches concerning the memory model
# $(LDFLAGS_ADDITIONAL_POST) defines all additional linker switches that will be
#                            placed at the end of all switches
# It is highly recommended to add the compiler switch description in a comment
# E.g.: LD_ENV  = export LD_BIN="$(COMPILER_BIN)"
#       LD      = $(COMPILER_BIN)\clnk.exe
#------------------------------------------------------------------------------
# -a        absolute output             -l<lib>   library name
# -ar       relocatable output          -m<file>  map file name
# -b        disable debug syms merge    -o<file>  output file name
# -c        ROM initialization          -q        quiet
# -cr       RAM initialization          -r        relinkable output
# -e<sym>   entry point                 -s        strip symbol table
# -f<val>   fill value                  -u<sym>   undefine sym
# -g<sym>   sym is global even if -h    -w        warn about output sections
# -h        global symbols static       -x        exhaustively search libs
# -i<dir>   library search path       
#
# -heap 0x<size>  set heap size
# -stack 0x<size> set stack size
#
# (Linking is enabled only if -z is used and -c is not used)
# With linker v4.4.0 -h option has the consequence that no global symbols with addresses are listed in the map file
# If you use with the compiler v4.4.0 -mv7R4 option, then you must use the linker option --be32 to.
#------------------------------------------------------------------------------
LD_ENV    =
LD        = $(COMPILER_BIN)\lnk470.exe

LDFLAGS_ADDITIONAL_PRE  =
LDFLAGS_WARNING         = -w 
LDFLAGS_OPTIMIZATION    =
LDFLAGS_MEMORYMODEL     =

LDFLAGS_ADDITIONAL_POST = $(LDFLAGS_VECTOR_MAKESUPPORT) \
                          $(LDFLAGS_VECTOR_OPTIONS)     \
                          $(LDFLAGS_CUSTOMER_OPTIONS)

#Please add only absolutely mandatory options below (options which are needed by the makesupport)
LDFLAGS_VECTOR_MAKESUPPORT = -i $(COMPILER_LIB)                     \
                             -l $(ROOT)\BSW\SecMod\libSYSSERVICE_SECMODHIS_LIB.lib \
                             -m $(PROJECT_NAME).map                 \
                             -o $(PROJECT_NAME).$(BINARY_SUFFIX)    \
                             $(LDFLAGS_ADDITIONAL_POST_ADDITIONAL)


#Please add any other option below (vector default options)
LDFLAGS_VECTOR_OPTIONS =     -a                                     \
                             -c                                     \
                             -x

#Please enter all customer options below and remove LDFLAGS_VECTOR_OPTIONS from LDFLAGS_ADDITIONAL_POST
LDFLAGS_CUSTOMER_OPTIONS = --verbose_diagnostics                    \
                           --zero_init=off

ifeq ($(DERIVATIVE),MSF542)
LDFLAGS_ADDITIONAL_POST_ADDITIONAL += -stack 0x1000
else
LDFLAGS_ADDITIONAL_POST_ADDITIONAL += -e_c_int00
endif

ifeq ($(CPU_OPTIMIZATION),DEFAULT)
LDFLAGS_ADDITIONAL_POST_ADDITIONAL +=
else
LDFLAGS_ADDITIONAL_POST_ADDITIONAL += --be32
endif

#------------------------------------------------------------------------------
# $(LDVERSION) defines the linker switch for version report
# E.g.: LDVERSION = -v
#------------------------------------------------------------------------------
LDVERSION = --help
LD_LC = 1
#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# $(LB_ENV) is the environment necessary to run the librarian
# $(LB) defines the path to the C-librarian
# $(LBFLAGS) defines all librarian switches
# It is highly recommended to add the librarian switch description in a comment
# E.g.: LB_ENV  = export LB_BIN="$(COMPILER_BIN)";
#       LB      = $(COMPILER_BIN)\cx6808.exe
#------------------------------------------------------------------------------
# TMS320C2000 Archiver              Version 3.03
# Tools Copyright (c) 1996-2002     Texas Instruments Incorporated
# 
#  Syntax : ar2000 -[arxdt][quvse] archive files ...
#           ar2000 @command_file
#
# @ uses the contents of the specified file instead of command line entries.
#   You can use this command to avoid limitations on command line length imposed
#   by the host operating system. Use a ; at the beginning of a line in the
#   command file to include comments.
#
# a adds the specified files to the
#   library. This command does not replace an existing member that has the same
#   name as an added file; it simply appends new members to the end of the
#   archive.
#
# d deletes the specified members from the library. 
#
# r replaces the specified
#   members in the library. If you do not specify filenames, the archiver
#   replaces the library members with files of the same name in the current
#   directory. If the specified file is not found in the library, the archiver
#   adds it instead of replacing it. 
#
# t prints a table of contents of the
#   library. If you specify filenames, only those files are listed. If you do not
#   specify any filenames, the archiver lists all the members in the specified
#   library.
#
# x extracts the specified files. If you do not specify member names, the
#   archiver extracts all library members. When the archiver extracts a member,
#   it simply copies the member into the current directory; it does not remove it
#   from the library.
# -q (quiet) suppresses the banner and status messages.
# -s prints a list of the global symbols that are defined in the library.
#    (This option is valid only with the a, r, and d commands.)
# -u replaces library members only if the replacement has a more recent
#    modification date. You must use the r command with the -u option to specify
#    which members to replace.
# -v (verbose) provides a file-by-file description of the creation of a new
#    library from an old library and its members.
#------------------------------------------------------------------------------
LB_ENV   = $(LD_ENV)
LB       = $(COMPILER_BIN)/ar470.exe
LBFLAGS  = -rq $@

#------------------------------------------------------------------------------
# $(LBVERSION) defines the archiver switch for version report
# E.g.: LBVERSION = -v
#------------------------------------------------------------------------------
LBVERSION =
LB_LC     = 1

#------------------------------------------------------------------------------
# $(OBJ_OUTPUT) defines the way, how to generate error files
# It should be one of:
#     MOVE  - Object files are generated by compiler/linker and will be moved
#             to $(OBJ_PATH) after generation by external tool
#     FLAGS - Object file path definition is supported by compiler.
#             Therefore the $(CFLAGS) have to be adjusted.
#------------------------------------------------------------------------------
OBJ_OUTPUT = FLAGS

#------------------------------------------------------------------------------
# $(ERR_OUTPUT) defines the way, how to generate error files
# It should be one of:
#     MOVE  - Error files are generated by compiler/linker and will be moved
#             to $(ERR_PATH) after generation by external tool
#     PIPE  - Error will be printed to sterr/stdout and redirected to
#             $(ERR_PATH)
#     FLAGS - Error file generation with path definition is supported by
#             compiler. Therefore e.g. the $(CFLAGS) have to be adjusted.
#------------------------------------------------------------------------------
ERR_OUTPUT = PIPE

#------------------------------------------------------------------------------
# $(LIB_OUTPUT) defines the way, how to generate lib files
# It should be one of:
#     MOVE  - Lib files are generated by compiler/linker and will be moved
#             to $(LIB_PATH) after generation.
#     FLAGS - Object file path definition is supported by compiler.
#             Therefore the $(LB_FLAGS) have to be adjusted.
#------------------------------------------------------------------------------
LIB_OUTPUT = FLAGS

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Suffix of generated object files. Generated objects are then *.$(OBJ_SUFFIX)
#------------------------------------------------------------------------------
OBJ_SUFFIX = obj

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Suffix of assembler files.
#------------------------------------------------------------------------------
ASM_SUFFIX = asm

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Suffix linker command file.
#------------------------------------------------------------------------------
LNK_SUFFIX = lcf

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Suffix of generated library files. Generated objects are then *.$(LIB_SUFFIX)
#------------------------------------------------------------------------------
LIB_SUFFIX = lib

#------------------------------------------------------------------------------
#------------------------- MUST be filled out ---------------------------------
# Suffix binary output file.
#------------------------------------------------------------------------------
BINARY_SUFFIX = out

#------------------------------------------------------------------------------
# Suffixes of generated error files which are placed in $(ERR_PATH)
# (necessary for Visual Studio / Codewright project file generation)
# Default (nothing entered) is err
# E.g. ERR_SUFFIX_LIST = err
#------------------------------------------------------------------------------
ERR_SUFFIX_LIST =

#------------------------------------------------------------------------------
# Suffixes of generated asm/list files which are placed in $(LST_PATH)
# (necessary for Visual Studio / Codewright project file generation)
# Default (nothing entered) is lst
# E.g. LST_SUFFIX_LIST = lst asm
#------------------------------------------------------------------------------
LST_SUFFIX_LIST =

#------------------------------------------------------------------------------
# SimpleOS OIL-file
# Location of the oil file which has to be configured similar to FakeOs
# Location is relative to the Makefile
# E.g.: SIMPLEOS_OIL = simpleos.oil
#------------------------------------------------------------------------------
SIMPLEOS_OIL  = simpleos2_3.oil

#------------------------------------------------------------------------------
# Path to the platform specific OSEKos (root of appl, bin, doc, include, src)
# E.g.: OSEK_PATH = C:\OSEK\$(PLATFORM)
#------------------------------------------------------------------------------
OSEKOS_PATH   = D:\OSEK23\TMS470

#------------------------------------------------------------------------------
# Path to the OSEKos gentool
# E.g.: OIL_GEN = $(OSEKOS_PATH)\..\oiltool\gen\gen$(PLATFORM).exe
#------------------------------------------------------------------------------
OIL_GEN       = $(OSEKOS_PATH)\..\oiltool\gen\gen$(PLATFORM).exe

#------------------------------------------------------------------------------
# Define patterns which have to be replaced in OIL-File
# E.g.:
# OIL_PATTERN    = DERIVATIVE FREQUENCY MMODEL
# OIL_DERIVATIVE = $(CPU_TYPE)      # DERIVATIVE will be replaced by $(CPU_TYPE)
# OIL_FREQUENCY  = $(BRS_TIMEBASE_CLOCK) # FREQUENCY will be replaced by $(BRS_TIMEBASE_CLOCK)
# OIL_MMODEL     = $(MEM_MODEL)     # MMODEL will be replaced by $(MEM_MODEL)
#------------------------------------------------------------------------------
OIL_PATTERN    = _BRS_INCLUDE   \
                 _BRS_FREQUENCY \
                 _BRS_SYS       \
                 _BRS_CANRXTX

OIL__BRS_INCLUDE   = TMS470R1
OIL__BRS_FREQUENCY = $(BRS_TIMEBASE_CLOCK)
OIL__BRS_SYS       = CanIrqHandlerSys
OIL__BRS_CANRXTX   = CanIrqHandlerRxTx

#------------------------------------------------------------------------------
# Location of v_cfg.h relative to the Makefiles
# This is needed to retrive $(CPU_TYPE) for derivative control
# E.g.: $(GENDATA_DIR)
#------------------------------------------------------------------------------
V_CFG_LOC = $(GENDATA_DIR)

#------------------------------------------------------------------------------
# List of (regexp) pattern used by the distclean target
# Pattern. dleted by default are:
#    *~
#    *.*~
#    *.~*
#    *.bak
#    *.log
#    $(GENDATA_DIR)\*.txt
#    $(ERR_LIST_WOP)
#    $(LST_N_MAP_LIST_WOP)
#    $(SIMPLEOS_OIL)_
#    $(PROJECT_NAME).map
#    $(PROJECT_NAME).dep
#    $(PROJECT_NAME).dsw
#    $(PROJECT_NAME).dsp
#    $(PROJECT_NAME).ncb
#    $(PROJECT_NAME).opt
#    $(PROJECT_NAME).plg
#    $(PROJECT_NAME).doc.cfg
#    $(GENDATA_DIR)\BrsVInfo.h
#------------------------------------------------------------------------------
DIST_CLEAN_PATTERN_LIST =

#------------------------------------------------------------------------------
# Additional includes essentially for compilation
#------------------------------------------------------------------------------
ADDITIONAL_INCLUDES +=

#------------------------------------------------------------------------------
# Additional objects essentially for linking
# E.g.: ADDITIONAL_OBJECTS = $(OBJ_PATH)\myobject.$(OBJ_SUFFIX)
#------------------------------------------------------------------------------
ADDITIONAL_OBJECTS =

ifeq ($(DRVCAN_USED),1)
ifeq ($(USECAN32),1)
ADD_CAN := $(subst /,\,$(dir $(call compSources,DRVCAN)))can_32.c
DRVCAN_SOURCES += $(ADD_CAN)
endif
endif

#------------------------------------------------------------------------------
# List of assembler source files
# E.g.: ASM_SOURCES = $(GENDATA_OS_DIR)\intvect.$(ASM_SUFFIX)
#------------------------------------------------------------------------------
#          Watch out when using the following blocks! 
#          Redundancy with Makefile.project.part.defines leads to errors. Choose your files there.
#ifeq ($(ZBRS_USED),1)
#ASM_SOURCES += ..\..\shared\zBrs_Tms470\intvect.$(ASM_SUFFIX)
#else
#ASM_SOURCES = $(GENDATA_OS_DIR)\intvect.$(ASM_SUFFIX)
#endif

#ifeq ($(OSEKOS_USED),1)
#ASM_SOURCES += $(OSEKOS_PATH)\SRC\osekasm.$(ASM_SUFFIX) \
#               $(OSEKOS_PATH)\SRC\osekint.$(ASM_SUFFIX)
#endif

#ifeq ($(AUTOSAROS_USED),1)
#else
#ASM_SOURCES += source\resetvect.$(ASM_SUFFIX)
#endif

#------------------------------------------------------------------------------
# Additional dependencies (for files where automatic dependency generation fails)
# E.g.:
# $(OBJ_PATH)/can_drv.$(OBJ_SUFFIX): $(OBJ_PATH)/%.$(OBJ_SUFFIX): \
#         $(subst \,/,                                            \
#                 $(ROOT)\$(dir $(call compSources,DRVCAN))%.c             \
#                 $(ROOT)\$(dir $(call compSources,DRVCAN))can_inc.h       \
#                 $(GENDATA_DIR)/v_cfg.h                          \
#                 $(GENDATA_DIR)/can_cfg.h                        \
#         )
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Add Startup code to application source list
# E.g.: APP_SOURCE_LST += source\startup.c
#------------------------------------------------------------------------------
#          Watch out when using the following blocks! 
#          Redundancy with Makefile.project.part.defines leads to errors. Choose your startup files there.
#ifeq ($(DERIVATIVE),PSFC61)
#APP_SOURCE_LST += source\startupPSFC61.c
#ASM_SOURCES += source\stackPSF7xx_all.asm
#endif

#ifeq ($(FAMILY_$(DERIVATIVE)),CORTEX_R4F)
#APP_SOURCE_LST += ..\..\shared\zBrs_Tms470\sys_startup_TMS570_CR4F.c
#ASM_SOURCES    += ..\..\shared\zBrs_Tms470\sys_startup_TMS570_CR4F_Core.$(ASM_SUFFIX)
#endif
#ifeq ($(DERIVATIVE),MSF542)
#APP_SOURCE_LST += source\startupMSF542.c
#APP_SOURCE_LST += source\exceptions.c
#ASM_SOURCES += source\stackMSF542.asm
#endif

#ifeq ($(DERIVATIVE),PSF762)
#APP_SOURCE_LST += source\startupPSF762.c
#ASM_SOURCES += source\stackPSF7xx_all.asm
#endif

#ifeq ($(DERIVATIVE),PSF761)
#APP_SOURCE_LST += source\startup.c
#endif

#ifneq ($(findstring $(DERIVATIVE), R1B31 R1VF45A R1VF336 R1F316 R1VF338 R1VF348 AVF68X PVF241PN),)
#APP_SOURCE_LST += source\startup.c
#endif


#ifeq ($(DERIVATIVE),MSF542)
#APP_SOURCE_LST +=
#ASM_SOURCES +=
#endif


#------------------------------------------------------------------------------
# Dependency defines
# If compiler specific defines are questioned in platform specific files they
# can be manually defined here for the dependencies generation
# E.g.:
#------------------------------------------------------------------------------
DEP_DEFINES = $(PCLINT_DEF)

#------------------------------------------------------------------------------
# Turn strict c setting for dependency generation off
# E.g.: STRICT_C_OFF =   (ON)
#       STRICT_C_OFF = 1 (OFF)
#------------------------------------------------------------------------------
STRICT_C_OFF =

###############################################################################
# resource information generation
###############################################################################

#------------------------------------------------------------------------------
# Select the source, which contains the memory information
# E.g.: RESOURCE_FILE = LIST
#       RESOURCE_FILE = MAP
#       RESOURCE_FILE = NONE
#------------------------------------------------------------------------------
RESOURCE_FILE = MAP

#------------------------------------------------------------------------------
# The extension of the map or list file.
# E.g.: RESOURCE_FILE_EXT = map
#       RESOURCE_FILE_EXT = lst
#------------------------------------------------------------------------------
RESOURCE_FILE_EXT = map

#------------------------------------------------------------------------------
# This is the main resource script written in the language AWK.
# For more information about AWK see the following resources:
#   http://torvalds.cs.mtsu.edu/~neal/awkcard.pdf (recommendable)
#   http://www.uni-magdeburg.de/urzs/awk/
#   http://de.wikipedia.org/wiki/AWK
# Note: The character '$' must be escaped with a second '$'. So write '$$'.
#       Also the character '#' must be escaped using a backslash -> \#.
# For a more detailed description about what is going on in this script look
# at the file:  fetch-memory-info-template-documented.awk
# In order to see what the ressource scanner is doing it is highly recommended
# to set the environment variable GAWK_DEBUG=1. This option will generate a
# further file ($(PROJECT_NAME)_resource_debug.txt) in your log directory.
# Here is a short description about the instructions which has to be configured:
# RS means Record Separator and is set to a char which is used to split the
#   files into records. For each record the processing instructions are done.
#   The record string is assigned to the variable $0 (remember to access via $$0).
# FS means Field Separator and splits each record into fields. These fields
#   are referenced from $1, $2... $NF, where NF is the number of fields.
#   If FIELDWIDTHS is set, the value of FS is ignored.
# FIELDWIDTHS is a space separated list, each field is expected to have a fixed
#   width. The value of FS is ignored.
# IGNORECASE is a bool variable. When set to 1 comparsions are made case-
#   insensitive in comparsions with '==' and regular expressions.
# memoryTable maps the second argument of addEntry() to the according section.
# startProcessing() and stopProcessing() implements a little restriction
#   to which section of the file the instructions are executed.
# addEntry(module, memory, size, isHex) is the main API function you have to
#   use. It adds an entry to the internal array. You specify with the param.
#   how many bytes (param:size) to which module (param:module) and to which
#   type of memory (param:memory) you want to add. If you set the param
#   isHex to 1 the param size is treated as being hexadecimal.
# Other API functions are: trim(str), getStringWithinParentheses(str),
#   getStringWithinBrackets(str)
#------------------------------------------------------------------------------
RESOURCE_FETCH_SCRIPT = BEGIN {                                             \
  RS  = "\n";                                                               \
  FS  = " ";                                                                \
  IGNORECASE = 1;                                                           \
  memoryTable["ram"]    = "bss|data|.FLASHDRV";                             \
  memoryTable["iram"]   = "cinit";                                          \
  memoryTable["const"]  = "const|.FBLHEADER";                               \
  memoryTable["code"]   = "text|tramp";                                     \
  memoryTable["ignore"] = "debug|intvecs|init|startup|0]|--HOLE--|osarmvect|osvtable|__TI|rle|reset|.APPLVECT"; \
  memoryTableOrder      = "const,code,ram,iram,ignore";                     \
}                                                                           \
                                                                            \
/SECTION ALLOCATION MAP/   { startProcessing() }                            \
/LINKER GENERATED COPY TABLES/   { stopProcessing() }                       \
                           { process() }                                    \
                                                                            \
$$1 ~ /^[0-9a-f]+$$/ {                                                      \
  addEntry($$(NF-1), $$NF, $$2, 1);                                         \
}

###############################################################################
######### DO NOT remove these lines from the end of the Makefile!!! ###########
###############################################################################

ifeq ($(findstring $(DERIVATIVE), R1B31 R1VF45A R1VF336 R1F316 R1VF338 R1VF348 AVF68X PVF241PN PSF761 PSFC61 PSFC66 PSF762 MSF542 LS20216 LS30336),)
  $(error Derivative $(DERIVATIVE) is not supported! Supported derivatives: PVF241PN PVF249 PVF348 PSF764 MF04207 MF06607 MSF542 PSF761 PSFC61 PSFC66 PSF762 LS0432 LS0805 LS10206 LS10216 LS1227 LS20216 LS2124 LS2125 LS2134 LS2135 LS3134 LS3135 LS3137 )
endif


#------------------------------------------------------------------------------
# Enable CPU Optimization for CORTEX R4
#------------------------------------------------------------------------------
ifeq ($(findstring $(CPU_OPTIMIZATION), DEFAULT CORTEX_R4 CORTEX_M3),)
  $(error Option $(CPU_OPTIMIZATION) for CPU_OPTIMIZATION is not supported! Supported options: DEFAULT, CORTEX_R4, CORTEX_M3)
endif

ifeq ($(findstring $(INSTRUCTION_SET),ARM THUMB),)
    $(error INSTRUCTION_SET not set correctly! Must be one of ARM THUMB)
endif

#------------------------------------------------------------------------------
# Check if all necessary variables are set
#------------------------------------------------------------------------------
ifeq ($(MAKESUPPORT_DIR),)
    $(error MAKESUPPORT_DIR not set in m.bat or m.bat not called)
endif

ifeq ($(PLATFORM),)
    $(error Variable PLATFORM is not defined)
endif

ifeq ($(COMPILER_MANUFACTURER),)
    $(error Variable COMPILER_MANUFACTURER is not defined)
endif

ifeq ($(EMULATOR),)
    $(error Variable EMULATOR is not defined)
endif

ifeq ($(VERSION),)
    $(error Variable VERSION is not defined)
endif

ifeq ($(MAKESUPPORT_DIR),)
    $(error Variable MAKESUPPORT_DIR is not defined)
endif

#------------------------------------------------------------------------------
# List of variables in Makefile.config which are required and must be set by
# user. Check is performed by global makefile
#------------------------------------------------------------------------------
REQUIRED_MK_CONF_VARS = BRS_TIMEBASE_CLOCK   \
                        DERIVATIVE           \
                        INSTRUCTION_SET      \
                        CPU_OPTIMIZATION     

#------------------------------------------------------------------------------
# Platform/compiler/derivative/emulator dependant makefile is included here
# It defines a rule to generate the linker and a rule to generate the
# symbol preprocessor command file.
# A template for this Makefile can be found under
#     R:\Can_Base\CANtate\BRS\PlatformSpecific\...\OrganiAndMake\...
#------------------------------------------------------------------------------
include Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make

#------------------------------------------------------------------------------
# Include the main makefile where all the targets are defined
#------------------------------------------------------------------------------
include $(MAKESUPPORT_DIR)\Global.Makefile.target.make.$(VERSION)

# End of Makefile
