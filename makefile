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

#SCAN VARIATIONS
scan:
	make
	./disk 53 200 SCAN requests.txt

ahead:
	make
	./disk 53 200 SCAN scan-ahead.txt

behind:
	make
	./disk 53 200 SCAN scan-behind.txt

#OUT OF BOUNDS
oob:
	make
	./disk 53 200 FCFS error.txt

clean: 
	rm *.o disk

finalsubmission:
	make clean
	rm .*
	rm *.pdf
	rm *.code-workspace
	rm .gitignore
	rm -rf .vscode
	rm -rf .git

git:
	make clean
	git add -A
	git commit -m $(msg)
	git push