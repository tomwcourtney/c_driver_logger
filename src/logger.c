/* put log code here */
#include "logger.h"
 
typedef struct {
    const char * id; 
    bool enabled;
    write_function write;
    logger_verbosity_t verbosity;
} logger_destination_t;

logger_destination_t destination = {0};
logger_verbosity_t global_verbosity = OFF;

void logger_init()
{
    // Set global verbosity to something other than off
    logger_set_global_verbosity(OFF);
    return;
}

void logger_log(logger_verbosity_t verbosity, const char *  message, ...)
{
    if(destination.enabled)
    {
        if(global_verbosity == OFF)
        {
            if (verbosity <= destination.verbosity)
            {
                destination.write(message);
            }
        }
        else{
            if(verbosity <= global_verbosity)
            {
                destination.write(message);
            }
        }
    }
}

uint32_t logger_get_output_count(void)
{
    return 0;
}

void logger_register_destination(write_function fn_ptr, logger_verbosity_t verbosity, bool enabled, const char * id)
{
    destination.id = id;
    destination.enabled = enabled;
    destination.write = fn_ptr;
    destination.verbosity = verbosity;
}

void logger_set_global_verbosity(logger_verbosity_t verbosity)
{
    global_verbosity = verbosity;
}

void logger_disable_dest(const char * id)
{
    if(!(strcmp(destination.id,id)))
    {
        destination.enabled = false;
    }
    return;
}

void logger_enable_dest(const char * id)
{
    if(!(strcmp(destination.id,id)))
    {
        destination.enabled = true;
    }
    return;
}