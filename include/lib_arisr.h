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

/**
 * @brief Get the field of the control section
 * @param ctrl
 * @param offset
 * @param mask
 * @return ARISR_UINT8
 */
ARISR_UINT8 ARISR_proto_ctrl_getField(void *ctrl, ARISR_UINT8 offset, ARISR_UINT8 mask);

/**
 * @brief Receive the ARISr protocol
 * @param buffer
 * @param data
 * @return ARISR_ERR
 */
ARISR_ERR ARISR_proto_recv(ARISR_CHUNK_RAW *buffer, char *data);

#endif


/* COPYRIGHT ARIS Alliance */