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
#include <time.h>

enum SchedMode {FCFS = 0, SSTF, SCAN};
typedef enum SchedMode mode;

struct outputBlock{
    int reqtotal;
    int disttotal;
    float seektotal;
    float rotlattotal;
    float transtimetotal;
    float acctimetotal;
};
typedef struct outputBlock output;

int getBlocks(int _currentpos, int _requests[], output* _results)
{//calculates seek time, rotational latency, and transfer time of each block request according to the FCFS scheduling algorithm.
    srand(time(NULL));
    int dist;
    float seek;
    float rotlat;
    float transtime = 0.031;
    float acctime;


    for(int req = 0; req < _results->reqtotal; req++){
        printf("\nRequest %d: get block %s\n", (req + 1), _requests[req]); //print the line
        //required output
        printf("target head pos: %d\n", _requests[req]);
        printf("current head pos: %d\n", _currentpos);
        //calc head travel distance
        dist = abs(_requests[req] - _currentpos);
        _results->disttotal += dist;
        printf("distance to target: %d\n", dist);

        //calculate seek time
        seek = dist * 0.1;
        _results->seektotal += seek;
        printf("seek time: %f\n", seek);

        //calc rotational latency
        rotlat = ((float)rand()/(float)(RAND_MAX)) * 8.3;
        _results->rotlattotal += rotlat;
        printf("rotational latency: %f\n", rotlat);

        //transfer time
        _results->transtimetotal += transtime;
        printf("transfer time: %f\n", transtime);

        //calc access time
        acctime = seek + rotlat + transtime;
        _results->acctimetotal += acctime;
        printf("access time: %f\n", acctime);

        _currentpos = _requests[req]; //update current position
    }

    return 0;
}

int main(int argcount, char* argv[])
{//the main train.
    
    int __startheadpos = atoi(argv[1]);
    const int MAX_BLOCK_ADDR = (atoi(argv[2]) - 1);
    printf("%s\n", argv[3]);

    FILE* reqfile = fopen(argv[4],"r");
    if (reqfile == NULL){//file isn't open
            printf("\nCouldn't open page references file.\n");
            return 0;
        }
        else//file is open
            printf("\nPage references file opened.\n");

    output __results;
    __results.reqtotal = 0;
    char currentrequest[10];
    int blocknum;

    //grab each block request line from the text file
    while (fgets(currentrequest, 10, reqfile) != NULL)
        __results.reqtotal++;

    rewind(reqfile);//start over from beginning of file
    int requests[__results.reqtotal]; //create array for requests

    int index = 0;
    while (fgets(currentrequest, 10, reqfile) != NULL)
    {//grab each block request line from the text file
        sscanf(currentrequest,"%d", &blocknum); //parse line
        requests[index] = blocknum; //store in array
        index++;
    }

    getBlocks(__startheadpos, requests, &__results); //do calculations on request string

    fclose(reqfile);
    return 0;
}

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