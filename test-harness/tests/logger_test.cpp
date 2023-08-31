#include "CppUTest/TestHarness.h"

extern "C"
{
    #include <string.h>
    #include "../spies/logger_spy.h"
    #include "../../inc/logger.h"
	/*
	 * Add your c-only include files here
	 */
}

TEST_GROUP(LoggerTest)
{
    void setup()
    {
        logger_spy_init();
        logger_init(NULL);
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
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "spy_destination");

    logger_log(LOGGER_DEBUG, "Some string");

    LONGS_EQUAL(strcmp(logger_spy_get_string(), "Some string"),0);
}

/*
change verbosity level for destination and check that higer verbosity messages are not logged
*/
TEST(LoggerTest, higher_verbosity_not_logged)
{
    logger_register_destination(logger_spy_write, LOGGER_ERR, true, "spy_destination");

    logger_log(LOGGER_WARNING, "A Test");

    LONGS_EQUAL(0, strlen(logger_spy_get_string()));
}

/*
change verbosity level for destination and check that lower and equal verbosity messages are logged
*/
TEST(LoggerTest, lower_verbosity_logged)
{
    logger_register_destination(logger_spy_write, LOGGER_WARNING, true, "spy_destination");

    logger_log(LOGGER_ERR, "A Test");

    STRCMP_EQUAL("A Test", logger_spy_get_string());
}


/*
Log a higher verbosity if the global verbosity is set higher than the destination
*/
TEST(LoggerTest, global_verbosity_allows_higher_verbosity)
{
    // Register the destination
    logger_register_destination(logger_spy_write, LOGGER_ERR, true, "spy_destination");

    // Set global verbosity to something other than off
    logger_set_global_verbosity(LOGGER_WARNING);

    // Do a log that wouldn't log if it weren't for global verbosity
    logger_log(LOGGER_WARNING,"message");

    // Check the value of the log
    STRCMP_EQUAL(logger_spy_get_string(), "message");

}

/*
change verbosity all and check that higher and equal verbosity messages are logged
*/
TEST(LoggerTest, global_verbosity_blocks_more_verbose_logs)
{
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "spy_destination");

    logger_set_global_verbosity(LOGGER_ERR);

    // do a log that should go through, except for global verbosity
    logger_log(LOGGER_DEBUG, "help");

    LONGS_EQUAL(0, strlen(logger_spy_get_string()));
}

/*disable destination and check that messages are no longer logged*/
TEST(LoggerTest, testing_disable_can_disable)
{
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "spy_destination");
    // disable destination
    logger_disable_dest("spy_destination");
    // Log message 
    logger_log(LOGGER_ERR, "help");
    //Check no log message got through
    LONGS_EQUAL(0, strlen(logger_spy_get_string()));

}
/*
enable destination and check that messages are logged
*/
TEST(LoggerTest, enable_destination_then_check_logging_works)
{

    // Register destination disabled
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, false, "spy_destination");

    // Log
    logger_log(LOGGER_DEBUG, "Message");

    // Check spy
    CHECK(strlen(logger_spy_get_string())==0);

    // Enable destination
    logger_enable_dest("spy_destination");

    // Log
    logger_log(LOGGER_DEBUG, "Message");

    // Check
    STRCMP_EQUAL("Message", logger_spy_get_string());
}

/*
disable all destinations and check that no messages get logged
*/

TEST(LoggerTest, disable_all_destinations_no_messages_logged)
{
    // register destination enabled
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "spy_destination");

    // log
    logger_log(LOGGER_DEBUG, "yeet");

    // check logged
    STRCMP_EQUAL("yeet", logger_spy_get_string());

    // disable all destinations
    logger_disable_all();

    // log
    logger_log(LOGGER_DEBUG, "leet");

    // check didn't log
    CHECK(strlen(logger_spy_get_string()) == 0);
}

/*
enable all and check that messages are now logged
*/
TEST(LoggerTest, enable_all_destinations_see_messages_logged)
{
    // register destination enabled
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "spy_destination");

    // disable all destinations
    logger_disable_all();

    // log
    logger_log(LOGGER_DEBUG, "yeet");

    CHECK(strlen(logger_spy_get_string()) == 0);

    // enable all destinations
    logger_enable_all();

    // log
    logger_log(LOGGER_DEBUG, "yeet");
    
    // check logged
    STRCMP_EQUAL("yeet", logger_spy_get_string());
    
}

/*
Create two destinations and log message and verify that it's logged in both locations
*/
TEST(LoggerTest, log_to_two_destinations)
{
    // Register destinations
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "dest1");
    logger_register_destination(logger_spy_write2, LOGGER_DEBUG, true, "dest2");

    // Log to both destinations
    logger_log(LOGGER_DEBUG, "crumb");

    // Check both logs
    STRCMP_EQUAL("crumb", logger_spy_get_string());
    STRCMP_EQUAL("crumb", logger_spy_get_string_2());
}

