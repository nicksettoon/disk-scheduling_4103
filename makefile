#Name: Nicklaus Settoon
#Project: PA-3 (Disk Scheduling Algorithm)
#File: makefile
#Instructor: Feng Chen
#Class: cs4103-sp20
#LogonID: cs410361

main: main.o
	gcc -g main.o -o main

main.o: main.c
	gcc -c main.c

fcfs:
	make
	./main 53 200 FCFS requests.txt

sstf:
	make
	./main 53 200 SSTF requests.txt

#SCAN VARIATIONS
scan:
	make
	./main 53 200 SCAN requests.txt
# r:
# 	make
# 	./main 53 200 SCAN scan-right.txt

# l:
# 	make
# 	./main 53 200 SCAN scan-left.txt


clean: 
	rm *.o main

submit:
	make clean
	rm .*
	rm *.pdf
	rm *.code-workspace

git:
	make clean
	git add -A
	git commit -m $(msg)
	git push