Complete the table below with your results, and then fill in the final two sections at the end.

Please do not edit or remove table or section headings, as the autograder script uses these to
locate the start and end of the table.

Each row of the table will be split into columns using the Python "split()" method. Therefore,
- fill in each column with values;
- only use whitespace between columns;
- do not add any non-numeric characters (which would cause the value to be read as zero);
- do not worry if your columns are not perfectly aligned.

For the parallel speed-up S, please note that:
- the time you should use is already output by the provided code;
- take as the serial execution time the time output by the code when run with a single process.
  Hence, the speed-up for 1 process in the table below must be 1.00.


No. Machines:   Total No. Processes:     Mean time (average of 3 runs) in seconds:        Parallel speed-up, S:
=============   ====================     =========================================        =====================
1                       1                               0.005403377                                1
1                       2                               0.00396907                             1.361371052
1                       4                               0.003644917                            1.482441713                                                                          
1                       8                               0.002369157                            2.28071715                            
2                       16                              0.0218562                              0.247223991                                                     
2                       32                              0.041313367                            0.130790042                                           

Please state the number of cores per machine (for Bragg 2.05, this is typically 12):
12 (both)

A brief interpretation of your results:
On one machine, increasing the number of processes improves the value of S as we would expect. This indicates the parallel implementation is working effectively.
When the number of machines increases from 1 to 2 we see a big jump in execution time. Working with 2 machines is actually slower than running in serial. This indicates there is increased overhead when communicating between machines. Increasing the number of machines on 2 machines from 16 to 32 greatly reduces the value of S, emphasising the overhead of inter-node communication.
