/** @file sys_common.h
*   @brief Common Header File
*   @date 22.July.2009
*   @version 1.01.000
*
*   This file contains:
*   - General Definitions
*   .
*   which are relevant for all drivers.
*/

/* (c) Texas Instruments 2009-2010, All rights reserved. */



/* USER CODE BEGIN (0) */
/* USER CODE END */


/* General Definitios */

/** @def NULL
*   @brief NULL definition
*/
#ifndef NULL
    #define NULL ((void *) 0U)
#endif

/** @def TRUE
*   @brief definition for TRUE
*/
#ifndef TRUE
    #define TRUE 1U
#endif

/** @def FALSE
*   @brief BOOLEAN definition for FALSE
*/
#ifndef FALSE
    #define FALSE 0U
#endif

#ifndef _UINT64_T_DECLARED
typedef unsigned long long uint64_t;
#define	_UINT64_T_DECLARED
#endif

#ifndef _UINT32_T_DECLARED
typedef unsigned int uint32_t;
#define	_UINT32_T_DECLARED
#endif

#ifndef _UINT16_T_DECLARED
typedef unsigned short uint16_t;
#define	_UINT16_T_DECLARED
#endif

#ifndef _UINT8_T_DECLARED
typedef unsigned char uint8_t;
#define	_UINT8_T_DECLARED
#endif

/* USER CODE BEGIN (1) */
/* USER CODE END */


/** @file system.h
*   @brief System Driver Header File
*   @date 23.July.2009
*   @version 1.01.001
*
*   This file contains:
*   - Definitions
*   - Types
*   .
*   which are relevant for the System driver.
*/

/* (c) Texas Instruments 2009-2010, All rights reserved. */




/* USER CODE BEGIN (0) */
/* USER CODE END */


/* System General Definitions */

/** @enum systemInterrupt
*   @brief Alias names for clock sources
*
*   This enumeration is used to provide alias names for the clock sources:
*     - IRQ
*     - FIQ
*/
enum systemInterrupt
{
    SYS_IRQ, /**< Alias for IRQ interrupt */
    SYS_FIQ  /**< Alias for FIQ interrupt */
};

/** @enum systemClockSource
*   @brief Alias names for clock sources
*
*   This enumeration is used to provide alias names for the clock sources:
*     - Oscillator
*     - Pll1
*     - External1
*     - Low Power Oscillator Low
*     - Low Power Oscillator High
*     - PLL2
*     - External2
*     - Synchronous VCLK1
*/
enum systemClockSource
{
    SYS_OSC       		= 0,  /**< Alias for oscillator clock Source                */
    SYS_PLL1      		= 1,  /**< Alias for Pll1 clock Source                      */
    SYS_EXTERNAL1  		= 3, /**< Alias for external clock Source                  */
    SYS_LPO_LOW   		= 4,  /**< Alias for low power oscillator low clock Source  */
    SYS_LPO_HIGH  		= 5,  /**< Alias for low power oscillator high clock Source */
    SYS_PLL2    		= 6,			 /**< Alias for Pll2 clock Source                      */
    SYS_EXTERNAL2 		= 7,  /**< Alias for external 2 clock Source                */
    SYS_VCLK      		= 9   /**< Alias for synchronous VCLK1 clock Source         */
};

#define SYS_DOZE_MODE   0x000F3F02U
#define SYS_SNOOZE_MODE 0x000F3F03U
#define SYS_SLEEP_MODE  0x000FFFFFU
#define LPO_TRIM_VALUE  (((*(uint32_t   *)0xF00801B4U) & 0xFFFF0000)>>16)

/** @def SYS_PRE1
*   @brief Alias name for RTI1CLK PRE clock source
*
*   This is an alias name for the RTI1CLK pre clock source.
*   This can be either:
*     - Oscillator
*     - Pll
*     - 32 kHz Oscillator
*     - External
*     - Low Power Oscillator Low
*     - Low Power Oscillator High
*     - Flexray Pll
*/
#define SYS_PRE1 SYS_PLL1

/** @def SYS_PRE2
*   @brief Alias name for RTI2CLK pre clock source
*
*   This is an alias name for the RTI2CLK pre clock source.
*   This can be either:
*     - Oscillator
*     - Pll
*     - 32 kHz Oscillator
*     - External
*     - Low Power Oscillator Low
*     - Low Power Oscillator High
*     - Flexray Pll
*/
#define SYS_PRE2 SYS_PLL1

/* USER CODE BEGIN (1) */
/* USER CODE END */


