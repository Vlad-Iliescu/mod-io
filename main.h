#ifndef MOD_IO_MAIN_H
#define MOD_IO_MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include "i2c.h"

#define CHECK_BIT(var, pos) ((var) & (1 << (pos - 1)))

/**
 * Write errors and information to logfile
 * @param msg -> string to be written
 * @param debug -> debug mode only
 */
void error_log(char *, bool debug);

/**
 * Write errors and information to logfile and stop the execution
 * @param msg -> string to be written
 */
void error(const char *msg);

/**
 *Listen to the socket for messages
 * @param newsockfd
 */
void readio(int newsockfd);

#endif //MOD_IO_MAIN_H
