#include "CppUTest/TestHarness.h"
#include "../spies/logger_spy.h"


extern "C"
{
	/*
	 * Add your c-only include files here
	 */
}

TEST_GROUP(MyCode)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(MyCode, test1)
{
    /*
     * Instantiate your class, or call the function, you want to test.
     * Then delete this comment
     */
    CHECK(true);
}

// 


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


