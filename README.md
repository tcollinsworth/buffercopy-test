This is a test to see how fast C/C++ can build large (200K * 15 byte) buffers vs Python.

Python being interpreted and dynamic is too slow to build large byte buffers for a binary response, i.e., Google flatbuffers.

Python auto-generated flatbuffer code takes ~ 1 second to build a buffer containing 200K * (10 byte + 4 byte) payload.

Custom Python code assebling the buffer takes 100+ milliseconds


# C++ results

200K   | 1M   | Description
-------|------|--------------
~1 ms  | 6 ms | Built from static buffers with memcpy
~1.5 ms|~8 ms | Built from list of buffers and scores accessing with indexes linearly (CPU pipeline/cache efficient)
2 ms   |33 ms | Built from list of buffers and scores accessing with random indexes (NOT CPU pipeline/cache efficient)

Notice how the first two which are pipeline/cache efficient are linear and the last which is NOT pipeline/cache efficient increases not 5x like the size, but 16.5x due to the pipeline cache misses trashing the data flow into the microprocessor.

# Executing Test

* In buffercopy.cpp, set the ASSET_NUM to 200000 or 1000000 or whatever.
* Uncomment one test block, compile and run.
* compile.sh will build and run
* Execute make to only build the buffercopy executable

Running all blocks pollutes the timing of the other blocks.
The first will be slower and later faster due to pipelining, caching, memory allocation.

