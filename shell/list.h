/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */


#ifndef P0_LIST_H
#define P0_LIST_H


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define LNULL NULL


typedef char tString[1024];
typedef int tNum;
typedef struct tItem{
    tString string;
    tNum num;
}tItem;

typedef struct tNode* tPos;

struct tNode{
    tItem data;
    tPos next;
};
typedef tPos tList;


void createEmptyList(tList* L);

bool isEmptyList(tList L);

tPos first (tList L);

tPos next(tPos p);

tPos last (tList L);

bool insertItem(tItem d, tList* L);

tItem getItem(tPos p);

tPos findItem(tNum d,tList L);

void deleteList (tList* L);

void deleteAtPosition (tPos p,tList* L);


#endif