/* System Register Frame 1 Definition */
/** @struct systemBase1
*   @brief System Register Frame 1 Definition
*
*   This type is used to access the System 1 Registers.
*/
/** @typedef systemBASE1_t
*   @brief System Register Frame 1 Type Definition
*
*   This type is used to access the System 1 Registers.
*/
typedef volatile struct systemBase1
{
    uint32_t SYSPC1;                 /* 0x0000 */
    uint32_t SYSPC2;                 /* 0x0004 */
    uint32_t SYSPC3;                 /* 0x0008 */
    uint32_t SYSPC4;                 /* 0x000C */
    uint32_t SYSPC5;                 /* 0x0010 */
    uint32_t SYSPC6;                 /* 0x0014 */
    uint32_t SYSPC7;                 /* 0x0018 */
    uint32_t SYSPC8;                 /* 0x001C */
    uint32_t SYSPC9;                 /* 0x0020 */
    uint32_t SSWPLL1;                /* 0x0024 */
    uint32_t SSWPLL2;                /* 0x0028 */
    uint32_t SSWPLL3;                /* 0x002C */
    uint32_t CSDIS;                  /* 0x0030 */
    uint32_t CSDISSET;               /* 0x0034 */
    uint32_t CSDISCLR;               /* 0x0038 */
    uint32_t CSDDIS;                 /* 0x003C */
    uint32_t CSDDISSET;              /* 0x0040 */
    uint32_t CSDDISCLR;              /* 0x0044 */
    uint32_t GHVSRC;                 /* 0x0048 */
    uint32_t VCLKASRC;               /* 0x004C */
    uint32_t RCLKSRC;                /* 0x0050 */
    uint32_t CSVSTAT;                /* 0x0054 */
    uint32_t MSTGCR;                 /* 0x0058 */
    uint32_t MINITGCR;               /* 0x005C */
    uint32_t MSINENA;                /* 0x0060 */
    uint32_t MSTFAIL;                /* 0x0064 */
    uint32_t MSTCGSTAT;              /* 0x0068 */
    uint32_t MINISTAT;               /* 0x006C */
    uint32_t PLLCTL1;                /* 0x0070 */
    uint32_t PLLCTL2;                /* 0x0074 */
    uint32_t UERFLAG;                /* 0x0078 */
    uint32_t DIEIDL;                 /* 0x007C */
    uint32_t DIEIDH;                 /* 0x0080 */
    uint32_t VRCTL;                  /* 0x0084 */
    uint32_t LPOMONCTL;              /* 0x0088 */
    uint32_t CLKTEST;                /* 0x008C */
    uint32_t DFTCTRLREG1;            /* 0x0090 */
    uint32_t DFTCTRLREG2;            /* 0x0094 */
    uint32_t : 32U;                  /* 0x0098 */
    uint32_t : 32U;                  /* 0x009C */
    uint32_t GPREG1;                 /* 0x00A0 */
    uint32_t BTRMSEL;                /* 0x00A4 */
    uint32_t IMPFASTS;               /* 0x00A8 */
    uint32_t IMPFTADD;               /* 0x00AC */
    uint32_t SSISR1;                 /* 0x00B0 */
    uint32_t SSISR2;                 /* 0x00B4 */
    uint32_t SSISR3;                 /* 0x00B8 */
    uint32_t SSISR4;                 /* 0x00BC */
    uint32_t RAMGCR;                 /* 0x00C0 */
    uint32_t BMMCR1;                 /* 0x00C4 */
    uint32_t BMMCR2;                 /* 0x00C8 */
    uint32_t MMUGCR;                 /* 0x00CC */
#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
    uint32_t        : 8U;            /* 0x00D0 */
    uint32_t PENA   : 1U;            /* 0x00D0 */
    uint32_t        : 7U;            /* 0x00D0 */
    uint32_t VCLKR  : 4U;            /* 0x00D0 */
    uint32_t        : 4U;            /* 0x00D0 */
    uint32_t VCLK2R : 4U;            /* 0x00D0 */
    uint32_t        : 4U;            /* 0x00D0 */
#else
    uint32_t        : 4U;            /* 0x00D0 */
    uint32_t VCLK2R : 4U;            /* 0x00D0 */
    uint32_t        : 4U;            /* 0x00D0 */
    uint32_t VCLKR  : 4U;            /* 0x00D0 */
    uint32_t        : 7U;            /* 0x00D0 */
    uint32_t PENA   : 1U;            /* 0x00D0 */
    uint32_t        : 8U;            /* 0x00D0 */
#endif
    uint32_t ECPCNTL;                /* 0x00D4 */
    uint32_t DSPGCR;                 /* 0x00D8 */
    uint32_t DEVCR1;                 /* 0x00DC */
    uint32_t SYSECR;                 /* 0x00E0 */
    uint32_t SYSESR;                 /* 0x00E4 */
    uint32_t ITIFLAG;                /* 0x00E8 */
    uint32_t GBLSTAT;                /* 0x00EC */
    uint32_t DEV;                    /* 0x00F0 */
    uint32_t SSIVEC;                 /* 0x00F4 */
    uint32_t SSIF;                   /* 0x00F8 */
} systemBASE1_t;


/** @def systemREG1
*   @brief System Register Frame 1 Pointer
*
*   This pointer is used by the system driver to access the system frame 1 registers.
*/
#define systemREG1 ((systemBASE1_t *)0xFFFFFF00U)

/** @def systemPORT
*   @brief ECLK GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of System/Eclk
*   (use the GIO drivers to access the port pins).
*/
#define systemPORT ((gioPORT_t *)0xFFFFFF04U)

/* USER CODE BEGIN (2) */
/* USER CODE END */


