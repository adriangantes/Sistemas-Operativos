/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */

#include "listProcess.h"


bool createNodePro(tPosPro *p) {
    *p = malloc(sizeof(struct tNodePro));
    return *p != NULL;
}

void createEmptyListPro(tListPro *L) {
    if (createNodePro(L)){
        (*L)->next = NULL;
    }
}

void dateTimePro(char dateTime[]){
    time_t pressent;

    time(&pressent);

    struct tm *local = localtime(&pressent);
    int day, month, hours, minutes, year, seconds;
    day = local->tm_mday;
    month = local->tm_mon + 1;
    hours = local->tm_hour;
    minutes = local->tm_min;
    year = local->tm_year + 1900;
    seconds = local->tm_sec;

    sprintf(dateTime, "%d/%d/%d  %d:%d:%d", year, month, day, hours, minutes, seconds);
}

bool insertElementPro(tListPro *L, int pid, char *trozos[]){
    tPosPro q;
    int i = 0;

    if(!createNodePro(&q))
        return false;

    q->next = NULL;
    q->data.pid = pid;
    q->data.signal = 0;
    strcpy(q->data.status, "ACTIVE  ");
    strcpy(q->data.commandLine, " ");
    dateTimePro(q->data.datetime);

    q->data.userid = getuid();

    while(trozos[i] != NULL){
        if (strcmp(trozos[i], "&") != 0){
            strcat(q->data.commandLine, trozos[i]);
            strcat(q->data.commandLine, " ");
            i++;
        }else{
            break;
        }
    }

    tPosPro p;
    for(p = (*L) ; p->next != NULL; p = p->next);
    p->next = q;

    return true;
}


void removeElementPro(tListPro *L, tPosPro p){
    tPosPro i;

    if (p == NULL)
        return;

    if (p == *L) {
        return;
    } else if (p->next == NULL) {
        for (i = *L; i->next != p; i = i->next);
        i->next = NULL;
    } else {
        i = p->next;
        p->data.pid = i->data.pid;
        p->data.userid = i->data.userid;
        p->data.signal = i->data.signal;
        strcpy(p->data.status, i->data.status);
        strcpy(p->data.datetime, i->data.datetime);
        strcpy(p->data.commandLine, i->data.commandLine);
        p->next = i->next;
        p = i;
    }
    free(p);
}

void removeAtPidPro(int pid, tListPro *L){

    for(tPosPro p = (*L)->next; p != NULL; p = p->next){
        if(p->data.pid == pid){
            removeElementPro(L, p);
            return;
        }
    }
}

struct SEN{
    char *nombre;
    int senal;
};

static struct SEN sigstrnum[]={
        {"HUP", SIGHUP},
        {"INT", SIGINT},
        {"QUIT", SIGQUIT},
        {"ILL", SIGILL},
        {"TRAP", SIGTRAP},
        {"ABRT", SIGABRT},
        {"IOT", SIGIOT},
        {"BUS", SIGBUS},
        {"FPE", SIGFPE},
        {"KILL", SIGKILL},
        {"USR1", SIGUSR1},
        {"SEGV", SIGSEGV},
        {"USR2", SIGUSR2},
        {"PIPE", SIGPIPE},
        {"ALRM", SIGALRM},
        {"TERM", SIGTERM},
        {"CHLD", SIGCHLD},
        {"CONT", SIGCONT},
        {"STOP", SIGSTOP},
        {"TSTP", SIGTSTP},
        {"TTIN", SIGTTIN},
        {"TTOU", SIGTTOU},
        {"URG", SIGURG},
        {"XCPU", SIGXCPU},
        {"XFSZ", SIGXFSZ},
        {"VTALRM", SIGVTALRM},
        {"PROF", SIGPROF},
        {"WINCH", SIGWINCH},
        {"IO", SIGIO},
        {"SYS", SIGSYS},

#ifdef SIGPOLL
        {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
        {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
        {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
        {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
        {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
        {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
        {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
        {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
        {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
        {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
        {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
        {"WAITING", SIGWAITING},
#endif
        {NULL, -1}
};

char * nombreSenal(int sen){
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;

    if(sen == 0){
        return("000");
    }
    return ("SIGUNKNOWN");
}


void actualiceStatePro(tPosPro p){
    int stat;

    if (waitpid(p->data.pid, &stat, WNOHANG | WUNTRACED | WCONTINUED)==p->data.pid) {
        if(WIFSTOPPED(stat)){
            strcpy(p->data.status,"STOPPED");
            p->data.signal = WSTOPSIG(stat);
        }else if(WIFSIGNALED(stat)){
            strcpy(p->data.status, "SIGNALED");
            p->data.signal = WTERMSIG(stat);
        }else if(WIFEXITED(stat)){
            strcpy(p->data.status, "FINISHED");
            p->data.signal = WEXITSTATUS(stat);
        }
    }
}



void printStatusProcess(tPosPro p){
    int prio;
    prio = getpriority(PRIO_PROCESS, p->data.pid);
    struct passwd *uname = NULL;
    char * username = (uname = getpwuid(p->data.userid)) == NULL ? "UNKNOWN" : uname->pw_name;

    actualiceStatePro(p);

    printf("%d\t\t%s\tp=%d\t%s\t%s\t(%03d)\t%s\n", p->data.pid, username, prio, p->data.datetime,
           p->data.status, p->data.signal, p->data.commandLine);
}

void printListPro(tListPro L){
    for(tPosPro p = L->next; p!=NULL; p = p->next){
        printStatusProcess(p);
    }
}

void printInfoByPid(int pid, tListPro L){
    for(tPosPro p = L->next; p != NULL; p = p->next){
        if(p->data.pid == pid){
            printStatusProcess(p);
            return;
        }
    }
    printf("ERROR: The process doesn't exist\n");
}


bool processExist(int pid, tListPro L){
    for(tPosPro p = L->next; p != NULL; p = p->next){
        if(pid == p->data.pid){
            return true;
        }
    }

    return false;
}


void deleteListPro(tListPro *L){
    tPosPro p = (*L)->next;

    while (p != NULL) {
        removeElementPro(L, p);
        p = (*L)->next;
    }

    (*L)->next = NULL;
}

void clearListTerminatedSignaledPro(tListPro *L, bool isSignal){
    tPosPro p = (*L)->next;

    while(p != NULL){
        actualiceStatePro(p);
        if(isSignal){
            if(!strcmp(p->data.status, "SIGNALED")){
                removeElementPro(L, p);
                p = (*L)->next;
            }else{
                p = p->next;
            }
        }else{
            if(!strcmp(p->data.status, "FINISHED")){
                removeElementPro(L, p);
                p = (*L)->next;
            }else{
                p = p->next;
            }
        }
    }
    printListPro(*L);
}

void removeHeadPro(tListPro *L){
    if ((*L)->next == NULL) {
        free(*L);
    } else
        printf("This can only remove the head nodeP not the whole list.");
}

