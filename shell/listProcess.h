/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */

#ifndef P0_LISTPROCESS_H
#define P0_LISTPROCESS_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/resource.h>

#define MAX 1024

typedef struct {
    int pid;
    int userid;
    int signal;
    char status[11];
    char datetime[21];
    char commandLine[MAX/2];

} tItemPro;

typedef struct tNodePro *tPosPro;

struct tNodePro {

    tItemPro data;
    tPosPro next;
};

typedef tPosPro tListPro;


void createEmptyListPro(tListPro *L);

bool insertElementPro(tListPro *L, int pid, char *trozos[]);

void removeElementPro(tListPro *L, tPosPro p);

void removeAtPidPro(int pid, tListPro *L);

void printListPro(tListPro L);

void printInfoByPid(int pid, tListPro L);

bool processExist(int pid, tListPro L);

void deleteListPro(tListPro *L);

void clearListTerminatedSignaledPro(tListPro *L, bool isSignal);

void removeHeadPro(tListPro *L);

char * nombreSenal(int sen);

#endif