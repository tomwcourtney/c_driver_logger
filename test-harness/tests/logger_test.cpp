#include "CppUTest/TestHarness.h"

extern "C"
{
    #include <string.h>
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
define a destination and send string and see that its logged
*/
TEST(LoggerTest, log_and_see_log)
{
    logger_register_destination(logger_spy_write, DEBUG, true, "spy_destination");

    logger_log(DEBUG, "Some string");

    LONGS_EQUAL(strcmp(logger_spy_get_string(), "Some string"),0);
}

/*
change verbosity level for destination and check that higer verbosity messages are not logged
*/
TEST(LoggerTest, higher_verbosity_not_logged)
{
    logger_register_destination(logger_spy_write, WARNING, true, "spy_destination");

    logger_log(INFO, "A Test");

    LONGS_EQUAL(0, strlen(logger_spy_get_string()));
}

/*
change verbosity level for destination and check that lower and equal verbosity messages are logged
*/
TEST(LoggerTest, lower_verbosity_logged)
{
    logger_register_destination(logger_spy_write, WARNING, true, "spy_destination");

    logger_log(ERROR, "A Test");

    STRCMP_EQUAL("A Test", logger_spy_get_string());
}


/*
change verbosity all and check that lower verbosity messages aren't logged
*/
TEST(LoggerTest, global_verbosity_allows_higher_verbosity)
{
    // Register the destination
    logger_register_destination(logger_spy_write, ERROR, true, "spy_destination");

    // Set global verbosity to something other than off
    logger_set_global_verbosity(WARNING);

    // Do a log that shouldn't log 
    logger_log(WARNING,"message");

    // Check the value of the log
    STRCMP_EQUAL("message", logger_spy_get_string());

}

/*
change verbosity all and check that higher and equal verbosity messages are logged
*/
TEST(LoggerTest, global_verbosity_blocks_more_verbose_logs)
{
    logger_register_destination(logger_spy_write, DEBUG, true, "spy_destination");

    logger_set_global_verbosity(ERROR);

    // do a log that should go through, except for global verbosity
    logger_log(DEBUG, "help");

    LONGS_EQUAL(0, strlen(logger_spy_get_string()));
}

/*disable destination and check that messages are no longer logged*/
TEST(LoggerTest, testing_disable_can_disable)
{
    logger_register_destination(logger_spy_write, DEBUG, true, "spy_destination");
    // disable destination
    logger_disable_dest("spy_destination");
    // Log message 
    logger_log(ERROR, "help");
    //Check no log message got through
    LONGS_EQUAL(0, strlen(logger_spy_get_string()));

}
/*
enable destination and check that messages are logged
disable all destinations and check that no messages get logged
enable all and check that messages are now logged
*/


