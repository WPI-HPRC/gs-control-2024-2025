Backend
=======
Here, you'll find documentation on every aspect of the Backend class. There's some conceptual information, as well as technical.
The Backend is responsible for all data collection and handling, as well as interfacing with the radio modules.

The conceptual overview will discuss functions and operation of the Backend without going into much technical detail,
while the technical documentation will go into more detail about what the code for each function is specifically doing.

Table of Contents
-----------------
.. toctree::
    Conceptual Overview
    Structs and Enums

########
Conceptual Overview
########

The Backend is implemented as a singleton class, allowing it to be accessed from anywhere at any time, as long as ``Backend.h`` is included.
The Backend instance can be accessed by calling

.. code-block:: cpp

    Backend::getInstance();

The first time the Backend is accessed, a new ``static`` instance is created and then returned for every subsequent call of ``getInstance()``.

*******
Startup
*******
When the Backend starts up, it gets a list of available serial ports. The webserver is initialized, which is important for some communications,
especially for data transfer between the Backend and the Frontend from 2023-2024. It then initializes the data simulator.
Currently, the data simulator runs when the macro ``SIMULATE_DATA`` is defined. *This should be changed so a button can simply be pressed.*
Next, the Backend attempts to connect to the Ground Station Modem. A macro, ``GROUND_STATION_MODULE`` has been defined in `Constants.h`
that stores the name of the FTDI chip of the modem. After attempting to connect to the modem, a couple of timers are set up.
The first is a loop timer that will run the cycles on all connected radio modules. Another timer is also
set up to update the real-time clock on the frontend.

***********
Radio Tests
***********
Radio tests are sent to the radio modules by the backend, and the results of these tests are sent to the Frontend through the Backend.
Due to the asynchronous nature of the radio modules, the code gets a little messy. There needs to be a function to start each test, one to stop
each test, and one for when a test is complete. Additionally, some tests can "fail", which also must be handled.

================
Throughput Tests
================
A throughput test, as the name suggests, tests the throughput of two radio modules. It sends data with a specified size at
a specified rate to a specified address. Due to the need to send data at varying rates, a timer is needed to choreograph the
sending of data.

Running Throughput Tests
************************
There are three functions that handle the running of throughput tests, one called  ``runThroughputTest`` and another called ``_runThroughputTest``,
and a third called ``runThroughputTestsWithRange``. The underscore (``_``) denotes the private version of a public method. In this case, the public
method ``runThroughputTest`` starts a throughput test with given constant parameters. ``runThroughputTestsWithRange`` allows for a range
of parameters to be specified, and these parameters are iterated through in order to allow the user to find the best operating conditions given
a certain set of constant parameters. If this function did not exist, the user would have to manually change each parameter every time they wanted
to test something different, which would take a while and be inconvenient.

The method ``_runThroughputTest`` is what actually starts the individual throughput test itself. ``runThroughputTestsWithRange`` calls this method while
also setting up the ranges of parameters to be iterated through, while ``runThroughputTest`` simply runs the test with a single set of parameters. While
it appears somewhat convoluted, the distinction between the different ways to run a throughput test is important.

Handling Throughput Tests
*************************
When a throughput test is complete, ``throughputTestComplete`` is called. This method collects the data from the most recent throughput test and
does some calculations. Next, it converts the struct into a JSON object to be logged to a ``.csv`` file. A signal is then emitted to send out data
about the most recent throughput test. Finally, the method checks if there are any more throughput tests to be run (if ``runThroughputTestsWithRange`` had been called).
If there are more throughput tests to be run, the next throughput test is started.

After each throughput test, ``throughputTestComplete`` is always called. By calling the next throughput test, if necessary,
it is guaranteed that all throughput tests can be run. This is essentially a recursive process, but due to the nature of how timers work,
it is not truly recursive in implementation. By setting up the code this way, the user can see the live results of the most recent throughput test
even while more are being run.

==========
Link Tests
==========
A link test is, intuitively, used to test the link between two radio modules. The term "link" refers to how strong the connection is between the two modules. During a
link test, a radio module will record the strength of the signal it receives, as well as how many packets were successfully transmitted, giving us insight into both
how strong the connection was, and how well data was able to be transferred. This is important, since information in the XBee datasheet is given under idealized
conditions, which are not representative of the real word.

Running Link Tests
******************
Unlike throughput tests, only one method is necessary to run a link test, which is called ``runLinkTest``. This method takes in the address of the remote radio module
with which to complete the link test, the size of the packets we want to send, how many packets to send, and how many times we want to repeat the link test (or if
we want to let it run indefinitely). The looping of link tests is handled by the ``RadioModule`` class itself, so that will not be discussed in this document. When
``runLinkTest`` is called, it relays information to the ``RadioModule`` so that the Backend does not have to be concerned with the exact implementation of link tests.

