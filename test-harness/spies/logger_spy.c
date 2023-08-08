#include "logger_spy.h"
#include <stdio.h>
#include <string.h> 

char last_string[MAX_STR_LEN+1] = {0};
char ret_string[MAX_STR_LEN+1] = {0};

char last_string2[MAX_STR_LEN+1] = {0};
char ret_string2[MAX_STR_LEN+1] = {0};

void logger_spy_init(void)
{
    memset(last_string, 0, MAX_STR_LEN);
    memset(last_string2, 0, MAX_STR_LEN);
    return;
}

char * logger_spy_get_string(void)
{
    strcpy(ret_string, last_string);
    memset(last_string, 0, MAX_STR_LEN);
    return ret_string;
}

char * logger_spy_get_string_2(void)
{
    strcpy(ret_string2, last_string2);
    memset(last_string2, 0, MAX_STR_LEN);
    return ret_string2;
}

void logger_spy_write(const char * data)
{
    strncpy(last_string, data, MAX_STR_LEN);
}

void logger_spy_write2(const char * data)
{
    strncpy(last_string2, data, MAX_STR_LEN);
}



