/*********************************************************************
 * This file needs to be included in fbl_inc.h for Demo Application. 
 ********************************************************************/

/* This enabled Jump to boot function and some required modifications of Standard bootloader sources in order to work as application example */
#define FBL_USE_FBL_AS_DEMOAPPL 

/* This removes application start */
#define FBL_NO_JSR_APPL

/* This disables the FblHeader-address check */
#if defined( FBL_ENABLE_SYSTEM_CHECK )
# undef FBL_ENABLE_SYSTEM_CHECK
# define FBL_DISABLE_SYSTEM_CHECK
#endif

/* This overwirtes configuration to always enable 
 * - permanent Diagnostic Identifiers, which are mandatory for any application. 
 */
#if defined( FBL_DISABLE_CAN_CONFIGURATION )
# undef FBL_DISABLE_CAN_CONFIGURATION
# define FBL_ENABLE_CAN_CONFIGURATION
#endif

/* Stub functionality which is not available */
#define ApplFblStartApplication()

/* This cannot be checked because of absent applvect. Is not required in Appl-context anyway.. */
#if defined(FblCheckBootVectTableIsValid)
# undef FblCheckBootVectTableIsValid
# define FblCheckBootVectTableIsValid() 0
#endif
