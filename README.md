# 4103_CLOCK-page-replacement
#Name: Nicklaus Settoon
#Project: PA-3 (Disk Scheduling Algorithm)
#File: README.md
#Instructor: Feng Chen
#Class: cs4103-sp20
#LogonID: cs410361

This is a program which simulates three disk scheduling algorithms a HDD with parametered conditions.

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
REQ#: GET BLOCK(BLK#)
current head pos: integer
target head pos: integer
distance to target: integer
seek time: float
rotational latency: float
transfer time: float
access time: float

ORDER OF SORTED REQUESTS:
REQ#0: BLK#
REQ#1: BLK#
...
REQ#n: BLK#

FINAL RESULTS:
Results valid up to block FINAL_READ_BLK.
Total disk requests: integer
Total disk head movement: integer
Total seek time: float
Total rotation latency time: float
Total transfer time: float
Total access time: float