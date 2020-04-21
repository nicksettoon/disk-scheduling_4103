#Name: Nicklaus Settoon
#Project: PA-3 (Disk Scheduling Algorithm)
#File: makefile
#Instructor: Feng Chen
#Class: cs4103-sp20
#LogonID: cs410361

disk: main.o
	gcc -g main.o -o disk

main.o: main.c
	gcc -c main.c

fcfs:
	make
	./disk 53 200 FCFS requests.txt

sstf:
	make
	./disk 53 200 SSTF requests.txt

scan:
	make
	./disk 53 200 SCAN requests.txt

clean: 
	rm *.o disk

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