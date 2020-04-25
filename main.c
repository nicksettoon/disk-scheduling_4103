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

void printRequests(int _requests[], int _count){
    printf("\nPrinting Requests:\n");
    for(int i = 0; i < _count; i++){
        printf("%d: %d\n", i, _requests[i]);
    }
}

void merge(int arr[], int _left, int _mid, int _right) 
{ 
    int i, j, k; 
    int n1 = _mid - _left + 1; 
    int n2 =  _right - _mid; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[_left + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[_mid + 1+ j]; 
  
    /* Merge the temp arrays back into arr[_left.._right]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = _left; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
/* _left is for left index and _right is right index of the 
   sub-array of arr to be sorted */
void mergeSort(int arr[], int _left, int _right) 
{ 
    if (_left < _right) 
    { 
        // Same as (_left+_right)/2, but avoids overflow for 
        // large _left and h 
        int _mid = _left+(_right-_left)/2; 
  
        // Sort first and second halves 
        mergeSort(arr, _left, _mid); 
        mergeSort(arr, _mid+1, _right); 
  
        merge(arr, _left, _mid, _right); 
    } 
} 

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

void scheduleSCAN(int _currentpos, int _requests[], int _reqcount)
{//reorders the array of blocks into SCAN order.
    int i = 0; //general index for _requests array

    //sort given array least to greatest
    mergeSort(_requests, 0, (_reqcount - 1));

    //add one element to array to signal getBlocks to visit beginning of disk, but not read.
    int* tempreq = realloc(_requests, (_reqcount+1) * sizeof(int));
    _requests = tempreq;
    int outputrequests[_reqcount+1];//output array must be same size

    //search sorted array for first block larger than _currentpos
    for(i; i < _reqcount; i++){
        if(_requests[i] > _currentpos)
            break;
    }
    int rightblkindex = i;//save index of larger block #

    //count backwards through _requests, adding elements to outputrequest
    i--;//move i back to lower block
    int j = 0; //new index for outputrequests
    while(i >= 0){
        outputrequests[j] = _requests[i];
        i--;
        j++;
    }

    //i will be -1 here and inserting it signals getBlocks to visit, but skip reading block 0.
    outputrequests[j] = i;
    
    //start pulling blocks from _requests again, starting from rightblockindex onward
    i = rightblkindex; //reset i to larger block #
    for(i; i < _reqcount; i++){
        j++;
        outputrequests[j] = _requests[i];
    }

    //copy outputreqests into _requests to pass it back to main
    for(i = 0; i < _reqcount+1; i++){
        _requests[i] = outputrequests[i];
    }
}

void scheduleSSTF(int _currentpos, int _requests[], int _reqcount)
{//reorders the array of blocks into Shortest-Seek-Time-First order.
    int outputrequests[_reqcount];
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
        outputrequests[i] = _requests[minblkindex];
        //move current position to that block
        _currentpos = outputrequests[i];
        //set element in old _requests array to INT_MAX so disttonextblock is maximal for that element, essentially skipping that request in the for loop
        _requests[minblkindex] = __INT32_MAX__;
    }

    for(int i = 0; i < _reqcount; i++)
    {//copy outputreqests into _requests to pass it back to main
        _requests[i] = outputrequests[i];
    }
}


int main(int argcount, char* argv[])
{//the main train.
    int __startheadpos = atoi(argv[1]); //starting head pos for simulation
    const int MAX_BLOCK_ADDR = (atoi(argv[2]) - 1); //max # of blocks
    output __results; //results struct for tracking
    __results.reqtotal = 0; //init # of req to 0
    char currentrequest[10]; //var for reading in line of txt file
    int blocknum; //int holder for currentrequest's block #
    // printf("%s\n", argv[3]);

    FILE* reqfile = fopen(argv[4],"r"); //open file
    if (reqfile == NULL){//file isn't open
            printf("\nCouldn't open page references file.\n");
            return 0;
        }
        else//file is open
            printf("\nPage references file opened.\n");

    //grab each line from the text file
    while (fgets(currentrequest, 10, reqfile) != NULL)
        __results.reqtotal++;//count # of requests

    rewind(reqfile);//start over from beginning of file
    int* requests = malloc(__results.reqtotal * sizeof(int)); //create array for requests

    int index = 0;
    while (fgets(currentrequest, 10, reqfile) != NULL)
    {//grab each line from the text file
        sscanf(currentrequest,"%d", &blocknum); //parse line
        requests[index] = blocknum; //store in array
        index++;
    }

    // printRequests(requests, __results.reqtotal);
    // reschedule requests into SSTF order
    // scheduleSSTF(__startheadpos, requests, __results.reqtotal);
    // printRequests(requests, __results.reqtotal);

    printRequests(requests, __results.reqtotal);
    // reschedule requests into SCAN order
    scheduleSCAN(__startheadpos, requests, __results.reqtotal);
    printRequests(requests, __results.reqtotal+1);

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