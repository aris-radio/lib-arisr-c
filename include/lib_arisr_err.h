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
 * @file lib_arisr_err.h
 * @brief This file contains error code and message definitions for the ARISr protocol.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#ifndef LIB_ARISR_ERR_H
#define LIB_ARISR_ERR_H

#include <stdint.h>

#include "lib_arisr_base.h"

/******************************************************************************/
//
// TYPES
//

typedef ARISR_UINT32	ARISR_ERR;

/******************************************************************************/
//
// GENERIC ERROR CODES
//

#define kARISR_OK					       (ARISR_ERR)0
#define kARISR_ERR_GENERIC			       (ARISR_ERR)1
#define kARISR_ERR_NOT_SAME_ID		       (ARISR_ERR)2
#define kARISR_ERR_NOT_SAME_ARIS	       (ARISR_ERR)3
#define kARISR_ERR_NOT_SAME_CRC_HEADER	   (ARISR_ERR)4
#define kARISR_ERR_NOT_SAME_CRC_DATA	   (ARISR_ERR)5
#define kARISR_ERR_INVALID_ARGUMENT        (ARISR_ERR)6
#define kARISR_ERR_INVALID_PADDING         (ARISR_ERR)7
#define kARISR_ERR_CANNOT_DECRYPT          (ARISR_ERR)8
#define kARISR_ERR_NOT_SAME_END            (ARISR_ERR)9
#define kARISR_ERR_BUFFER_OVERFLOW         (ARISR_ERR)10
#define kARISR_ERR_NULL_ORIGIN             (ARISR_ERR)11
#define kARISR_ERR_NULL_DESTINATION        (ARISR_ERR)12

/******************************************************************************/

// Error names
static const char *ARISR_ERR_NAMES[] __attribute__((unused)) = {
    "kARISR_OK",
    "kARISR_ERR_GENERIC",
    "kARISR_ERR_NOT_SAME_ID",
    "kARISR_ERR_NOT_SAME_ARIS",
    "kARISR_ERR_NOT_SAME_CRC_HEADER",
    "kARISR_ERR_NOT_SAME_CRC_DATA",
    "kARISR_ERR_INVALID_ARGUMENT",
    "kARISR_ERR_INVALID_PADDING",
    "kARISR_ERR_CANNOT_DECRYPT",
    "kARISR_ERR_NOT_SAME_END",
    "kARISR_ERR_BUFFER_OVERFLOW",
    "kARISR_ERR_NULL_ORIGIN",
    "kARISR_ERR_NULL_DESTINATION"
};

#endif

/* COPYRIGHT ARIS Alliance */