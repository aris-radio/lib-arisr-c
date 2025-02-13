/**
 * @attention

    Copyright (C) 2025  - ARIS Alliance

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 **********************************************************************************
 * @file lib_arisr_base.h
 * @brief This file contains base definitions for the ARISr protocol.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#ifndef LIB_ARISR_BASE_H
#define LIB_ARISR_BASE_H

#include <stdint.h>

/******************************************************************************/
//
// OPERATING SYSTEM / ENVIRONMENT SETTINGS
//

// ARISR_ENV_WIN32	(Target = Windows 32)
// ARISR_ENV_UNIX	(Target = UNIX)


#if defined(_WIN32) || defined(_WIN64)
/* Win32 */
#define ARISR_ENV_WIN32
#elif defined(__unix__)
/* Unix (assumed) */
#define ARISR_ENV_UNIX
#else
/* Unknown */
#define ARISR_ENV_UNKNOWN
#endif

#if 0
#ifdef ARISR_ENV_WIN32
/* Include Windows header file */
#include <windows.h>
#endif /* ARISR_ENV_WIN32 */
#endif

// Debug
#ifdef DEBUG
#define ARISR_ENV_DEBUG
#endif


/******************************************************************************/
//
// TYPES
//

// Force use types from stdint.h
#define ARISR_FORCE_USE_STDINT 1

#if defined(ARISR_ENV_WIN32) && !ARISR_FORCE_USE_STDINT
typedef unsigned char   ARISR_UINT8;
typedef char            ARISR_SINT8;
typedef unsigned short	ARISR_UINT16;
typedef short			      ARISR_SINT16;
typedef unsigned int	  ARISR_UINT32;
typedef int				      ARISR_SINT32;
#elif defined(ARISR_ENV_UNIX) && !ARISR_FORCE_USE_STDINT
typedef unsigned char   ARISR_UINT8;
typedef char            ARISR_SINT8;
typedef unsigned int	  ARISR_UINT16;
typedef int 			      ARISR_SINT16;
typedef unsigned long 	ARISR_UINT32;
typedef long			      ARISR_SINT32;
#else
typedef uint8_t         ARISR_UINT8;
typedef int8_t          ARISR_SINT8;
typedef uint16_t    	  ARISR_UINT16;
typedef int16_t 		    ARISR_SINT16;
typedef uint32_t    	  ARISR_UINT32;
typedef int32_t			    ARISR_SINT32;
#endif

// NB not used by all library code!
typedef ARISR_UINT8  ARISR_CHAR;
typedef ARISR_UINT32 ARISR_UCHAR; // unicode character

/******************************************************************************/
//
// CONSTANTS (INTEGER TYPES)
//

#define ARISR_SIZE_OF_UINT8	    3	/* 0..255    */
#define ARISR_SIZE_OF_SINT8	    4	/* -127..128 */
#define ARISR_SIZE_OF_UINT16	  5	/* 0-65535   */
#define ARISR_SIZE_OF_SINT16	  6

#define ARISR_SIZE_OF_UINT24	  8

#define ARISR_SIZE_OF_UINT32	  10
#define ARISR_SIZE_OF_SINT32	  11

#define ARISR_MAX_UINT8		      0xff
#define ARISR_MAX_UINT16		    0xffff
#define ARISR_MAX_UINT24		    0xffffffL
#define ARISR_MAX_UINT32		    0xffffffffL


/******************************************************************************/
//
// CONSTANTS (ASCII CHARACTERS)
//

#define kARISR_ASCII_NULL			      0x00
#define kARISR_ASCII_HT			        0x09
#define kARISR_ASCII_LF			        0x0A
#define kARISR_ASCII_CR			        0x0D
#define kARISR_ASCII_SP			        0x20

#define kARISR_ASCII_QUOTE			    0x22
#define kARISR_ASCII_APOSTROPHE	    0x27
#define kARISR_ASCII_FORWARDSLASH	  0x2F
#define kARISR_ASCII_BACKSLASH		  0x5C

/******************************************************************************/
//
// MACROS (BIT MANIPULATION)
//

#define F_ARISR_BIT_TEST(value,bit)\
 ((((ARISR_UINT32)(value)) & ((ARISR_UINT32)1 << (bit))) ? 1 : 0)

#define F_ARISR_BIT_SET(value,bit)\
 (((ARISR_UINT32)(value)) |= ((ARISR_UINT32)1 << (bit)))

#define F_ARISR_BIT_CLEAR(value,bit)\
 (((ARISR_UINT32)(value)) &= ~((ARISR_UINT32)1 << (bit)))


