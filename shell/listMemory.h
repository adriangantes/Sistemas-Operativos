/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */


#ifndef SHELLC_LIST_BLOCKS_H
#define SHELLC_LIST_BLOCKS_H


#include <stdbool.h>
#include <stddef.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#define MAX 1024

typedef struct {
    void *address;
    size_t size;
    char time[19];
    char allocation_type[13];
    key_t key;
    char file_name[MAX];
    int file_descriptor;
    int shmid;
} tItemMem;

typedef struct tNodeMem *tPosMem;

struct tNodeMem {

    tItemMem data;
    tPosMem next;
};

typedef tPosMem tListMem;


void createEmptyListMem(tListMem *L);

bool isEmptyListMem(tListMem L);

tPosMem firstMem (tListMem L);

tPosMem nextMem (tPosMem p);

bool insertItemMem(tItemMem d, tListMem *L);

void deleteAtPositionMem(tListMem *L, tPosMem p, int fromListRemove);

void printListMem(tListMem L, char memType);

void deleteListMem(tListMem *L);

tPosMem findItemMem (void *address, tListMem L);

tItemMem getItemMem (tPosMem p);

void removeHeadMem(tListMem *L);

#endif