/*
Create two destinations, disable only one, send a log and verify it is sent to the enabled destination but not to the disabled destination
*/
TEST(LoggerTest, disable_one_destination)
{
    // Register destinations
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "dest1");
    logger_register_destination(logger_spy_write2, LOGGER_DEBUG, true, "dest2");

    // disable one of the destinations
    logger_disable_dest("dest1");

    // log message 
    logger_log(LOGGER_DEBUG, "hey");

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
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "dest1");
    logger_register_destination(logger_spy_write2, LOGGER_DEBUG, true, "dest2");

    // disable all
    logger_disable_all();
    
    // log
    logger_log(LOGGER_DEBUG, "eif0tkpasif");

    // check no log
    STRCMP_EQUAL("", logger_spy_get_string());
    STRCMP_EQUAL("", logger_spy_get_string_2());

    // enable all
    logger_enable_all();

    // log
    logger_log(LOGGER_DEBUG, "LKJOAFJOPKPO");

    // check log
    STRCMP_EQUAL("LKJOAFJOPKPO", logger_spy_get_string());
    STRCMP_EQUAL("LKJOAFJOPKPO", logger_spy_get_string_2());
}

/*Create two destinations, set one verbosity higher then the other and check that message only gets through one destions*/
TEST(LoggerTest, create_two_destinations_set_one_verbosity_higher_then_other_verify_message_goes_to_only_one_dest)
{
    // Register destinations
    // Verbosity 1 LOGGER_DEBUG
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "dest1");
    // Verbosity 2 LOGGER_ERR
    logger_register_destination(logger_spy_write2, LOGGER_ERR, true, "dest2");
       
    // Log LOGGER_ERR
    logger_log(LOGGER_WARNING, "LKJOAFJOPKPO");
    
    // Check that only destination 2 got through
    STRCMP_EQUAL("LKJOAFJOPKPO", logger_spy_get_string());
    STRCMP_EQUAL("", logger_spy_get_string_2());
}



/*
Make sure adding more destinations than MAX_DESTINATIONS doesn't fuck up
*/
TEST(LoggerTest, overflow_max_destination)
{
    // Adding the two logs to fill buffer
    logger_register_destination(logger_spy_write, LOGGER_ERR, true, "dest1");
    logger_register_destination(logger_spy_write2, LOGGER_ERR, true, "dest2");
    
    // Overflowing buffer massivly 
    for(int i =0; i<2048; i++)
    {
        logger_register_destination(logger_spy_write, LOGGER_ERR, true, "dest3");
    }
    // Log LOGGER_ERR
    logger_log(LOGGER_ERR, "LKJOAFJOPKPO");

    // Check that string got recived by the 
    // two detinations
    STRCMP_EQUAL("LKJOAFJOPKPO", logger_spy_get_string());
    STRCMP_EQUAL("LKJOAFJOPKPO", logger_spy_get_string_2());

}

/*
Log a string that is max string length+1 and that the logged message contains the string up to max sting length long 
*/
TEST(LoggerTest, strings_are_cut_off_at_max_sring_length)
{

    // Define a destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "dest");
    // logg a message to desitation that is one longer then the max string length
    std::string test_string(MAX_STR_LEN+1, 'a');
    logger_log(LOGGER_ERR,test_string.c_str());

    // Test to see if string matches one of MAX_STR_LEN
    test_string.erase(test_string.begin() + MAX_STR_LEN);
    STRCMP_EQUAL(test_string.c_str(), logger_spy_get_string());
}

/*
attempting to register destination with an ID thats already registered fails
*/
TEST(LoggerTest, register_existing_id_fails)
{
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff");
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff");

    LONGS_EQUAL(1, logger_get_output_count());
}

/*
Initialise the logger with no get time function
*/
TEST(LoggerTest, initialise_logger_with_no_get_time_function)
{
    // Initialise the logger with a null get time function
    logger_init(NULL);
    // Register a destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff");
    // Log to destination
    logger_log(LOGGER_ERR, "Test");
    // Check that the log has no timestamp in it
    STRCMP_EQUAL("Test", logger_spy_get_string());
}

/*
Initialise the logger with a get time function and check that a log has the correct timestamp
*/
TEST(LoggerTest, initialise_the_logger_with_get_time_function)
{
    // Initialise the logger with a null get time function
    logger_init(logger_spy_get_time);
    // Register a destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff");
    // Log to destination
    logger_log(LOGGER_ERR, "Test");
    // Check that the log has no timestamp in it
    STRCMP_EQUAL("[1970-1-1T00:00:00] Test", logger_spy_get_string());
}

