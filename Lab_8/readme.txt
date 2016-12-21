Vamsi Mokkapati
404-464-206
CS 35L Homework 8
TA: Sharath Gopal

To parallelize my implementation of main.c, I used a three
dimensional array called picSegment that contained the three
floating points that represent color. This array stored all the
scaled_color values for each pixel value for width and height
so that it could be printed out separately in the main function
after creating and joining together the threads.

First, I made my thread function from the code initially given within
the main function, and modified it so that it can perform the 
parallelization. I implemented my code such that given n threads,
every column would be taken care of by the (column % n)th 
thread; for example, if there are four threads, columns 0, 1, 2, and 
3 will be taken care of each of the 4 threads respectively, while 
column 4 will be implemented by first thread, and so on. To do this, 
I simply modified the outermost for loop out of the sequence of four 
nested for loops and made sure px initially receives the thread number, 
and is incremented by globalNthreads, which is a variable containing the
value of how many threads are being used. 

An issue I ran into while writing this program was that initially, I
wasn’t able to successfully use my value of how many threads there 
were, nthreads, within my thread function. This value is needed, since
each thread has to jump “nthreads” columns to arrive at the next 
column it will work on. To fix this, I made a different global 
variable and set it equal to the nthreads value from the main function.
Then I used the global variable globalNthreads in my threadFunc.

After running my program with “make clean check”, I arrived at the 
following results for how well my parallel implementation of SRT 
improves performance:

1 thread:
real 0m58.750s
user 0m58.743s
sys  0m0.001s

2 threads:
real 0m29.029s
user 0m57.058s
sys  0m0.001s

4 threads:
real 0m15.289s
user 0m59.174s
sys  0m0.003s

8 threads:
real 0m8.935s
user 1m4.894s
sys  0m0.004s

It is evident that the parallelization of the task is successful, 
since the real time approximately halves as the number of threads
double. This happens since the image is split into approximately 
equal sections for each thread to work on, with no dependencies 
from one pixel to another; therefore the ray tracing
problem can be seen to be embarrassingly parallel. While the real 
time decreases exponentially due to parallelization, it can be seen 
that the user and sys times generally increase in small amounts; this
is because more time is spent in user mode when more threads are 
used. Since the CPU spends more time in kernel when more threads
are used, the sys time also increases as thread number increases.

