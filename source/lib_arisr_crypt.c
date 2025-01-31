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
#include "lib_arisr_err.h"
#include "lib_arisr_interface.h"
#include "lib_arisr_crypt.h"

// =============================================
ARISR_UINT16 ARISR_crypt_crc16_calculate(const ARISR_UINT8 *data, const ARISR_UINT32 length) 
{
    ARISR_UINT16 crc = CRC16_INITIAL_VALUE;

    for (ARISR_UINT32 i = 0; i < length; i++) {
        ARISR_UINT8 index = (crc >> 8) ^ data[i];  // Get index
        crc = (crc << 8) ^ crc16_table[index]; // XOR with table value
    }
    return crc;
}


// =============================================
ARISR_ERR ARISR_aes_aris_decrypt(const ARISR_AES128_KEY *key, const ARISR_UINT8 *aris) 
{
    ARISR_UINT8 i;

    if (!key || !aris) {
        return kARISR_ERR_GENERIC;
    }

    ARISR_UINT8 result[ARISR_PROTO_ARIS_SIZE];

    // Las byte (16 bytes → index 15)
    ARISR_UINT8 last_byte = key[ARISR_AES128_BLOCK_SIZE - 1];

    for (i = 0; i < ARISR_PROTO_ARIS_SIZE; i++) 
        result[i] = (ARISR_UINT8)(aris[i] - last_byte);
    

    // Verify 'A' 'R' 'I' 'S'
    if (result[0] == ARISR_PROTO_ARIS_TEXT[0] && result[1] == ARISR_PROTO_ARIS_TEXT[1] && result[2] == ARISR_PROTO_ARIS_TEXT[2] && result[3] == ARISR_PROTO_ARIS_TEXT[3]) {
        return kARISR_OK;
    }

    return kARISR_ERR_NOT_SAME_ARIS;
}

// =============================================
ARISR_ERR ARISR_aes_aris_encrypt(const ARISR_AES128_KEY *key, ARISR_UINT8 *aris) 
{
    ARISR_UINT8 i;

    if (!key || !aris) {
        return kARISR_ERR_GENERIC;
    }

    ARISR_UINT8 last_byte = key[ARISR_AES128_BLOCK_SIZE - 1];

    for (i = 0; i < ARISR_PROTO_ARIS_SIZE; i++) {
        aris[i] = (ARISR_UINT8)(aris[i] + last_byte);
    }

    // No hay verificación aquí, pues es cifrado. Simplemente retornamos OK.
    return kARISR_OK;
}


/* COPYRIGHT ARIS Alliance */