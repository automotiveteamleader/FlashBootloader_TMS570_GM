###############################################################################
# File Name  : Makefile.ALL.ALL.ALL.make                                      #
# Description: Linker/Symbol Preprocessor command file generation             #
# Application: GNU Make version 3.79.1, by Richard Stallman and Roland McGrath#
#              Built for Windows32                                            #
# FAQ        : The naming convention for this file is:                        #
#               Makefile.<PLATFORM>.<COMPILER>.<EMULATOR>.make                #
#              It is included by Makefile. There you have to define the       #
#              variables PLATFORM, COMPILER_MANUFACTURER, EMULATOR            #
#              This is only a template!                                       #
#                                                                             #
#              Examples can be found under:                                   #
#               [...]\BRS\PlatformSpecific\ECU\OrganiAndMake\can\OEM\COMPILER #
#              or on one of its parents (when it is more generic).            #
#                                                                             #
#              PLEASE deposit your completed make environment under the dir   #
#              given above, if it doesn't exist there)                        #
#                                                                             #
#-----------------------------------------------------------------------------#
#               C O P Y R I G H T                                             #
#-----------------------------------------------------------------------------#
# Copyright (c) 2002 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               A U T H O R   I D E N T I T Y                                 #
#-----------------------------------------------------------------------------#
# Initials     Name                      Company                              #
# --------     ---------------------     -------------------------------------#
# Uf           Ulrich Frank              Vector Informatik GmbH               #
# kkn          Kevin Kögler V.           Vector Informatik GmbH               #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version  Sign Description                                      #
# ----------   -------  ---- -------------------------------------------------#
# 2002/08/18   3.0      Uf   Initial version                                  #
# 2006/11/16   4.0      Ard   Add PSF761 Derivative                           #
# 2007/07/24   4.1      Ard   Support TI 4.4.0 Compiler                       #
#                             Support compiler optimization for CORTEXR4      #
# 2007/11/05   4.2      kbr   Add PSFC61 and  PSF762 Derivative               #
# 2007/12/05   4.3      Ard   Move Stack for 761, 762, C61 to 8000000         #
#                                easier handling of stack initialisation      #
# 2009/01/26   4.4      kbr   Add PSFC66 Derivative and  AUTOSAROS            #
# 2012/08/28   4.5      Fsn   Changed RAM/ROM mapping to DERIVATIVE           #
#                             instead of PLANET_DERIVATIVE                    #
# 2013/02/11   4.6      Fsn   Added FR Stack to Cortex-R4 derivatives         #
#                             Added support for LS30336 derivative            #
# 2013/05/17   4.7      Fsn   Moved RTS Lib include to Makefile               #
# 2013/09/24   4.8      kkn   Multiple TMS570/470 derivatives are now         #
#                             supported.                                      #
#                             BRS_TIMEBASE_Clock renamed to BRS_TIMEBASE      #
# 2013/11/29   4.9      Fsn   Added OS_USECASE                                #
# 2014/01/07   5.0      kkn   Memory mapping is now defined in                #
#                             Makefile.derivative.memorymap                   #
###############################################################################
#------------------------------------------------------------------------------
# include the memory map (RAM/ROM area)
#------------------------------------------------------------------------------
include Makefile.derivative.memorymap

ifeq ($(OS_USECASE),OSEKOS) 
         RESETVECT =   $(ECHO) "    .reset : {} > 0x0           /* Reset VECTOR (Startup)          */" >> $@; 
           INTVECT =   $(ECHO) "    .osarmvect : {} > 0x4       /* INTERRUPT VECTORS               */" >> $@; \
                       $(ECHO) "    .osvtable : {} > VECTORS                 "                         >> $@; 
   SECTION_OPTIONS =   $(ECHO) "--retain=\"*(.osarmvect)\""                                            >> $@; \
                       $(ECHO) "--retain=\"*(.reset)\""                                                >> $@; 
endif
ifeq ($(OS_USECASE),ASR) 
         RESETVECT =   $(ECHO) "    .reset : {} > 0x0           /* Reset VECTOR (Startup)          */" >> $@; 
           INTVECT =   $(ECHO) "    .osarmvect : {} > 0x4       /* INTERRUPT VECTORS               */" >> $@; \
                       $(ECHO) "    .osvtable : {} > VECTORS                 "                         >> $@;
