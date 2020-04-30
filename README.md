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

RUN COMMANDS:
format: disk CURRENT_HEAD_POS MAX_LOGICAL_BLKS SCHED_ALGO REQUESTS_FILE.txt
example: $ ./disk 53 200 FCFS requests.txt

run example with fcfs algo
$ make fcfs

run example with sstf algo
$ make sstf

run example with scan algo
$ make scan

run scan algo, but requests are all in front of CURRENT_HEAD_POS
$ make ahead

run scan algo, but requests are all behind CURRENT_HEAD_POS
$ make behind

run fcfs with requests outside of the MAX_LOGICAL_BLKS
$ make outofbounds

OUTPUT:
