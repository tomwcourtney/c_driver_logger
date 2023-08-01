#define MAX_STR_LEN 512

#include <stdint.h>
#include <stdarg.h>

typedef enum{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
}logger_verbosity_t;

void logger_spy_init(void);
char * logger_spy_get_string(void);
void logger_spy_write(char * data);
void logger_log(logger_verbosity_t verbosity,char * message, ...);