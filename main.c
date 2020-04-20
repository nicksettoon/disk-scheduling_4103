/*
Name: Nicklaus Settoon
Project: PA-2 (Page Replacement Algorithm)
File: main.c
Instructor: Feng Chen
Class: cs4103-sp20
LogonID: cs410361
*/

#include <stdio.h>
#include <stdlib.h>

int __totalmemaccesstime = 0;

struct page{
    int data;
    int pagenum;
};
typedef struct page pg;

pg* makePage(int _data, int _pagenum)
{//creates a new page object with the give parameters
    pg* newpage = malloc(sizeof *newpage);
    newpage->data = _data;
    newpage->pagenum = _pagenum;

    return newpage;
}

struct page_table_entry{
    struct page* pgaddr;
    unsigned int dirty : 1;
    unsigned int ref : 1;
    struct page_table_entry* prventry;
    struct page_table_entry* nxtentry;
};
typedef struct page_table_entry pgentry;

struct page_table{
    int currlength;
    int maxlength;
    unsigned int full : 1;
    pgentry* head;
    pgentry* tail;
};
typedef struct page_table table;

pgentry* insertPage(table* _tabletarget, pg* _pagetarget, pgentry* _prventry)
{//creates an page table entry for the target page and inserts it after the target entry
    if (_tabletarget->full == 1) {//if page table is full
        printf("Cannot insert page: %d. Table is full.\n", _pagetarget->pagenum);
        return NULL;
    }

    //printf("Inserting page: %d", _pagetarget->pagenum);
    //make new page entry object
    pgentry* newentry = malloc(sizeof *newentry);
    newentry->pgaddr = _pagetarget;
    newentry->dirty = 0;
    newentry->ref = 0;

    if (_tabletarget->head == NULL){//if table is empty
        _tabletarget->head = newentry;
        newentry->prventry = NULL;
        newentry->nxtentry = NULL;
    }
    else if (_tabletarget->tail == NULL)
    {//if table only has a head
        _tabletarget->tail = newentry;
        newentry->prventry = _tabletarget->head;
        newentry->nxtentry = _tabletarget->head;
        _tabletarget->head->prventry = newentry;
        _tabletarget->head->nxtentry = newentry;
    }
    else
    {
        //fix newentry's pointers
        newentry->prventry = _prventry;
        newentry->nxtentry = _prventry->nxtentry;
        //fix entries to left and right of newentry
        _prventry->nxtentry = newentry;
        newentry->nxtentry->prventry = newentry;
    }

    //update table metadata
    _tabletarget->currlength++;
    if (_tabletarget->currlength == _tabletarget->maxlength)
        _tabletarget->full = 1;
    
    return newentry;
}

void deletePageEntry(table* _tabletarget, pgentry* _entrytarget)
{
    //temp store appropriate pointers
    pg* page = _entrytarget->pgaddr;
    pgentry* prv = _entrytarget->prventry;
    pgentry* nxt = _entrytarget->nxtentry;

    //stitch linked list together
    prv->nxtentry = nxt;
    nxt->prventry = prv;
    _tabletarget->currlength--;
    _tabletarget->full = 0;

    //if deleting the head of the linked list
    if(_tabletarget->head = _entrytarget)
        _tabletarget->head = nxt;

    //if deleting the tail of the linked list
    if(_tabletarget->tail = _entrytarget)
        _tabletarget->tail = prv;

    //free memory for page and its entry
    free(page);
    free(_entrytarget);
}

pgentry* findPage(table* _tabletarget, int _pagetarget, int _memaccesscost)
{//searches page table and returns page entry if found, NULL if not found.

    pgentry* currentry = _tabletarget->head;
    unsigned int found = 0;

    do{
        __totalmemaccesstime += _memaccesscost;
        if(currentry == NULL)
            return NULL;
            
        if (currentry->pgaddr->pagenum == _pagetarget) {//if find page entry with correct page number
            found = 1;
            break;
        }
        else //move to next entry
            currentry = currentry->nxtentry;
    } while (currentry != _tabletarget->head);
    
    if (found == 1)
        return currentry;
    else
        return NULL;
}

pgentry* findVictimPage(table* _tabletarget, pgentry* _clockstart, int _memaccesscost)
{//CLOCK algorithm implementation for finding least recently used page for eviction from memory.
    pgentry* clockhand;
    if (_clockstart == NULL)
        clockhand = _tabletarget->head;
    else
        clockhand = _clockstart;
    unsigned int found = 0;

    do{
        __totalmemaccesstime += _memaccesscost;
        if (clockhand->ref == 0) {//if find page entry with correct page number
            found = 1;
            break;
        }
        else
        {
            clockhand->ref = 0; //set ref bit to 0
            clockhand = clockhand->nxtentry;
        }
    } while(1);
    
    if (found == 1)
        return clockhand;
    else
        return NULL;
}