/* System Register Frame 2 Definition */
/** @struct systemBase2
*   @brief System Register Frame 2 Definition
*
*   This type is used to access the System 2 Registers.
*/
/** @typedef systemBASE2_t
*   @brief System Register Frame 2 Type Definition
*
*   This type is used to access the System 2 Registers.
*/
typedef volatile struct systemBase2
{
    uint32_t PLLCTL3;        /* 0x0000 */
    uint32_t : 32U;          /* 0x0004 */
    uint32_t STCCLKDIV;      /* 0x0008 */
    uint32_t : 32U;          /* 0x000C */
    uint32_t : 32U;          /* 0x0010 */
    uint32_t : 32U;          /* 0x0014 */
    uint32_t : 32U;          /* 0x0018 */
    uint32_t : 32U;          /* 0x001C */
    uint32_t : 32U;          /* 0x0020 */
    uint32_t ECPCNTRL0;      /* 0x0024 */
    uint32_t : 32U;          /* 0x0028 */
    uint32_t : 32U;          /* 0x002C */
    uint32_t : 32U;          /* 0x0030 */
    uint32_t : 32U;          /* 0x0034 */
    uint32_t : 32U;          /* 0x0038 */
#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
    uint32_t VCLK3R : 4U;    /* 0x003C */
    uint32_t        : 4U;    /* 0x003C */
    uint32_t VCLK4R : 4U;    /* 0x003C */
    uint32_t        : 20U;   /* 0x003C */
#else
    uint32_t        : 20U;   /* 0x003C */
    uint32_t VCLK4R : 4U;    /* 0x003C */
    uint32_t        : 4U;    /* 0x003C */
    uint32_t VCLK3R : 4U;    /* 0x003C */
#endif
    uint32_t VCLKACON1;      /* 0x0040 */
    uint32_t : 32U;          /* 0x0044 */
    uint32_t : 32U;          /* 0x0048*/
    uint32_t : 32U;          /* 0x004C */
    uint32_t : 32U;          /* 0x0050 */
    uint32_t : 32U;          /* 0x0054 */
    uint32_t : 32U;          /* 0x0058 */
    uint32_t : 32U;          /* 0x005C */
    uint32_t : 32U;          /* 0x0060 */
    uint32_t : 32U;          /* 0x0064 */
    uint32_t : 32U;       	  /* 0x0068 */
    uint32_t : 32U;       	  /* 0x006C */
    uint32_t  CLKSLIP;       /* 0x0070 */
    uint32_t : 32U;   	      /* 0x0074 */
    uint32_t : 32U;  		      /* 0x0078*/
    uint32_t : 32U;      	   /* 0x007C */
    uint32_t : 32U;          /* 0x0080 */
    uint32_t : 32U;          /* 0x0084 */
    uint32_t : 32U;       	  /* 0x0088 */
    uint32_t : 32U;      	   /* 0x008C */
    uint32_t : 32U;          /* 0x0090 */
    uint32_t : 32U;          /* 0x0094 */
    uint32_t : 32U;       	  /* 0x0098 */
    uint32_t : 32U;       	  /* 0x009C */
    uint32_t : 32U;          /* 0x00A0 */
    uint32_t : 32U;          /* 0x00A4 */
    uint32_t : 32U;       	  /* 0x00A8 */
    uint32_t : 32U;       	  /* 0x00AC */
    uint32_t : 32U;          /* 0x00B0 */
    uint32_t : 32U;          /* 0x00B4 */
    uint32_t : 32U;       	  /* 0x00B8 */
    uint32_t : 32U;       	  /* 0x00BC */
    uint32_t : 32U;          /* 0x00C0 */
    uint32_t : 32U;          /* 0x00C4 */
    uint32_t : 32U;       	  /* 0x00C8 */
    uint32_t : 32U;       	  /* 0x00CC */
    uint32_t : 32U;          /* 0x00D0 */
    uint32_t : 32U;          /* 0x00D4 */
    uint32_t : 32U;       	  /* 0x00D8 */
    uint32_t : 32U;       	  /* 0x00DC */
    uint32_t : 32U;          /* 0x00E0 */
    uint32_t : 32U;          /* 0x00E4 */
    uint32_t : 32U;       	  /* 0x00E8 */
    uint32_t  EFC_CTLEN;     /* 0x00EC */
    uint32_t  DIEIDL_REG0;   /* 0x00F0 */
    uint32_t  DIEIDH_REG1;   /* 0x00F4 */
    uint32_t  DIEIDL_REG2;   /* 0x00F8 */
    uint32_t  DIEIDH_REG3;   /* 0x00FC */
} systemBASE2_t;


/** @def systemREG2
*   @brief System Register Frame 2 Pointer
*
*   This pointer is used by the system driver to access the system frame 2 registers.
*/
#define systemREG2 ((systemBASE2_t *)0xFFFFE100U)

/* USER CODE BEGIN (3) */
/* USER CODE END */


/** @struct pcrBase
*   @brief Pcr Register Frame Definition
*
*   This type is used to access the Pcr Registers.
*/
/** @typedef pcrBASE_t
*   @brief PCR Register Frame Type Definition
*
*   This type is used to access the PCR Registers.
*/
typedef volatile struct pcrBase
{
    uint32_t PMPROTSET0;    /* 0x0000 */
    uint32_t PMPROTSET1;    /* 0x0004 */
    uint32_t : 32U;         /* 0x0008 */
    uint32_t : 32U;         /* 0x000C */
    uint32_t PMPROTCLR0;    /* 0x0010 */
    uint32_t PMPROTCLR1;    /* 0x0014 */
    uint32_t : 32U;         /* 0x0018 */
    uint32_t : 32U;         /* 0x001C */
    uint32_t PPROTSET0;     /* 0x0020 */
    uint32_t PPROTSET1;     /* 0x0024 */
    uint32_t PPROTSET2;     /* 0x0028 */
    uint32_t PPROTSET3;     /* 0x002C */
    uint32_t : 32U;         /* 0x0030 */
    uint32_t : 32U;         /* 0x0034 */
    uint32_t : 32U;         /* 0x0038 */
    uint32_t : 32U;         /* 0x003C */
    uint32_t PPROTCLR0;     /* 0x0040 */
    uint32_t PPROTCLR1;     /* 0x0044 */
    uint32_t PPROTCLR2;     /* 0x0048 */
    uint32_t PPROTCLR3;     /* 0x004C */
    uint32_t : 32U;         /* 0x0050 */
    uint32_t : 32U;         /* 0x0054 */
    uint32_t : 32U;         /* 0x0058 */
    uint32_t : 32U;         /* 0x005C */
    uint32_t PCSPWRDWNSET0; /* 0x0060 */
    uint32_t PCSPWRDWNSET1; /* 0x0064 */
    uint32_t : 32U;         /* 0x0068 */
    uint32_t : 32U;         /* 0x006C */
    uint32_t PCSPWRDWNCLR0; /* 0x0070 */
    uint32_t PCSPWRDWNCLR1; /* 0x0074 */
    uint32_t : 32U;         /* 0x0078 */
    uint32_t : 32U;         /* 0x007C */
    uint32_t PSPWRDWNSET0;  /* 0x0080 */
    uint32_t PSPWRDWNSET1;  /* 0x0084 */
    uint32_t PSPWRDWNSET2;  /* 0x0088 */
    uint32_t PSPWRDWNSET3;  /* 0x008C */
    uint32_t : 32U;         /* 0x0090 */
    uint32_t : 32U;         /* 0x0094 */
    uint32_t : 32U;         /* 0x0098 */
    uint32_t : 32U;         /* 0x009C */
    uint32_t PSPWRDWNCLR0;  /* 0x00A0 */
    uint32_t PSPWRDWNCLR1;  /* 0x00A4 */
    uint32_t PSPWRDWNCLR2;  /* 0x00A8 */
    uint32_t PSPWRDWNCLR3;  /* 0x00AC */
} pcrBASE_t;

