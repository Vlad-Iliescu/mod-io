#include "logger.h"

log_strategy logging = LOG_TO_CONSOLE;

void log(const char *msg, bool debug) {
    if (logging == NO_LOGGING) {
        // logging disabled
        return;
    }

    char tmp_str[100];
    time_t cur_time = time(NULL);
    strftime(tmp_str, 100, "%Y-%m-%d %H:%M:%S", localtime(&cur_time));

    if (logging == LOG_TO_FILE) {
        char log_file[255];
        struct stat st = {0};

        if (stat(LOG_PATH, &st) == -1) {
            mkdir(LOG_PATH, 0755);
        }

        sprintf(log_file, "%s/%s", LOG_PATH, LOG_FILE);

        if (debug) {
            // create vars needed for error_handling
            FILE *err_log = fopen(log_file, "a");

            if (err_log == NULL) {
                puts("Error opening logfile");
                return;
            }
            // format error message and write it to the logfile

            fprintf(err_log, "%s -> %s\n", tmp_str, msg);
            fclose(err_log);
            err_log = NULL;
        }
    } else {
        if (debug) {
            fprintf(stderr, "%s -> %s\n", tmp_str, msg);
        }
    }
    return;
}