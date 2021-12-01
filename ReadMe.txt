CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Using the tool
 * Final outcome
 * Further Details
 * Sample Program Details
 * Notes


INTRODUCTION
------------

bottleneck_analyzer.sh is a general purpose tool used for doing Bottleneck analysis on any C++ programs that are written
using openMP.

REQUIREMENTS
------------

This tool requires operf and perf to be installed on the machine.


INSTALLATION
------------

No need to Install, It's just a bash script.
Please give permission if required - chmod +x bottleneck_analyzer.sh


USING THE TOOL
----------------

simply run the tool as any other bash script.

./bottleneck_analyzer.sh <c++ file name> <Arguments to the executable to be generated from the c++ file if any>

example:
    ./bottleneck_analyzer.sh matmul.cpp 1
    ./bottleneck_analyzer.sh memory_access.cpp 8 0
    ./bottleneck_analyzer.sh thread_overhead.cpp 8 0

The output will be on the standard console.

FINAL OUTCOME
----------------

The final outcome will suggest for possible benchmark issues that the program can have,
The benchmark criteria are.


1)  Bad memory access patterns.
2)  False sharing.
3)  Overheads due to OMP constructs
4)  Overheads due to long/numerous critical regions
5)  Load imbalance.



FURTHER DETAILS
----------------

Along with the tool there are few programs that can be used for testing.


To compile - 
g++ -fopenmp -Wall -o <executable file name> <c++ file name>

For perf metric regarding L1 caching and cache miss -
perf stat -o out.txt -e cache-references,cache-misses,L1-dcache-load-misses,L1-dcache-loads ./<executable file name> <Arguments to the executable if any>

For Operf metrics regarding instructions detail -
operf  ./<executable file name> <Arguments to the executable if any>

For Operf metrics regarding threads detail - 
operf  -t ./<executable file name> <Arguments to the executable if any>



SAMPLE PROGRAM DETAILS
------------------------

    Matrix Multiplication
    ----------------------------------------------------------------------------------------------------------------------------------

    ./bottleneck_analyzer.sh matmul.cpp     - Arguments <type of matrix multiplication (0|1)>  
                                                  0 - transpose the other matrix so that we can multiply row wise, to promote good memory access patterns
                                                  1 - generic matrix multiplication, were we multiply row of the first matrix to the column of other matrix. 
    example:
            ./bottleneck_analyzer.sh matmul.cpp  0
            
            

    Intergrals Calcuation Using Trapezoidal rules
    ----------------------------------------------------------------------------------------------------------------------------------

    ./bottleneck_analyzer.sh trap.cpp        - Arguments <a> <b> <n>  (calculate intergrals using trapeziodal rules, from limits a to b and n divisions)
    ./bottleneck_analyzer.sh trap2.cpp       - Arguments <a> <b> <n>  (calculate intergrals using trapeziodal rules, from limits a to b and n divisions)
    ./bottleneck_analyzer.sh trap3.cpp       - Arguments <a> <b> <n>  (calculate intergrals using trapeziodal rules, from limits a to b and n divisions)

    example:
            ./bottleneck_analyzer.sh trap2.cpp 0 10 10000
            ./bottleneck_analyzer.sh trap3.cpp 0 10 10000

    With Trap2 and Trap3 we see better critical region performance.

    
    
    Bottleneck Programs
    ----------------------------------------------------------------------------------------------------------------------------------

    ./bottleneck_analyzer.sh critical_overhead.cpp   - Arguments <number of threads>  <bottleneck enabled (0|1)> (argument 1 decides number of threads to run on, argument 2 enables or disables bottleneck)
    ./bottleneck_analyzer.sh false_sharing.cpp       - Arguments <number of threads>  <bottleneck enabled (0|1)> (argument 1 decides number of threads to run on, argument 2 enables or disables bottleneck)
    ./bottleneck_analyzer.sh thread_overhead.cpp     - Arguments <number of threads>  <bottleneck enabled (0|1)> (argument 1 decides number of threads to run on, argument 2 enables or disables bottleneck)
    ./bottleneck_analyzer.sh load_balancing.cpp      - Arguments <number of threads>  <bottleneck enabled (0|1)> (argument 1 decides number of threads to run on, argument 2 enables or disables bottleneck)
    ./bottleneck_analyzer.sh memory_access.cpp       - Arguments <number of threads>  <bottleneck enabled (0|1)> (argument 1 decides number of threads to run on, argument 2 enables or disables bottleneck)


    example:
        ./bottleneck_analyzer.sh critical_overhead.cpp 8 1   ---> Running critical overhead example with 8 threads and enables the bottleneck present in the code.
        ./bottleneck_analyzer.sh load_balancing.cpp 8 0     ---> Running load balance example with 16 threads and disabling the bottleneck with a better version.


NOTES
------
** We estimated the benchmarks for each bottlenecks through experimentation. Occasionally other bottlenecks may also be flagged based on the machines performance/utilization.

