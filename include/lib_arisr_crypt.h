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
 * @file lib_arisr_crypt.h
 * @brief This file contains the implementation of cryptographic functions for the ARISr protocol.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#ifndef LIB_ARISR_CRYPT_H
#define LIB_ARISR_CRYPT_H

#include <stdio.h>
#include <stdint.h>
#include "lib_arisr_base.h"

// Polinomio standard CRC-16-CCITT (X^16 + X^12 + X^5 + 1) = 0x1021
// CRC-16-CCITT-FALSE or CRC-16/IBM-CCITT or CRC-16/AUTOSAR or CRC-16/IBM-3740
#define CRC16_POLYNOMIAL 0x1021
#define CRC16_INITIAL_VALUE 0xFFFF  // Initial value for CRC16 calculation

static const ARISR_UINT16 crc16_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/**
 * @brief Computes the CRC-16 checksum for the given data buffer.
 *
 * This function calculates a **16-bit Cyclic Redundancy Check (CRC-16)**
 * over a given data buffer. CRC-16 is commonly used for error detection in
 * communication protocols and data integrity verification.
 *
 * The specific polynomial and initial value used should be documented elsewhere
 * in the cryptographic implementation details.
 *
 * @param data   Pointer to the input data buffer to be processed.
 * @param length Number of bytes in the data buffer.
 * @return The computed 16-bit CRC value as an ARISR_UINT16.
 *
 * @note Ensure that the provided `data` pointer is valid and `length` is greater than zero.
 *       If `data` is NULL or `length` is 0, the behavior may be undefined.
 *
 * @warning The CRC calculation might differ depending on the chosen polynomial, bit order,
 *          and initial value. Ensure compatibility with expected CRC implementations.
 */
ARISR_UINT16 ARISR_crypt_crc16_calculate(const ARISR_UINT8 *data, const ARISR_UINT32 length);



// =================================================================================================

// AES-128
#define ARISR_AES128_BLOCK_SIZE 16
#define ARISR_AES128_KEY_SIZE   128

#pragma pack(1)
typedef 
    ARISR_UINT8 ARISR_AES128_KEY[ARISR_AES128_BLOCK_SIZE];
#pragma pack()

/**
 * @brief Static function to check if the AES key is zero.
 * 
 * @param key The AES key to check.
 * @return 0 if the key is zero, 1 otherwise.
 */
#define ARISR_AES_IS_ZERO_KEY(key) (memcmp((key), (const ARISR_UINT8[16]){0}, 16) == 0)

/**
 * @brief Performs a cyclic subtraction (mod 256) on the 4-byte 'aris' array using the last byte of a 16-byte AES key.
 *
 * This function retrieves the last byte from the 16-byte AES key (index 15),
 * then for each of the four bytes in 'aris', subtracts that byte mod 256.
 * 
 * If the final 'aris' matches the ASCII string "ARIS", it returns kARISR_OK;
 * otherwise, kARISR_ERR_GENERIC.
 *
 * @param key   A 16-byte AES-128 key (constant array).
 * @param aris  A 4-byte array to be modified in place via cyclic subtraction.
 * @return kARISR_OK if 'aris' becomes "ARIS" after the subtraction,
 *         otherwise kARISR_ERR_GENERIC.
 */
ARISR_ERR ARISR_aes_aris_decrypt(const ARISR_AES128_KEY key, const ARISR_UINT8 *aris);

/**
 * @brief Encrypts the 4-byte 'aris' array by adding the last byte of a 128-bit (16-byte) AES key.
 *
 * This function retrieves the last byte from the 16-byte AES key (index 15),
 * then for each of the four bytes in 'aris', adds that byte (mod 256).
 * 
 * The result is placed in 'aris' (in-place). This mirrors the decryption function
 * which subtracts the same byte. Decryption + Encryption should restore the original
 * "ARIS" string if used correctly.
 *
 * @param key   Pointer to a 16-byte AES-128 key union.
 * @param aris  A 4-byte buffer to be modified in place by adding the last byte of 'key'.
 * @return kARISR_OK if the encryption process succeeds,
 *         otherwise kARISR_ERR_GENERIC if inputs are null.
 */
ARISR_ERR ARISR_aes_aris_encrypt(const ARISR_AES128_KEY key, ARISR_UINT8 *aris);

/**
 * @brief Encrypt data using AES-128 in ECB mode with PKCS#7 padding.
 *
 * This function encrypts the input data of arbitrary length by applying PKCS#7 padding so that
 * its length becomes a multiple of AES_BLOCKLEN (typically 16 bytes), and then encrypts it using AES-128 ECB mode.
 *
 * @param key         The AES-128 encryption key.
 * @param input       Pointer to the input data to be encrypted.
 * @param input_len   Length of the input data in bytes.
 * @param output      Pointer to the pointer where the encrypted (and padded) data will be stored (dynamically allocated).
 * @param output_len  Pointer to the variable where the length of the encrypted data will be stored.
 * @return int        Returns kARISR_OK on success, or an error code (e.g., kARISR_ERR_INVALID_ARGUMENT, kARISR_ERR_MEMORY).
 *
 * @note The caller is responsible for freeing the memory allocated for *output.
 */
ARISR_ERR ARISR_aes_data_encrypt(const ARISR_AES128_KEY key,
                                 const ARISR_UINT8 *input,
                                 ARISR_UINT32 input_len,
                                 ARISR_UINT8 **output,
                                 ARISR_UINT32 *output_len);

/**
 * @brief Decrypt data encrypted using AES-128 in ECB mode with PKCS#7 padding.
 *
 * This function decrypts the input encrypted data and removes the PKCS#7 padding.
 * It returns the decrypted data and its original length (without padding).
 *
 * @param key         The AES-128 decryption key.
 * @param input       Pointer to the input encrypted data.
 * @param input_len   Length of the input encrypted data in bytes (must be a multiple of AES_BLOCKLEN).
 * @param output      Pointer to the pointer where the decrypted data will be stored (dynamically allocated).
 * @param output_len  Pointer to the variable where the length of the decrypted data (excluding padding) will be stored.
 * @return int        Returns kARISR_OK on success, or an error code (e.g., kARISR_ERR_INVALID_ARGUMENT, kARISR_ERR_MEMORY, kARISR_ERR_INVALID_PADDING).
 *
 * @note The caller is responsible for freeing the memory allocated for *output.
 */
ARISR_ERR ARISR_aes_data_decrypt(const ARISR_AES128_KEY key,
                                 const ARISR_UINT8 *input,
                                 ARISR_UINT32 input_len,
                                 ARISR_UINT8 **output,
                                 ARISR_UINT32 *output_len);
#endif

/* COPYRIGHT ARIS Alliance */