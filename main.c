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
#include <ctype.h>

enum SchedMode {FCFS = 0, SSTF, SCAN};
typedef enum SchedMode mode;

int main(int argcount, char* argv[])
{//the main train.
    
    int __currentheadpos = atoi(argv[1]);
    const int MAX_DISK_BLOCKS = atoi(argv[2]);

    printf("%s\n", argv[3]);

    // mode SCHED_ALGO = (mode)enum.Parse(typeof(mode), argv[3]);

    // printf("%d\n", SCHED_ALGO);

    // switch (SCHED_ALGO)
    // {
    // case FCFS:
    //     printf("Using FCFS algo.");
    //     break;

    // case SSTF:
    //     printf("Using SSTF algo.");
    //     break;

    // case SCAN:
    //     printf("Using SCAN algo.");
    //     break;
    
    // default:
    //     printf("Did not recognize algorithm type.");
    //     break;
    // }

    // FILE* requests = fopen(argv[4],"r");

    // if (requests == NULL){//file isn't open
    //         printf("\nCouldn't open page references file.\n");
    //         return 0;
    //     }
    //     else//file is open
    //         printf("\nPage references file opened.\n");

    char currentrequest[10];
    int __totalreqcount = 0;
    int blocknum;

    // while (fgets(currentrequest, 10, requests) != NULL)
    //     {//grab each block request line from the text file
    //         __totalreqcount++;
    //         printf("\n%d: %s", __totalreqcount, currentrequest); //print the line
    //         sscanf(currentrequest,"%d", &blocknum); //parse line
    //         printf("Blocknum: %d\n", blocknum);
    //     }

    return 0;
}