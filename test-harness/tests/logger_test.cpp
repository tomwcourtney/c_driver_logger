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
Log a higher verbosity if the global verbosity is set higher than the destination
*/
TEST(LoggerTest, global_verbosity_allows_higher_verbosity)
{
    // Register the destination
    logger_register_destination(logger_spy_write, ERROR, true, "spy_destination");

    // Set global verbosity to something other than off
    logger_set_global_verbosity(WARNING);

    // Do a log that wouldn't log if it weren't for global verbosity
    logger_log(WARNING,"message");

    // Check the value of the log
    STRCMP_EQUAL(logger_spy_get_string(), "message");

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
*/
TEST(LoggerTest, enable_destination_then_check_logging_works)
{

    // Register destination disabled
    logger_register_destination(logger_spy_write, DEBUG, false, "spy_destination");

    // Log
    logger_log(DEBUG, "Message");

    // Check spy
    CHECK(strlen(logger_spy_get_string())==0);

    // Enable destination
    logger_enable_dest("spy_destination");

    // Log
    logger_log(DEBUG, "Message");

    // Check
    STRCMP_EQUAL("Message", logger_spy_get_string());
}

/*
disable all destinations and check that no messages get logged
*/

TEST(LoggerTest, disable_all_destinations_no_messages_logged)
{
    // register destination enabled
    logger_register_destination(logger_spy_write, DEBUG, true, "spy_destination");

    // log
    logger_log(DEBUG, "yeet");

    // check logged
    STRCMP_EQUAL("yeet", logger_spy_get_string());

    // disable all destinations
    logger_disable_all();

    // log
    logger_log(DEBUG, "leet");

    // check didn't log
    CHECK(strlen(logger_spy_get_string()) == 0);
}

/*
enable all and check that messages are now logged
*/
TEST(LoggerTest, enable_all_destinations_see_messages_logged)
{
    // register destination enabled
    logger_register_destination(logger_spy_write, DEBUG, true, "spy_destination");

    // disable all destinations
    logger_disable_all();

    // log
    logger_log(DEBUG, "yeet");

    CHECK(strlen(logger_spy_get_string()) == 0);

    // enable all destinations
    logger_enable_all();

    // log
    logger_log(DEBUG, "yeet");
    
    // check logged
    STRCMP_EQUAL("yeet", logger_spy_get_string());
    
}

/*
Create two destinations and log message and verify that it's logged in both locations
*/
TEST(LoggerTest, log_to_two_destinations)
{
    // Register destinations
    logger_register_destination(logger_spy_write, DEBUG, true, "dest1");
    logger_register_destination(logger_spy_write2, DEBUG, true, "dest2");

    // Log to both destinations
    logger_log(DEBUG, "crumb");

    // Check both logs
    STRCMP_EQUAL("crumb", logger_spy_get_string());
    STRCMP_EQUAL("crumb", logger_spy_get_string_2());
}

/*disable one destinations and check that no messages get logged*/
TEST(LoggerTest, disable_one_destinations)
{
    // Register destinations
    logger_register_destination(logger_spy_write, DEBUG, true, "dest1");
    logger_register_destination(logger_spy_write2, DEBUG, true, "dest2");

    // disable one of the destinations
    logger_disable_dest("dest1");

    // log message 
    logger_log(DEBUG, "hey");

    // check it didnt arrive to destiation 1
    STRCMP_EQUAL("", logger_spy_get_string());
    // check it did arrive to the second
    STRCMP_EQUAL("hey", logger_spy_get_string_2());
}

/*
enable all destinations and check that messages are now logged
*/
TEST(LoggerTest, enable_all_destinations)
{
    // Register destinations
    logger_register_destination(logger_spy_write, DEBUG, true, "dest1");
    logger_register_destination(logger_spy_write2, DEBUG, true, "dest2");

    // disable all
    logger_disable_all();
    
    // log
    logger_log(DEBUG, "eif0tkpasif");

    // check no log
    STRCMP_EQUAL("", logger_spy_get_string());
    STRCMP_EQUAL("", logger_spy_get_string_2());

    // enable all
    logger_enable_all();

    // log
    logger_log(DEBUG, "LKJOAFJOPKPO");

    // check log
    STRCMP_EQUAL("LKJOAFJOPKPO", logger_spy_get_string());
    STRCMP_EQUAL("LKJOAFJOPKPO", logger_spy_get_string_2());
}

/*
Create two destinations, disable only one, send a log and verify it is sent to the enabled destination but not to the disabled destination
Create two destinations, set one verbosity higher then the other and check that message only gets through one destions
Make sure adding more destinations than MAX_DESTINATIONS doesn't fuck
*/