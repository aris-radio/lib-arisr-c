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
*/
#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "lib_arisr_interface.h"

/*
 * Definition of log levels.
 * You can adjust the minimum log level to display by modifying LOG_CURRENT_LEVEL.
 */
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

/*
 * Minimum log level to display messages.
 * Messages with a level lower than this will be ignored.
 * You can define LOG_CURRENT_LEVEL before including this header to customize it.
 */
#ifndef LOG_CURRENT_LEVEL
#define LOG_CURRENT_LEVEL LOG_LEVEL_DEBUG
#endif

/*
 * Internal function to get the string representation of a log level.
 */
static inline const char* log_level_str(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        default:              return "UNKNOWN";
    }
}

/*
 * Internal function to get the current date and time formatted as "YYYY-MM-DD HH:MM:SS".
 */
static inline void get_current_time(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *timenow = localtime(&now);
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", timenow);
}

/*
 * Main internal function that performs the logging.
 * It prints the date/time, log level, the file, and the line number where the log was called,
 * along with the formatted log message.
 */
static inline void logger(LogLevel level, const char *fmt, ...) {
    if (level < LOG_CURRENT_LEVEL) {
        return;
    }

    char time_buffer[20];
    get_current_time(time_buffer, sizeof(time_buffer));

    // Print the log header with time, level, file, and line information
    fprintf(stderr, "[%s] [%s] ", time_buffer, log_level_str(level));

    // Print the formatted message
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
}

/*
 * Macros to simplify logging in your code.
 */
#define LOG_DEBUG(fmt, ...) logger(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  logger(LOG_LEVEL_INFO,  fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  logger(LOG_LEVEL_WARN,  fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

/*
 * ---------------------------------------------------------------------------
 * Logger-Integrated Hex Dump
 * ---------------------------------------------------------------------------
 *
 * This function logs a hex dump of arbitrary data using the same logger.
 * It prints a decorative header line and then outputs the data in rows,
 * each row displaying the offset, hexadecimal values in a fixed number of columns,
 * and the corresponding ASCII representation.
 *
 * Parameters:
 *   - data: pointer to the data to dump.
 *   - length: number of bytes in the data.
 *   - columns: number of bytes to show per row (e.g., 16).
 */
static inline void hex_dump(const ARISR_UINT8 *data, size_t length) {
    int columns = 12;
    for (size_t i = 0; i < length; i += columns) {
        char lineBuffer[512];
        int pos = 0;
        pos += snprintf(lineBuffer + pos, sizeof(lineBuffer) - pos, "%04zx: ", i);
        for (int j = 0; j < columns; j++) {
            if (i + j < length) {
                pos += snprintf(lineBuffer + pos, sizeof(lineBuffer) - pos, "%02X ", data[i + j]);
            } else {
                pos += snprintf(lineBuffer + pos, sizeof(lineBuffer) - pos, "   ");
            }
        }
        pos += snprintf(lineBuffer + pos, sizeof(lineBuffer) - pos, " |");
        for (int j = 0; j < columns; j++) {
            if (i + j < length) {
                unsigned char byte = data[i + j];
                char c = (byte >= 32 && byte <= 126) ? byte : '.';
                pos += snprintf(lineBuffer + pos, sizeof(lineBuffer) - pos, "%c", c);
            } else {
                pos += snprintf(lineBuffer + pos, sizeof(lineBuffer) - pos, " ");
            }
        }
        pos += snprintf(lineBuffer + pos, sizeof(lineBuffer) - pos, "|");
        LOG_INFO("%s", lineBuffer);
    }
}


#endif /* SIMPLE_LOGGER_H */

// COPYRIGHT ARIS Alliance