Link tests can also be canceled, which is important since they can be set to run indefinitely. When ``cancelLinkTest`` is called, the Backend sends information to the
correct ``RadioModule`` to tell it to stop the loop test.

Handling Link Tests
*******************
When a link test is complete, ``linkTestComplete`` is called. This method takes data from the most recent link test and emits a signal to update the Frontend accordingly.
The logic for this is much simpler than that for throughput tests. It is also possible for link tests to fail, in which case the ``RadioModule`` emits a signal to notify the
Backend, which in turn emits a signal to update the Frontend accordingly. Again, much of this logic is implemented by the ``RadioModule``.

#################
Structs and Enums
#################
The ``Backend`` class holds various structs and enums that are necessary for data storage and organization.

=======
Structs
=======
There are three structs contained within the Backend: ``ThroughputTestParams``, ``ThroughputTestResults``, and ``Telemetry``. The first two are only important to the Backend,
while the third is vital to the rest of the project as well.

ThroughputTestParams
********************
``ThroughputTestParams``, as you can guess, holds parameters for running throughput tests. This struct is used for starting a throughput test,
and is populated when a throughput test is started by the user. It is laid out as follows:

.. code-block:: cpp

    struct ThroughputTestParams
    {
        RadioModule *receiveModule;
        uint64_t destinationAddress;
        uint8_t payloadSize;
        uint packetRate;
        uint duration;
        uint8_t transmitOptions;
    };

The first field, ``receiveModule``, is a pointer to the radio module that was running the throughput test.

The second field, ``destinationAddress``, is a ``uint64_t`` which holds the address of the radio module data was being sent to.

The last field, ``transmitOptions``, is a ``uint8_t`` which holds transmit options that can be specified by the user. More information on what these transmit options
can look like can be found in XBee-specific documentation.

The rest of the fields are not annotated here, since they are fairly self-explanatory.

ThroughputTestResults
*********************
``ThroughputTestResults`` holds information for a completed throughput test. This struct is used to communicate the results of a throughput test
and display it on the Frontend, along with logging it to a file. It is laid out as followed:

.. code-block:: cpp

    struct ThroughputTestResults
        {
            uint8_t payloadSize;
            uint packetRate;
            uint duration;
            uint8_t transmitOptions;
            uint numPacketsReceived;
            float percentReceived;
            float throughput;
        }

The struct holds parameters for the test, which are defined above in ``ThroughputTestParams``. Additionally, information about how the test went is included.
These are the number of packets received, the percent of packets received, and the calculated throughput during the test.

Telemetry
*********
The ``Telemetry`` struct is one of the most important in the Ground Station, as it holds information about a received packet. This struct is used universally
throughout the project to communicate telemetry. It is laid out as follows:

.. code-block:: cpp

    struct Telemetry
    {
        GroundStation::PacketType packetType;
        union data
        {
            GroundStation::RocketTelemPacket *rocketData;
            GroundStation::PayloadTelemPacket *payloadData;
        } data;
    };

The first field in the struct, ``packetType``, represents the type of information in the packet. Currently, only rocket data and payload data exist as possible
packet types, but this will be extended as the telemetry system becomes more complex. A value of ``unknown`` is also supported as a default value.

The second field, ``data``, is a union which holds the actual data contained within the packet. A union is used so that this single field can hold any possible
packet data, which can then be extracted with the use of ``packetType``. The packet data is represented by a pointer. As mentioned previously, the only two current
telemetry packets supported are ``rocketData`` and ``payloadData``.

Extraction of data may look something like this:

.. code-block:: cpp

    Telemetry telemetry = getTelemetry();
    if (telemetry.packetType == GroundStation::Rocket)
    {
        GroundStation::RocketTelemPacket *rocketData = telemetry.data.rocketData;
        // Handle rocket data
    }
    else if (telemetry.packetType == GroundStation::Payload)
    {
        GroundStation::PayloadTelemPacket *payloadData = telemetry.data.payloadData;
        // Handle payload data
    }
    else
    {
        // Unknown packet type
    }

Note that with more packet types, using a switch-case statement may be more efficient

=====
Enums
=====
There is currently only one enum, ``RadioModuleType``, that exists in the Backend.

RadioModuleType
***************
The ``RadioModuleType`` enum is used in some Ground Station testing procedures, where a radio module is connected and treated as either a Rocket or Payload radio module.
This can change the information the radio modules transmit and how data is received. This enum is not used much, but it is worth mentioning since it is present. The enum
looks like this:

.. code-block:: cpp

    enum RadioModuleType
    {
        Default,
        Rocket,
        Payload
    };
.. note::
    The documentation for the Backend is in active development.