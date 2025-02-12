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
#include "log.h"
#include "test.h"



/**
 * @brief Prints the raw contents of an ARISR_CHUNK_RAW buffer.
 *
 * This function outputs the contents of the ARISR_CHUNK_RAW structure 
 * in a readable format for debugging purposes. It prints the raw 
 * binary data, including IDs, control fields, addresses, and payload data.
 *
 * @param buffer Pointer to the ARISR_CHUNK_RAW structure to be printed.
 */
void printBufferRaw(ARISR_CHUNK_RAW *buffer);

/**
 * @brief Prints the parsed contents of an ARISR_CHUNK buffer.
 *
 * This function outputs the structured and decoded contents of an 
 * ARISR_CHUNK structure. It prints the extracted and decrypted data, 
 * including metadata, control fields, addresses, and payload information.
 *
 * @param buffer Pointer to the ARISR_CHUNK structure to be printed.
 */
void printBuffer(ARISR_CHUNK *buffer);

/**
 * @brief Do all the checks
 *
 * This function returns the checking of the buffer.
 *
 * @param buffer Pointer to the ARISR_CHUNK structure to be printed.
 */
int checkBuffer(ARISR_CHUNK *interface, int i);



void printBufferRaw(ARISR_CHUNK_RAW *buffer)
{
    int j, destinations, from_relay, more_headers, data_length = 0;

    destinations = ARISR_proto_ctrl_getField(buffer->ctrl, ARISR_CTRL_DESTS_MASK, ARISR_CTRL_DESTS_SHIFT);
    from_relay   = ARISR_proto_ctrl_getField(buffer->ctrl, ARISR_CTRL_FROM_MASK, ARISR_CTRL_FROM_SHIFT);
    more_headers = ARISR_proto_ctrl_getField(buffer->ctrl, ARISR_CTRL_MH_MASK, ARISR_CTRL_MH_SHIFT);

    LOG_INFO("[ID]          %02X %02X %02X %02X", buffer->id[0], buffer->id[1], buffer->id[2], buffer->id[3]);
    LOG_INFO("[ARIS]        %02X %02X %02X %02X", buffer->aris[0], buffer->aris[1], buffer->aris[2], buffer->aris[3]);
    LOG_INFO("[CTRL]        %02X %02X %02X %02X", buffer->ctrl[0], buffer->ctrl[1], buffer->ctrl[2], buffer->ctrl[3]);
    LOG_INFO("[ORIGIN]      %02X %02X %02X %02X %02X %02X", buffer->origin[0], buffer->origin[1], buffer->origin[2], buffer->origin[3], buffer->origin[4], buffer->origin[5]);
    LOG_INFO("[DEST A]      %02X %02X %02X %02X %02X %02X", buffer->destinationA[0], buffer->destinationA[1], buffer->destinationA[2], buffer->destinationA[3], buffer->destinationA[4], buffer->destinationA[5]);

    // Every destination in the array if exists
    if (destinations > 0 && buffer->destinationsB) {
        LOG_INFO("[DEST B] ");
        for (j = 0; j < destinations; j++) {
            LOG_INFO("  [%03d]          %02X %02X %02X %02X %02X %02X", j, buffer->destinationsB[j][0], buffer->destinationsB[j][1], buffer->destinationsB[j][2], buffer->destinationsB[j][3], buffer->destinationsB[j][4], buffer->destinationsB[j][5]);
        }
    }

    // If from_relay is set, print the destinationC
    if (from_relay) {
        LOG_INFO("[DEST C]      %02X %02X %02X %02X %02X %02X", buffer->destinationC[0], buffer->destinationC[1], buffer->destinationC[2], buffer->destinationC[3], buffer->destinationC[4], buffer->destinationC[5]);
    }

    // If more_headers is set, print the control section 2 every field
    if (more_headers) {
        LOG_INFO("[CTRL2]       %02X %02X %02X %02X", buffer->ctrl2[0], buffer->ctrl2[1], buffer->ctrl2[2], buffer->ctrl2[3]);
        data_length  = ARISR_proto_ctrl_getField(buffer->ctrl2, ARISR_CTRL2_DATA_LENGTH_MASK, ARISR_CTRL2_DATA_LENGTH_SHIFT);

        // The real data length is 'data_length' as n * 8 Bytes
        data_length *= 8;
    }

    LOG_INFO("[CRC H]       %02X %02X", buffer->crc_header[0], buffer->crc_header[1]);
    LOG_INFO("[CRC D]       %02X %02X", buffer->crc_data[0], buffer->crc_data[1]);

    LOG_INFO("[END]         %02X %02X %02X %02X", buffer->end[0], buffer->end[1], buffer->end[2], buffer->end[3]);

    if (data_length > 0) {
        LOG_INFO("");
        LOG_INFO("[DATA] ");
        hex_dump(buffer->data, data_length);
    }
}

