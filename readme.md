# c_driver_logger
Generalised and portable c driver for logging timestamped messages to multiple outputs with optional serverity tags. Should be portable to anything that runs c and prints text. 

The logger is destination based, meaning that multiple outputs can be registered to the module 
and (depending on verbosity and serverity settings) all messages will be logged to all desitations. 

For any more detailed questions that the readme doesn't cover please consult the doxygen documenation and the 
tests in test-harness. 

## Table of Contents
- [Initialization](#init)
- [Configuration](#config)
    - [Time Stamping](#ts)
    - [Serverity Pre-pend ](#spp)
    - [Colour Toggle](#ct)
## Initialization
The minimum setup the driver needs to function is as follows:
```C
// Init the module
logger_init(NULL);
// Create a destination 
logger_register_destination(printf, LOGGER_DEBUG, true, "exampleDest");
// Log a message 
logger_log(LOGGER_WARNING, "Hello World %d\n", 3);
```
## Configuration
#### Time Stamping 
To enable message timestamping, the driver user will need to create a function that returns a timestamp in your desired 
format, and pass that in during init. For example:
```C
void _get_time(char * time)
{
    strcpy(time, "2019-22-1");
}
// Init the module
logger_init(_get_time);
....
```
Log message will then be prepended with "[2019-22-1]", or whatever timestamping string you decide to return
#### Serverity Pre-pend 
Message serverity pre-pention can be turned on by the following:
```C
// Turn prepention on globally 
logger_set_global_verbosity_prepend(true);
```
and turned off by passing in false. Messages will then have a single letter prepended signifiying there serverity. i.e 
an error would look like "[E] A big error has occured!!". 

#### Colour Toggle 
Colour can be toggled on and off per destination, it works via the use of ANSI escape codes which are not supported on all terminals and are likely unwanted when logging to a text file. Colour can be toggled via the following:
```C
// Toggle desired terminals colours on
logger_set_dest_colour("terminalOutput", true);
// Toggle colours off 
logger_set_dest_colour("TextOutput", false);
```

