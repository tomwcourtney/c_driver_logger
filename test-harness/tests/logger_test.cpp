#include "CppUTest/TestHarness.h"


extern "C"
{
#include "../spies/logger_spy.h"
#include "../../src/logger.h"
	/*
	 * Add your c-only include files here
	 */
}

TEST_GROUP(LoggerTest)
{
    void setup()
    {
        logger_spy_init();
        logger_init();
    }

    void teardown()
    {
    }
};

/* 
do_init_check_no_outputs
*/
TEST(LoggerTest, do_init_check_no_outputs)
{
    LONGS_EQUAL(logger_get_output_count(), 0);
}


/*
Log when there is no destination and ensure no bytes are sent
*/
TEST(LoggerTest, log_and_see_log)
{
    logger_register_destination(logger_spy_write);

    logger_log(DEBUG, "Some string");

    LONGS_EQUAL(strcmp(logger_spy_get_string(), "Some string"),0);
}



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


