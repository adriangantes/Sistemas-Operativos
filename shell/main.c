/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */


#include "list.h"
#include "help.h"
#include "comands.h"


void imprimirPrompt (){
    printf ("\n--> ");
}

void leerEntrada (char cadena[]){
    fgets(cadena, MAX, stdin);
}

int TrocearCadena(char * cadena, char * trozos[]) {

    int i=1;

    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void procesarEntrada (char cadena[], bool *terminado, tList *hist, tList files, tListMem mem, tListPro pro, bool clean, char *argv3[]){
    char  *trozos[MAX];

    size_t longitud = strcspn(cadena, "\n");
    char stringCopy[MAX];
    strncpy(stringCopy, cadena, MAX);
    stringCopy[longitud] = '\0';

    int ult = 0;
    if (!isEmptyList(*hist)){
        ult = getItem(last(*hist)).num;
    }

    int numTrozos = TrocearCadena(cadena, trozos);

    if (numTrozos > 0){

        if (strcmp(trozos[0], "quit") == 0 || strcmp(trozos[0], "exit") == 0 || strcmp(trozos[0], "bye") == 0 ){
            *terminado = true;
            clean = true;
        }

        else if (strcmp(trozos[0], "hist") == 0){
            clean = comand_HIST(trozos, numTrozos, hist);
        }

        else if (strcmp(trozos[0], "authors") == 0){
            comand_AUTHORS(trozos, numTrozos);
        }

        else if (strcmp(trozos[0], "pid") == 0){
            comand_PID(trozos, numTrozos);
        }

        else if (strcmp(trozos[0], "date") == 0){
            comand_DATE();
        }

        else if (strcmp(trozos[0], "time") == 0){
            comand_TIME();
        }

        else if (strcmp(trozos[0], "infosys") == 0){
            comand_INFOSYS();
        }

        else if (strcmp(trozos[0], "help") == 0){
            if (numTrozos >1){
                printComandHelp(trozos);
            }else{
                printHelp();
            }
        }

        else if (strcmp(trozos[0], "open") == 0){
            if (numTrozos > 2){
                comand_OPEN(trozos, files);
            }else{
                printf("Unable to open file, open mode not specified.\n");
            }
        }

        else if (strcmp(trozos[0], "chdir") == 0){
            comand_CHDIR(trozos, numTrozos);
        }

        else if (strcmp(trozos[0], "close") == 0) {
            if (numTrozos > 1) {
                comand_CLOSE(trozos, files);
            }else{
                printf ("Unspecified archive.\n");
            }
        }

        else if (strcmp(trozos[0], "dup") == 0) {
            if (numTrozos > 1) {
                comand_DUP (trozos, files);
            }else{
                printf ("Unspecified archive.\n");
            }
        }

        else if (strcmp(trozos[0], "listopen") == 0) {
            comand_LISTOPEN(files);
        }

        else if (strcmp(trozos[0], "comand") == 0){
            if (numTrozos > 1){

                char *endptr ;
                long numero = strtol(trozos[1],&endptr,10);
                if (numero>0 && (int)numero <= ult){

                    tPos p = findItem((int)numero,*hist);
                    if (p != LNULL){
                        tItem i = getItem(p);
                        if ((int)numero == i.num && strcmp(i.string, stringCopy) == 0){
                            printf ("Recursive comand\n");
                        }else if (clean){
                            procesarEntrada(i.string, terminado, hist, files, mem, pro, clean, argv3);
                        }else{
                            clean = true;
                            procesarEntrada(i.string, terminado, hist, files, mem, pro, clean, argv3);
                            clean = false;
                        }
                    }
                }else{
                    printf ("Command not found\n");
                }
            }else{
                printf ("Unspecified command.\n");
            }
        }

        else if (strcmp(trozos[0], "create") == 0){
            if (numTrozos > 1){
                comand_CREATE (trozos);
            }else{
                printf ("Unspecified name archive.\n");
            }
        }

        else if (strcmp(trozos[0], "delete") == 0){
            if (numTrozos > 1){
                comand_DELETE (trozos);
            }else{
                printf ("Unspecified name for archive or directory.\n");
            }
        }

        else if (strcmp(trozos[0], "deltree") == 0){
            if (numTrozos > 1){

                if(comand_DELTREE(trozos[1])){
                    printf ("Successful \"%s\" directory cleanup.\n", trozos[1]);
                }else{
                    printf ("Imposible clean.\n");
                }

            }else{
                printf ("Unspecified name for archive or directory.\n");
            }
        }

        else if (strcmp(trozos[0], "stat") == 0){
            if (numTrozos > 1){
                comand_STAT(trozos);
            }else{
                printf ("Insufficient arguments.\n");
            }

        }

        else if (strcmp(trozos[0], "list") == 0){
            if (numTrozos > 1){
                comand_LIST (trozos);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "malloc") == 0){
            if (numTrozos > 1){
                comand_MALLOC(trozos, mem);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "shared") == 0){
            if (numTrozos > 1){
                comand_SHARED(trozos, mem);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "mmap") == 0){
            if (numTrozos > 1){
                comand_MMAP(trozos, mem);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }


        else if (strcmp(trozos[0], "read") == 0){
            if (numTrozos > 2){
                comand_READ(trozos);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "write") == 0){
            if (numTrozos > 3){
                comand_WRITE(trozos);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "memdump") == 0){
            if (numTrozos > 1){
                comand_MEMDUMP(trozos);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "memfill") == 0){
            if (numTrozos > 1){
                comand_MEMFILL(trozos, mem);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "mem") == 0){
            comand_MEM(trozos, mem);
        }

        else if (strcmp(trozos[0], "recurse") == 0){
            if (numTrozos > 1){
                comand_RECURSE(trozos);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "uid") == 0){
            comand_UID(trozos);
        }

        else if (strcmp(trozos[0], "showvar") == 0){
            comand_SHOWVAR(trozos, argv3);
        }

        else if (strcmp(trozos[0], "changevar") == 0){
            if (numTrozos > 3){
                comand_CHANGEVAR(trozos, argv3);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "subsvar") == 0){
            if (numTrozos > 4){
                comand_SUBSVAR(trozos, argv3);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "showenv") == 0){
            comand_SHOWENV(trozos, argv3);
        }

        else if (strcmp(trozos[0], "fork") == 0){
            comand_FORK(pro);
        }

        else if (strcmp(trozos[0], "exec") == 0){
            if (numTrozos > 1){
                comand_EXEC(trozos+1, true, pro);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "jobs") == 0){
            printListPro(pro);
        }

        else if (strcmp(trozos[0], "deljobs") == 0){
            if (numTrozos > 1){
                comand_DELJOBS(trozos, pro);
            }else{
                printf ("Insufficient arguments.\n");
            }
        }

        else if (strcmp(trozos[0], "job") == 0){
            comand_JOB(trozos, pro);
        }

        else{
            comand_EXEC(trozos, false, pro);
        }
    }

    if (!clean){
        tItem i;
        i.num = ult + 1;
        strncpy(i.string, stringCopy, longitud);
        i.string[longitud] = '\0';
        insertItem(i, hist);
    }

}

int main(int argc, char *argv[], char *argv3[]) {

    bool terminado = false;
    bool clean = false;
    char *estandar[3] = {"Entrada estandar", "Salida estandar", "Error estandar"};
    char cadena[MAX];

    static tList hist;
    static tList files;
    static tListMem mem;
    static tListPro pro;
    tItem item;

    createEmptyList (&hist);
    createEmptyList (&files);
    createEmptyListMem (&mem);
    createEmptyListPro (&pro);

    for (int i = 0; i<3;i++){
        strcpy(item.string, estandar[i]);
        item.string[sizeof(item.string) - 1] = '\0';
        item.num = i;
        insertItem(item, &files);
    }

    while (!terminado){
        imprimirPrompt ();
        leerEntrada (cadena);
        printf ("\n");
        procesarEntrada (cadena, &terminado, &hist, files, mem, pro, clean, argv3);
    }

    deleteList (&hist);
    deleteList (&files);
    deleteListMem (&mem);
    removeHeadMem(&mem);
    deleteListPro (&pro);
    removeHeadPro(&pro);

    return 0;
}