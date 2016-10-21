#ifndef MOD_IO_MAIN_H
#define MOD_IO_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "i2c.h"
#include "logger.h"

#define CHECK_BIT(var, pos) ((var) & (1 << (pos - 1)))

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
