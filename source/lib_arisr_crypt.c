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
 * @file lib_arisr_crypt.c
 * @brief This file contains the implementation of cryptographic functions for the ARISr protocol.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#include "lib_arisr_base.h"
#include "lib_arisr_crypt.h"

ARISR_UINT16 ARISR_crypt_crc16_calculate(const uint8_t *data, ARISR_UINT32 length) {
    ARISR_UINT16 crc = CRC16_INITIAL_VALUE;

    for (ARISR_UINT32 i = 0; i < length; i++) {
        ARISR_UINT8 index = (crc >> 8) ^ data[i];  // Get index
        crc = (crc << 8) ^ crc16_table[index]; // XOR with table value
    }
    return crc;
}

/* COPYRIGHT ARIS Alliance */