void printBuffer(ARISR_CHUNK *buffer)
{
    int j;

    LOG_INFO("[ID]          %02X %02X %02X %02X", buffer->id[0], buffer->id[1], buffer->id[2], buffer->id[3]);
    LOG_INFO("[ARIS]        %02X %02X %02X %02X", buffer->aris[0], buffer->aris[1], buffer->aris[2], buffer->aris[3]);
    LOG_INFO("[CTRL]");
    LOG_INFO("  [VER]          %d", buffer->ctrl.version);
    LOG_INFO("  [DEST]         %d", buffer->ctrl.destinations);
    LOG_INFO("  [OPT]          %d", buffer->ctrl.option);
    LOG_INFO("  [FROM]         %d", buffer->ctrl.from);
    LOG_INFO("  [SEQ]          %d", buffer->ctrl.sequence);
    LOG_INFO("  [RET]          %d", buffer->ctrl.retry);
    LOG_INFO("  [MD]           %d", buffer->ctrl.more_data);
    LOG_INFO("  [ID]           %d", buffer->ctrl.identifier);
    LOG_INFO("  [MH]           %d", buffer->ctrl.more_header);

    LOG_INFO("[ORIGIN]      %02X %02X %02X %02X %02X %02X", buffer->origin[0], buffer->origin[1], buffer->origin[2], buffer->origin[3], buffer->origin[4], buffer->origin[5]);
    LOG_INFO("[DEST A]      %02X %02X %02X %02X %02X %02X", buffer->destinationA[0], buffer->destinationA[1], buffer->destinationA[2], buffer->destinationA[3], buffer->destinationA[4], buffer->destinationA[5]);

    // Every destination in the array if exists
    if (buffer->ctrl.destinations > 0 && buffer->destinationsB) {
        LOG_INFO("[DEST B] ");
        for (j = 0; j < buffer->ctrl.destinations; j++) {
            LOG_INFO("  [%03d]          %02X %02X %02X %02X %02X %02X", j, buffer->destinationsB[j][0], buffer->destinationsB[j][1], buffer->destinationsB[j][2], buffer->destinationsB[j][3], buffer->destinationsB[j][4], buffer->destinationsB[j][5]);
        }
    }

    // If from_relay is set, print the destinationC
    if (buffer->ctrl.from) {
        LOG_INFO("[DEST C]      %02X %02X %02X %02X %02X %02X", buffer->destinationC[0], buffer->destinationC[1], buffer->destinationC[2], buffer->destinationC[3], buffer->destinationC[4], buffer->destinationC[5]);
    }

    // If more_headers is set, print the control section 2 every field
    if (buffer->ctrl.more_header) {
        LOG_INFO("[CTRL2]");
        LOG_INFO("  [DL]           %d", buffer->ctrl2.data_length);
        LOG_INFO("  [FEAT]         %d", buffer->ctrl2.feature);
        LOG_INFO("  [NEG]          %d", buffer->ctrl2.neg_answer);
        LOG_INFO("  [FREQ]         %d", buffer->ctrl2.freq_switch);
    }

    LOG_INFO("[CRC H]       %02X %02X", buffer->crc_header[0], buffer->crc_header[1]);
    LOG_INFO("[CRC D]       %02X %02X", buffer->crc_data[0], buffer->crc_data[1]);

    LOG_INFO("[END]         %02X %02X %02X %02X", buffer->end[0], buffer->end[1], buffer->end[2], buffer->end[3]);

    if (buffer->ctrl2.data_length > 0) {
        LOG_INFO("");
        LOG_INFO("[DATA] ");
        hex_dump(buffer->data, buffer->ctrl2.data_length);
    }
}

