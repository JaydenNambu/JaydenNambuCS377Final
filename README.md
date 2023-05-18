# CS 377 Final

[Video](https://drive.google.com/file/d/13_C--NcQAo1fmX0_PP2E3Poifqy4EjkA/view)\
For my final project I expanded the scheduler from project 3 to try and implement MLFQ scheduling.\
I added burst_time and off_time time atttributes to processes, burst represents how long an interactive process takes before switching off the CPU 
while off_time represents how long the process waits for input, it is assumed that burst and off times are consistent for each process.
Non interactive processes simply have a burst_time as long or longer than their duration, so they cannot surpass the burst timer.\
I was unable to successfully implement interactive jobs in my MLFQ and have split my attempts into two functions. MLFQ and interactiveMLFQ. 
MLFQ ignores interactivity which results in it perforing the same as round robin, while interactiveMLFQ attempts to address this, but errors out when ran. 
