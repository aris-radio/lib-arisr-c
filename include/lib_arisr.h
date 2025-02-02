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
 * @file lib_arisr.h
 * @brief This file contains the headers of the ARISr library.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#ifndef LIB_ARISR_H
#define LIB_ARISR_H

#include <stdint.h>

#include "lib_arisr_base.h"
#include "lib_arisr_interface.h"
#include "lib_arisr_comm.h"
#include "lib_arisr_err.h"
#include "lib_arisr_crypt.h"
#include "lib_arisr.h"


/**
 * @brief Cleans (resets) the raw chunk buffer, freeing any allocated memory.
 *
 * @param buffer Pointer to the ARISR_CHUNK_RAW structure.
 * @return kARISR_OK on success, or kARISR_ERR_GENERIC if buffer is NULL.
 */
ARISR_ERR ARISR_proto_raw_chunk_clean(ARISR_CHUNK_RAW *buffer);

/**
 * @brief Retrieves specific bits from a 32-bit control structure, using offset and mask.
 *
 * @param ctrl   Pointer to an ARISR_CHUNK_CTRL_RAW.
 * @param mask   Bit mask to apply after shifting.
 * @param shift  Bit offset to start from to the last byte.
 * @return The extracted bits as an 8-bit value.
 */
ARISR_UINT8 ARISR_proto_ctrl_getField(const ARISR_UINT8 *ctrl, ARISR_UINT32 mask, ARISR_UINT8 shift);

/**
 * @brief Receives and parses raw data into an ARISR_CHUNK_RAW structure.
 *
 * This function reads the incoming data byte by byte, separating the protocol fields,
 * allocating memory where needed, and checking the CRC values for both header and data.
 *
 * @param buffer Pointer to the ARISR_CHUNK_RAW structure where parsed data will be stored.
 * @param data   Pointer to the raw input data buffer (e.g., from the network or file).
 * @return kARISR_OK on success, or an error code for invalid parameters, CRC mismatch, etc.
 */
ARISR_ERR ARISR_proto_recv(ARISR_CHUNK_RAW *buffer, const ARISR_UINT8 *data, const ARISR_AES128_KEY key);


#endif


/* COPYRIGHT ARIS Alliance */