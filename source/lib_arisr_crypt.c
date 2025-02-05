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

#include <stdlib.h>
#include <string.h>

#include "lib_arisr_aes.h"
#include "lib_arisr_base.h"
#include "lib_arisr_err.h"
#include "lib_arisr_interface.h"
#include "lib_arisr_crypt.h"

// =============================================
ARISR_UINT16 ARISR_crypt_crc16_calculate(const ARISR_UINT8 *data, const ARISR_UINT32 length) 
{
    ARISR_UINT16 crc = CRC16_INITIAL_VALUE;
    ARISR_UINT32 i;

    for (i = 0; i < length; i++) {
        ARISR_UINT8 index = (crc >> 8) ^ data[i];  // Get index
        crc = (crc << 8) ^ crc16_table[index]; // XOR with table value
    }
    return crc;
}

// =============================================
ARISR_ERR ARISR_aes_aris_decrypt(const ARISR_AES128_KEY key, const ARISR_UINT8 *aris) 
{
    ARISR_UINT8 i;

    if (!aris) {
        return kARISR_ERR_GENERIC;
    }

    // Copy as result and then modify in place
    ARISR_UINT8 result[ARISR_PROTO_ARIS_SIZE];
    memcpy(result, aris, ARISR_PROTO_ARIS_SIZE);

    // Las byte (16 bytes → index 15)
    ARISR_UINT8 last_byte = (ARISR_AES_IS_ZERO_KEY(key)) ? 0 : (ARISR_UINT8)key[ARISR_AES128_BLOCK_SIZE - 1];

    // Check if the last byte is not zero, if not calculate the subtraction if yes jump to the next step
    if (last_byte)
    for (i = 0; i < ARISR_PROTO_ARIS_SIZE; i++)
        result[i] = (ARISR_UINT8)(aris[i] - last_byte);

    // Verify 'A' 'R' 'I' 'S'
    if (result[0] == ARISR_PROTO_ARIS_TEXT[0] && result[1] == ARISR_PROTO_ARIS_TEXT[1] && result[2] == ARISR_PROTO_ARIS_TEXT[2] && result[3] == ARISR_PROTO_ARIS_TEXT[3]) {
        return kARISR_OK;
    }

    return kARISR_ERR_NOT_SAME_ARIS;
}

// =============================================
ARISR_ERR ARISR_aes_aris_encrypt(const ARISR_AES128_KEY key, ARISR_UINT8 *aris) 
{
    ARISR_UINT8 i;

    if (!aris) {
        return kARISR_ERR_GENERIC;
    }

    // Las byte (16 bytes → index 15)
    ARISR_UINT8 last_byte = (ARISR_AES_IS_ZERO_KEY(key)) ? 0 : (ARISR_UINT8)key[ARISR_AES128_BLOCK_SIZE - 1];

    // Module increment
    if (last_byte)
    for (i = 0; i < ARISR_PROTO_ARIS_SIZE; i++) {
        aris[i] = (ARISR_UINT8)(aris[i] + last_byte);
    }

    // No hay verificación aquí, pues es cifrado. Simplemente retornamos OK.
    return kARISR_OK;
}

// =============================================
ARISR_ERR ARISR_aes_data_encrypt(const ARISR_AES128_KEY key,
                                 const ARISR_UINT8 *input,
                                 ARISR_UINT32 input_len,
                                 ARISR_UINT8 **output,
                                 ARISR_UINT32 *output_len)
{
    ARISR_UINT32 offset;
    ARISR_UINT8 *padded_data;
    // Validate input parameters
    if (!input || input_len == 0 || !output || !output_len) {
        return kARISR_ERR_INVALID_ARGUMENT;
    }

    // Calculate PKCS#7 padding requirements
    // Always add padding (even if input is block-aligned) per RFC 5652
    const ARISR_UINT8 pad_value = AES_BLOCKLEN - (input_len % AES_BLOCKLEN);
    const ARISR_UINT32 padded_len = input_len + pad_value;

    // Security boundary check: prevent integer overflow
    if (padded_len < input_len) {
        return kARISR_ERR_BUFFER_OVERFLOW;
    }

    // Allocate secure buffer for padded data
    padded_data = (ARISR_UINT8 *)malloc(padded_len);
    if (!padded_data) {
        return kARISR_ERR_GENERIC;
    }

    // Prepare plaintext with padding
    memcpy(padded_data, input, input_len);
    memset(padded_data + input_len, pad_value, pad_value);

    // Initialize AES context
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);

    // Encrypt using ECB mode (warning: ECB is insecure for most real-world use)
    // Process each block independently
    for (offset = 0; offset < padded_len; offset += AES_BLOCKLEN) {
        AES_ECB_encrypt(&ctx, padded_data + offset);
    }

    // Set output parameters - transfer ownership of buffer to caller
    *output = padded_data;
    *output_len = padded_len;

    return kARISR_OK;
}


// =============================================
ARISR_ERR ARISR_aes_data_decrypt(const ARISR_AES128_KEY key,
                                 const ARISR_UINT8 *input,
                                 ARISR_UINT32 input_len,
                                 ARISR_UINT8 **output,
                                 ARISR_UINT32 *output_len)
{
    ARISR_UINT32 i, original_len;
    ARISR_UINT8 pad, *decrypted_data, *resized;

    // Strict argument validation
    if (!input || input_len == 0 || input_len % AES_BLOCKLEN != 0 || !output || !output_len) {
        return kARISR_ERR_INVALID_ARGUMENT;
    }

    // Allocate memory for decrypted data
    decrypted_data = malloc(input_len);
    if (!decrypted_data) {
        return kARISR_ERR_GENERIC;
    }

    // Copy encrypted data to buffer
    memcpy(decrypted_data, input, input_len);

    // Initialize AES context
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);

    // Perform ECB mode decryption (block-by-block)
    for (ARISR_UINT32 i = 0; i < input_len; i += AES_BLOCKLEN) {
        AES_ECB_decrypt(&ctx, decrypted_data + i);
    }

    // PKCS#7 Padding Validation
    // --------------------------
    // 1. Get padding value from last byte
    pad = decrypted_data[input_len - 1];
    
    // 2. Validate padding range (1-16 for AES-128)
    if (pad == 0 || pad > AES_BLOCKLEN) {
        free(decrypted_data);
        return kARISR_ERR_INVALID_PADDING;
    }

    // 3. Verify all padding bytes match the padding value
    for (i = 1; i <= pad; ++i) {
        if (decrypted_data[input_len - i] != pad) {
            free(decrypted_data);
            return kARISR_ERR_INVALID_PADDING;
        }
    }

    // Calculate actual data length without padding
    original_len = input_len - pad;

    // Optimize memory usage by resizing the buffer
    resized = realloc(decrypted_data, original_len);
    if (!resized) {
        free(decrypted_data);
        return kARISR_ERR_GENERIC;
    }

    // Set output parameters
    *output = resized;
    *output_len = original_len;

    return kARISR_OK;
}
/* COPYRIGHT ARIS Alliance */