int checkBuffer(ARISR_CHUNK *interface, int i)
{
    // Check if version match
    if (interface->ctrl.version != ARISR_RAW_TEST_UNPACK[i-1].version) {
        LOG_ERROR("TEST %zu FAILED VERSION MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.version, ARISR_RAW_TEST_UNPACK[i-1].version);
        return -1;
    }

    // Check if destinations match
    if (interface->ctrl.destinations != ARISR_RAW_TEST_UNPACK[i-1].destinations) {
        LOG_ERROR("TEST %zu FAILED DESTINATIONS MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.destinations, ARISR_RAW_TEST_UNPACK[i-1].destinations);
        return -1;
    }

    // Check if option match
    if (interface->ctrl.option != ARISR_RAW_TEST_UNPACK[i-1].option) {
        LOG_ERROR("TEST %zu FAILED OPTION MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.option, ARISR_RAW_TEST_UNPACK[i-1].option);
        return -1;
    }

    // Check if from match
    if (interface->ctrl.from != ARISR_RAW_TEST_UNPACK[i-1].from) {
        LOG_ERROR("TEST %zu FAILED FROM MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.from, ARISR_RAW_TEST_UNPACK[i-1].from);
        return -1;
    }

    // Check if sequence match
    if (interface->ctrl.sequence != ARISR_RAW_TEST_UNPACK[i-1].sequence) {
        LOG_ERROR("TEST %zu FAILED SEQUENCE MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.sequence, ARISR_RAW_TEST_UNPACK[i-1].sequence);
        return -1;
    }

    // Check if retry match
    if (interface->ctrl.retry != ARISR_RAW_TEST_UNPACK[i-1].retry) {
        LOG_ERROR("TEST %zu FAILED RETRY MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.retry, ARISR_RAW_TEST_UNPACK[i-1].retry);
        return -1;
    }

    // Check if more_data match
    if (interface->ctrl.more_data != ARISR_RAW_TEST_UNPACK[i-1].more_data) {
        LOG_ERROR("TEST %zu FAILED MORE DATA MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.more_data, ARISR_RAW_TEST_UNPACK[i-1].more_data);
        return -1;
    }

    // Check if identifier match
    if (interface->ctrl.identifier != ARISR_RAW_TEST_UNPACK[i-1].identifier) {
        LOG_ERROR("TEST %zu FAILED IDENTIFIER MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.identifier, ARISR_RAW_TEST_UNPACK[i-1].identifier);
        return -1;
    }

    // Check if more_header match
    if (interface->ctrl.more_header != ARISR_RAW_TEST_UNPACK[i-1].more_header) {
        LOG_ERROR("TEST %zu FAILED MORE HEADER MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl.more_header, ARISR_RAW_TEST_UNPACK[i-1].more_header);
        return -1;
    }

    // Check if data_length match
    if (interface->ctrl2.data_length != ARISR_RAW_TEST_UNPACK[i-1].data_length) {
        LOG_ERROR("TEST %zu FAILED DATA LENGTH MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl2.data_length, ARISR_RAW_TEST_UNPACK[i-1].data_length);
        return -1;
    }

    // Check if feature match
    if (interface->ctrl2.feature != ARISR_RAW_TEST_UNPACK[i-1].feature) {
        LOG_ERROR("TEST %zu FAILED FEATURE MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl2.feature, ARISR_RAW_TEST_UNPACK[i-1].feature);
        return -1;
    }

    // Check if neg_answer match
    if (interface->ctrl2.neg_answer != ARISR_RAW_TEST_UNPACK[i-1].neg_answer) {
        LOG_ERROR("TEST %zu FAILED NEG ANSWER MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl2.neg_answer, ARISR_RAW_TEST_UNPACK[i-1].neg_answer);
        return -1;
    }

    // Check if freq_switch match
    if (interface->ctrl2.freq_switch != ARISR_RAW_TEST_UNPACK[i-1].freq_switch) {
        LOG_ERROR("TEST %zu FAILED FREQ SWITCH MISMATCH = %d AND EXPECTED = %d", i, interface->ctrl2.freq_switch, ARISR_RAW_TEST_UNPACK[i-1].freq_switch);
        return -1;
    }

    // Check if data match
    if (memcmp(interface->data, ARISR_RAW_TEST_UNPACK[i-1].data_plain, interface->ctrl2.data_length) != 0) {
        LOG_ERROR("TEST %zu FAILED DATA MISMATCH", i);
        return -1;
    }


    return 0;
}
// =================================================================================================

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    ARISR_UINT16 i;
    ARISR_ERR err;

    // Key
    ARISR_AES128_KEY key = ARISR_MSG_KEY;
    ARISR_UINT8 id[] = ARISR_ID;

    ARISR_CHUNK_RAW buffer;
    ARISR_CHUNK interface;
    ARISR_UINT8 *raw = NULL;
    ARISR_UINT32 raw_length;

    // =====================================

    LOG_INFO("--------------  TEST UNIT  ----------------");
    LOG_INFO("------   Start unpacking raw data   -------");
    LOG_INFO("-------------------------------------------");
    // 16 bytes key
    LOG_INFO("  > Using key:");
    hex_dump(key, ARISR_AES128_BLOCK_SIZE);
    LOG_INFO("-------------------------------------------");


    // ========== TEST RECV AND UNPACK ==============

    for (i = 1; i <= sizeof(ARISR_RAW_TEST_UNPACK) / sizeof(ARISR_RAW_TEST_UNPACK[0]); i++) {
        LOG_INFO("  > Test %zu:", i);
        // Test x - Receive and parse raw data case 1
        // DUMP
        hex_dump(ARISR_RAW_TEST_UNPACK[i-1].msg, ARISR_RAW_TEST_UNPACK[i-1].length);

        if ((err = ARISR_proto_recv(&buffer, ARISR_RAW_TEST_UNPACK[i-1].msg, key, id)) != ARISR_RAW_TEST_UNPACK[i-1].expected_recv) {
            LOG_ERROR("TEST %zu FAILED WITH ERROR = %d (%s) AND EXPECTED = %d", i, err, ARISR_ERR_NAMES[err], ARISR_RAW_TEST_UNPACK[i-1].expected_recv);
            return err;
        }

        LOG_INFO("-");
        LOG_INFO("[TEST %zu PASSED] Output = %d", i, err);
        LOG_INFO("-");
        printBufferRaw(&buffer);

        if (err == kARISR_OK) {
            //
            LOG_INFO("-------------------------------------------");
            LOG_INFO("  > Interface of Test %zu:", i);
            // Unpack the raw data into a structured ARISR_CHUNK
            if ((err = ARISR_proto_unpack(&interface, &buffer, key)) != ARISR_RAW_TEST_UNPACK[i-1].expected_unpack) {
                LOG_ERROR("TEST %zu FAILED UNPACKING WITH ERROR = %d (%s)", i, err, ARISR_ERR_NAMES[err]);
                return err;
            }
            // Clean up the raw buffer
            ARISR_proto_raw_chunk_clean(&buffer);

            if (err == kARISR_OK) {
                if (checkBuffer(&interface, i) != 0) {
                    return -1;
                }
            }

            LOG_INFO("-");
            LOG_INFO("[TEST %zu PASSED] Unpacking = %d", i, err);
            LOG_INFO("-");

            if (err == kARISR_OK) {
                // Print the interface buffer
                printBuffer(&interface);
            } else {
                LOG_INFO("-");
                LOG_INFO("TEST PASSED, BUT CONTENT CANNOT BE PARSED due to ERR = %d", err);
            }
        }

        // Clean up the buffer
        ARISR_proto_chunk_clean(&interface);
        LOG_INFO("-------------------------------------------");
        LOG_INFO("");
        LOG_INFO("-------------------------------------------");

    }

    LOG_INFO("--------------  TEST UNIT  ----------------");
    LOG_INFO("------   Testing with null key    ---------");
    LOG_INFO("-------------------------------------------");

    // Test with null key
    LOG_INFO("  > Test with null key:");
    hex_dump(ARISR_MSG_RAW_12, sizeof(ARISR_MSG_RAW_12));
    LOG_INFO("-------------------------------------------");
    if ((err = ARISR_proto_recv(&buffer, ARISR_MSG_RAW_12, NULL, id)) != kARISR_OK) {
        LOG_ERROR("TEST FAILED WITH ERROR = %d (%s) AND EXPECTED = %d", err, ARISR_ERR_NAMES[err] ,kARISR_OK);
        return err;
    }

    LOG_INFO("-");
    LOG_INFO("[TEST PASSED] Output = %d", err);
    LOG_INFO("-");
    printBufferRaw(&buffer);

    // Unpacking
    if ((err = ARISR_proto_unpack(&interface, &buffer, NULL)) != kARISR_OK) {
        LOG_ERROR("TEST FAILED UNPACKING WITH ERROR = %d (%s)", err, ARISR_ERR_NAMES[err]);
        return err;
    }
    LOG_INFO("-------------------------------------------");
    LOG_INFO("  > Interface of Test %zu:", i);
    printBuffer(&interface);

    // Clean up the buffer
    ARISR_proto_raw_chunk_clean(&buffer);
    ARISR_proto_chunk_clean(&interface);


    // =====================================

    LOG_INFO("--------------  TEST UNIT  ----------------");
    LOG_INFO("-------   Start packing raw data   --------");
    LOG_INFO("-------------------------------------------");
    // 16 bytes key
    LOG_INFO("  > Using key:");
    hex_dump(key, ARISR_AES128_BLOCK_SIZE);
    LOG_INFO("-------------------------------------------");

    // ========== TEST PACK AND SEND ==============
    for (i = 1; i <= sizeof(ARISR_RAW_TEST_PACK) / sizeof(ARISR_RAW_TEST_PACK[0]); i++) {
        LOG_INFO("  > Test %zu:", i);
        // Test x - Pack and send raw data case 1
        // DUMP
        printBuffer((ARISR_CHUNK *) ARISR_RAW_TEST_PACK[i-1].chunk);

        if ((err = ARISR_proto_pack(&buffer, (ARISR_CHUNK *) ARISR_RAW_TEST_PACK[i-1].chunk, key)) != kARISR_OK) {
            LOG_ERROR("TEST %zu FAILED WITH ERROR = %d (%s) AND EXPECTED = %d", i, err, ARISR_ERR_NAMES[err], kARISR_OK);
            return err;
        }
        LOG_INFO("-------------------------------------------");
        LOG_INFO("-");
        LOG_INFO("[TEST %zu PASSED] Output = %d (%s)", i, err, ARISR_ERR_NAMES[err]);
        LOG_INFO("-");
        printBufferRaw(&buffer);

        if (err == kARISR_OK) {
            //
            LOG_INFO("-------------------------------------------");
            LOG_INFO("  > Data of Test %zu:", i);

            if ((err = ARISR_proto_send(&raw, &buffer, &raw_length)) != kARISR_OK) {
                LOG_ERROR("TEST %zu FAILED IN RAW WITH ERROR = %d (%s)", i, err, ARISR_ERR_NAMES[err]);
                return err;
            }

            // Clean up the raw buffer
            ARISR_proto_raw_chunk_clean(&buffer);

            LOG_INFO("-");
            LOG_INFO("[TEST %zu PASSED] Output = %d", i, err);
            LOG_INFO("-");

            // DUMP
            hex_dump(raw, raw_length);
            LOG_INFO("RAW LENGTH = %d", raw_length);
            
            LOG_INFO("-");

            // Check if the raw data matches the expected
            if (raw_length != ARISR_RAW_TEST_PACK[i-1].expected_length || memcmp(raw, ARISR_RAW_TEST_PACK[i-1].expected_raw, raw_length) != 0) {
                LOG_ERROR("TEST %zu FAILED RAW MISMATCH", i);
                return -1;
            }

            LOG_INFO("TEST %zu PASSED RAW MATCH", i);

            LOG_INFO("-------------------------------------------");
            LOG_INFO("");
            LOG_INFO("-------------------------------------------");

            free(raw);
        }

    }


    // ========== TEST PARSE AND BUILD ==============
    LOG_INFO("--------------  TEST UNIT  ----------------");
    LOG_INFO("-------   Start parsing raw data   --------");
    LOG_INFO("-------------------------------------------");
    // 16 bytes key
    LOG_INFO("  > Using key:");
    hex_dump(key, ARISR_AES128_BLOCK_SIZE);
    LOG_INFO("-------------------------------------------");


    // ========== TEST RECV AND UNPACK ==============

    for (i = 1; i <= sizeof(ARISR_RAW_TEST_UNPACK) / sizeof(ARISR_RAW_TEST_UNPACK[0]); i++) {
        LOG_INFO("  > Test %zu:", i);
        // Test x - Receive and parse raw data case 1
        // DUMP
        hex_dump(ARISR_RAW_TEST_UNPACK[i-1].msg, ARISR_RAW_TEST_UNPACK[i-1].length);
        
        //
        LOG_INFO("  > Interface of Test %zu:", i);
        // Parse the raw data into a structured ARISR_CHUNK
        if ((err = ARISR_proto_parse(&interface, ARISR_RAW_TEST_UNPACK[i-1].msg, key, id)) != ARISR_RAW_TEST_UNPACK[i-1].expected_recv && err != ARISR_RAW_TEST_UNPACK[i-1].expected_unpack) {
            LOG_ERROR("TEST %zu FAILED PARSING WITH ERROR = %d (%s) AND EXPECTED = %d", i, err, ARISR_ERR_NAMES[err], ARISR_RAW_TEST_UNPACK[i-1].expected_recv);
            return err;
        }

        if (err == kARISR_OK) {
            if (checkBuffer(&interface, i) != 0) {
                return -1;
            }

            LOG_INFO("-");
            LOG_INFO("[TEST %zu PASSED] Parsing = %d", i, err);
            LOG_INFO("-");
    
            printBuffer(&interface);
        } else {
            LOG_INFO("-");
            LOG_INFO("TEST PASSED, BUT CONTENT CANNOT BE PARSED due to ERR = %d", err);
        }

        // Clean up the buffer
        ARISR_proto_chunk_clean(&interface);
        LOG_INFO("-------------------------------------------");
        LOG_INFO("");
        LOG_INFO("-------------------------------------------");

    }


    // =====================================

    LOG_INFO("--------------  TEST UNIT  ----------------");
    LOG_INFO("-------  Start building raw data   --------");
    LOG_INFO("-------------------------------------------");
    // 16 bytes key
    LOG_INFO("  > Using key:");
    hex_dump(key, ARISR_AES128_BLOCK_SIZE);
    LOG_INFO("-------------------------------------------");

    // ========== TEST PACK AND SEND ==============
    for (i = 1; i <= sizeof(ARISR_RAW_TEST_PACK) / sizeof(ARISR_RAW_TEST_PACK[0]); i++) {
        LOG_INFO("  > Test %zu:", i);
        // Test x - Pack and send raw data case 1
        // DUMP
        printBuffer((ARISR_CHUNK *) ARISR_RAW_TEST_PACK[i-1].chunk);

        if ((err = ARISR_proto_build(&raw, &raw_length, (ARISR_CHUNK *) ARISR_RAW_TEST_PACK[i-1].chunk, key)) != kARISR_OK) {
            LOG_ERROR("TEST %zu FAILED WITH ERROR = %d (%s) AND EXPECTED = %d", i, err, ARISR_ERR_NAMES[err], kARISR_OK);
            return err;
        }

        fprintf(stdout, "RAW LENGTH = %d\n", raw_length);
        fprintf(stdout, "RAW = ");
        hex_dump(raw, raw_length);

        fprintf(stdout, "EXPECTED LENGTH = %d\n", ARISR_RAW_TEST_PACK[i-1].expected_length);
        fprintf(stdout, "EXPECTED RAW = ");
        hex_dump(ARISR_RAW_TEST_PACK[i-1].expected_raw, ARISR_RAW_TEST_PACK[i-1].expected_length);
        // Check if the raw data matches the expected
        if (raw_length != ARISR_RAW_TEST_PACK[i-1].expected_length || memcmp(raw, ARISR_RAW_TEST_PACK[i-1].expected_raw, raw_length) != 0) {
            LOG_ERROR("TEST %zu FAILED RAW MISMATCH", i);
            return -1;
        }

        LOG_INFO("-------------------------------------------");
        LOG_INFO("-");
        LOG_INFO("[TEST %zu PASSED] Output = %d (%s)", i, err, ARISR_ERR_NAMES[err]);
        LOG_INFO("-");
        // DUMP
        hex_dump(raw, raw_length);
        LOG_INFO("-");
        LOG_INFO("RAW LENGTH = %d", raw_length);

        LOG_INFO("-------------------------------------------");
        LOG_INFO("");
        LOG_INFO("-------------------------------------------");

        free(raw);
    }

    return 0;
}

// COPYRIGHT 2025 - ARIS Alliance