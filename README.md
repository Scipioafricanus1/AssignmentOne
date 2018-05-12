# AssignmentOne
Process Scheduler.

Objective of assignment: read in an input file using I/O redirection. The file contains the number of cores a processor in NCORES X
where X is the number of cores.
NEW X means new process that starts at X time since the start of the program.
CORE X means a process' instruction is to use one of the cores as long as there is an open core left to use.
SSD X means a process is to use the SSD for X amount of milliseconds.
INPUT X means a similar thing as the previous two.
The next time a NEW comes, a new process begins.

Actual time is not actually being used, but instead we are using a sum of each new process' times. The final solution is nondeterministic.
Each new process is monotonically increasing in arrival time, but even if this were not the case, I could sort the processes in
my process table by the arrival time of the NEW process and it would work fine.
We assume there is no errors in the input.


- If there are no cores left when a core request begins, the process is put in a ready queue and put in READY state.
- If the SSD is being used, the process is put into a BLOCKED state and its own queue.
- If the INPUT is being used, the process is put into a BLOCKED state as well and put into its own queue.