else
    RESET = $(ECHO) ""                                                                        >> $@; 
    VECT  = $(ECHO) "    .intvecs : {} > VECTORS     /* INTERRUPT VECTORS                 */" >> $@;
endif


# Old solution, does not work with TI 4.4.0 as _StackUSER_ will be the only symbol located correctly
define STACK_1
         $(ECHO) "                    _StackUSER_  = .+ (0x1000 - (256+128+128+128+128));"                    >> $@; \
         $(ECHO) "                    _StackFIQ_   = _StackUSER_  + 128;"                                     >> $@; \
         $(ECHO) "                    _StackIRQ_   = _StackFIQ_   + 256;"                                     >> $@; \
         $(ECHO) "                    _StackABORT_ = _StackIRQ_   + 128;"                                     >> $@; \
         $(ECHO) "                    _StackUND_   = _StackABORT_ + 128;"                                     >> $@; \
         $(ECHO) "                    _StackSUPER_ = _StackUND_   + 128;"                                     >> $@;
endef

define STACK_2
         $(ECHO) "                    _StackUSER_  = .+ (0x1000 - (256+128+128+128+128));"                    >> $@; \
         $(ECHO) "                    _StackFIQ_   = .+ (0x1000 - (256+128+128+128));"                        >> $@; \
         $(ECHO) "                    _StackIRQ_   = .+ (0x1000 - (128+128+128));"                            >> $@; \
         $(ECHO) "                    _StackABORT_ = .+ (0x1000 - (128+128));"                                >> $@; \
         $(ECHO) "                    _StackUND_   = .+ (0x1000 - (128));"                                    >> $@; \
         $(ECHO) "                    _StackSUPER_ = .+ (0x1000);"                                            >> $@;
endef




ifeq ($(DERIVATIVE),MSF542)
   define VEC_SEC
   $(ECHO) "   VECTORS (X)    :  origin=0x00000000   length=0x00000280" >> $@;
   endef
else
   define VEC_SEC
   $(ECHO) "   VECTORS (X)    :  origin=0x00000000   length=0x00000200" >> $@;
   endef
endif

