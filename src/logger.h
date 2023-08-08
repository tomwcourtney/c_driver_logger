#ifndef LOGGER_GUARD_H
#define LOGGER_GUARD_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum{
    OFF,
    ERROR,   //Lowest 
    WARNING,
    INFO,
    DEBUG,
}logger_verbosity_t;

typedef void (*write_function)(const char *);

void logger_init();
uint32_t logger_get_output_count();
void logger_log(logger_verbosity_t verbosity, const char *  message, ...);
void logger_register_destination(write_function fn_ptr, logger_verbosity_t verbosity, bool enabled, const char * id);
void logger_set_global_verbosity(logger_verbosity_t);
void logger_disable_dest(const char * id);
void logger_enable_dest(const char * id);
#endif