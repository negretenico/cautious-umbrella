# cautious-umbrella

1. Lost-update counter (atomicity)

Start 8 threads, and have each one increment a shared counter 10M times.
Try four versions: a plain int, a volatile int, std::atomic with fetch_add, and a mutex. Compare the final counts and the timings.
Goal: explain in one sentence why volatile still loses updates.

2. Overcommit probe (demand paging and OOM)

malloc 4x your RAM and check the return value.
Write one byte per 4 KB page, and watch RSS in /proc/self/status climb until the OOM killer shows up in dmesg.
Segfault on purpose in a separate run, and compare the signal you get.
Bonus: rerun with vm.overcommit_memory=2 and see where it fails instead.

3. False-sharing benchmark (cache lines)

Have two threads each increment their own field of a shared struct.
Time it with the fields next to each other, then padded with alignas(std::hardware_destructive_interference_size).
Confirm the slowdown with perf stat -e cache-misses.

4. Priority inversion simulator (scheduling)

Pin three SCHED_FIFO threads to one core.
Low priority: takes a mutex and does slow work.
High priority: blocks on that same mutex.
Medium priority: busy-loops.
Measure how long the high-priority thread waits. Then switch the mutex to PTHREAD_PRIO_INHERIT and measure again.
This needs root or CAP_SYS_NICE.

5. Message-passing litmus test (reordering)

Run the flag-plus-data pattern millions of times with memory_order_relaxed, and count how often the reader sees stale data.
Switch to release/acquire and confirm the count drops to zero.
On x86 you’ll likely see zero stale reads either way. Explaining why is part of the exercise.
