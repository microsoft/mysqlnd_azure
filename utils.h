#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern FILE *logfile;
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

#define OPEN_LOGFILE(filename) \
  do {\
    if (filename != NULL) {\
      logfile = fopen(filename, "a"); \
    }\
  } while (0)

#define CLOSE_LOGFILE() \
  do {\
    if (logfile != NULL) { \
      fclose(logfile);\
    } } while (0)

#define AZURE_LOG(level, format, ...) \
  do { \
    if (logfile != NULL) { \
      time_t now = time(NULL); \
      char timestr[20];\
      strftime(timestr, 20, TIME_FORMAT, localtime(&now)); \
      fprintf(logfile, " %s [" level "] " format "\n", timestr, \
                              ## __VA_ARGS__);        \
      fflush(logfile); \
    } \
  } while (0)

#endif // UTILS_H
