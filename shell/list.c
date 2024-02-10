/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */

#include "list.h"

void createEmptyList (tList* L){
    *L=LNULL;
}

bool createNode(tPos *p){
    *p=malloc(sizeof (struct tNode));
    return *p !=LNULL;
}

tPos findPosition(tList L, tItem d){
    tPos p;

    for ( p = L ; (p->next != LNULL) && (p->next->data.num < d.num); p = p->next);
    return p;
}

bool insertItem(tItem d, tList* L){
    tPos q, p;

    if(!createNode(&q))
        return false;
    else{
        q->data=d;
        q->next=LNULL;
        if(*L==LNULL){
            *L =q;
        }else if(d.num < (*L)->data.num){
            q->next = *L;
            *L = q;
        }else{
            p = findPosition(*L, d);
            q->next=p->next;
            p->next=q;
        }
        return true;
    }
}

tPos findItem(tNum d,tList L){
    tPos p;
    for(p=L;(p!=LNULL) && (p->data.num != d);p=p->next);
    return p;
}

tPos first (tList L){
    return L;
}

tPos next (tPos p){
    return p->next;
}

tPos last (tList L){
    tPos p;
    for(p = L; p->next != LNULL ; p=p->next);
    return p;
}

bool isEmptyList(tList L){
    return L==LNULL;
}

tItem getItem(tPos p){
    return p->data;
}

void deleteList(tList* L){
    tPos p;

    while (*L !=LNULL){
        p=*L;
        *L =(*L)->next;
        free(p);
    }
}

void deleteAtPosition (tPos p,tList* L){
    tPos q;
    if (p == *L){
        *L = (*L)->next;
    }else if(p->next == LNULL){
        for(q = *L; q->next != p; q = q->next);
        q->next = LNULL;
    }else{
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q;
    }

    free(p);
    p=LNULL;
}
