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
 * @file lib_arisr_comm.h
 * @brief This file contains the parsed interface of the ARISr protocol.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#ifndef LIB_ARISR_COMM_H
#define LIB_ARISR_COMM_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lib_arisr_base.h"
#include "lib_arisr_interface.h"


/* ******************************************************** */
// Define data struct
#pragma pack(1)
typedef ARISR_UINT8 ARISR_UINT48[6];
#pragma pack()

// Set Integer To value
/**
 * @brief Convert number into 6 bytes
 * @param data 
 * @param buffer
 */
static inline void ARISR_UINT48_Set(ARISR_UINT48 *data, const uint8_t *buffer) {
    if (data && buffer) {
        memcpy(data, buffer, 6);
    }
}

// Get Integer From value
/**
 * @brief Get number from 6 bytes
 * @param data
 * @return int64_t number
 */
static inline int64_t ARISR_UINT48_GetInt(const ARISR_UINT48 *data) {
    int64_t resultado = 0;
    if (data) {
        memcpy(&resultado, data, 6);
    }
    return resultado;
}

typedef struct {
    ARISR_UINT8 version;
    ARISR_UINT8 destinations;
    ARISR_UINT8 option;
    ARISR_UINT8 from;
    ARISR_UINT8 sequence;
    ARISR_UINT8 retry;
    ARISR_UINT8 more_data;
    ARISR_UINT8 identifier;
    ARISR_UINT8 more_header;
} ARISR_CHUNK_CTRL;

typedef struct {
    ARISR_UINT8 data_length;
    ARISR_UINT8 feature;
    ARISR_UINT8 neg_answer;
    ARISR_UINT8 freq_switch;
} ARISR_CHUNK_CTRL2;

typedef struct {
    ARISR_UINT32 id;                    // 4 Bytes
    ARISR_UINT32 aris;                  // 4 Bytes
    ARISR_CHUNK_CTRL ctrl;              // 4 Bytes
    ARISR_UINT48 origin;                // 6 Bytes
    ARISR_UINT48 destinationA;          // 6 Bytes
    ARISR_UINT48 *destinationsB;        // ... n*6 Bytes
    ARISR_UINT48 destinationC;          // 6 Bytes
    ARISR_CHUNK_CTRL2 ctrl2;            // 4 Bytes
    ARISR_UINT16 crc_header;            // 2 Bytes
    ARISR_UINT8 *data;                  // n Bytes
    ARISR_UINT16 crc_data[2];           // 2 Bytes
    ARISR_UINT32 end;                   // 4 Bytes
} ARISR_CHUNK;



#endif

/* COPYRIGHT ARIS Alliance */