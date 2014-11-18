/*
 * Copyright ANPR Systems B.V.
 * This file is part of ANPR Systems PMS. You should have received a copy of the
 * ANPR Systems PMS license along with the ANPR Systems PMS. See the file ./License_ANPR Systems.txt
 * 
 * If you have any questions please contact us at info@anpr-systems.nl
 * 
 * @version:    $Id$
 * @package:    ANPR PMS Olimex MOD-IO Board UEXT based
 * @copyright:  Copyright ANPR Systems B.V.
 * @author:     Pieter van de Ven <pieter@anpr-systems.nl>
 * @license:    http://www.anpr-systems.nl/licenses/ktg_anpr
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include "i2c.h"
#define CHECK_BIT(var, pos) ((var) & (1 << (pos - 1)))

void error(const char *msg) {
    perror(msg);
    exit(-1);
}

char** str_split(char* a_str, const char a_delim) {
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof (char*) * count);

    if (result) {
        size_t idx = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;

}

/**
 *
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char **argv) {
    // declare vars
    int sockfd, newsockfd, portno, file = 0, oo = 0, temp = 0;
    socklen_t clilen;
    char buffer[256], str_out[512], tmp_out[128];
    char* success = "false";
    char** input;
    int** ov;
    float vcc;
    unsigned char bufferd[2], data[2], address;
    struct sockaddr_in serv_addr, cli_addr;
    int n, i;

    if (argc < 2) {
        printf("Too few arguments.\n Type -help.\n");
        exit(-1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR Opening socket");
    }
    // set default values
    bzero((char *) &serv_addr, sizeof (serv_addr));
    // set portnumber from arguments
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    // bind socket to address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        error("ERROR on binding");
    }

    while (1 == 1) {
        //listen to the socket
        listen(sockfd, 5);
        clilen = sizeof (cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
        }
        bzero(buffer, 256);
        bzero(str_out, 512);
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            error("ERROR reading from socket");
        }
        // input string is build as <<command>>;<<address>>;<<value>>
        // output string is build as {"Answer":{<<command>>:{<<value>>},{"success": {"true" OR "false"}}}}
        input = str_split(buffer, ';');
        strncpy(str_out, "{\"Answer\":{", sizeof (str_out));
        address = strtol(input[1], NULL, 0);
        sprintf(tmp_out, "\"%s\":", input[0]);
        strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));
        success = "true";
        
        I2C_Open(&file, address);
        if (file > 0) {
            if (!strcmp(input[0], "SO")) {
                oo = ov[address];
                bufferd[0] = 0x10;
                if (strtol(input[2], NULL, 0) == 1 && !CHECK_BIT(oo, 1)) {
                    bufferd[1] = oo + 1;
                }
                if (strtol(input[2], NULL, 0) == 2 && !CHECK_BIT(oo, 2)) {
                    bufferd[1] = oo + 2;
                }
                if (strtol(input[2], NULL, 0) == 3 && !CHECK_BIT(oo, 3)) {
                    bufferd[1] = oo + 4;
                }
                if (strtol(input[2], NULL, 0) == 4 && !CHECK_BIT(oo, 4)) {
                    bufferd[1] = oo + 8;
                }
                oo = bufferd[1];
                ov[address] = oo;
                sprintf(tmp_out, "%d", oo);
                strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));
                I2C_Send(&file, bufferd, 2);
            }
            if (!strcmp(input[0], "RO")) {
                bufferd[0] = 0x10;
                oo = ov[address];
                bufferd[1] = oo;
                if (strtol(input[2], NULL, 0) == 1 && CHECK_BIT(oo, 1)) {
                    bufferd[1] = oo - 1;
                }
                if (strtol(input[2], NULL, 0) == 2 && CHECK_BIT(oo, 2)) {
                    bufferd[1] = oo - 2;
                }
                if (strtol(input[2], NULL, 0) == 3 && CHECK_BIT(oo, 3)) {
                    bufferd[1] = oo - 4;
                }
                if (strtol(input[2], NULL, 0) == 4 && CHECK_BIT(oo, 4)) {
                    bufferd[1] = oo - 8;
                }
                oo = bufferd[1];
                ov[address] = oo;
                sprintf(tmp_out, "%d", oo);
                strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));
                I2C_Send(&file, bufferd, 2);
            }
            if (!strcmp(input[0], "DI")) {
                bufferd[0] = 0x20;
                I2C_Send(&file, bufferd, 1);
                I2C_Read(&file, data, 1);
                strncat(str_out, "{\"Input\":{", sizeof (str_out) - strlen(str_out));
                for (i = 0; i < 4; i++) {
                    sprintf(tmp_out, "\"%d\":%d", i + 1, (data[0] >> i) & 0x01);
                    strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));
                    if (i < 3) {
                        strncat(str_out, ",", sizeof (str_out) - strlen(str_out));
                    } else {
                        strncat(str_out, "}}", sizeof (str_out) - strlen(str_out));
                    }
                }
            }
            if (!strcmp(input[0], "AI")) {
                strncat(str_out, "{\"Analoginput\":{", sizeof (str_out) - strlen(str_out));
                bufferd[0] = 0x30;
                I2C_Send(&file, bufferd, 1);
                I2C_Read(&file, data, 2);
                for (i = 0; i < 8; i++) {
                    temp |= ((data[0] & 0x80) ? 1 : 0) << i;
                    data[0] <<= 1;
                }
                temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
                temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
                vcc = (3.3 * temp) / 1023;
                sprintf(tmp_out, "\"1\":%.3f,", vcc);
                strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));
                // analog input 2
                bufferd[0] = 0x31;
                I2C_Send(&file, bufferd, 1);
                I2C_Read(&file, data, 2);
                for (i = 0; i < 8; i++) {
                    temp |= ((data[0] & 0x80) ? 1 : 0) << i;
                    data[0] <<= 1;
                }
                temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
                temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
                vcc = (3.3 * temp) / 1023;
                sprintf(tmp_out, "\"2\":%.3f,", vcc);
                strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));
                // analog input 3
                bufferd[0] = 0x32;
                I2C_Send(&file, bufferd, 1);
                I2C_Read(&file, data, 2);
                for (i = 0; i < 8; i++) {
                    temp |= ((data[0] & 0x80) ? 1 : 0) << i;
                    data[0] <<= 1;
                }
                temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
                temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
                vcc = (3.3 * temp) / 1023;
                sprintf(tmp_out, "\"3\":%.3f,", vcc);
                strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));
                // analog input 4
                bufferd[0] = 0x33;
                I2C_Send(&file, bufferd, 1);
                I2C_Read(&file, data, 2);
                for (i = 0; i < 8; i++) {
                    temp |= ((data[0] & 0x80) ? 1 : 0) << i;
                    data[0] <<= 1;
                }
                temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
                temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
                vcc = (3.3 * temp) / 1023;
                sprintf(tmp_out, "\"4\":%.3f}}", vcc);
                strncat(str_out, tmp_out, sizeof (str_out) - strlen(str_out));

            }
            I2C_Close(&file);
        } else {
            success = "false";
        }
        strncat(str_out, ",\"success\":", sizeof (str_out) - strlen(str_out));
        strncat(str_out, success, sizeof (str_out) - strlen(str_out));
        strncat(str_out, "}}", sizeof (str_out) - strlen(str_out));

        n = write(newsockfd, str_out, sizeof (str_out));
    }
    return 0;
}