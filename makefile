#Name: Nicklaus Settoon
#Project: PA-2 (Page Replacement Algorithm)
#File: makefile
#Instructor: Feng Chen
#Class: cs4103-sp20
#LogonID: cs410361

clock: main.o
	gcc -g main.o -o clock

main.o: main.c
	gcc -c main.c

full:
	make
	./clock 20 pageref.txt 1 10 20

small:
	make
	./clock 20 pageref-small.txt 1 10 20

clean: 
	rm *.o clock

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