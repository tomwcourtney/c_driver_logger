/**
 * @file logger.h
 * @author Pat, tom and dean 
 * @brief logger.h is a general use logger created for use in C embedded projects. 
 */

#ifndef LOGGER_GUARD_H
#define LOGGER_GUARD_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LOG_SIZE 1024

/**
 * @brief Typedef'd enum representing verbosity levels, in increasing order of verbosity from least verbose (ERROR, only logs ERRORs) to most verbose (DEBUG, will log all). When a destination is configured with a certain verbosity,
 * messages logged with that verbosity or LOWER will be sent to the destination. e.g. a destination with verbosity INFO will receive logs logged with verbosity INFO, WARNING and ERROR, but not DEBUG.
*/
typedef enum{
    OFF,
    ERROR,   //Lowest 
    WARNING,
    INFO,
    DEBUG,
}logger_verbosity_t;

/**
 * @brief Definition of our function pointer type for the write funtions.
*/
typedef void (*write_function)(const char *);

/**
 * @brief Writes a timestamp over the char array passed in as parameter.
*/
typedef void (*get_time_function)(char *);

/**
 * @brief Zeros destination head counter, sets the global verbosity level to off 
 * and does all house keeping needed before defining destiatnions and logging messages
 * 
 * @param [in] fn_ptr - The function that is to be used for retrieving log timestamp.
 */
void logger_init(get_time_function fn_ptr);

/**
 * @brief Returns the amount of registered destions
 * 
 * @return The current total number of destinations 
 */
uint32_t logger_get_output_count();

/**
 * @brief Function that does the logging 
 * 
 * @param [in] verbosity - The message verbosity
 * @param [in] message   - Message content 
 * @param [in] ... Additional Variadic arguments for formatting the string  
 */
void logger_log(logger_verbosity_t verbosity, const char *  message, ...);

/**
 * @brief Function to register destinations for logging and setting the default config for the 
 * destination. Note the id input is needed to change config of the destination after creation.
 * 
 * @param [in] fn_ptr - pointer to the ouput function that will be called and passed the message and variadic arguments when logger_log is called 
 * @param [in] verbosity - the minium verbosity level that will be allowed to be logged to this destination
 * @param [in] enabled - if false the destination wont log messages until the logger_enable_dest() is called and the destination re-enabled
 * @param [in] id  - The id string accociated with this destination, needed to identify destination when changing config
 */
void logger_register_destination(write_function fn_ptr, logger_verbosity_t verbosity, bool enabled, const char * id);

/**
 * @brief Sets a global verbosity which overrides the specific verbosity of all registered destinations. This function can use a special verbosity (OFF)
 * which disables the global verbosity functionality i.e. set_global_verbosity(OFF) doesn't turn off all destinations, rather turns off the global verbosity,
 * restoring all destinations to their previous verbosity levels
 * @param [in] verbosity 
 */
void logger_set_global_verbosity(logger_verbosity_t verbosity);

/**
 * @brief Toggles a destination off; no logs will be sent to the destination.
 *
 * @param [in] id - Unique identifier string of the destination.
 */
void logger_disable_dest(const char * id);

/**
 * @brief Enable a specific destination output to be logged.
 * 
 * @param [in] id - String that is the unique identifier of the destination.
 */
void logger_enable_dest(const char * id);

/**
 * @brief Disable all logger destinations. All logger output will cease until logger_enable_all is called, or individual destinations are enabled.
 * 
 */
void logger_disable_all(void);

/**
 * @brief Enable all logger destination to output.
 *
 */
void logger_enable_all(void);

/**
 * @brief Enable a specific destination output to be logged.
 * 
 * @param [in] id - String that is the unique identifier of the destination.
 */
logger_set_global_timestamping(bool );

#endif