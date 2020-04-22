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
$ make fcfs
$ make sstf
$ make scan

disk CURRENT_HEAD_POS MAX_LOGICAL_BLKS_ON_DISK SCHED_ALGO REQUESTS_FILE.txt
$ ./disk 53 200 FCFS requests.txt

OUTPUT:
