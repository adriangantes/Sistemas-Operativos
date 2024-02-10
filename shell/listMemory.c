/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */

#include "listMemory.h"

#define LNULL NULL

bool createNodeMem(tPosMem *p) {
    *p = malloc(sizeof(struct tNodeMem));
    return *p != NULL;
}

void createEmptyListMem(tListMem *L) {
    if (createNodeMem(L)){
        (*L)->next = LNULL;
        (*L)->data.address = NULL;

    }
}

bool isEmptyListMem(tListMem L) {
    return (L->next == NULL);
}

void getMonthName(int month, char mon[]){
    switch (month)
    {
        case 1:
            strcpy(mon, "Jan");
            break;
        case 2:
            strcpy(mon, "Feb");
            break;
        case 3:
            strcpy(mon, "Mar");
            break;
        case 4:
            strcpy(mon, "Apr");
            break;
        case 5:
            strcpy(mon, "May");
            break;
        case 6:
            strcpy(mon, "Jun");
            break;
        case 7:
            strcpy(mon, "Jul");
            break;
        case 8:
            strcpy(mon, "Aug");
            break;
        case 9:
            strcpy(mon, "Sep");
            break;
        case 10:
            strcpy(mon, "Oct");
            break;
        case 11:
            strcpy(mon, "Nov");
            break;
        case 12:
            strcpy(mon, "Dec");
            break;
        default:
            break;
    }
}

void dateTime(char dateTime[]){
    time_t pressent;
    char mon[3];

    time(&pressent);

    struct tm *local = localtime(&pressent);
    int day, month, hours, minutes;
    day = local->tm_mday;
    month = local->tm_mon + 1;
    hours = local->tm_hour;
    minutes = local->tm_min;


    getMonthName(month, mon);

    sprintf(dateTime, "%s  %d  %d:%d", mon, day, hours, minutes);
}

tPosMem firstMem (tListMem L){
    return L;
}

tPosMem nextMem (tPosMem p){
    return p->next;
}

bool insertItemMem(tItemMem d, tListMem *L) {
    tPosMem p, q;

    dateTime(d.time);

    if (!createNodeMem(&q)){
        return false;
    }else{
        q->data=d;
        q->next=LNULL;
        if ((*L)->next == LNULL)
            (*L)->next = q;
        else {
            for (p = *L; p->next != LNULL; p = p->next);
            p->next = q;
        }
        return true;
    }
}

void removeMalloc(tListMem *LM, tPosMem p){

    tPosMem i;
    if (p->next == NULL) {
        for (i = *LM; i->next != p; i = i->next);
        i->next = NULL;
        free(p->data.address);
    } else {
        i = p->next;
        free(p->data.address);
        p->data.address = i->data.address;
        p->data = i->data;
        p->next = i->next;
        p = i;
    }

    free(p);

}

void removeKey(key_t key){

    key_t clave = key;
    int id;

    if (clave == IPC_PRIVATE)
    {
        return;
    }
    if ((id = shmget(clave, 0, 0777)) == -1)
    {
        return;
    }
    shmctl(id, IPC_RMID, NULL);

}

void removeShared(tListMem *LM, tPosMem p, int fromListRemove){

    tPosMem i;
    if (p->next == NULL) {
        for (i = *LM; i->next != p; i = i->next);
        i->next = NULL;

        if(fromListRemove){
            if(shmdt(p->data.address) == -1){
                printf("Imposible to deallocate the shared memory\n");
            }
            removeKey(p->data.key);
        }

    } else {
        i = p->next;

        if(fromListRemove){
            if(shmdt(p->data.address) == -1)
            {
                printf("Imposible to deallocate the shared memory\n");
            }
            removeKey(p->data.key);
        }

        p->data.address = i->data.address;
        p->data = i->data;
        p->next = i->next;
        p = i;
    }

    free(p);
}

void removeMmap(tListMem *LM, tPosMem p){
    tPosMem i;
    if (p->next == NULL) {
        for (i = *LM; i->next != p; i = i->next);
        i->next = NULL;
        munmap(p->data.address, p->data.size);
    } else {
        i = p->next;
        munmap(p->data.address, p->data.size);
        p->data.address = i->data.address;
        p->data = i->data;
        p->next = i->next;
        p = i;
    }

    free(p);

}

void deleteAtPositionMem(tListMem *LM, tPosMem p, int fromListRemove) {

    if (p == NULL){
        return;
    }

    if (p == *LM) {
        return;
    }else if(!strcmp(p->data.allocation_type, "malloc")){
        removeMalloc(LM, p);
    }else if(!strcmp(p->data.allocation_type, "shared")){
        removeShared(LM, p, fromListRemove);
    }else if(!strcmp(p->data.allocation_type, "mmap")){
        removeMmap(LM, p);
    }


}

void deleteListMem(tListMem *L) {
    tPosMem p = (*L)->next;

    while (p != NULL) {
        deleteAtPositionMem(L, p, 1);
        p = (*L)->next;
    }

    (*L)->next = NULL;
}

void printListMem(tListMem L, char memType) {
    printf ("\n");
    if(memType == 'M')
        printf("******List of blocks asignated malloc to the process ");
    else if(memType == 'S')
        printf("******List of blocks asignated shared to the process ");
    else if(memType == 'P')
        printf("******List of blocks asignated mmap to the process ");
    else
        printf("******List of blocks asignated to the process ");

    printf("%d\n", getpid());

    for(tPosMem p = L->next; p != NULL; p = p->next){
        if((strcmp("shared", p->data.allocation_type) == 0) && memType != 'M' && memType != 'P')
            printf("\t%p %18ld  %s %s (key %d)\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type, p->data.key);

        if(strcmp("malloc", p->data.allocation_type) == 0 && memType != 'S' && memType != 'P')
            printf("\t%p %18ld  %s %s\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type);

        if(strcmp("mmap", p->data.allocation_type) == 0 && memType != 'S' && memType != 'M')
            printf("\t%p %18ld  %s %s (descriptor %d)\n", p->data.address, p->data.size, p->data.time, p->data.file_name, p->data.file_descriptor);
        
    }
}

tPosMem findItemMem (void *address, tListMem L){
    tPosMem p = L;

    while (p != NULL) {
        if (address == p->data.address){
            return p;
        }
        p = p->next;
    }

    return NULL;
}

tItemMem getItemMem (tPosMem p){
    return p->data;
}

void removeHeadMem(tListMem *L) {
    if ((*L)->next == NULL) {
        free(*L);
    } else
        printf("This can only remove the head nodeM not the whole list.");
}