/** @def pcrREG
*   @brief Pcr Register Frame Pointer
*
*   This pointer is used by the system driver to access the Pcr registers.
*/
#define pcrREG ((pcrBASE_t *)0xFFFFE000U)
/* #define pcrREG ((pcrBASE_t *)0xFFFF1000U) */

/* USER CODE BEGIN (4) */
/* USER CODE END */


/* FlashW General Definitions */


/** @enum flashWPowerModes
*   @brief Alias names for flash bank power modes
*
*   This enumeration is used to provide alias names for the flash bank power modes:
*     - sleep
*     - standby
*     - active
*/
enum flashWPowerModes
{
    SYS_SLEEP   = 0U, /**< Alias for flash bank power mode sleep   */
    SYS_STANDBY = 1U, /**< Alias for flash bank power mode standby */
    SYS_ACTIVE  = 3U  /**< Alias for flash bank power mode active  */
};

/* USER CODE BEGIN (5) */
/* USER CODE END */


/** @struct flashWBase
*   @brief Flash Wrapper Register Frame Definition
*
*   This type is used to access the Flash Wrapper Registers.
*/
/** @typedef flashWBASE_t
*   @brief Flash Wrapper Register Frame Type Definition
*
*   This type is used to access the Flash Wrapper Registers.
*/
typedef volatile struct flashWBase
{
    uint32_t FRDCNTL;     /* 0x0000 */
    uint32_t FSPRD;       /* 0x0004 */
    uint32_t FEDACCTRL1;  /* 0x0008 */
    uint32_t FEDACCTRL2;  /* 0x000C */
    uint32_t FCORERRCNT;  /* 0x0010 */
    uint32_t FCORERRADD;  /* 0x0014 */
    uint32_t FCORERRPOS;  /* 0x0018 */
    uint32_t FEDACSTATUS; /* 0x001C */
    uint32_t FUNCERRADD;  /* 0x0020 */
    uint32_t FEDACSDIS;   /* 0x0024 */
    uint32_t FPRIMADDTAG; /* 0x0028 */
    uint32_t FREDUADDTAG; /* 0x002C */
    uint32_t FBPROT;      /* 0x0030 */
    uint32_t FBSE;        /* 0x0034 */
    uint32_t FBBUSY;      /* 0x0038 */
    uint32_t FBAC;        /* 0x003C */
    uint32_t FBFALLBACK;  /* 0x0040 */
    uint32_t FBPRDY;      /* 0x0044 */
    uint32_t FPAC1;       /* 0x0048 */
    uint32_t FPAC2;       /* 0x004C */
    uint32_t FMAC;        /* 0x0050 */
    uint32_t FMSTAT;      /* 0x0054 */
    uint32_t FEMUDMSW;    /* 0x0058 */
    uint32_t FEMUDLSW;    /* 0x005C */
    uint32_t FEMUECC;     /* 0x0060 */
    uint32_t FLOCK;       /* 0x0064 */
    uint32_t FEMUADDR;    /* 0x0068 */
    uint32_t FDIAGCTRL;   /* 0x006C */
    uint32_t FRAWDATAH;   /* 0x0070 */
    uint32_t FRAWDATAL;   /* 0x0074 */
    uint32_t FRAWECC;     /* 0x0078 */
    uint32_t FPAROVR;     /* 0x007C */
    uint32_t FVREADCT;    /* 0x0080 */
    uint32_t FVHVCT1;     /* 0x0084 */
    uint32_t FVHVCT2;     /* 0x0088 */
    uint32_t FVNVCT;      /* 0x008C */
    uint32_t FVPPCT;      /* 0x0090 */
    uint32_t FVWLCT;      /* 0x0094 */
    uint32_t FEFUSE;      /* 0x0098 */
    uint32_t : 32U;       /* 0x009C */
    uint32_t : 32U;       /* 0x00A0 */
    uint32_t : 32U;       /* 0x00A4 */
    uint32_t : 32U;       /* 0x00A8 */
    uint32_t : 32U;       /* 0x00AC */
    uint32_t : 32U;       /* 0x00B0 */
    uint32_t : 32U;       /* 0x00B4 */
    uint32_t : 32U;       /* 0x00B8 */
    uint32_t : 32U;       /* 0x00BC */
    uint32_t FEDACSDIS2;  /* 0x00C0 */
    uint32_t : 32U;       /* 0x00C4 */
    uint32_t : 32U;       /* 0x00C8 */
    uint32_t : 32U;       /* 0x00CC */
    uint32_t : 32U;       /* 0x00D0 */
    uint32_t : 32U;       /* 0x00D4 */
    uint32_t : 32U;       /* 0x00D8 */
    uint32_t : 32U;       /* 0x00DC */
    uint32_t : 32U;       /* 0x00E0 */
    uint32_t : 32U;       /* 0x00E4 */
    uint32_t : 32U;       /* 0x00E8 */
    uint32_t : 32U;       /* 0x00EC */
    uint32_t : 32U;       /* 0x00F0 */
    uint32_t : 32U;       /* 0x00F4 */
    uint32_t : 32U;       /* 0x00F8 */
    uint32_t : 32U;       /* 0x00FC */
    uint32_t FBSTROBES;   /* 0x0100 */
    uint32_t FPSTROBES;   /* 0x0104 */
    uint32_t FBMODE;      /* 0x0108 */
    uint32_t FTCR;        /* 0x010C */
    uint32_t FADDR;       /* 0x0110 */
    uint32_t FWRITE;      /* 0x0114 */
    uint32_t FCBITSEL;    /* 0x0118 */
    uint32_t FTCTRL;      /* 0x011C */
    uint32_t FWPWRITE0;   /* 0x0120 */
    uint32_t FWPWRITE1;   /* 0x0124 */
    uint32_t FWPWRITE2;   /* 0x0128 */
    uint32_t FWPWRITE3;   /* 0x012C */
    uint32_t FWPWRITE4;   /* 0x0130 */
} flashWBASE_t;

