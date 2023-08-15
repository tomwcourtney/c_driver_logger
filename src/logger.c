/* put log code here */
#include "logger.h"
#include <stdio.h>

#define MAX_DESTINATIONS 2
uint8_t destinations_head = 0;

#define TIMESTAMP_MAX_SIZE 50

typedef struct {
    const char * id; 
    bool enabled;
    write_function write;
    logger_verbosity_t verbosity;
} logger_destination_t;

logger_destination_t destinations[MAX_DESTINATIONS] = {0};

logger_verbosity_t global_verbosity = OFF;

// This function is where the logger gets time from
get_time_function get_time = NULL;

/**
 * @brief Checks if a destination with the same ID already exists in the registered destination list
 *
 * @param [in] id - ID to be searched for
 *
 * @return True if ID exists, false otherwise
 */
static bool logger_destination_exists(const char * id);

void logger_init(get_time_function fn_ptr)
{
    // Set global verbosity to something other than off
    destinations_head = 0;
    logger_set_global_verbosity(OFF);
    get_time = fn_ptr;
    return;
}

void logger_log(logger_verbosity_t verbosity, const char *  message, ...)
{
    char logged_message[MAX_LOG_SIZE] = {0};

    // If the logger has a get time function
    if(get_time != NULL)
    {
        char timestamp[TIMESTAMP_MAX_SIZE] = {0};
        get_time(timestamp);
        sprintf(logged_message, "[%s] ", timestamp);    
   
    }

    strncat(logged_message, message, MAX_LOG_SIZE);

    for(uint8_t i = 0; i < destinations_head; i++)
    {
        if(destinations[i].enabled)
        {
            if(global_verbosity == OFF)
            {
                if (verbosity <= destinations[i].verbosity)
                {
                    destinations[i].write(logged_message);
                }
            }
            else{
                if(verbosity <= global_verbosity)
                {
                    destinations[i].write(logged_message);
                }
            }
        }
    }
}

uint32_t logger_get_output_count(void)
{
    return destinations_head;
}

void logger_register_destination(write_function fn_ptr, logger_verbosity_t verbosity, bool enabled, const char * id)
{
    if (destinations_head >= MAX_DESTINATIONS)
    {
        return;
    }

    if (logger_destination_exists(id))
    {
        return;
    }

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

static bool logger_destination_exists(const char * id)
{
    bool found = false;

    for (int i = 0; i < destinations_head; i++)
    {
        if (strcmp(id, destinations[i].id) == 0)
        {
            found = true;
        }
    }
    return found;
}