pgentry* swapIn(table* _tabletarget, pgentry* _entrytarget, int _pagetarget)
{//swaps the page frame from disk to memory, returning the entry pointer.
    //make new page
    pg* newpage = makePage(0,_pagetarget);
    //make new page table entry
    pgentry* newentry = insertPage(_tabletarget, newpage, _entrytarget);
    return newentry;
}

void swapOut(table* _tabletarget, pgentry* _entrytarget)
{//swaps the page frame from memory to disk if it is dirty, just frees page otherwise.
    
    deletePageEntry(_tabletarget, _entrytarget);
}


int main(int argcount, char* argv[])
{//main is where the heart is.
    //get page frame count and create page table
    const int PAGE_FRAMES = atoi(argv[1]);
    table pages;
    pages.full = 0;
    pages.currlength = 0;
    pages.maxlength = PAGE_FRAMES;
    pages.head = NULL;
    pages.tail = NULL;

    //open page reference file
    FILE* pagerefs = fopen(argv[2],"r");

    //set memory access cost and init cost counter
    const int MEM_ACCESS_COST = atoi(argv[3]);

    //set disk read cost and init cost counter
    const int SWAP_IN_COST = atoi(argv[4]);
    int __totalswapintime = 0;

    //set disk write cost and init cost counter
    const int SWAP_OUT_COST = atoi(argv[5]);
    int __totalswapouttime = 0;

    //counter for page references read from file
    int __totalrefcount = 0;

    //counter for page faults on read
    int __readfaults = 0;

    //counter for page faults on write
    int __writefaults = 0;

    if (pagerefs == NULL){//file isn't open
        printf("Couldn't open page references file.\n");
        return 0;
    }
    else//file is open
        printf("Page references file opened.\n");
    
    char currreadref[10];//need to set readline width = page frame digits + 2 
    char operation; //page operation read from file
    int pagenum;    //page number read from file
    pgentry* clockhand = NULL;

    
    while (fgets(currreadref, 10, pagerefs) != NULL)
    {//grab each page reference line in the text file
        __totalrefcount++;
        printf("\n%d: %s", __totalrefcount, currreadref); //print the line
        sscanf(currreadref,"%s %d", &operation, &pagenum); //parse line
        pgentry* currpage = findPage(&pages, pagenum, MEM_ACCESS_COST);
        pgentry* newentry;
        pgentry* swapintarget;

        if (currpage != NULL) {//page is found in memory (page hit)
            printf("PAGE HIT! Page#: %d\n", currpage->pgaddr->pagenum);
            __totalmemaccesstime += MEM_ACCESS_COST; //increment counter
            currpage->ref = 1; //touch the page
            //if reference is a write
            if (operation == 'w' || operation == 'W')
                currpage->dirty = 1; //write to page
        }
        else { //if page isn't in memory (page miss)
            printf("PAGE MISS! Page#: %d\n", pagenum);
            if (pages.full == 1) {//if page table is full
                printf("Page table full. Table count: %d\n", pages.currlength);
                //find victim page
                clockhand = findVictimPage(&pages, clockhand, MEM_ACCESS_COST);
            
                //prep for swapin after removal of clockhand's page
                swapintarget = clockhand->prventry;
                //swap the victim page out
                if(clockhand->dirty == 1) //if page has been written to
                    __totalswapouttime += SWAP_OUT_COST; //increment counter
                swapOut(&pages, clockhand);
                //swap new page in to memory at clockhand's old position
                newentry = swapIn(&pages, swapintarget, pagenum);
                __totalswapintime += SWAP_IN_COST;
                //move clockhand forward one entry
                clockhand = newentry->nxtentry;
            }
            else {//page table has space
                swapintarget = pages.tail;
                //swap new page in to target location
                newentry = swapIn(&pages, swapintarget, pagenum);
                __totalswapintime += SWAP_IN_COST;
            }

            newentry->ref = 1; //touch the page
            __totalmemaccesstime += MEM_ACCESS_COST; //increment counter

            //if reference is a read
            if (operation == 'r' || operation == 'R')
                __readfaults++; //increment counter
                //printf("%d: read page %d\n", __totalrefcount, pagenum);
            else if (operation == 'w' || operation == 'W')
            {//if reference is a write
                // printf("%d: write page %d\n", __totalrefcount, pagenum);
                __writefaults++; //increment counter
                newentry->dirty = 1; //write to page
            }
            else {
                printf("Did not understand reference #%d.\n", __totalrefcount);
            }
        }
        printf("\n");
    }
    fclose(pagerefs);
    
    printf("\nTotal Page References: %d", __totalrefcount);
    printf("\nTotal Page Faults on Reads: %d", __readfaults);
    printf("\nTotal Page Faults on Writes: %d", __writefaults);
    printf("\nTotal Memory Access Times: %d", __totalmemaccesstime);
    printf("\nTotal Time Spent Swapping Pages In: %d", __totalswapintime);
    printf("\nTotal Time Spent Swapping Pages Out: %d", __totalswapouttime);

    return 0;
}