/** @def flashWREG
*   @brief Flash Wrapper Register Frame Pointer
*
*   This pointer is used by the system driver to access the flash wrapper registers.
*/
#define flashWREG ((flashWBASE_t *)(0xFFF87000U))

/* USER CODE BEGIN (6) */
/* USER CODE END */

#define FSM_WR_ENA_HL      (*(volatile uint32_t *)0xFFF87288U)
#define EEPROM_CONFIG_HL   (*(volatile uint32_t *)0xFFF872B8U)

/* System Interface Functions */
void systemInit(void);
void systemPowerDown(uint32_t mode);



/** @file system.c
*   @brief System Driver Source File
*   @date 21.July.2009
*   @version 1.01.000
*
*   This file contains:
*   - API Funcions
*   .
*   which are relevant for the System driver.
*/

/* (c) Texas Instruments 2009-2010, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */


/* Include Files */


/* USER CODE BEGIN (1) */
/* USER CODE END */


/** @fn void systemInit(void)
*   @brief Initializes System Driver
*
*   This function initializes the System driver.
*
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void systemInit(void)
{
/* USER CODE BEGIN (3) */
/* USER CODE END */

    /** @b Initialize @b Flash @b Wrapper: */

    /** - Setup flash read mode, address wait states and data wait states */
    flashWREG->FRDCNTL =  0x00000000U
                       | (3U << 8U)
                       | (1U << 4U)
                       |  1U;

    /** - Setup flash access wait states for bank 7 */
    FSM_WR_ENA_HL    = 0x5U;
    EEPROM_CONFIG_HL = 0x00000002U
                    | (9U << 16U) ;

/* USER CODE BEGIN (7) */
/* USER CODE END */

    /** - Disable write access to flash state machine registers */
    FSM_WR_ENA_HL    = 0xAU;

    /** - Setup flash bank power modes */
    flashWREG->FBFALLBACK = 0x00000000
                          | (SYS_ACTIVE << 14U)
                          | (SYS_SLEEP << 12U)
                          | (SYS_SLEEP << 10U)
                          | (SYS_SLEEP << 8U)
                          | (SYS_SLEEP << 6U)
                          | (SYS_SLEEP << 4U)
                          | (SYS_ACTIVE << 2U)
                          |  SYS_ACTIVE;

    /** @b Initialize @b Lpo: */
    /** Load TRIM values from OTP if present else load user defined values */
    if(LPO_TRIM_VALUE != 0xFFFF)
    {

        systemREG1->LPOMONCTL  = (1U << 24U)
                                | LPO_TRIM_VALUE;
    }
    else
    {

    	systemREG1->LPOMONCTL 	= (1U << 24U)
                                 | (16U << 8U)
                                 | 8U;
    }

    /** @b Initialize @b Pll2: */

    /**   - Setup pll2 control register :
    *     - Setup Pll output clock divider
    *     - Setup reference clock divider
    *     - Setup internal Pll output divider
    *     - Setup Pll multiplier
    */

    systemREG2->PLLCTL3 = ((2U - 1U) << 29U)
                        | ((1U - 1U)<< 24U)
                        | ((6U - 1U)<< 16U)
                        | ((120U - 1U) << 8U);


    /** @b Initialize @b Pll1: */

    /**   - Setup pll control register 1:
    *     - Setup reset on oscillator slip
    *     - Setup bypass on pll slip
    *     - Setup Pll output clock divider
    *     - Setup reset on oscillator fail
    *     - Setup reference clock divider
    *     - Setup Pll multiplier
    */
    systemREG1->PLLCTL1 =  0x00000000U
                        |  0x20000000U
                        | ((1U - 1U)<< 24U)
                        |  0x00000000U
                        | ((5U - 1U)<< 16U)
                        | ((80U - 1U)<< 8U);

    /**   - Setup pll control register 2
    *     - Enable/Disable frequency modulation
    *     - Setup spreading rate
    *     - Setup bandwidth adjustment
    *     - Setup internal Pll output divider
    *     - Setup spreading amount
    */
    systemREG1->PLLCTL2 =  0x00000000U
                        | (255U << 22U)
                        | (7U << 12U)
                        | ((2U - 1U)<< 9U)
                        |  61U;


    /** @b Initialize @b Clock @b Tree: */

    /** - Start clock source lock */
    systemREG1->CSDIS = 0x00000000U
                      | 0x00000000U
                      | 0x00000008U
                      | 0x00000080U
                      | 0x00000000U
                      | 0x00000000U
                      | 0x00000000U;

    /** - Wait for until clocks are locked */
    while ((systemREG1->CSVSTAT & ((systemREG1->CSDIS ^ 0xFF) & 0xFB)) != ((systemREG1->CSDIS ^ 0xFF) & 0xFB))
{
}
    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    systemREG1->GHVSRC = (SYS_PLL1 << 24U)
                       | (SYS_PLL1 << 16U)
                       |  SYS_PLL1;

    /** - Power-up all peripharals */
    pcrREG->PSPWRDWNCLR0 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR1 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR2 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR3 = 0xFFFFFFFFU;

    /** - Setup synchronous peripheral clock dividers for VCLK1, VCLK2, VCLK3, VCLK4 */
