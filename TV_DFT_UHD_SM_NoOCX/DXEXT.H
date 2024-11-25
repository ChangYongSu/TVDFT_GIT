/*****************************************************************************
 *
 * Driver Extension for DXFG/UFG Frame Grabber Driver 
 *
 *****************************************************************************/

#ifndef _DXEXT_H_
#define _DXEXT_H_

// Enables DXFG Extended Driver Interface
#define ENABLE_EXTENDED_INTERFACE	TRUE



#if ENABLE_EXTENDED_INTERFACE

#define DXFG_HAS_SYNCSEL 0x2

/*
 * DXFG/UFG Specific Message Identifiers
 */
#define DXFG_PRIVATE_MESSAGE_START	(DVM_START+DVM_USER)
typedef enum _MESSAGEID
{
	// Messages of driverExtVersion v1.X
	// Channel Independent Message
	DXFG_GET_VERSION = DXFG_PRIVATE_MESSAGE_START,

	// VIDEO_EXTERNALIN Messages
	DXFG_ENUM_SOURCES,
	DXFG_SET_SOURCE,
	DXFG_GET_SOURCE,
	DXFG_ENUM_PARAMETERS,
	DXFG_QUERY_MENU,
	DXFG_SET_PARAMETER,
	DXFG_GET_PARAMETER,

	// VIDEO_IN Message
	DXFG_ENUM_PIXELFORMATS,

	// End of private message definition
	DXFG_PRIVATE_MESSAGE_END
} dxfg_message_id_t;


/*
 * Device Types used for BoardType
 */
typedef enum _DEVICE
{
	FG01 = 0,
	FG02 = 1,					//FG02 Dexon card
	FG02_UFG_110 = 3,			//FG02 Unigraf card (sync separator with 110MHz limit)
	FG02_UFG_160 = 7			//FG02 Unigraf card (no limit)
} dxfg_device_t;

/*
 * Source Types
 */
typedef enum _SOURCE
{
	// Source types of driverExtVersion vX.1
	DXFG_SRC_RGB,
	DXFG_SRC_TMDS,
	DXFG_SRC_NTSC,
	DXFG_SRC_PAL,
	DXFG_SRC_SECAM,

	// Total number of predefined source types
	DXFG_NUM_SOURCES
} dxfg_source_t;


/*
 * Parameter Types
 */
typedef enum _PARAMETERTYPE
{
	// Parameter types of driverExtVersion vX.1
	DXFG_PAR_TYPE_INTEGER,
	DXFG_PAR_TYPE_BOOLEAN,
	DXFG_PAR_TYPE_MENU,
	DXFG_PAR_TYPE_BUTTON,
	DXFG_PAR_TYPE_STRING,

	// Total number of parameter types
	DXFG_NUM_PAR_TYPES
} dxfg_parameter_type_t;


/*
 * Parameter Identifiers
 */
