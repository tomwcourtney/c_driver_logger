/* put log code here */
#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

#define MAX_DESTINATIONS 2
uint8_t destinations_head = 0;

#define TIMESTAMP_MAX_SIZE 50
#define ID_MAX_SIZE 50
#define VERB_TAG_MAX_SIZE 5

/** 
 * @brief Structure for a destination to send your logs to.
 *
 * @param id        - Unique string identifier of the  
 * @param enabled   - True if destination should log
 * @param write     - Function that the destination writes out to when logging
 * @param severity  - The minimum severity level message that the destination will log
 * @param colour    - True if message should log with ansi colour codes. Colour corresponds to severity.
*/
typedef struct {
    char id[ID_MAX_SIZE];
    bool enabled;
    write_function write;
    logger_verbosity_t verbosity;
    bool colour;
} logger_destination_t;



// These tags correspond to the severity of a log. 
// They're the first character of each severity.
const char * severity_tags[] = 
{
    "O",
    "E",
    "W",
    "I",
    "D"
};

// These are the ansi codes for terminal colours
const char * colours[] = 
{
    "off",      // Place holder for off    
    "\x1b[31m", // Red      - Error
    "\x1b[33m", // Yellow   - Warning
    "\x1b[32m", // Green    - Info
    "\x1b[0m",  // Off      - Debug 
    "\x1b[34m", // Blue     - Not used
    "\x1b[35m", // Magenta  - Not used
    "\x1b[36m"  // Cyan     - Not used
};

logger_destination_t destinations[MAX_DESTINATIONS] = {0};

logger_verbosity_t global_verbosity = LOGGER_OFF;
bool global_timestamping = true;
bool global_verbosity_prepend = true;

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
    logger_set_global_verbosity(LOGGER_OFF);
    get_time = fn_ptr;
    global_timestamping = true;
    return;
}

void logger_log(logger_verbosity_t verbosity, const char *  message, ...)
{
    char formatted_message[MAX_LOG_SIZE+1] = {0};
    char logged_message[MAX_LOG_SIZE+1] = {0};
    char logged_message_colour[MAX_LOG_SIZE+1] = {0};
    
    // unpack args and build formatted string
    va_list args;
    va_start(args, message);
    vsnprintf(formatted_message, MAX_LOG_SIZE, message, args);
    va_end(args);

    // Add the colour
    snprintf(logged_message_colour, MAX_LOG_SIZE, "%s", colours[verbosity]); 

    // If the logger has a get time function
    if(get_time != NULL && global_timestamping)
    {
        char timestamp[TIMESTAMP_MAX_SIZE+1] = {0};
        char timestamp_brackets[TIMESTAMP_MAX_SIZE+1+3] = {0};
        get_time(timestamp);
        snprintf(logged_message, MAX_LOG_SIZE, "[%s] ", timestamp);    
        snprintf(timestamp_brackets, TIMESTAMP_MAX_SIZE+1+3, "[%s] ", timestamp);
        strncat(logged_message_colour, timestamp_brackets, MAX_LOG_SIZE);
    }

    // If the logger versobity tag is turned on
    if(global_verbosity_prepend)
    {
        char severity_tag[VERB_TAG_MAX_SIZE] = {0};
        snprintf(severity_tag, VERB_TAG_MAX_SIZE, "[%s] ", severity_tags[verbosity]);   
        strncat(logged_message, severity_tag, MAX_LOG_SIZE);
        strncat(logged_message_colour, severity_tag, MAX_LOG_SIZE);
    }

    strncat(logged_message, formatted_message, MAX_LOG_SIZE);
    strncat(logged_message_colour, formatted_message, MAX_LOG_SIZE);

    for(uint8_t i = 0; i < destinations_head; i++)
    {
        if(destinations[i].enabled)
        {
            if(global_verbosity == LOGGER_OFF)
            {
                if (verbosity <= destinations[i].verbosity)
                {
                    if(destinations[i].colour){
                        destinations[i].write(logged_message_colour);
                    }else{
                        destinations[i].write(logged_message);
                    }
                }
            }
            else{
                if(verbosity <= global_verbosity)
                {
                    if(destinations[i].colour)
                        destinations[i].write(logged_message_colour);
                    else
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

    strncpy(destinations[destinations_head].id, id, ID_MAX_SIZE);
    destinations[destinations_head].enabled = enabled;
    destinations[destinations_head].write = fn_ptr;
    destinations[destinations_head].verbosity = verbosity;
    destinations[destinations_head].colour = false;
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

void logger_set_global_timestamping(bool timestamping)
{
    global_timestamping = timestamping;
}

void logger_set_global_verbosity_prepend(bool verbprepend)
{
    global_verbosity_prepend = verbprepend;
}

void logger_set_dest_colour(const char *id, bool b_isColourOn)
{
    for(uint8_t i = 0 ; i < destinations_head; i++)
    {
        if(!(strcmp(destinations[i].id,id)))
        {
            destinations[i].colour = b_isColourOn;
        }
    }

    return;
}