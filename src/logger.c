/* put log code here */
#include "logger.h"

typedef struct{
    write_function write;
}logger_destination_t;


logger_destination_t destination = {0};

void logger_init()
{
    
    return;
}

void logger_log(logger_verbosity_t verbosity, const char *  message, ...)
{
    destination.write(message);
}

uint32_t logger_get_output_count(void)
{
    return 0;
}

void logger_register_destination(write_function fn_ptr)
{
    destination.write = fn_ptr;
}