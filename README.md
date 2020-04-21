# 4103_CLOCK-page-replacement
#Name: Nicklaus Settoon
#Project: PA-3 (Disk Scheduling Algorithm)
#File: README.md
#Instructor: Feng Chen
#Class: cs4103-sp20
#LogonID: cs410361

This is a program which simulates a clock page replacement algorithm on a machine with parametered conditions.

COMPILATION:
$ make

RUN:
$ ./clock avail_page_frames page_references.txt page_access_time page_swapin_time page_swapout_time
    e.g. $ ./clock 15 pageref.txt 1 10 20

OUTPUT:

Page References: simple count of how many page references were made. Should be the number of lines in the file.

Page Faults on READ: number of page misses on read references.

Page Faults on WRITE: number of page misses on write references.

Cummulative memory access time: total time spent accessing pages(incremented on page hit and on page swap in)

Cummulative time spent swapping pages in: total time spent waiting for page arrival in memory.

Cummulative time spent swapping pages out: total time spent waiting for page write out to disk.
