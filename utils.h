#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "php_mysqlnd_azure.h"

extern FILE *logfile;
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

// Azure Log Levels, 1(ERROR), 2(INFO), 3(DEBUG)
#define ALOGERR  1
#define ALOGINFO 2
#define ALOGDBG  3

#define OPEN_LOGFILE(filename)                                                \
  do {                                                                        \
    if (filename != NULL) {                                                   \
      logfile = fopen(filename, "a");                                         \
    }                                                                         \
  } while (0)

#define CLOSE_LOGFILE()                                                       \
  do {                                                                        \
    if (logfile != NULL) {                                                    \
      fclose(logfile);                                                        \
    } } while (0)

#define AZURE_LOG(level, format, ...)                                          \
  do {                                                                         \
      if (MYSQLND_AZURE_G(logOutput) && level <= MYSQLND_AZURE_G(logLevel)) {  \
        time_t now = time(NULL);                                               \
        char timestr[20];                                                      \
        char *levelstr = level == 1 ? "ERROR" : level == 2 ? "INFO " : "DEBUG";\
        strftime(timestr, 20, TIME_FORMAT, localtime(&now));                   \
        if (MYSQLND_AZURE_G(logOutput) == 1) {                                 \
          printf("%s [%s] " format "\n", timestr, levelstr, ## __VA_ARGS__);   \
        } else if (MYSQLND_AZURE_G(logOutput) == 2) {                          \
          fprintf(logfile, "%s [%s] " format "\n", timestr, levelstr,          \
                                  ## __VA_ARGS__);                             \
          fflush(logfile);                                                     \
        }                                                                      \
      }                                                                        \
  } while (0)

#define AZURE_LOG_SYS(format, ...)                                             \
  do {                                                                         \
      if (MYSQLND_AZURE_G(logOutput)) {                                        \
      time_t now = time(NULL);                                                 \
      char timestr[20];                                                        \
      strftime(timestr, 20, TIME_FORMAT, localtime(&now));                     \
      if (MYSQLND_AZURE_G(logOutput) == 1) {                                   \
        printf("%s [SYSTM] " format "\n", timestr,  ## __VA_ARGS__);          \
      } else if (MYSQLND_AZURE_G(logOutput) == 2) {                            \
        fprintf(logfile, "%s [SYSTM] " format "\n", timestr,                  \
                                ## __VA_ARGS__);                               \
        fflush(logfile);                                                       \
      }                                                                        \
    }                                                                          \
  } while (0)


#endif // UTILS_H
