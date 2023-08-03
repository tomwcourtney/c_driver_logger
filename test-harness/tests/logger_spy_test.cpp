#include "CppUTest/TestHarness.h"


extern "C"
{
    #include <string.h>
    #include "../spies/logger_spy.h"
}

TEST_GROUP(LoggerSpyTestGroup)
{
    void setup()
    {
        logger_spy_init();
    }

    void teardown()
    {
    }
};

// Spy empty when nothing done.
TEST(LoggerSpyTestGroup, spy_empty_when_nothing_done)
{
    // Init the logger
    logger_spy_init();

    // Length of string in spy is 0
    uint32_t len = strlen(logger_spy_get_string());

    // Check length of string is 0
    CHECK(len == 0);
}

// spy contains string that is last sent
TEST(LoggerSpyTestGroup, spy_contains_string_that_is_last_sent)
{
    // test out
    char write_out[MAX_STR_LEN] = {0};
    sprintf(write_out, "memes");

    // write little
    logger_spy_write(write_out);

    // get stored string
    char * read_out;
    read_out = logger_spy_get_string();

    // make sure both strings equal
    CHECK(strcmp(write_out, read_out) == 0);
}

// big string overwritten by little string works as expected
TEST(LoggerSpyTestGroup, big_string_overwritten_by_little_string_works_as_expected)
{
    char write_big[MAX_STR_LEN] = {0};
    sprintf(write_big, "hellllloo");
    // test out
    char write_little[MAX_STR_LEN] = {0};
    sprintf(write_little, "memes");

    logger_spy_write(write_big);
    logger_spy_write(write_little);

    // get stored string
    char * read_out;
    read_out = logger_spy_get_string();

    // make sure both strings equal
    CHECK(strcmp(write_little, read_out) == 0);
}

// Check that we cannot overflow the buffer.
TEST(LoggerSpyTestGroup, test_that_the_spy_string_does_not_overflow)
{
    char str[MAX_STR_LEN+11] = {0};
    memset(str,'a', MAX_STR_LEN+10);
    logger_spy_write(str);
    
    uint32_t buffer_len = strlen(logger_spy_get_string());

    CHECK(buffer_len == MAX_STR_LEN);
}




/* 
Do nothing and check no bytes sent
*/

/*
Log when there is no destination and ensure no bytes are sent
*/

/*

define a destination and send string and see that its logged
change verbosity level for destination and check that lower verbosity messages are not logged
change verbosity level for destination and check that higher and equal verbosity messages are logged
change verbosity all and check that lower verbosity messages aren't logged
change verbosity all and check that higher and equal verbosity messages are logged
disable destination and check that messages are no longer logged
enable destination and check that messages are logged
disable all destinations and check that no messages get logged
enable all and check that messages are now logged

*/


