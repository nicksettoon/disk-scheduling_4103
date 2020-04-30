/*
Name: Nicklaus Settoon
Project: PA-3 (Disk Scheduling Algorithm)
File: main.c
Instructor: Feng Chen
Class: cs4103-sp20
LogonID: cs410361
*/

//IMPORTS//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

//ENUMS//
enum SchedMode {FCFS = 0, SSTF, SCAN};
typedef enum SchedMode mode;

//STRUCTS//
const static struct {
    mode diskalgo;
    const char* name;
} mode_map [] = {
    {0, "FCFS"},
    {0, "fcfs"},
    {1, "SSTF"},
    {1, "sstf"},
    {2, "SCAN"},
    {2, "scan"},
};

struct reqArray{
    int* reqs;
    int size;
    int currblk;
    int maxblk;
};
typedef struct reqArray requests;
void reqPrt(requests* _requests)
{//prints a reqArry struct
    // printf("\nPrinting Requests:\n");
    for(int i = 0; i < _requests->size; i++){
        printf("%d: %d\n", i, _requests->reqs[i]);
    }
}

struct outputBlock{
    int reqtotal;
    int disttotal;
    float seektotal;
    float rotlattotal;
    float transtimetotal;
    float acctimetotal;
};
typedef struct outputBlock output;
void resPrt(output* _results)
{//prints the outputBlock struct data
    printf("Total disk requests: %d\n",_results->reqtotal);
    printf("Total disk head movement: %d\n",_results->disttotal);
    printf("Total seek time: %f\n",_results->seektotal);
    printf("Total rotation latency time: %f\n",_results->rotlattotal);
    printf("Total transfer time: %f\n",_results->transtimetotal);
    printf("Total access time: %f\n",_results->acctimetotal);
}

//FUNCs//
int getBlocks(requests* _requests, output* _results);//calc block accesses
void scheduleSSTF(requests* _requests);//reorders requests into SSTF order
//reorders requests into SCAN order (headed to lower address)
void scheduleSCAN(requests* _requests);
void mergeSort(int arr[], int _left, int _right);//sorts list for SCAN
void merge(int arr[], int _left, int _mid, int _right);//merges array for sort

//MAIN//
int main(int argcount, char* argv[])
{//the main train
    int __startheadpos = atoi(argv[1]); //starting head pos for simulation
    const int MAX_BLOCK_ADDR = (atoi(argv[2]) - 1); //max # of blocks
    output __results; //results struct for tracking
    //initialize values to 0
    __results.reqtotal = 0;
    __results.disttotal = 0;
    __results.seektotal = 0;
    __results.rotlattotal = 0;
    __results.transtimetotal = 0;
    __results.acctimetotal = 0;
    char currentrequest[10]; //var for reading in line of txt file
    int blocknum; //int holder for currentrequest's block #

    //map input string in argv[3] to desired algorithm
    int modeset = -1;
    for(int j = 0; j < sizeof (mode_map)/sizeof(mode_map[0]); ++j)
        if (!strcmp(argv[3],mode_map[j].name))
            modeset = mode_map[j].diskalgo;
    if(modeset == -1)
    {
        printf("%s is not a valid disk scheduling algo.\n", argv[3]);
        return 0;
    }
    
    FILE* reqfile = fopen(argv[4],"r"); //open file
    if (reqfile == NULL){//file isn't open
            printf("\nCouldn't open page references file.\n");
            return 0;
        } else//file is open
            printf("\nPage references file successfully opened.\n");

    // grab each line from the text file
    while (fgets(currentrequest, 10, reqfile) != NULL)
        __results.reqtotal++;//count # of requests

    rewind(reqfile);//start over from beginning of file

    //create reqArray
    requests __requests;
    __requests.reqs = malloc(0);
    __requests.size = __results.reqtotal;
    __requests.currblk = __startheadpos;
    int* tempreqs = realloc(__requests.reqs, (__requests.size) * sizeof(int));
    __requests.reqs = tempreqs;
    __requests.maxblk = MAX_BLOCK_ADDR;

    int index = 0;
    while (fgets(currentrequest, 10, reqfile) != NULL)
    {//grab each line from the text file
        sscanf(currentrequest,"%d", &blocknum); //parse line
        __requests.reqs[index] = blocknum; //store in array
        index++;
    }

    int oob;
    printf("\nREQUESTS BEFORE RE-ORDER:\n");
    switch (modeset)
    {
    case 0://FCFS is just FIFO queue, so no req array manipulation
        printf("Using FCFS algo.\n");
        reqPrt(&__requests);
        oob = getBlocks(&__requests, &__results); 
        break;

    case 1://reschedule requests into SSTF order then calc
        printf("Using SSTF algo.\n");
        reqPrt(&__requests);
        scheduleSSTF(&__requests);
        oob = getBlocks(&__requests, &__results); 
        break;

    case 2://reschedule requests into SCAN order then calc
        printf("Using SCAN algo.\n");
        reqPrt(&__requests);
        scheduleSCAN(&__requests);
        oob = getBlocks(&__requests, &__results); 
        break;
    }

    if(oob)
        printf("\nReached out of bounds block %d.\n", __requests.reqs[oob-1]);
    else
        oob = __requests.size;

    printf("\nORDER OF SORTED REQUESTS:\n");
    reqPrt(&__requests);

    printf("\nFINAL RESULTS: (valid up to and including block %d)\n",__requests.reqs[oob-1]);
    resPrt(&__results);

    //cleanup
    fclose(reqfile);
    free(__requests.reqs);
    return 0;
}