/*    systemREG1->VCLKR   = 1U;*/
    systemREG1->VCLKR   = 15U;
    systemREG1->VCLK2R  = 1U;
    systemREG2->VCLK3R  = 1U;

    /** - Setup RTICLK1 and RTICLK2 clocks */
    systemREG1->RCLKSRC = (1U << 24U)
                        | (SYS_VCLK << 16U)
                        | (1U << 8U)
                        |  SYS_VCLK;

    /** - Setup asynchronous peripheral clock sources for AVCLK1 and AVCLK2 */
    systemREG1->VCLKASRC = (SYS_VCLK << 8U)
                          |  SYS_VCLK;

    systemREG2->VCLKACON1 = (1U << 24)
                           | 1 << 20U
                           | (SYS_VCLK << 16)
                           | (1U << 1)
                           | 1 << 4U
                           | SYS_VCLK;

    /** - Enable Peripherals */
    systemREG1->PENA = 1U;

    /** - set ECLK pins functional mode */
    systemREG1->SYSPC1 = 0U;

    /** - set ECLK pins default output value */
    systemREG1->SYSPC4 = 0U;

    /** - set ECLK pins output direction */
    systemREG1->SYSPC2 = 1U;

    /** - set ECLK pins open drain enable */
    systemREG1->SYSPC7 = 0U;

    /** - set ECLK pins pullup/pulldown enable */
    systemREG1->SYSPC8 = 0U;

    /** - set ECLK pins pullup/pulldown select */
    systemREG1->SYSPC9 = 1U;

    /** - Setup ECLK */
    systemREG1->ECPCNTL = (0U << 24U)
                        | (0U << 23U)
                        | ((8U - 1U) & 0xFFFFU);

    /** @note: HCLK >= VCLK2 >= VCLK_sys */

/* USER CODE BEGIN (4) */
/* USER CODE END */
}

void systemPowerDown(uint32_t mode)
{
    /* Disable clock sources */
    systemREG1->CSDISSET = mode & 0x000000FFU;

    /* Disable clock domains */
    systemREG1->CSDDIS = (mode >> 8U) & 0x00000FFFU;

    /* Idle CPU */
    asm(" wfi");
}


/* USER CODE BEGIN (5) */
/* USER CODE END */

/** @file sys_startup.c
*   @brief Startup Source File
*   @date 06.June.2010
*   @version 1.01.001
*
*   This file contains:
*   - Include Files
*   - Type Definitions
*   - External Functions
*   - VIM RAM Setup
*   - Startup Routine
*   .
*   which are relevant for the Startup.
*/

/* (c) Texas Instruments 2009-2010, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */


/* Include Files */


/* USER CODE BEGIN (1) */
/* USER CODE END */


/* Type Definitions */

typedef void (*handler_fptr)(const uint8_t *in, uint8_t *out);

/* USER CODE BEGIN (2) */
/* USER CODE END */


/* External Functions */

#pragma WEAK(__TI_Handler_Table_Base)
#pragma WEAK(__TI_Handler_Table_Limit)
#pragma WEAK(__TI_CINIT_Base)
#pragma WEAK(__TI_CINIT_Limit)

extern uint32_t   __TI_Handler_Table_Base;
extern uint32_t   __TI_Handler_Table_Limit;
extern uint32_t   __TI_CINIT_Base;
extern uint32_t   __TI_CINIT_Limit;
extern uint32_t   __TI_PINIT_Base;
extern uint32_t   __TI_PINIT_Limit;
extern uint32_t * __binit__;

extern void main(void);
extern void exit(void);

/* USER CODE BEGIN (3) */
/* USER CODE END */


#if 0
/* Vim Ram Definition */
/** @struct vimRam
*   @brief Vim Ram Definition
*
*   This type is used to access the Vim Ram.
*/
/** @typedef vimRAM_t
*   @brief Vim Ram Type Definition
*
*   This type is used to access the Vim Ram.
*/
typedef volatile struct vimRam
{
    t_isrFuncPTR ISR[VIM_CHANNELS];
} vimRAM_t;

#define vimRAM ((vimRAM_t *)0xFFF82000U)

static const t_isrFuncPTR s_vim_init[] =
{
    &phantomInterrupt,
    &esmHighInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
    &phantomInterrupt,
};
#endif


/* Startup Routine */

/* USER CODE BEGIN (4) */
/* USER CODE END */

extern void _coreInitRegisters_();
extern void _coreInitStackPointer_();