/******************************************************************************/
//
// MACROS (BYTE-ARR -> INTEGER)
//

// converts 2 bytes to UINT16 - assumes logical ordering of the integer
#define F_ARISR_BYTES_TO_UINT16(byteArr)\
 ((((ARISR_UINT16)((byteArr)[0]) & ARISR_MAX_UINT8) << 8) +\
   ((ARISR_UINT16)((byteArr)[1]) & ARISR_MAX_UINT8))

// converts 3 bytes to UINT24 (ie 32) - assumes logical ordering of the integer
#define F_ARISR_BYTES_TO_UINT24(byteArr)\
 ((((ARISR_UINT32)((byteArr)[0]) & ARISR_MAX_UINT8) << 16) +\
  (((ARISR_UINT32)((byteArr)[1]) & ARISR_MAX_UINT8) <<  8) +\
   ((ARISR_UINT32)((byteArr)[2]) & ARISR_MAX_UINT8))

// converts 4 bytes to UINT32 - assumes logical ordering of the integer
#define F_ARISR_BYTES_TO_UINT32(byteArr)\
 ((((ARISR_UINT32)((byteArr)[0]) & ARISR_MAX_UINT8) << 24) +\
  (((ARISR_UINT32)((byteArr)[1]) & ARISR_MAX_UINT8) << 16) +\
  (((ARISR_UINT32)((byteArr)[2]) & ARISR_MAX_UINT8) <<  8) +\
   ((ARISR_UINT32)((byteArr)[3]) & ARISR_MAX_UINT8))

/******************************************************************************/
//
// MACROS (INTEGER -> BYTE-ARR)
//

// writes the UINT16 to the specified memory address (logical ordering)
// NB this macro DOES NOT advance the pointer
#define F_ARISR_UINT16_TO_BYTES(iVal,oArr)\
{ (oArr)[0]=((iVal) >> 8) & ARISR_MAX_UINT8;\
  (oArr)[1]= (iVal)       & ARISR_MAX_UINT8; }

// writes the UINT24 to the specified memory address (logical ordering)
// NB this macro DOES NOT advance the pointer
#define F_ARISR_UINT24_TO_BYTES(iVal,oArr)\
 {(oArr)[0]=((iVal) >> 16) & ARISR_MAX_UINT8;\
  (oArr)[1]=((iVal) >>  8) & ARISR_MAX_UINT8;\
  (oArr)[2]= (iVal)        & ARISR_MAX_UINT8;};

// writes the UINT32 to the specified memory address (logical ordering)
// NB this macro DOES NOT advance the pointer
#define F_ARISR_UINT32_TO_BYTES(iVal,oArr)\
 {(oArr)[0]=((iVal) >> 24) & ARISR_MAX_UINT8;\
  (oArr)[1]=((iVal) >> 16) & ARISR_MAX_UINT8;\
  (oArr)[2]=((iVal) >>  8) & ARISR_MAX_UINT8;\
  (oArr)[3]= (iVal)        & ARISR_MAX_UINT8;};

/******************************************************************************/
//
// MACROS (ASCII-HEX <--> NIBBLE)
//

#define F_ARISR_ASCHEX_2_NIBBLE(aschex)\
 ( ((aschex)>='a') ? (((aschex)-'a')%6)+10 : (((aschex)>='A') ? (((aschex)-'A')%6)+10 : ((aschex)-'0')%10) )

#define F_ARISR_NIBBLE_2_ASCHEX(nibble)\
 ( ((nibble)>9) ? (((nibble)-10)%6)+'A' : ((nibble)+'0') )

/******************************************************************************/
//
// MACROS (INTEGER COMPARISON)
//

#define MIN(x,y) ((x)<(y)?(x):(y))

#define MAX(x,y) ((x)>(y)?(x):(y))

/******************************************************************************/
//
// MACROS (INTEGER ADDITION)
//

// adds the amount indicated by 'addition' to 'initial' unless it would exceed
// the value indicated by 'limit', in which case the returned amount is limited
#define F_ARISR_UINT32_AddWithLimit(initial,addition,limit)\
 ((((ARISR_UINT32)(initial)>=(ARISR_UINT32)(limit))||\
   ((ARISR_UINT32)(addition)>=(ARISR_UINT32)(limit))||\
   (((ARISR_UINT32)(limit)-(ARISR_UINT32)(initial))<(ARISR_UINT32)(addition)))\
   ? (ARISR_UINT32)limit : (ARISR_UINT32)(initial)+(ARISR_UINT32)(addition))


#endif


/* COPYRIGHT ARIS Alliance */