/*
when logger_set_global_time_stamping(false) is called, no destiations add timestamps to there messages
*/
TEST(LoggerTest, toggle_timestamps_no_timestamp)
{
    // Initialise the logger with a null get time function
    logger_init(logger_spy_get_time);
    // Register a destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff");
    // toggle timestamping off
    logger_set_global_timestamping(false);
    // log
    logger_log(LOGGER_ERR, "Test");
    // check log doesn't contain timestamp
    STRCMP_EQUAL("Test", logger_spy_get_string());
}

/*
when logger_set_global_time_stamping(true) is called, all destiations add timestamps to there messages
*/
TEST(LoggerTest, turn_global_timestamping_on_all_destinations_add_timestamp)
{
    // init logger with time function
    logger_init(logger_spy_get_time);
    // register 2 destinations
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    logger_register_destination(logger_spy_write2, LOGGER_DEBUG, true, "jeff2");
    // toggle timestamping off
    logger_set_global_timestamping(false);
    // toggle timestamping on
    logger_set_global_timestamping(true);
    // log
    logger_log(LOGGER_ERR, "Test");

    // check both destinations get timestamped
    STRCMP_EQUAL("[1970-1-1T00:00:00] Test", logger_spy_get_string());
    STRCMP_EQUAL("[1970-1-1T00:00:00] Test", logger_spy_get_string_2());
}

/*
when logger_set_global_verb_prepend(false) is called, no destinations add verbosity to front of message
*/
TEST(LoggerTest, global_verbosity_false_does_nothing)
{
    // init logger with time function
    logger_init(logger_spy_get_time);
    // register destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    // set global verbosity prepernd to false 
    logger_set_global_verbosity_prepend(false);
    // log a message
    logger_log(LOGGER_ERR, "dog");
    // check message has no verbosity added
    STRCMP_EQUAL("[1970-1-1T00:00:00] dog", logger_spy_get_string());
}

/*
when logger_set_global_verb_prepend(true) is called, all destinations add verbosity to front of message
*/
TEST(LoggerTest, global_verbosity_true_add_verbosity_tag_to_log)
{
    // init logger with time function
    logger_init(logger_spy_get_time);
    // register destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    // set global verbosity prepernd to false 
    logger_set_global_verbosity_prepend(true);
    // log a message
    logger_log(LOGGER_ERR, "dog");
    // check message has no verbosity added
    STRCMP_EQUAL("[1970-1-1T00:00:00] [E] dog", logger_spy_get_string());
}

/*
when timestamping is off, and verbosity prepend on, log doesn't shopw timestamp but shows verbosity
*/
TEST(LoggerTest, global_verbosity_on_timestamping_off_log_shows_verbosity_not_timestamp)
{
    // init logger with time function
    logger_init(logger_spy_get_time);
    // register destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    // set global verbosity prepernd to false 
    logger_set_global_timestamping(false);
    logger_set_global_verbosity_prepend(true);
    // log a message
    logger_log(LOGGER_ERR, "dog");
    // check message has no verbosity added
    STRCMP_EQUAL("[E] dog", logger_spy_get_string());
}

/*
when colour is toggled on LOGGER_ERR messages appear red 
*/
TEST(LoggerTest, colour_on_logs_are_coloured_LOGGER_ERR)
{
    // Register a destination
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    logger_set_global_verbosity_prepend(false);
    // toggle colout on 
    logger_set_dest_colour("jeff1", true);
    // Do a log
    logger_log(LOGGER_ERR, "dog");

    // Check that the log has a colour appended to it
    STRCMP_EQUAL("\x1b[31mdog", logger_spy_get_string());
}

/*
when colour is toggled on LOGGER_WARNING messages appear Yellow 
*/
TEST(LoggerTest, colour_on_logs_are_coloured_LOGGER_WARNING)
{
    // Register a destination
    logger_set_global_timestamping(false);
    logger_set_global_verbosity_prepend(false);
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    // toggle colout on 
    logger_set_dest_colour("jeff1", true);
    // Do a log
    logger_log(LOGGER_WARNING, "dog");

    // Check that the log has a colour appended to it
    STRCMP_EQUAL("\x1b[33mdog", logger_spy_get_string());
}

