## Answers

1.  List all of the main states a process may be in at any point in time on a
    standard Unix system. Briefly explain what each of these states mean.

    START: initial state
    READY: ready to be "taken on" by a `process`or
    RUNNING: being `process`ed
    WAITING: async waiting
    EXIT: finished or terminated, waiting to be removed from main mem

2.  What is a Zombie Process? How does it get created? How does it get destroyed?

    Zombie processes are processes that exit but haven't been "processed" by
    its parent. This happens because the parent hasn't called `wait()` on the
    child process that exited (the "zombie"). Zombie processes are destroyed
    when its parent process reads its exit code (`wait()`).

3.  Describe the job of the Scheduler in the OS in general.

    The Scheduler is in charge of allocating CPU work to queue(s), which in turn
    manage processes. The effect is an OS being able to "multitask".

4.  Describe the benefits of the MLFQ over a plain Round-Robin scheduler.

    With MLFQ, the turnaround time for each process having CPU time is reduced.
    This is done through priority level queues versus just round-robin-ing
    each process for set amounts of CPU time.
