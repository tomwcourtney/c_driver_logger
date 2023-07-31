#include "logger_spy.h"
#include <stdio.h>
#include <string.h> 

char last_string[MAX_STR_LEN+1] = {0};

void logger_spy_init(void)
{
    memset(last_string, 'a', MAX_STR_LEN);
    return;
}

char * logger_spy_get_string(void)
{
    return last_string;
}