/*
when colour is toggled on LOGGER_DEBUG messages appear Green 
*/
TEST(LoggerTest, colour_on_logs_are_coloured_LOGGER_DEBUG)
{
    // Register a destination
    logger_set_global_timestamping(false);
    logger_set_global_verbosity_prepend(false);

    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    // toggle colout on 
    logger_set_dest_colour("jeff1", true);
    // Do a log
    logger_log(LOGGER_ERR, "dog");

    // Check that the log has a colour appended to it
    STRCMP_EQUAL("\x1b[31mdog", logger_spy_get_string());
}
/*
when colour is toggled on LOGGER_DEBUG have no colour
*/
TEST(LoggerTest, colour_on_logs_are_not_coloured_LOGGER_DEBUG)
{
   
    // Register a destination
    logger_set_global_timestamping(false);
    logger_set_global_verbosity_prepend(false);
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    // toggle colout on 
    logger_set_dest_colour("jeff1", true);
    // Do a log
    logger_log(LOGGER_DEBUG, "dog");

    // Check that the log has a colour appended to it
    STRCMP_EQUAL("\x1b[0mdog", logger_spy_get_string());
}

/*
Colours can be toggled per desitionation using the logger_set_dest_colour(id, bool)
*/
TEST(LoggerTest, colour_logs_can_be_toggled_per_dest)
{
    // Register a destination
    logger_set_global_timestamping(false);
    logger_set_global_verbosity_prepend(false);
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "jeff1");
    logger_register_destination(logger_spy_write2, LOGGER_DEBUG, true, "jeff2");
    // toggle colout on 
    logger_set_dest_colour("jeff1", true);
    logger_set_dest_colour("jeff2", false);
    // Do a log
    logger_log(LOGGER_DEBUG, "dog");

    // Check the true destination has colour in it 
    STRCMP_EQUAL("\x1b[0mdog", logger_spy_get_string());
    // Check the false destination has colour in it 
    STRCMP_EQUAL("dog", logger_spy_get_string_2());
}

/*
Colours work with timestamp prepend
*/
TEST(LoggerTest, colour_logs_with_timestamp)
{
    logger_init(logger_spy_get_time);
    logger_set_global_verbosity_prepend(false);

    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "yeeter");

    logger_set_dest_colour("yeeter", true);

    logger_log(LOGGER_WARNING, "REE");

    STRCMP_EQUAL("\x1b[33m[1970-1-1T00:00:00] REE", logger_spy_get_string());
}

/*
Colours work with verbosity prepend
*/
TEST(LoggerTest, colour_logs_with_verbosity)
{
    logger_init(NULL);
    logger_set_global_verbosity_prepend(true);

    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "yeeter");

    logger_set_dest_colour("yeeter", true);

    logger_log(LOGGER_WARNING, "REE");

    STRCMP_EQUAL("\x1b[33m[W] REE", logger_spy_get_string());
}

/*
Colours work with both prepends
*/
TEST(LoggerTest, colour_logs_with_verbosity_and_timestamp)
{
    logger_init(logger_spy_get_time);
    logger_set_global_verbosity_prepend(true);

    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "yeeter");

    logger_set_dest_colour("yeeter", true);

    logger_log(LOGGER_WARNING, "REE");

    STRCMP_EQUAL("\x1b[33m[1970-1-1T00:00:00] [W] REE", logger_spy_get_string());
}

/*
Test that variadic parameters/format string works when no color/timestamp/verbosity
*/
TEST(LoggerTest, variadic_parameters_log_no_prepends)
{
    logger_init(NULL);
    logger_set_global_verbosity_prepend(false);
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "vari");

    logger_log(LOGGER_WARNING, "Test: %d\n", 3);

    STRCMP_EQUAL("Test: 3\n", logger_spy_get_string());
}

/*
Test that variadic parameters/format string works with colour
*/
TEST(LoggerTest, variadic_parameters_log_colour)
{
    logger_init(NULL);
    logger_set_global_verbosity_prepend(false);
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "vari");
    logger_set_dest_colour("vari", true);

    logger_log(LOGGER_WARNING, "Test: %d\n", 3);

    STRCMP_EQUAL("\x1b[33mTest: 3\n", logger_spy_get_string());
}

/*
Test that variadic parameters/format string works with colour and verbosity
*/
TEST(LoggerTest, variadic_parameters_log_colour_and_verbosity)
{
    logger_init(NULL);
    logger_set_global_verbosity_prepend(true);
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "vari");
    logger_set_dest_colour("vari", true);

    logger_log(LOGGER_WARNING, "Test: %d\n", 3);

    STRCMP_EQUAL("\x1b[33m[W] Test: 3\n", logger_spy_get_string());
}

/*
Test that variadic parameters/format string works with colour, verbosity and timestamp
*/
TEST(LoggerTest, variadic_parameters_log_colour_verbosity_and_time)
{
    logger_init(logger_spy_get_time);
    logger_set_global_verbosity_prepend(true);
    logger_register_destination(logger_spy_write, LOGGER_DEBUG, true, "vari");
    logger_set_dest_colour("vari", true);

    logger_log(LOGGER_WARNING, "Test: %d\n", 3);

    STRCMP_EQUAL("\x1b[33m[1970-1-1T00:00:00] [W] Test: 3\n", logger_spy_get_string());
}