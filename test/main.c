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
 * @file test/main.c
 * @brief This file contains the main entry point for the ARISr library test suite.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h> // For malloc, free

#include "lib_arisr.h"
#include "test.h"


void printBuffer(ARISR_CHUNK_RAW *buffer)
{
    int i, j, destinations, from_relay, more_headers, data_length;

    destinations = ARISR_proto_ctrl_getField(buffer->ctrl, ARISR_CTRL_DESTS_MASK, ARISR_CTRL_DESTS_SHIFT);
    from_relay   = ARISR_proto_ctrl_getField(buffer->ctrl, ARISR_CTRL_FROM_MASK, ARISR_CTRL_FROM_SHIFT);
    more_headers = ARISR_proto_ctrl_getField(buffer->ctrl, ARISR_CTRL_MH_MASK, ARISR_CTRL_MH_SHIFT);

    printf("ID: ");
    for (i = 0; i < ARISR_PROTO_ID_SIZE; i++) {
        printf("%02X ", buffer->id[i]);
    }
    printf("\n");

    printf("ARIS: ");
    for (i = 0; i < ARISR_PROTO_ARIS_SIZE; i++) {
        printf("%02X ", buffer->aris[i]);
    }
    printf("\n");

    printf("CTRL: ");
    // See every field in the control section
    for (i = 0; i < ARISR_PROTO_ARIS_SIZE; i++) {
        printf("%02X ", buffer->ctrl[i]);
    }
    printf("\n");

    printf("ORIGIN: ");
    for (i = 0; i < ARISR_ADDRESS_SIZE; i++) {
        printf("%02X ", buffer->origin[i]);
    }
    printf("\n");

    printf("DESTINATION A: ");
    for (i = 0; i < ARISR_ADDRESS_SIZE; i++) {
        printf("%02X ", buffer->destinationA[i]);
    }
    printf("\n");

    printf("DESTINATION B: ");
    // Every destination in the array if exists
    if (buffer->destinationsB) {
        for (j = 0; j < destinations; j++) {
            for (i = 0; i < ARISR_ADDRESS_SIZE; i++) {
                printf("    %d: %02X ", i, buffer->destinationsB[j][i]);
            }
            printf("\n");
        }
    }
    printf("\n");

    printf("DESTINATION C: ");
    // If from_relay is set, print the destinationC
    if (from_relay) {
        for (i = 0; i < ARISR_ADDRESS_SIZE; i++) {
            printf("%02X ", buffer->destinationC[i]);
        }
    }
    printf("\n");

    printf("CTRL2: ");
    // If more_headers is set, print the control section 2 every field
    if (more_headers) {
        data_length  = ARISR_proto_ctrl_getField(buffer->ctrl2, ARISR_CTRL2_DATA_LENGTH_MASK, ARISR_CTRL2_DATA_LENGTH_SHIFT);

        // The real data length is 'data_length' as n * 8 Bytes
        data_length *= 8;

        printf("Data length: %d\n", data_length);
        for (i = 0; i < ARISR_CTRL2_SECTION_SIZE; i++) {
            printf("%02X ", buffer->ctrl2[i]);
        }

        printf("DATA: ");
        for (i = 0; i < data_length; i++) {
            printf("%02X ", buffer->data[i]);
        }
        printf("\n");
    }
    printf("\n");

    printf("CRC HEADER: ");
    for (i = 0; i < ARISR_CRC_SIZE; i++) {
        printf("%02X ", buffer->crc_header[i]);
    }
    printf("\n");

    printf("CRC DATA: ");
    for (i = 0; i < ARISR_CRC_SIZE; i++) {
        printf("%02X ", buffer->crc_data[i]);
    }
    printf("\n");

    printf("END: ");
    for (i = 0; i < ARISR_PROTO_ID_SIZE; i++) {
        printf("%02X ", buffer->end[i]);
    }
    printf("\n");
}

// =================================================================================================

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    ARISR_ERR err;

    // Key
    ARISR_AES128_KEY key = ARISR_MSG_KEY;

    // =====================================

    // Test 1 - Receive and parse raw data case 1
    ARISR_CHUNK_RAW buffer;

    if ((err = ARISR_proto_recv(&buffer, ARISR_MSG_RAW_1, key)) != kARISR_OK) {
        printf("Test 1 failed with error = %d\n", err);
        return err;
    }

    // Print the buffer contents
    printf("Test 1 passed\n");
    // printBuffer(&buffer);

    // Clean up the buffer
    ARISR_proto_raw_chunk_clean(&buffer);

    // =====================================

    // Test 2 - Receive and parse raw data case 2

    if ((err = ARISR_proto_recv(&buffer, ARISR_MSG_RAW_2, key)) != kARISR_OK) {
        printf("Test 2 failed with error = %d\n", err);
        return err;
    }

    // Print the buffer contents
    printf("Test 2 passed\n");
    // printBuffer(&buffer);


    return 0;
}