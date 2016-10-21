#ifndef MOD_IO_LOGGER_H
#define MOD_IO_LOGGER_H

#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#define LOG_PATH "logs"
#define LOG_FILE "modio.log"

typedef enum {
    LOG_TO_FILE,
    LOG_TO_CONSOLE,
    NO_LOGGING
} log_strategy;

/**
 * Write errors and information to logfile
 * @param msg -> string to be written
 * @param debug -> debug mode only
 */
void log(const char *msg, bool debug);

#endif //MOD_IO_LOGGER_H
