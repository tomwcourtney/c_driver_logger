#define MAX_STR_LEN 512

#include <stdint.h>

void logger_spy_init(void);
char * logger_spy_get_string(void);
void logger_spy_write(char * data);
