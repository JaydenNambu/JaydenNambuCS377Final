# CS 377 Final

[Video](https://github.com/JaydenNambu/JaydenNambuCS377Final/new/master)\
For my final project I expanded the scheduler from project 3 to try and implement MLFQ scheduling.\
I added burst_time and off_time time atttributes to processes, burst represents how long an interactive process takes before switching off the CPU 
while off_time represents how long the process waits for input, it is assumed that burst and off times are consistent for each process.
Non interactive processes simply have a burst_time as long or longer than their duration, so they cannot surpass the burst timer.




Hi my name is Jayden Nambu, and I ampesenting my fnal project for operating systems
I tried to extend the scheduler homework to include funcctionality for MLFQ.