typedef enum _PARAMETERID
{
	// Parameter identifiers of driverExtVersion vX.1
	DXFG_PAR_ID_BRIGHTNESS,		// integer
	DXFG_PAR_ID_CONTRAST,		// integer
	DXFG_PAR_ID_HUE,			// integer
	DXFG_PAR_ID_SATURATION,		// integer

	DXFG_PAR_ID_LINEDOUBLER,	// boolean
	DXFG_PAR_ID_HPOSITION,		// integer
	DXFG_PAR_ID_VPOSITION,		// integer

	DXFG_PAR_ID_DETECTTIMING,	// button
	DXFG_PAR_ID_HRES,			// integer
	DXFG_PAR_ID_VRES,			// integer
	DXFG_PAR_ID_HSTART,			// integer
	DXFG_PAR_ID_VSTART,			// integer
	DXFG_PAR_ID_HTOTAL,			// integer
	DXFG_PAR_ID_VFREQ,			// integer

	DXFG_PAR_ID_SYNCTYPE,		// menu
	DXFG_PAR_ID_HSPOLARITY,		// boolean
	DXFG_PAR_ID_VSPOLARITY,		// boolean

	DXFG_PAR_ID_INTERLACED,		// boolean
	DXFG_PAR_ID_INVERTFIELDS,	// boolean

	DXFG_PAR_ID_PHASE,			// integer

	DXFG_PAR_ID_REDOFFSET,		// integer
	DXFG_PAR_ID_GREENOFFSET,	// integer
	DXFG_PAR_ID_BLUEOFFSET,		// integer
	DXFG_PAR_ID_REDGAIN,		// integer
	DXFG_PAR_ID_GREENGAIN,		// integer
	DXFG_PAR_ID_BLUEGAIN,		// integer

	DXFG_PAR_ID_SYNCLOSS,		// boolean
	DXFG_PAR_ID_CALIBRATEPHASE,	// button
	DXFG_PAR_ID_CALIBRATEHSTART,	// button

	/*Sync Mode Selection parameters */

	DXFG_PAR_ID_SYNCSEL_AUTODETECT,		//boolean
	DXFG_PAR_ID_SYNCSEL_TYPE,			//menu
	DXFG_PAR_ID_SYNCSEL_INTERLACED,		//boolean
	DXFG_PAR_ID_SYNCSEL_CONTROL_REG,	//integer
	DXFG_PAR_ID_SYNCSEL_STATUS_REG,		//integer
	DXFG_PAR_ID_SYNCSEL_FORCE_INTERLACED,	//boolean
	DXFG_PAR_ID_SYNCSEL_EVEN_FRAME,		//boolean
	DXFG_PAR_ID_SYNCSEL_PLL_STABILE,	//boolean
	DXFG_PAR_ID_SYNCSEL_VSHS_ERR,		//boolean
	DXFG_PAR_ID_SYNCSEL_HS,				//boolean
	DXFG_PAR_ID_SYNCSEL_VS,				//boolean
	DXFG_PAR_ID_SYNCSEL_NOHS,			//boolean
	DXFG_PAR_ID_SYNCSEL_NOVS,			//boolean
	DXFG_PAR_ID_UVS_INPUT,				//integer
	DXFG_PAR_ID_UVS_HW,					//string
	DXFG_PAR_ID_UVS_SW,					//string
	DXFG_PAR_ID_UVS_ADDRESS,			//integer

	// Total number of parameter identifiers
	DXFG_NUM_PAR_IDS
} dxfg_parameter_id_t;

/*
 * Structure Definitions
 */

// DXFG_GET_VERSION
typedef struct
{
	WCHAR driverName[32];
	DWORD productVersionMS;
	DWORD productVersionLS;
	DWORD driverExtVersion;
	DWORD deviceType;
} dxfg_version_t;


// DXFG_ENUM_PARAMETERS
typedef struct
{
	dxfg_parameter_id_t id;		// Predefined ID of parameter
	WCHAR name[32];				// Suggested label for control
	dxfg_parameter_type_t type;	// Control type
	int minimum;				// Theoretical minimum value
	int maximum;				// Theoretical maximum value
	int step;					// Increment between values of integer parameter
	int default_value;
	DWORD flags;
} dxfg_enum_parameters_t;


// DXFG_QUERY_MENU
typedef struct
{
	int index;					// The index of the menu item: 0...maximum-1
	WCHAR item[32];				// Menu item string
} dxfg_query_menu_t;


// DXFG_GET_PARAMETER
#define DXFG_PARAMETER_DISABLED		0x00000001
typedef struct
{
	int value;					// actual value of the parameter
	int minimum;				// actual value range
	int maximum;
	DWORD flags;
	char svalue[30];			// actual value of the string parameter
} dxfg_get_parameter_t;


// DXFG_ENUM_PIXELFORMATS
typedef struct
{
	WORD biBitCount;			// see BITMAPINFOHEADER
	DWORD biCompression;
} dxfg_enum_pixelformats_t;


#endif // ENABLE_EXTENDED_INTERFACE


#endif // _DXEXT_H_
