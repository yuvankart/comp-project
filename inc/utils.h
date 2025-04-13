#ifndef UTILS_H
#define UTILS_H

#define LOG_ERROR(msg, line) \
    fprintf(stderr, "Error at line %d: %s\n", line, msg)

#endif