#-----------------------------------------------------------------------------
# rule to generate linker command file
#-----------------------------------------------------------------------------
$(PROJECT_NAME).$(LNK_SUFFIX): Makefile \
                               Makefile.Config \
                               Makefile.Project.Part.Defines \
                               Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make \
			       Makefile.derivative.memorymap
	@$(ECHO) "/****************************************************************************************/"  > $@; \
   $(ECHO) "/* Options                                                                              */" >> $@; \
   $(ECHO) "/****************************************************************************************/" >> $@; \
   $(ECHO) "--retain=\"*(.FBLHEADER)\"                                                                " >> $@; \
   $(ECHO) "--retain=\"*(.FBLMAGIC)\"                                                                 " >> $@; \
   $(ECHO) "--retain=\"*(.LOGZONE)\"                                                                  " >> $@; \
   $(ECHO) "--retain=\"*(.FLASHDRV)\"                                                                 " >> $@; \
   $(ECHO) "-l rtsv7R4_T_be_v3D16_eabi.lib                                                            " >> $@; \
   $(ECHO) "/****************************************************************************************/" >> $@; \
   $(ECHO) "/* SPECIFY THE SYSTEM MEMORY MAP for $(DERIVATIVE) */                                     " >> $@; \
   $(ECHO) "/****************************************************************************************/" >> $@; \
   $(ECHO) "MEMORY"                                                                                     >> $@; \
   $(ECHO) "{"                                                                                          >> $@; \
   $(VEC_SEC)                                                                                                  \
   $(REGION_$(DERIVATIVE))                                                                                     \
   $(ECHO) ""                                                                                           >> $@; \
   $(ECHO) "   /* sar module addresses */"                                                              >> $@; \
   $(ECHO) "   MMC     (RW)   :  origin=0xFFFFFD00   length=0x40"                                       >> $@; \
   $(ECHO) "   DEC     (RW)   :  origin=0xFFFFFE00   length=0x60"                                       >> $@; \
   $(ECHO) "   SYS     (RW)   :  origin=0xFFFFFFD0   length=0x30"                                       >> $@; \
   $(ECHO) "}"                                                                                          >> $@; \
   $(ECHO) "/****************************************************************************************/" >> $@; \
   $(ECHO) ""                                                                                           >> $@; \
   $(ECHO) ""                                                                                           >> $@; \
   $(ECHO) "/****************************************************************************************/" >> $@; \
   $(ECHO) "/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY                                          */" >> $@; \
   $(ECHO) "/****************************************************************************************/" >> $@; \
   $(ECHO) "SECTIONS"                                                                                   >> $@; \
   $(ECHO) "{"                                                                                          >> $@; \
   $(ECHO) "    .stack   : {"                                                                           >> $@; \
   $(STACK_2)                                                                                                  \
   $(ECHO) "               } > STACKS       /* SOFTWARE SYSTEM STACK             */"                    >> $@; \
   $(ECHO) ""                                                                                           >> $@; \
   $(ECHO) "    .bss     : {} > RAM         /* GLOBAL & STATIC VARS              */"                    >> $@; \
   $(ECHO) "    .sysmem  : {} > RAM         /* DYNAMIC MEMORY ALLOCATION AREA    */"                    >> $@; \
   $(ECHO) "    .data    : {} > RAM         /* INITIALIZED DATA                  */"                    >> $@; \
   $(ECHO) "    .FLASHDRV: {} > FLDRV       /* Flash Driver Buffer               */"                    >> $@; \
   $(ECHO) "    .LOGZONE : {} > LOGZONE     /* Meta Data For PSA                 */"                    >> $@; \
   $(ECHO) "    .FBLMAGIC: {} > MAGICFLG    /* MAGIC FLAGS LOCATION              */"                    >> $@; \
   $(ECHO) "    .EEOIO   : {} > EEPIO       /* Emulated EEPROM Location          */"                    >> $@; \
   $(ECHO) ""                                                                                           >> $@; \
   $(RESET)                                                                                                    \
   $(VECT)                                                                                                     \
   $(ECHO) ""                                                                                           >> $@; \
   $(ECHO) "    .FBLHEADER: {} > FBLHDR     /* FBL HEADER -> GENy attribut Header Address */"           >> $@; \
   $(ECHO) "    .startup : {} > ROM         /* START ADDRESS                     */"                    >> $@; \
   $(ECHO) "    .text    : {} > ROM         /* CODE                              */"                    >> $@; \
   $(ECHO) "    .const   : {} > ROM         /* CONSTANT DATA                     */"                    >> $@; \
   $(ECHO) "    .cinit   : {} > ROM         /* INITIALIZATION TABLES             */"                    >> $@; \
   $(ECHO) "    .binit   : {} > ROM         /* INITIALIZATION TABLES             */"                    >> $@; \
   $(ECHO) "    .RAMCODE : {                /* Initialized code segment in RAM   */"                    >> $@; \
   $(ECHO) "                  fbl_hw.obj(.text)"                                                        >> $@; \
   $(ECHO) "                  fbl_mem.obj(.text)"                                                       >> $@; \
   $(ECHO) "               } load = ROM, run = RAM, table(BINIT)"                                       >> $@; \
   $(ECHO) "    .APPLVECT: {} > APPLVECT    /* base address of the Application Vectortable */"          >> $@; \
   $(ECHO) ""                                                                                           >> $@; \
   $(ECHO) "    .MMC     : {_e_SARMMC_ST = .;}  > MMC"                                                  >> $@; \
   $(ECHO) "    .DEC     : {_e_SARDEC_ST = .;}  > DEC"                                                  >> $@; \
   $(ECHO) "    .SYS     : {_e_SARSYS_ST = .;}  > SYS"                                                  >> $@; \
   $(ECHO) "}"                                                                                          >> $@

COMMA=,
DOT  =.

# Keep TABS in following section (Make requires tabs)

convertflash:  $(PROJECT_NAME).hex
	$(subst \,/, \
    $(FLASH) -er -pr -cs $(PROJECT_NAME).hex -o $(subst $(COMMA),$(DOT),$(BRS_TIMEBASE_CLOCK)) -p 378; \
  )

flash:
	$(subst \,/, \
    $(FLASH) -er -pr -cs $(PROJECT_NAME).$(BINARY_SUFFIX) -p 378; \
  )

$(PROJECT_NAME).hex: $(PROJECT_NAME).$(BINARY_SUFFIX)
	$(subst \,/,$(HEX) $(HEX_FLAGS) $<)

# end of Makefile.TMS470.TI.ALL.make
