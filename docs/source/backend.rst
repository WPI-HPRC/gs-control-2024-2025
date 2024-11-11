Backend
=======
Here, you'll find documentation on every aspect of the Backend class. There's some conceptual information, as well as technical.
The Backend is responsible for all data collection and handling, as well as interfacing with the radio modules.

The conceptual overview will discuss functions and operation of the Backend without going into much technical detail,
while the technical documentation will go into more detail about what the code for each function is specifically doing.

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

.. note::
    The documentation for the Backend is in active development.