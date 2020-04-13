This is a test to see how fast C/C++ can build large (200K * 14 byte) buffers vs Python.

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

# Java results

200K   | 1M     | Description
-------|--------|--------------
~32 ms | 390 ms | Built from list of buffers and scores accessing with random indexes (NOT CPU pipeline/cache efficient)

## 200K

```
$ perf stat -e task-clock,cycles,instructions,cache-references,cache-misses  ./buffercopy
2152 us

 Performance counter stats for './buffercopy':

              5.93 msec task-clock                #    0.947 CPUs utilized          
        24,128,843      cycles                    #    4.070 GHz                    
        41,203,829      instructions              #    1.71  insn per cycle         
         1,389,426      cache-references          #  234.382 M/sec                  
           190,138      cache-misses              #   13.685 % of all cache refs
```

## 1M
* Assets were increased 5x.
* Instructions increased 4.7x.
* cache misses increased 22x.
* Initialization primarily increases writes, not reads, therefore would have little impact on cache misses.
```
$ perf stat -e task-clock,cycles,instructions,cache-references,cache-misses  ./buffercopy
26937 us

 Performance counter stats for './buffercopy':

             43.89 msec task-clock                #    0.994 CPUs utilized          
       170,305,319      cycles                    #    3.880 GHz                    
       193,843,202      instructions              #    1.14  insn per cycle         
         8,507,793      cache-references          #  193.835 M/sec                  
         4,233,899      cache-misses              #   49.765 % of all cache refs    
```

# Executing Test

* In buffercopy.cpp, set the ASSET_NUM to 200000 or 1000000 or whatever.
* Uncomment one test block, compile and run.
* compile.sh will build and run
* Execute make to only build the buffercopy executable

Running all blocks pollutes the timing of the other blocks.
The first will be slower and later faster due to pipelining, caching, memory allocation.


# Perf stats

http://www.brendangregg.com/blog/2017-05-09/cpu-utilization-is-wrong.html

http://www.brendangregg.com/perf.html


```
perf stat -e L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses ./buffercopy
```
Last-Level-Cache, then DRAM
```
perf stat -e LLC-load-misses,LLC-loads,LLC-store-misses,LLC-stores,branch-load-misses,branch-loads ./buffercopy
```
Data Translation Lookaside Buffer
```
perf stat -e dTLB-load-misses,dTLB-loads,dTLB-store-misses,dTLB-stores ./buffercopy
```
Instruction Translation Lookaside Buffer
```
perf stat -e iTLB-load-misses,iTLB-loads ./buffercopy
```

```
perf stat -e node-load-misses,node-loads,node-store-misses,node-stores ./buffercopy
```
