Backend
=======
Here, you'll find documentation on every aspect of the Backend class. There's some conceptual information, as well as technical.
The Backend is responsible for all data collection and handling, as well as interfacing with the radio modules.

########
Overview
########

The Backend is implemented as a singleton class, allowing it to be accessed from anywhere at any time, as long as ``Backend.h`` is included.
The Backend instance can be accessed by calling

.. code-block:: cpp

    Backend::getInstance();

The first time the Backend is accessed, a new ``static`` instance is created and then returned for every subsequent call of ``getInstance()``.

.. note::
    The documentation for the Backend is in active development.