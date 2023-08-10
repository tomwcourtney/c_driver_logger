/**
 * @file logger.h
 * @author Pat, tom and dean 
 * @brief logger.h is a general use logger created for use in C embedded projects. The logger supports defininfing
 * multipule "destinations" that log messages can be simutanously logged to. Verbosity can be configured on a per 
 * destination level.  
 */

#ifndef LOGGER_GUARD_H
#define LOGGER_GUARD_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Typedef'd enum reres
 *
*/
typedef enum{
    OFF,
    ERROR,   //Lowest 
    WARNING,
    INFO,
    DEBUG,
}logger_verbosity_t;

typedef void (*write_function)(const char *);

/**
 * @brief Zeros destination head counter, sets the global verbosity level to off 
 * and does all house keeping needed before defining destiatnions and logging messages
 * 
 */
void logger_init();

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t logger_get_output_count();

/**
 * @brief 
 * 
 * @param verbosity 
 * @param message 
 * @param ... 
 */
void logger_log(logger_verbosity_t verbosity, const char *  message, ...);

/**
 * @brief 
 * 
 * @param fn_ptr 
 * @param verbosity 
 * @param enabled 
 * @param id 
 */
void logger_register_destination(write_function fn_ptr, logger_verbosity_t verbosity, bool enabled, const char * id);

/**
 * @brief 
 * 
 * @param verbosity 
 */
void logger_set_global_verbosity(logger_verbosity_t verbosity);

/**
 * @brief - String that is the unique identifier of the destination.
 *
 * @param id 
 */
void logger_disable_dest(const char * id);

/**
 * @brief Enable a specific destination output to be logged.
 * 
 * @param id - String that is the unique identifier of the destination.
 */
void logger_enable_dest(const char * id);

/**
 * @brief Disable all logger destinations.
 * 
 */
void logger_disable_all(void);

/**
 * @brief Enable all logger destinations.
 *
 */
void logger_enable_all(void);

#endif