/* put log code here */
#include "logger.h"
 
#define MAX_DESTINATIONS 2
uint8_t destinations_head = 0;


typedef struct {
    const char * id; 
    bool enabled;
    write_function write;
    logger_verbosity_t verbosity;
} logger_destination_t;

logger_destination_t destinations[MAX_DESTINATIONS] = {0};

logger_verbosity_t global_verbosity = OFF;

void logger_init()
{
    // Set global verbosity to something other than off
    destinations_head = 0;
    logger_set_global_verbosity(OFF);
    return;
}

void logger_log(logger_verbosity_t verbosity, const char *  message, ...)
{
    for(uint8_t i = 0; i < destinations_head; i++)
    {
        if(destinations[i].enabled)
        {
            if(global_verbosity == OFF)
            {
                if (verbosity <= destinations[i].verbosity)
                {
                    destinations[i].write(message);
                }
            }
            else{
                if(verbosity <= global_verbosity)
                {
                    destinations[i].write(message);
                }
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
    destinations[destinations_head].id = id;
    destinations[destinations_head].enabled = enabled;
    destinations[destinations_head].write = fn_ptr;
    destinations[destinations_head].verbosity = verbosity;
    destinations_head++;
}

void logger_set_global_verbosity(logger_verbosity_t verbosity)
{
    global_verbosity = verbosity;
}

void logger_disable_dest(const char * id)
{
    for(uint8_t i = 0 ; i < destinations_head; i++)
    {
        if(!(strcmp(destinations[i].id,id)))
        {
            destinations[i].enabled = false;
        }
    }
    return;
}

void logger_enable_dest(const char * id)
{
    for(uint8_t i = 0 ; i < destinations_head; i++)
    {
        if(!(strcmp(destinations[i].id,id)))
        {
            destinations[i].enabled = true;
        }
    }
    
    return;
}

void logger_disable_all(void)
{
    for(uint8_t i = 0 ; i < destinations_head; i++)
    {
        destinations[i].enabled = false;
    }
}

void logger_enable_all(void)
{
    for(uint8_t i = 0 ; i < destinations_head; i++)
    {
        destinations[i].enabled = true;
    }
}