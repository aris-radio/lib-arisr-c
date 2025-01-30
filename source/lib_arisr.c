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
 * @file lib_arisr.c
 * @brief This file contains the implementation of the ARISr library.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lib_arisr_base.h"
#include "lib_arisr_interface.h"
#include "lib_arisr_comm.h"
#include "lib_arisr_err.h"
#include "lib_arisr_crypt.h"
#include "lib_arisr.h"


ARISR_UINT8 ARISR_proto_ctrl_getField(void *ctrl, ARISR_UINT8 offset, ARISR_UINT8 mask)
{
    // Return the field of the control section
    return (*(ARISR_UINT32 *)ctrl >> offset) & mask;
}

ARISR_ERR ARISR_proto_raw_chunk_clean(ARISR_CHUNK_RAW *buffer)
{
    // Clean the buffer
    if (buffer == NULL)
    {
        return kARISR_ERR_GENERIC;
    }
    // Remove malloc if exist
    if (buffer->destinationsB)
    {
        free(buffer->destinationsB);
    }

    if (buffer->data)
    {
        free(buffer->data);
    }

    // Clean the buffer
    memset(buffer, 0, sizeof(ARISR_CHUNK_RAW));

    return kARISR_OK;
}

ARISR_ERR ARISR_proto_recv(ARISR_CHUNK_RAW *buffer, char *data)
{
    // Clean the buffer
    if (ARISR_proto_raw_chunk_clean(buffer))
    {
        return kARISR_ERR_GENERIC;
    }

    // Pointer
    unsigned int p = 0;
    // Checkpoint is used save the size of headers and calculate crc

    ARISR_UINT8 version, destinations, from_relay, more_headers, data_length, *header_raw;
    ARISR_UINT16 crc;

    if (buffer == NULL || data == NULL)
    {
        return kARISR_ERR_GENERIC;
    }

    /* =============== ID & ARIS ================= */

    // Copy ID (4 Bytes) ARIS (4 Bytes) the data from the data to the buffer pointer
    memcpy(buffer, data, ARISR_PROTO_CRYPT_SIZE);

    // Increment the pointer by ARISR_PART1_SEGMENT_SIZE
    p += ARISR_PROTO_CRYPT_SIZE;

    /* =============== CTRL 1 ================= */

    // Generate the control section and copy the data from the data to the buffer pointer
    ARISR_CHUNK_CTRL_RAW ctrl;
    memcpy(&ctrl, data + p, ARISR_CTRL_SECTION_SIZE);

    // Increment the pointer by ARISR_CTRL_SECTION_SIZE
    p += ARISR_CTRL_SECTION_SIZE;

    // Assign the control section to the buffer pointer
    buffer->ctrl = &ctrl;

    // Get the version and destinations from the control section
    version = ARISR_proto_ctrl_getField(&ctrl, 0x0, ARISR_CTRL_VERSION_MASK);
    destinations = ARISR_proto_ctrl_getField(&ctrl, ARISR_CTRL_DESTS_OFFSET, ARISR_CTRL_DESTS_MASK);
    from_relay = ARISR_proto_ctrl_getField(&ctrl, ARISR_CTRL_FROM_OFFSET, ARISR_CTRL_FROM_MASK);

    /* =============== ORIGIN & DESTINATION ================= */

    // Copy Origin Address (6 Bytes) Destination Address (6 Bytes) the data from the data to the buffer pointer
    memcpy(buffer->origin, data + p, ARISR_ADDRESS_SIZE * 2);
    p += ARISR_ADDRESS_SIZE * 2;

    /* =============== DESTINATIONS B ================= */

    // Create n = destinations number of 6 Bytes
    ARISR_UINT8 (*destinations_data)[6] = (ARISR_UINT8 (*)[ARISR_ADDRESS_SIZE])malloc(destinations * sizeof(ARISR_UINT8[ARISR_ADDRESS_SIZE]));

    // Copy the data from the data to the buffer pointer
    memcpy(destinations_data, data + p, destinations * ARISR_ADDRESS_SIZE);

    // Increment the pointer by destinations * ARISR_ADDRESS_SIZE
    p += destinations * ARISR_ADDRESS_SIZE;

    // Assign the destinations data to the buffer pointer
    buffer->destinationsB = destinations_data;

    /* =============== DESTINATION C ================= */

    // If the package was sent from a relay
    if (from_relay)
    {
        // Copy Destination Address (6 Bytes) the data from the data to the buffer pointer
        memcpy(buffer->destinationC, data + p, ARISR_ADDRESS_SIZE);

        // Increment the pointer by ARISR_ADDRESS_SIZE
        p += ARISR_ADDRESS_SIZE;
    }

    // Check if exists more headers
    more_headers = ARISR_proto_ctrl_getField(&ctrl, ARISR_CTRL_MH_OFFSET, ARISR_CTRL_MH_MASK);

    /* =============== CTRL 2 ================= */

    ARISR_CHUNK_CTRL2_RAW ctrl2;

    if (more_headers)
    {
        // Generate the control section 2 and copy the data from the data to the buffer pointer
        memcpy(&ctrl2, data + p, ARISR_CTRL2_SECTION_SIZE);

        // Increment the pointer by ARISR_CTRL2_SECTION_SIZE
        p += ARISR_CTRL2_SECTION_SIZE;

        // Assign the control section 2 to the buffer pointer
        buffer->ctrl2 = &ctrl2;
    }
    else {
        buffer->ctrl2 = NULL;
        buffer->data = NULL;
    }

    /* =============== CRC HEADER ================= */

    // Copy the CRC Header (2 Bytes)
    memcpy(buffer->crc_header, data + p, ARISR_CRC_SIZE);

    // Generate 
    header_raw = (ARISR_UINT8 *)malloc(p);
    memcpy(header_raw, data, p);

    // Calculate the CRC Header
    crc = ARISR_crypt_crc16_calculate(header_raw, p);

    // Check if the CRC Header is correct
    if (crc != buffer->crc_header)
    {
        free(header_raw);
        ARISR_proto_raw_chunk_clean(buffer);
        return kARISR_ERR_NOT_SAME_CRC_HEADER;
    }

    // Increment the pointer by ARISR_CRC_SIZE
    p += ARISR_CRC_SIZE;

    free(header_raw);

    /* =============== DATA ================= */

    if (more_headers)
    {
        // Get the data length from the control section 2
        data_length = ARISR_proto_ctrl_getField(&ctrl2, ARISR_CTRL2_DATA_LENGTH_OFFSET, ARISR_CTRL2_DATA_LENGTH_MASK);

        // Copy the data from the data to the buffer pointer
        buffer->data = (ARISR_UINT8 *)malloc(data_length);
        memcpy(buffer->data, data + p, data_length);

        // Increment the pointer by data_length
        p += data_length;

        /* =============== CRC DATA ================= */

        // CRC Data
        memcpy(buffer->crc_data, data + p, ARISR_CRC_SIZE);

        crc = ARISR_crypt_crc16_calculate(buffer->data, data_length);

        // Check if the CRC Data is correct
        if (crc != buffer->crc_data)
        {
            ARISR_proto_raw_chunk_clean(buffer);
            return kARISR_ERR_NOT_SAME_CRC_DATA;
        }

        // Increment the pointer by ARISR_CRC_SIZE
        p += ARISR_CRC_SIZE;
    }

    /* =============== END ================= */

    // Copy the End (4 Bytes) the data from the data to the buffer pointer
    memcpy(buffer->end, data + p, ARISR_PROTO_ID_SIZE);

    return kARISR_OK; // Return OK
}


/* COPYRIGHT ARIS Alliance */