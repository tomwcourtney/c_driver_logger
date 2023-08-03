#ifndef LOGGER_GUARD_H
#define LOGGER_GUARD_H

#include <stdint.h>


typedef enum{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
}logger_verbosity_t;

typedef void (*write_function)(const char *);

void logger_init();
uint32_t logger_get_output_count();
void logger_log(logger_verbosity_t verbosity, const char *  message, ...);
void logger_register_destination(write_function);

#endif