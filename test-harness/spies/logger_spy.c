#include "logger_spy.h"
#include <stdio.h>
#include <string.h> 

char last_string[MAX_STR_LEN+1] = {0};

void logger_spy_init(void)
{
    memset(last_string, 0, MAX_STR_LEN);
    return;
}

char * logger_spy_get_string(void)
{
    return last_string;
}

void logger_spy_write(char * data)
{
    strncpy(last_string, data, MAX_STR_LEN);
}