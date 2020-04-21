/*
Name: Nicklaus Settoon
Project: PA-3 (Disk Scheduling Algorithm)
File: main.c
Instructor: Feng Chen
Class: cs4103-sp20
LogonID: cs410361
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argcount, char* argv[])
{//the main train.
     
    FILE* requests = fopen(argv[2],"r");

    if (requests == NULL){//file isn't open
            printf("Couldn't open page references file.\n");
            return 0;
        }
        else//file is open
            printf("Page references file opened.\n");

    char currentrequest[10];
    int __totalreqcount;
    int blocknum;

    while (fgets(currentrequest, 10, requests) != NULL)
        {//grab each block request line from the text file
            printf("\n%d", __totalreqcount, currentrequest); //print the line
            sscanf(currentrequest,"%d", &blocknum); //parse line
        }

    return 0;
}