#pragma INTERRUPT(_c_int00, RESET)

void _c_int00()
{

/* USER CODE BEGIN (5) */
/* USER CODE END */

	/* read the system exception status register */
	register uint32_t temp = systemREG1->SYSESR;

/* USER CODE BEGIN (6) */
/* USER CODE END */

	/* check for power-on reset condition */
	if (temp & 0x8000)
	{
		/* clear all reset status flags */
		systemREG1->SYSESR = 0xFFFF;

/* USER CODE BEGIN (7) */
/* USER CODE END */

		/* continue with normal start-up sequence */
	}
	else if (temp & 0x4000)
	{
		/* Reset caused due to oscillator failure.
		Add user code here to handle oscillator failure */

/* USER CODE BEGIN (8) */
/* USER CODE END */
	}
	else if (temp & 0x2000)
	{
		/* Reset caused due to windowed watchdog violation.
		Add user code here to handle watchdog violation */

/* USER CODE BEGIN (9) */
/* USER CODE END */
	}
	else if (temp & 0x20)
	{
		/* Reset caused due to CPU reset.
		CPU reset can be caused by CPU self-test completion, or
		by toggling the "CPU RESET" bit of the CPU Reset Control Register.
		Add user code to handle CPU reset:
		check for selftest completion without any error and continue start-up. */

/* USER CODE BEGIN (10) */
/* USER CODE END */
	}
	else if (temp & 0x10)
	{
		/* Reset caused due to software reset.
		Add user code to handle software reset. */

/* USER CODE BEGIN (11) */
/* USER CODE END */
	}
	else
	{
		/* Reset caused by nRST being driven low externally.
		Add user code to handle external reset. */

/* USER CODE BEGIN (12) */
/* USER CODE END */
	}
    /* Initialize Core Registers */
    _coreInitRegisters_();

    /* Initialize Stack Pointers */
    _coreInitStackPointer_();

    /* Initialize System */
    systemInit();

#if 0
    /* Initialize memory */
    _memoryInit_();

    /* Initialize VIM table */
    {
        uint32_t i;

        for (i = 0; i < 90U; i++)
        {
            vimRAM->ISR[i] = s_vim_init[i];
        }
    }

    /* set IRQ/FIQ priorities */
    vimREG->FIRQPR0 =  SYS_FIQ
                    | (SYS_FIQ <<  1U)
                    | (SYS_IRQ <<  2U)
                    | (SYS_IRQ <<  3U)
                    | (SYS_IRQ <<  4U)
                    | (SYS_IRQ <<  5U)
                    | (SYS_IRQ <<  6U)
                    | (SYS_IRQ <<  7U)
                    | (SYS_IRQ <<  8U)
                    | (SYS_IRQ <<  9U)
                    | (SYS_IRQ << 10U)
                    | (SYS_IRQ << 11U)
                    | (SYS_IRQ << 12U)
                    | (SYS_IRQ << 13U)
                    | (SYS_IRQ << 14U)
                    | (SYS_IRQ << 15U)
                    | (SYS_IRQ << 16U)
                    | (SYS_IRQ << 17U)
                    | (SYS_IRQ << 18U)
                    | (SYS_IRQ << 19U)
                    | (SYS_IRQ << 20U)
                    | (SYS_IRQ << 21U)
                    | (SYS_IRQ << 22U)
                    | (SYS_IRQ << 23U)
                    | (SYS_IRQ << 24U)
                    | (SYS_IRQ << 25U)
                    | (SYS_IRQ << 26U)
                    | (SYS_IRQ << 27U)
                    | (SYS_IRQ << 28U)
                    | (SYS_IRQ << 29U)
                    | (SYS_IRQ << 30U)
                    | (SYS_IRQ << 31U);

    vimREG->FIRQPR1 =  SYS_IRQ
                    | (SYS_IRQ <<  1U)
                    | (SYS_IRQ <<  2U)
                    | (SYS_IRQ <<  3U)
                    | (SYS_IRQ <<  4U)
                    | (SYS_IRQ <<  5U)
                    | (SYS_IRQ <<  6U)
                    | (SYS_IRQ <<  7U)
                    | (SYS_IRQ <<  8U)
                    | (SYS_IRQ <<  9U)
                    | (SYS_IRQ << 10U)
                    | (SYS_IRQ << 11U)
                    | (SYS_IRQ << 12U)
                    | (SYS_IRQ << 13U)
                    | (SYS_IRQ << 14U)
                    | (SYS_IRQ << 15U)
                    | (SYS_IRQ << 16U)
                    | (SYS_IRQ << 17U)
                    | (SYS_IRQ << 18U)
                    | (SYS_IRQ << 19U)
                    | (SYS_IRQ << 20U)
                    | (SYS_IRQ << 21U)
                    | (SYS_IRQ << 22U)
                    | (SYS_IRQ << 23U)
                    | (SYS_IRQ << 24U)
                    | (SYS_IRQ << 25U)
                    | (SYS_IRQ << 26U)
                    | (SYS_IRQ << 27U)
                    | (SYS_IRQ << 28U)
                    | (SYS_IRQ << 29U)
                    | (SYS_IRQ << 30U)
                    | (SYS_IRQ << 31U);


    vimREG->FIRQPR2 =  SYS_IRQ
                    | (SYS_IRQ << 1U)
                    | (SYS_IRQ << 2U)
                    | (SYS_IRQ << 3U)
                    | (SYS_IRQ << 4U)
                    | (SYS_IRQ << 5U)
                    | (SYS_IRQ << 6U)
                    | (SYS_IRQ << 7U)
                    | (SYS_IRQ << 8U)
                    | (SYS_IRQ << 9U)
                    | (SYS_IRQ << 10U)
                    | (SYS_IRQ << 11U)
                    | (SYS_IRQ << 12U)
                    | (SYS_IRQ << 13U)
                    | (SYS_IRQ << 14U)
                    | (SYS_IRQ << 15U)
                    | (SYS_IRQ << 16U)
                    | (SYS_IRQ << 17U)
                    | (SYS_IRQ << 18U)
                    | (SYS_IRQ << 19U)
                    | (SYS_IRQ << 20U)
                    | (SYS_IRQ << 21U)
                    | (SYS_IRQ << 22U)
                    | (SYS_IRQ << 23U)
                    | (SYS_IRQ << 24U)
                    | (SYS_IRQ << 25U);


    /* enable interrupts */
    vimREG->REQMASKSET0 = 1U
                        | (0U << 1U)
                        | (0U << 2U)
                        | (0U << 3U)
                        | (0U << 4U)
                        | (0U << 5U)
                        | (0U << 6U)
                        | (0U << 7U)
                        | (0U << 8U)
                        | (0U << 9U)
                        | (0U << 10U)
                        | (0U << 11U)
                        | (0U << 12U)
                        | (0U << 13U)
                        | (0U << 14U)
                        | (0U << 15U)
                        | (0U << 16U)
                        | (0U << 17U)
                        | (0U << 18U)
                        | (0U << 19U)
                        | (0U << 20U)
                        | (0U << 21U)
                        | (0U << 22U)
                        | (0U << 23U)
                        | (0U << 24U)
                        | (0U << 25U)
                        | (0U << 26U)
                        | (0U << 27U)
                        | (0U << 28U)
                        | (0U << 29U)
                        | (0U << 30U)
                        | (0U << 31U);

    vimREG->REQMASKSET1 = 0U
                        | (0U << 1U)
                        | (0U << 2U)
                        | (0U << 3U)
                        | (0U << 4U)
                        | (0U << 5U)
                        | (0U << 6U)
                        | (0U << 7U)
                        | (0U << 8U)
                        | (0U << 9U)
                        | (0U << 10U)
                        | (0U << 11U)
                        | (0U << 12U)
                        | (0U << 13U)
                        | (0U << 14U)
                        | (0U << 15U)
                        | (0U << 16U)
                        | (0U << 17U)
                        | (0U << 18U)
                        | (0U << 19U)
                        | (0U << 20U)
                        | (0U << 21U)
                        | (0U << 22U)
                        | (0U << 23U)
                        | (0U << 24U)
                        | (0U << 25U)
                        | (0U << 26U)
                        | (0U << 27U)
                        | (0U << 28U)
                        | (0U << 29U)
                        | (0U << 30U)
                        | (0U << 31U);

    vimREG->REQMASKSET2 = 0U
                        | (0U << 1U)
                        | (0U << 2U)
                        | (0U << 3U)
                        | (0U << 4U)
                        | (0U << 5U)
                        | (0U << 6U)
                        | (0U << 7U)
                        | (0U << 8U)
                        | (0U << 9U)
                        | (0U << 10U)
                        | (0U << 11U)
                        | (0U << 12U)
                        | (0U << 13U)
                        | (0U << 14U)
                        | (0U << 15U)
                        | (0U << 16U)
                        | (0U << 17U)
                        | (0U << 18U)
                        | (0U << 19U)
                        | (0U << 20U)
                        | (0U << 21U)
                        | (0U << 22U)
                        | (0U << 23U)
                        | (0U << 24U)
                        | (0U << 25U);

#endif

#if 1

    /* initalise copy table */
    if ((uint32_t *)&__binit__ != (uint32_t *)0xFFFFFFFFU)
    {
        extern void copy_in(void *binit);
        copy_in((void *)&__binit__);
    }

    /* initalise the C global variables */

/* Added to support Vector FBL:
   This code initializes with null the dummy eeprom section "EEPDATA". If you use the delivered dummy eeprom driver to store the validity information,
   please adapt your linker file as follows to avoid the initialization of this section

              .EEPDATA : {} load = EEP, run = EEP
   and check in the *.map file that the EEPDATA section is NOT PART of the cinit Section . e.g. .cinit..FLASHDRV.load) [load image, compression = zero_init]

   Following warning should be ignored:
   "DemoFbl.lcf", line 48: warning: LOAD placement ignored for ".EEPDATA":  object is uninitialized
*/

    if (&__TI_Handler_Table_Base < &__TI_Handler_Table_Limit)
    {
        uint8_t **tablePtr   = (uint8_t **)&__TI_CINIT_Base;
        uint8_t **tableLimit = (uint8_t **)&__TI_CINIT_Limit;

        while (tablePtr < tableLimit)
        {
            uint8_t *loadAdr = *tablePtr++;
            uint8_t *runAdr  = *tablePtr++;
            uint8_t  idx     = *loadAdr++;
            handler_fptr   handler = (handler_fptr)(&__TI_Handler_Table_Base)[idx];

            (*handler)((const uint8_t *)loadAdr, runAdr);
        }
    }

    /* initalise contructors */
    if (__TI_PINIT_Base < __TI_PINIT_Limit)
    {
        void (**p0)() = (void *)__TI_PINIT_Base;

        while ((uint32_t)p0 < __TI_PINIT_Limit)
        {
            void (*p)() = *p0++;
            p();
        }
    }
#endif

/* USER CODE BEGIN (13) */
/* USER CODE END */

    /* call the application */
    main();
    exit();
}


/* USER CODE BEGIN (14) */
/* USER CODE END */