int getBlocks(requests* _requests, output* _results)
{//calculates seek time, rotational latency, and transfer time of each block request in the order given.
    srand(time(NULL));
    int dist;
    int target;
    float seek;
    float rotlat;
    float transtime = 0.031;
    float acctime;
    int visitzero = 0;

    for(int req = 0; req < _requests->size; req++){
        printf("\n%d: GET BLOCK(%d)\n", (req + 1), _requests->reqs[req]); 
        if(_requests->reqs[req] == -1)
        {//this is a signal from SCAN algo to visit block 0, but not to read it
            visitzero = 1;
            target = 0;
        }
        else if(_requests->reqs[req] > _requests->maxblk)
        {
            printf("Target block is greater than max disk blocks.");
            return req;
        }
        else
            target = _requests->reqs[req];
        printf("current head pos: %d\n", _requests->currblk);
        printf("target head pos: %d\n", target);

        //calc head travel distance
        dist = abs(target - _requests->currblk);
        _results->disttotal += dist;
        printf("distance to target: %d\n", dist);
        //calculate seek time
        seek = dist * 0.1;
        _results->seektotal += seek;
        printf("seek time: %f\n", seek);

        if(!visitzero)
        {
            //calc rotational latency
            rotlat = ((float)rand()/(float)(RAND_MAX)) * 8.3;
            _results->rotlattotal += rotlat;
            printf("rotational latency: %f\n", rotlat);

            //add transfer time
            _results->transtimetotal += transtime;
            printf("transfer time: %f\n", transtime);

            //calc access time
            acctime = seek + rotlat + transtime;
            _results->acctimetotal += acctime;
            printf("access time: %f\n", acctime);
            _requests->currblk = _requests->reqs[req]; //update current position
        }
        else
        {
            printf("Visiting block 0.\nNo rotational latency or transfer time.\n");
            //calc access time
            acctime = seek;
            _results->acctimetotal += acctime;
            printf("access time: %f\n", acctime);
            _requests->currblk = 0; //update current position to block 0
            visitzero = 0;
        }
    }
    return 0;
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
    //clean up
    free(requests_.reqs);
}

void scheduleSCAN(requests* _requests)
{//reorders the array of blocks into SCAN order.
    int i = 0; //general index for _requests->reqs array

    //sort given array least to greatest
    mergeSort(_requests->reqs, 0, (_requests->size - 1)); 
    // reqPrt(_requests);

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
    // reqPrt(_requests);
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
    //clean up
    free(requests_.reqs);
}


void mergeSort(int arr[], int _left, int _right)
{//sorts a given array from least to greatest given its left and right endpoints
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

void merge(int arr[], int _left, int _mid, int _right) 
{//merges the left and right array
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