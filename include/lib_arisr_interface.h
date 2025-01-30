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
 * @file lib_arisr_interface.h
 * @brief This file contains the interface of the ARISr protocol.
 * @date 2025-01-30
 * @authors ARIS Alliance
*/

#ifndef LIB_ARISR_INTERFACE_H
#define LIB_ARISR_INTERFACE_H

#include <stdint.h>
#include "lib_arisr_base.h"


/*

    ARIS Radio protocol interface

    Protocol interface: ../docs/ARISRadioProtocol.pdf
*/

#define ARISR_PROTO_CRYPT_SIZE      8
#define ARISR_PROTO_ID_SIZE         4

#define ARISR_CTRL_SECTION_SIZE     4
#define ARISR_CTRL2_SECTION_SIZE    4

#define ARISR_ADDRESS_SIZE          6

#define ARISR_CRC_SIZE              2

/* CTRL 1*/

#define ARISR_CTRL_VERSION_MASK     0xF
#define ARISR_CTRL_DESTS_MASK       0xFF
#define ARISR_CTRL_OPTION_MASK      0x7
#define ARISR_CTRL_FROM_MASK        0x1
#define ARISR_CTRL_SEQUENCE_MASK    0x3F
#define ARISR_CTRL_RETRY_MASK       0x1
#define ARISR_CTRL_MD_MASK          0x1
#define ARISR_CTRL_ID_MASK          0x7F
#define ARISR_CTRL_MH_MASK          0x1 

#define ARISR_CTRL_VERSION_BITS    4
#define ARISR_CTRL_DESTS_BITS      8
#define ARISR_CTRL_OPTION_BITS     3
#define ARISR_CTRL_FROM_BITS       1
#define ARISR_CTRL_SEQUENCE_BITS   6
#define ARISR_CTRL_RETRY_BITS      1
#define ARISR_CTRL_MD_BITS         1
#define ARISR_CTRL_ID_BITS         7
#define ARISR_CTRL_MH_BITS         1

#define ARISR_CTRL_VERSION_OFFSET    0
#define ARISR_CTRL_DESTS_OFFSET      4
#define ARISR_CTRL_OPTION_OFFSET     12
#define ARISR_CTRL_FROM_OFFSET       15
#define ARISR_CTRL_SEQUENCE_OFFSET   16
#define ARISR_CTRL_RETRY_OFFSET      22
#define ARISR_CTRL_MD_OFFSET         23
#define ARISR_CTRL_ID_OFFSET         24
#define ARISR_CTRL_MH_OFFSET         31

/* CTRL 2*/

#define ARISR_CTRL2_DATA_LENGTH_MASK    0xFF
#define ARISR_CTRL2_FEATURE_MASK        0x1
#define ARISR_CTRL2_NEG_ANSWER_MASK     0x1
#define ARISR_CTRL2_FREQ_SWITCH_MASK    0x1

#define ARISR_CTRL2_DATA_LENGTH_BITS    8
#define ARISR_CTRL2_FEATURE_BITS        1
#define ARISR_CTRL2_NEG_ANSWER_BITS     1
#define ARISR_CTRL2_FREQ_SWITCH_BITS    1

#define ARISR_CTRL2_DATA_LENGTH_OFFSET    0
#define ARISR_CTRL2_FEATURE_OFFSET        8
#define ARISR_CTRL2_NEG_ANSWER_OFFSET     9
#define ARISR_CTRL2_FREQ_SWITCH_OFFSET    10

#pragma pack(1)
typedef struct {
    ARISR_UINT32 version      : ARISR_CTRL_VERSION_BITS;       // 4 Bits
    ARISR_UINT32 destinations : ARISR_CTRL_DESTS_BITS;         // 8 Bits
    ARISR_UINT32 option       : ARISR_CTRL_OPTION_BITS;        // 3 Bits
    ARISR_UINT32 from         : ARISR_CTRL_FROM_BITS;          // 1 Bit
    ARISR_UINT32 sequence     : ARISR_CTRL_SEQUENCE_BITS;      // 6 Bits
    ARISR_UINT32 retry        : ARISR_CTRL_RETRY_BITS;         // 1 Bit
    ARISR_UINT32 more_data    : ARISR_CTRL_MD_BITS;            // 1 Bit
    ARISR_UINT32 identifier   : ARISR_CTRL_ID_BITS;            // 7 Bits
    ARISR_UINT32 more_header  : ARISR_CTRL_MH_BITS;            // 1 Bit
} ARISR_CHUNK_CTRL_RAW;
#pragma pack()

#pragma pack(1)
typedef struct {
    ARISR_UINT32 data_length : 8;   // 8 Bits
    ARISR_UINT32 feature : 1;       // 1 Bits
    ARISR_UINT32 neg_answer : 1;    // 1 Bits
    ARISR_UINT32 freq_switch : 1;   // 1 Bits
    ARISR_UINT32 _blank: 21;        // 21 Bits (Not used, for future version or private use)
} ARISR_CHUNK_CTRL2_RAW;
#pragma pack()


typedef struct {
    ARISR_UINT8 id[4];                                  // 4 Bytes
    ARISR_UINT8 aris[4];                                // 4 Bytes
    ARISR_CHUNK_CTRL_RAW *ctrl;                         // 4 Bytes
    ARISR_UINT8 origin[ARISR_ADDRESS_SIZE];             // 6 Bytes
    ARISR_UINT8 destinationA[ARISR_ADDRESS_SIZE];       // 6 Bytes
    ARISR_UINT8 (*destinationsB)[ARISR_ADDRESS_SIZE];   // ... n*6 Bytes
    ARISR_UINT8 destinationC[ARISR_ADDRESS_SIZE];       // 6 Bytes
    ARISR_CHUNK_CTRL2_RAW *ctrl2;                       // 4 Bytes
    ARISR_UINT8 crc_header[2];                          // 2 Bytes
    ARISR_UINT8 *data;                                  // n Bytes
    ARISR_UINT8 crc_data[2];                            // 2 Bytes
    ARISR_UINT8 end[4];                                 // 4 Bytes
} ARISR_CHUNK_RAW;



#endif


/* COPYRIGHT ARIS Alliance */