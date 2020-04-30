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

struct reqArray{
    int* reqs;
    int size;
    int currblk;
};
typedef struct reqArray requests;

struct outputBlock{
    int reqtotal;
    int disttotal;
    float seektotal;
    float rotlattotal;
    float transtimetotal;
    float acctimetotal;
};
typedef struct outputBlock output;

void printRequests(requests* _requests){
    printf("\nPrinting Requests:\n");
    for(int i = 0; i < _requests->size; i++){
        printf("%d: %d\n", i, _requests->reqs[i]);
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

int getBlocks(requests* _requests, output* _results)
{//calculates seek time, rotational latency, and transfer time of each block request according to the FCFS scheduling algorithm.
    srand(time(NULL));
    int dist;
    float seek;
    float rotlat;
    float transtime = 0.031;
    float acctime;

    for(int req = 0; req < _requests->size; req++){
        printf("\n%d: get block %d\n", (req + 1), _requests->reqs[req]); 
        //required output
        printf("target head pos: %d\n", _requests[req]);
        printf("current head pos: %d\n", _requests->currblk);
        //calc head travel distance
        if(_requests->reqs[req] == -1)
        {//this is a signal to visit block 0, but not to read it
            //skip rotlat, transtime
            //add seek time
            //add acctime
        }
        else
            dist = abs(_requests->reqs[req] - _requests->currblk);
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

        _requests->currblk = _requests->reqs[req]; //update current position
    }
    return 0;
}

void scheduleSCAN(requests* _requests)
{//reorders the array of blocks into SCAN order.
    int i = 0; //general index for _requests->reqs array

    //sort given array least to greatest
    mergeSort(_requests->reqs, 0, (_requests->size - 1)); 
    // printRequests(_requests);

    //array for holding transformed requests list
    requests requests_;
    requests_.size = _requests->size;
    requests_.reqs = malloc(requests_.size);

    int rightblkindex;//save index of larger block # later
    //search sorted array for first block larger than _currentpos
    for(i; i < _requests->size; i++){
        if(_requests->reqs[i] > _requests->currblk)
        {//if any reqblk is greater than the currblk we have to visit zero block
            rightblkindex = i;//save index of larger block #
            i--;//move i back to smaller block #

            //resize reqs to fit order to visit zero block
            _requests->size += 1;
            int* inptresize = realloc(_requests->reqs, (_requests->size) * sizeof(int));
            _requests->reqs = inptresize;

            //resize outputreqs to fit _requests->reqs
            requests_.size += 1;
            int* otptresize = realloc(requests_.reqs, (requests_.size) * sizeof(int));
            requests_.reqs = otptresize;
            break;
        }
    }
    if (i >= _requests->size)
        i--;
    // printRequests(_requests);
    //count backwards through _requests, adding elements to requests_
    int j = 0; //new index for requests_
    while(i >= 0){
        requests_.reqs[j] = _requests->reqs[i];
        i--;
        j++;
    }

    if(j < requests_.size)
    {
        //i will be -1 here and inserting it signals getBlocks to visit, but skip reading block 0.
        requests_.reqs[j] = i;
        
        //start pulling blocks from _requests again, starting from rightblockindex onward
        i = rightblkindex; //reset i to larger block #
        for(i; i < _requests->size; i++){
            j++;
            requests_.reqs[j] = _requests->reqs[i];
        }
    }
    //copy outputreqests into _requests-> before returning to main
    for(i = 0; i < _requests->size + 1; i++){
        _requests->reqs[i] = requests_.reqs[i];
    }
}

void scheduleSSTF(requests* _requests)
{//reorders the array of blocks into Shortest-Seek-Time-First order.
    //array for holding transformed requests list
    requests requests_;
    requests_.size = _requests->size;
    requests_.reqs = malloc(requests_.size);
    int disttonextblock;
    int mindist;
    int minblkindex;

    for(int i = 0; i < requests_.size; i++)
    {
        mindist = __INT32_MAX__;//reset mindist
        for(int j = 0; j < requests_.size; j++)
        {//for each req, comapre to currentpos to find min dist block
            disttonextblock = abs(_requests->reqs[j] - _requests->currblk);
            if (disttonextblock < mindist)
            {//if new block's distance is less than current min
                mindist = disttonextblock; //move
                minblkindex = j;
            }
        }
        //set next block to min dist blk found
        requests_.reqs[i] = _requests->reqs[minblkindex];
        //move current position to that block
        _requests->currblk = requests_.reqs[i];
        //set element in old _requests array to INT_MAX so disttonextblock is maximal for that element, essentially skipping that request in the for loop
        _requests->reqs[minblkindex] = __INT32_MAX__;
    }

    for(int i = 0; i < requests_.size; i++)
    {//copy outputreqests into _requests to pass it back to main
        _requests->reqs[i] = requests_.reqs[i];
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

    mode SCHED_ALGO = (mode)SchedMode.Parse(typeof(mode), argv[3]);

    // // printf("%d\n", SCHED_ALGO);
    // FILE* reqfile = fopen(argv[4],"r"); //open file
    // if (reqfile == NULL){//file isn't open
    //         printf("\nCouldn't open page references file.\n");
    //         return 0;
    //     }
    //     else//file is open
    //         printf("\nPage references file opened.\n");

    // //grab each line from the text file
    // while (fgets(currentrequest, 10, reqfile) != NULL)
    //     __results.reqtotal++;//count # of requests

    // rewind(reqfile);//start over from beginning of file

    // //create reqArray
    // requests __requests;
    // __requests.reqs = malloc(0);
    // __requests.size = __results.reqtotal;
    // __requests.currblk = __startheadpos;
    // int* tempreqs = realloc(__requests.reqs, (__requests.size) * sizeof(int));
    // __requests.reqs = tempreqs;

    // int index = 0;
    // while (fgets(currentrequest, 10, reqfile) != NULL)
    // {//grab each line from the text file
    //     sscanf(currentrequest,"%d", &blocknum); //parse line
    //     __requests.reqs[index] = blocknum; //store in array
    //     index++;
    // }

    // // printRequests(requests, __results.reqtotal);
    // // reschedule requests into SSTF order
    // // scheduleSSTF(__startheadpos, requests, __results.reqtotal);
    // // printRequests(requests, __results.reqtotal);

    // printRequests(&__requests);
    // // reschedule requests into SCAN order
    // scheduleSCAN(&__requests);
    // printRequests(&__requests);

    // // do calculations on request string
    // // getBlocks(__startheadpos, requests, &__results); 

    // fclose(reqfile);

    // // printf("",__results.);
    // // printf("\nFINAL RESULTS\n");
    // // printf("Total disk requests: %d\n",__results.reqtotal);
    // // printf("Total disk head movement: %d\n",__results.disttotal);
    // // printf("Total seek time: %d\n",__results.seektotal);
    // // printf("Total rotation latency time: %d\n",__results.rotlattotal);
    // // printf("Total transfer time: %d\n",__results.transtimetotal);
    // // printf("Total access time: %d\n",__results.acctimetotal);
    // free(__requests.reqs);
    return 0;
}


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