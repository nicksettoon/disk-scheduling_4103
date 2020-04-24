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

// enum SchedMode {FCFS = 0, SSTF, SCAN};
// typedef enum SchedMode mode;

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
        printf("\n%d: get block %d\n", (req + 1), _requests[req]); 
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

void scheduleSSTF(int _currentpos, int _requests[], int _reqcount)
{//reorders the array of integers in Shortest-Seek-Time-First order.
    int requests[_reqcount];
    int disttonextblock;
    int mindist;
    int minblkindex;

    for(int i = 0; i < _reqcount; i++)
    {
        mindist = __INT32_MAX__;//reset mindist
        for(int j = 0; j < _reqcount; j++)
        {//for each req, comapre to currentpos to find min dist block
            disttonextblock = abs(_requests[j] - _currentpos);
            if (disttonextblock < mindist)
            {//if new block's distance is less than current min
                mindist = disttonextblock; //move
                minblkindex = j;
            }
        }
        //set next block to min dist blk found
        requests[i] = _requests[minblkindex];
        //move current position to that block
        _currentpos = requests[i];
        //set element in old _requests array to INT_MAX so disttonextblock is maximal for that element, essentially skipping that request in the for loop
        _requests[minblkindex] = __INT32_MAX__;
    }

    for(int i = 0; i < _reqcount; i++){
        _requests[i] = requests[i];
    }
}

void printRequests(int _requests[], int _count){
    printf("\nPrinting Requests:\n");
    for(int i = 0; i < _count; i++){
        printf("%d: %d\n", i, _requests[i]);
    }
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

    printRequests(requests, __results.reqtotal);
    // reschedule requests into SSTF order
    scheduleSSTF(__startheadpos, requests, __results.reqtotal);
    printRequests(requests, __results.reqtotal);

    // do calculations on request string
    // getBlocks(__startheadpos, requests, &__results); 

    fclose(reqfile);

    // printf("",__results.);
    // printf("\nFINAL RESULTS\n");
    // printf("Total disk requests: %d\n",__results.reqtotal);
    // printf("Total disk head movement: %d\n",__results.disttotal);
    // printf("Total seek time: %d\n",__results.seektotal);
    // printf("Total rotation latency time: %d\n",__results.rotlattotal);
    // printf("Total transfer time: %d\n",__results.transtimetotal);
    // printf("Total access time: %d\n",__results.acctimetotal);
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