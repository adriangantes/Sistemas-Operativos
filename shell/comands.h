/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */


#ifndef P0_COMANDS_H
#define P0_COMANDS_H


#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>
#include <dirent.h>
#include <wait.h>
#include <errno.h>
#include "list.h"
#include "listMemory.h"
#include "listProcess.h"

#define MAX 1024
#define TAMANO 2048

bool comand_HIST (char *trozos[], int numTrozos, tList *hist);

void comand_AUTHORS (char *trozos[], int numTrozos);

void comand_PID (char *trozos[], int numTrozos);

void comand_TIME ();

void comand_DATE ();

void comand_INFOSYS ();

void comand_OPEN (char * trozos[], tList files);

void comand_CHDIR (char *trozos[], int numTrozos);

void comand_CLOSE (char * trozos[], tList files);

void comand_DUP (char * trozos[], tList files);

void comand_LISTOPEN (tList files);

void comand_CREATE (char *trozos[]);

void comand_DELETE (char *trozos[]);

bool comand_DELTREE (const char *path);

void comand_STAT (char *trozos[]);

void comand_LIST (char *trozos[]);

void comand_MALLOC (char *trozos[], tListMem mem);

void comand_SHARED (char *trozos[], tListMem mem);

void comand_MMAP (char *trozos[], tListMem mem);

void comand_READ (char *trozos[]);

void comand_WRITE (char *trozos[]);

void comand_MEMDUMP (char *trozos[]);

void comand_MEMFILL (char *trozos[], tListMem mem);

void comand_MEM (char *trozos[], tListMem mem);

void comand_RECURSE (char *trozos[]);

void comand_UID (char *trozos[]);

void comand_SHOWVAR (char *trozos[], char *argv3[]);

void comand_CHANGEVAR (char *trozos[], char *argv3[]);

void comand_SUBSVAR (char *trozos[], char *argv3[]);

void comand_SHOWENV (char *trozos[], char *argv3[]);

void comand_FORK (tListPro pro);

void comand_EXEC (char *trozos[], bool isExecute, tListPro pro);

void comand_DELJOBS (char *trozos[], tListPro pro);

void comand_JOB (char *trozos[], tListPro pro);


#endif
