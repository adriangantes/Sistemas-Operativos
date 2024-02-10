/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */


#include "comands.h"
#include "help.h"


int delete (char *trozos);
int emptyDir(const char *path);
char LetraTF (mode_t m);
void print_permissions(mode_t mode);
void printDir (char *path, bool extra, bool acc, bool link);
void printLevelDirectories (const char *path, bool extra, bool acc, bool link, bool hid);
void printDirectories (char *path, bool extra, bool acc, bool link, int rec, bool hid);
void allocateMemory (void *p, size_t cont, unsigned char byte);
void *ObtenerMemoriaShmget(key_t clave, size_t tam, tListMem *LM);
ssize_t readFile(char *f, void *p, size_t cont);
ssize_t writeFile(char *f, void *p, size_t cont, int overwrite);
void *mapFile (char *file, int protection, tListMem mem);
void doPmap ();
void recursive (int n);
int findVar (char * var, char *e[]);
void onlyTheName(char *str);
int changeVar (char * var, char * valor, char *e[], int pos);
char * executable (char *s);
int OurExecvpe(char *file, char *const argv[], char *const envp[]);
void execute(char *trozos[]);
char * checkPrio(char *trozos[]);
bool checkBackground(char *trozos[]);


bool comand_HIST (char *trozos[], int numTrozos, tList *hist){
    if (numTrozos > 1){
        if(strcmp(trozos[1], "-c") == 0){
            deleteList(hist);
            return true;
        }else{
            char *endptr ;
            long numero = -(strtol(trozos[1],&endptr,10));
            if (numero>0){
                long numHist=0;
                for (tPos p=first(*hist); numHist<numero && p!=NULL; p=next(p)){
                    tItem i=getItem(p);
                    printf ("%d %s\n", i.num, i.string);
                    numHist=i.num;
                }
            }
        }
    }else{
        if (!isEmptyList(*hist)){
            for (tPos p=first(*hist); p!=NULL; p=next(p)){
                tItem i=getItem(p);
                printf ("%d %s\n", i.num, i.string);
            }
        }
    }
    return false;
}

void comand_AUTHORS (char *trozos[], int numTrozos){
    if (numTrozos > 1){
        if(strcmp(trozos[1],"-n")==0){
            printf("Adrian Edreira Gantes\n");
        }else if(strcmp(trozos[1], "-l") == 0){
            printf ("adrian.gantes@udc.es\n");
        }
    }else{
        printf("Adrian Edreira Gantes\n");
        printf("adrian.gantes@udc.es\n");
    }
}

void comand_PID (char *trozos[], int numTrozos){
    if (numTrozos > 1){
        if(strcmp(trozos[1],"-p")==0){
            printf( "Father PID: %d\n", getppid());
        }
    }else{
        printf( "PID: %d\n", getpid());
    }
}

void comand_TIME (){
    time_t date;struct tm *tm_info;
    time(&date);
    tm_info = localtime(&date);
    printf("Time: %02d:%02d:%02d\n",
           tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}

void comand_DATE (){
    time_t date;struct tm *tm_info;
    time(&date);
    tm_info = localtime(&date);
    printf("Date (DD/MM/YYYY): %02d/%02d/%04d\n",
           tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

void comand_INFOSYS (){
    struct utsname uname_puntero;
    uname(&uname_puntero);
    printf("Uname:\t\t%s\n", uname_puntero.sysname);
    printf("Architecture:\t%s\n", uname_puntero.machine);
    printf("Machine Name:\t%s\n", uname_puntero.nodename);
    printf("Version:\t%s\n", uname_puntero.version);
    printf("OS:\t\t%s\n", uname_puntero.release);
}

void comand_OPEN (char *trozos[], tList files){

    int mode = 0, df;
    size_t longitud = strcspn(trozos[1], "\n");
    char archivo[MAX] ;
    strncpy(archivo, trozos[1], longitud);
    archivo[longitud] = '\0';

    for (int i = 2; trozos[i] != NULL; i++) {
        if (!strcmp(trozos[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(trozos[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(trozos[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(trozos[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(trozos[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(trozos[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(trozos[i], "tr")) mode |= O_TRUNC;
    }

    df = open(archivo, mode, 0777);

    if (df == -1)
        perror("Unable to open file\n");
    else {
        tItem item;
        strncpy(item.string, archivo, sizeof(trozos[1]));
        item.num = df;
        insertItem(item, &files);
        printf("Created an entry in the open files table.\n");
    }
}

void comand_CHDIR (char *trozos[], int numTrozos){
    if (numTrozos > 1){
        if (chdir(trozos[1]) == 0) {
            printf("Succesful directory change.\n");
        } else {
            perror("Error trying change directory");
        }
    }else{
        char *actualDir = NULL;
        size_t size = pathconf(".", _PC_PATH_MAX);

        if ((actualDir = (char *)malloc((size_t)size)) != NULL) {
            if (getcwd(actualDir, (size_t)size) != NULL) {
                printf("Current directory: %s\n", actualDir);
            } else {
                perror("Can't obtain current directory");
            }
            free(actualDir);
        } else {
            perror("Memory allocation error");
        }
    }
}

void comand_CLOSE (char *trozos[], tList files){
    char *endptr ;
    long df = strtol(trozos[1],&endptr,10);

    if (close((int)df) == -1) {
        perror("Error closing the file");
    }else{
        deleteAtPosition(findItem((int)df, files),&files);
        close((int)df);
        printf ("File closed successfully.\n");
    }
}

void comand_DUP (char *trozos[], tList files){
    char *endptr ;
    long df = strtol(trozos[1],&endptr,10);
    tPos p = findItem((int)df,files);

    int new_df = dup((int)df);

    if (new_df == -1) {
        perror("Error duplicating the file");

    }else{
        tItem item;
        tItem i=getItem(p);
        strcpy(item.string, i.string);
        item.string[sizeof(item.string) - 1] = '\0';
        item.num = new_df;
        insertItem(item, &files);
        printf("File duplicated successfully.\n");
    }
}

void comand_LISTOPEN (tList files){
    for (tPos p= first(files); p != LNULL; p=next(p)){
        tItem i=getItem(p);
        printf ("Descriptor: %d\tName: %s\tModes:", i.num, i.string);
        int flags = fcntl(i.num, F_GETFL);

        if (flags == -1) {
            perror("Error obtaining access modes");
        } else {
            if (flags & O_EXCL)
                printf( "  ex");
            if (flags & O_APPEND)
                printf( "  ap");
            if (flags & O_TRUNC)
                printf( "  tr");
            if (flags & O_RDWR)
                printf( "  rw");
            else if (flags & O_WRONLY)
                printf( "  wo");
            else
                printf( "  ro");
        }
        printf ("\n");
    }
}

void comand_CREATE (char *trozos[]){
    if (strcmp(trozos[1], "-f") == 0){
        int df = open(trozos [2], O_CREAT, 0777);
        if (df == -1)
            perror("Error creating the file");
        else {
            printf("File created.\n");
            close(df);
        }
    }else{
        if (mkdir(trozos[1], 0777) == -1){
            perror("Error creating the directory");
        }else{
            printf("Directory created.\n");
        }
    }
}

void comand_DELETE (char *trozos[]){
    int estado;
    for (int i = 1; trozos[i] != NULL; i++) {

        estado = delete(trozos[i]);
        if (estado == 1){
            printf("Directory \"%s\" successfully deleted.\n", trozos[i]);
        }else if (estado == 2){
            printf("The directory \"%s\" is not empty and cannot be deleted.\n", trozos [i]);
        }else if (estado == 3){
            printf("File \"%s\" successfully deleted.\n", trozos[i]);
        }else if (estado == 4){
            printf("The file \"%s\" is not empty and cannot be deleted.", trozos[i]);
        }else{
            perror("Error deleting the directory or file");
        }
    }
}

bool comand_DELTREE (const char *path){
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (!dir) {
        perror("Error opening the directory");
        return false;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char entryPath[PATH_MAX];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

        struct stat entryStat;
        if (lstat(entryPath, &entryStat) == -1) {
            perror("Error getting information for the entry");
            continue;
        }

        if (S_ISDIR(entryStat.st_mode)) {
            comand_DELTREE(entryPath);
        } else if (S_ISREG(entryStat.st_mode)) {
            if (delete(entryPath) == 3){
                unlink(entryPath);
            }
        }
    }

    closedir(dir);

    if (rmdir(path) == -1) {
        return false;
    }
    return true;
}

void comand_STAT (char *trozos[]){
    bool link = false, acc = false, extra = false, mode = true;

    for (int i = 1; trozos[i] != NULL; i++){
        if(strcmp(trozos[i], "-link") == 0  && mode){
            link = true;
        }else if (strcmp(trozos[i], "-acc") == 0 && mode){
            acc = true;
        }else if (strcmp(trozos[i], "-long") == 0 && mode){
            extra = true;
        }else{
            mode = false;
            printDir (trozos[i], extra, acc, link);
            printf("\n");
        }
    }
}

void comand_LIST (char *trozos[]){

    bool link = false, acc = false, extra = false, mode = true;
    bool reca = false, recb = false, hid = false;

    for (int i = 1; trozos[i] != NULL; i++){
        if(strcmp(trozos[i], "-link") == 0  && mode){
            link = true;
        }else if (strcmp(trozos[i], "-acc") == 0 && mode){
            acc = true;
        }else if (strcmp(trozos[i], "-long") == 0 && mode){
            extra = true;
        }else if (strcmp(trozos[i], "-recb") == 0 && mode){
            if (!reca){
                recb = true;
            }
        }else if (strcmp(trozos[i], "-reca") == 0 && mode){
            if (!recb){
                reca = true;
            }
        }else if (strcmp(trozos[i], "-hid") == 0 && mode){
            hid = true;
        }else{
            mode = false;
            if (reca){
                printDirectories(trozos[i], extra, acc, link, 1, hid);
            }else if (recb){
                printDirectories(trozos[i], extra, acc, link, 2, hid);
            }else{
                printDirectories(trozos[i], extra, acc, link, 0, hid);
            }
        }
    }
}

void comand_MALLOC (char *trozos[], tListMem mem){
    size_t size;
    if (strcmp(trozos[1], "-free") == 0) {
        if ((size = (size_t) strtol(trozos[2], NULL, 10)) > 0){
            bool free = false;
            if (!isEmptyListMem(mem)) {
                for (tPosMem p = firstMem(mem); p != NULL; p = nextMem(p)) {
                    if (!strcmp(p->data.allocation_type, "malloc") && (p->data.size == size)) {
                        deleteAtPositionMem(&mem, p, 0);
                        free = true;
                        break;
                    }
                }
                if (free){
                    printf("Block memory deallocated successfully.\n");
                }else{
                    printf("Not block memory to deallocate.\n");
                }
            }
        }else{
            printf("Can't deallocote a block with 0 bytes or a negative number of bytes.\n");
        }
    }else {
        if ((size = (size_t) strtol(trozos[1], NULL, 10)) > 0) {
            void *p;
            p = malloc(size);
            memset(p, 0, size);
            tItemMem item;
            item.address = p;
            item.size = size;
            strcpy(item.allocation_type, "malloc");
            item.key = 0;
            strcpy(item.file_name, " ");
            item.file_descriptor = 0;
            item.shmid = 0;
            if (p != NULL){
                insertItemMem(item, &mem);
                printf("Inserted %ld bytes at %p.\n", size, p);
            }else{
                perror("Not enough memory");
            }
        }else {
            printf("Can't allocote memory.\n");
        }
    }
}

void comand_SHARED (char *trozos[], tListMem mem){

    if (strcmp (trozos[1], "-free") == 0){
        if (trozos[2] != NULL) {
            key_t key = (key_t) strtoul(trozos[2], NULL, 10);
            tPosMem p;
            bool free = false;

            for (p = firstMem(mem); p != NULL; p = nextMem(p)) {
                if (p->data.key == key) {
                    if (shmdt(p->data.address) == -1) {
                        if (shmctl(p->data.shmid, IPC_RMID, NULL) == -1)
                            printf("Imposible to deallocate the shared memory.\n");
                    }
                    deleteAtPositionMem(&mem, p, 0);
                    free = true;
                    break;
                }
            }

            if (free){
                printf ("Block memory deallocated successfully with key.\n");
            }else{
                printf ("Non-existent block memory with key.\n");
            }
        }
    }else if (strcmp (trozos[1], "-create") == 0){
        key_t cl;
        size_t tam;
        void *p;

        if (trozos[2] == NULL || trozos[3] == NULL) {
            printf ("Insufficient arguments.\n");
            return;
        }

        cl = (key_t) strtoul(trozos[2], NULL, 10);
        tam = (size_t) strtoul(trozos[3], NULL, 10);
        if (tam == 0) {
            printf("No se asignan bloques de 0 bytes\n");
            return;
        }
        if ((p = ObtenerMemoriaShmget(cl, tam, &mem)) != NULL){
            printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
        }else{
            printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
        }

    }else if ((strcmp (trozos[1], "-delkey") == 0)){
        key_t clave;
        int id;
        char *key = trozos[2];

        if (key == NULL || (clave = (key_t) strtoul(key, NULL, 10)) == IPC_PRIVATE) {
            printf("      delkey necesita clave_valida\n");
            return;
        }
        if ((id = shmget(clave, 0, 0666)) == -1) {
            perror("shmget: imposible obtener memoria compartida");
            return;
        }
        if (shmctl(id, IPC_RMID, NULL) == -1){
            perror("shmctl: imposible eliminar memoria compartida\n");
        }else{
            printf ("Key deleted.\n");
        }
    }else{
        if (trozos[1] != NULL) {
            void *p;
            key_t key = (key_t) strtoul(trozos[1], NULL, 10);
            if ((p = ObtenerMemoriaShmget(key, 0, &mem)) != NULL){
                printf("Memoria compartida de clave %d en %p\n", key, p);
            }else{
                printf("Imposible asignar memoria compartida clave %d: %s\n", key, strerror(errno));
            }
        }else{
            printf ("Insufficient arguments.\n");
        }
    }
}

void comand_MMAP (char *trozos[], tListMem mem){

    if (strcmp(trozos[1], "-free") == 0) {

        for (tPosMem p = firstMem(mem); p != NULL; p = nextMem(p)) {
            if (!strcmp(trozos[2], p->data.file_name)) {
                if (munmap(p->data.address, p->data.size) == -1){
                    perror("Error");
                }
                deleteAtPositionMem(&mem, p, 0);
                printf("File %s unmapped sucssefully.\n", p->data.file_name);
                break;
            }
        }

    }else {
        char *perm;
        void *p;
        int protection = 0;


        if (trozos[1]==NULL){
            printf ("Not enough arguments.\n");
        }
        if ((perm=trozos[2])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
        }

        if ((p = mapFile(trozos[1], protection, mem)) == NULL){
            perror("Unable to map file");
        }else{
            printf("Fichero %s mapeado en %p\n", trozos[1], p);
        }
    }
}

void comand_READ (char *trozos[]){
    void *p;
    size_t cont = -1;
    ssize_t n;
    if (trozos[1] == NULL || trozos[2] == NULL) {
        printf("Parameters missing\n");
        return;
    }

    sscanf(trozos[2], "%p", &p);
    if (trozos[3] != NULL)
        cont = (size_t) strtoul(trozos[3], NULL, 10);

    if ((n = readFile(trozos[1], p, cont)) == -1)
        perror("Imposible to read the file");
    else
        printf("Read %lld bytes of %s in %p.\n", (long long) n, trozos[1], p);
}

void comand_WRITE (char *trozos[]){
    void *p;
    size_t cont;
    ssize_t n;
    int overwrite;

    if (trozos[1] == NULL) {
        printf("Missing arguments\n");
        return;
    }

    if (!strcmp(trozos[1], "-o")) {
        if (trozos[2] != NULL && trozos[3] != NULL && trozos[4] != NULL) {
            overwrite = 1;
            sscanf(trozos[3], "%p", &p);
            cont = strtoul(trozos[4], NULL, 10);
            writeFile(trozos[2], p, cont, overwrite);
            if((n= (writeFile(trozos[2], p, cont, overwrite))) == -1){
                perror("Imposible to write the file");
            }else{
                printf("Wrote %lld bytes from %p in %s\n", (long long) n, p, trozos[2]);
            }
        }
    } else {
        if (trozos[2] != NULL && trozos[3] != NULL) {
            overwrite = 0;
            sscanf(trozos[2], "%p", &p);
            cont = (size_t) strtoul(trozos[3], NULL, 10);
            if((n = writeFile(trozos[1], p, cont, overwrite)) == -1){
                perror("Imposible to write the file");
            }else{
                printf("Wrote %lld bytes from %p in %s\n", (long long) n, p, trozos[1]);
            }
        }
    }
}

void comand_MEMDUMP (char *trozos[]){
    void *mem;
    size_t cont;

    if (trozos[1] != NULL) {
        sscanf(trozos[1], "%p", &mem);
        if(trozos[2] == NULL){
            cont = 25;
        }else{
            cont = (size_t) strtoul(trozos[2], NULL, 10);
        }

        printf("Dumping %zu byte of memory from memory position %p\n", cont, mem);
        unsigned char *arr = (unsigned char *) mem;
        size_t i;
        size_t j;
        size_t aux = 0;

        for (i = 0; i < cont; i++) {
            if (arr[i] != '0') {
                printf(" %c ", arr[i]);
            } else {
                printf("  ");
            }

            if (((i + 1) % 25 == 0) && (i != 0)) {

                printf("\n");
                for (j = aux; j <= i; j++) {
                    printf("%2x ", arr[j]);
                }
                printf("\n");
                aux = i + 1;
            }
        }

        cont = cont % 25;

        if (cont != 0) {

            printf("\n");
            for (j = 0; j < cont; j++) {
                printf("%2x ", arr[j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void comand_MEMFILL (char *trozos[], tListMem mem){

    void *p;
    size_t cont;
    unsigned char byte = 'A';

    if (trozos[1] != NULL) {
        sscanf(trozos[1], "%p", &p);
        tItemMem item = getItemMem(findItemMem(p, mem));
        cont = item.size;

        if(trozos[2] != NULL || trozos[3] == NULL){
            if (trozos[3] != NULL){
                cont = (size_t) strtoul(trozos[2], NULL, 10);

                if(trozos[3][0] == '\''){
                    if(trozos[3][1] != '\0')
                        byte = (unsigned char) trozos[3][1];
                }else{
                    byte = (unsigned char) strtoul(trozos[3], NULL, 16);
                }
                printf("Filling %zu bytes of memory with the byte %c(%x) from the memory position %p\n", cont, byte, byte, p);
                allocateMemory(p, cont, byte);
            }else{
                cont = (size_t) strtoul(trozos[2], NULL, 10);
                printf("Filling %zu bytes of memory with the byte %c(%x) from the memory position %p\n", cont, byte, byte, p);
                allocateMemory(p, cont, byte);
            }
        }else{
            printf("Filling %zu bytes of memory with the byte %c(%x) from the memory position %p\n", cont, byte, byte, p);
            allocateMemory(p, cont, byte);
        }
    }
}

int glob1 = 1;
int glob2 = 2;
int glob3 = 3;

void comand_MEM (char *trozos[], tListMem mem){
    int x, y, z;
    static int x1, y1, z1;

    if (trozos[1] == NULL || strcmp(trozos[1], "-all") == 0 || !strcmp(trozos[1], "-vars")) {
        printf("Local variables   \t%p,\t%p,\t%p\n", &x, &y, &z);
        printf("Global variables   \t%p,\t%p,\t%p\n", &glob1, &glob2, &glob3);
        printf("Static variables   \t%p,\t%p,\t%p\n", &x1, &y1, &z1);
    }

    if (trozos[1] == NULL || strcmp(trozos[1], "-all") == 0 || !strcmp(trozos[1], "-funcs")) {
        printf("Program functions \t%p,\t%p,\t%p\n", comand_AUTHORS, printHelp, comand_PID);
        printf("Library functions \t%p,\t%p,\t%p\n", stat, open, remove);
    }

    if (trozos[1] != NULL && !strcmp(trozos[1], "-pmap")) {
        doPmap();
    }

    if (trozos[1] == NULL || strcmp(trozos[1], "-all") == 0 || !strcmp(trozos[1], "-blocks")) {
        printListMem(mem, 'A');
    }
}

void comand_RECURSE (char *trozos[]){
    int num = (int)strtol(trozos[1],NULL,10);
    if (num <= 0){
        printf ("Number negative or 0.\n");
    }else{
        recursive(num);
    }
}




void comand_UID (char *trozos[]){

    if (trozos[1] == NULL || strcmp(trozos[1], "-get") == 0){
        int real = (int)getuid(), efec = (int)geteuid();
        printf("ID de usuario real: %d, (%s)\n", real, getpwuid(real)->pw_name);
        printf("ID de usuario efectivo: %d, (%s)\n", efec, getpwuid(efec)->pw_name);
    }else if (strcmp(trozos[1], "-set") == 0 && trozos[2] != NULL && strcmp(trozos[2], "-l") != 0){

        if (seteuid((int)strtol(trozos[2], NULL, 10)) == -1){
            perror ("Imposible change");
        }else{
            printf ("Effective change\n");
        }

    }else if (trozos[2] != NULL && strcmp(trozos[2], "-l") == 0){

        if (seteuid(getpwnam(trozos[3])->pw_uid) == -1){
            perror ("Imposible change");
        }else{
            printf ("Effective change\n");
        }

    }else{
        printf ("Imposible change\n");
    }
}

void comand_SHOWVAR (char *trozos[], char *argv3[]){

    int i = 0;

    if(trozos[1] == NULL){
        while(argv3[i] != NULL){
            printf("%p->main argv3[%d]=(%p) %s\n", &argv3[i], i, (void *) argv3[i], argv3[i]);
            i++;
        }
    }else{
        char **env = __environ;
        int posVar;

        if((posVar = findVar(trozos[1], env)) == -1){
            perror("ERROR");
        }else{
            char *variable, *var;

            variable = strdup(argv3[posVar]);
            onlyTheName(variable);
            var = getenv(variable);

            printf("With arg3 %s (%p)  @%p\n", argv3[posVar], argv3[posVar], &argv3[posVar]);
            printf("With arg3 %s (%p)  @%p\n", env[posVar], env[posVar], &env[posVar]);
            printf("With getenv %s %p\n", var, var);

            free(variable);
        }
    }
}

void comand_CHANGEVAR (char *trozos[], char *argv3[]){

    int pos = findVar(trozos[2], argv3);

    if (pos != -1){
        if(!strcmp(trozos[1], "-a")){

            if(changeVar(trozos[2], trozos[3], argv3, pos) == -1)
                perror("Imposible to change variable");
            else
                printf ("Successful change\n");

        }else if(!strcmp(trozos[1], "-e")){

            if (changeVar(trozos[2], trozos[3], __environ, pos) == -1)
                perror("Imposible to change variable");
            else
                printf("Successful change\n");

        }else if(!strcmp(trozos[1], "-p")){
            char *aux;

            if ((aux=(char *)malloc(strlen(trozos[2])+strlen(trozos[3])+2))==NULL){
                perror("Memory allocation failed");
                return;
            }
            strcpy(aux, trozos[2]);
            strcat(aux, "=");
            strcat(aux, trozos[3]);

            if(putenv(aux) != 0)
                perror("Imposible to change variable");
            else
                printf ("Successful change\n");
        }
    }else{
        perror("Imposible to change variable");
    }
}

void comand_SUBSVAR (char *trozos[], char *argv3[]){

    int pos = findVar(trozos[2], argv3);

    if (pos == -1){
        perror("Imposible to change variable");
    }else{
        if(!strcmp(trozos[1], "-a")){

            if(changeVar(trozos[3], trozos[4], argv3, pos) == -1)
                perror("Imposible to change variable");
            else
                printf ("Successful change\n");

        }else if(!strcmp(trozos[1], "-e")){

            if(changeVar(trozos[3], trozos[4], __environ, pos) == -1)
                perror("Imposible to change variable");
            else
                printf ("Successful change\n");
        }
    }
}

void comand_SHOWENV (char *trozos[], char *argv3[]){

    if(trozos[1] == NULL){
        comand_SHOWVAR(trozos, argv3);

    }else if(!strcmp(trozos[1], "-environ")){
        int i = 0;

        while(__environ[i] != NULL){
            printf("%p->main env[%d]=(%p) %s\n", &__environ[i], i, __environ[i], __environ[i]);
            i++;
        }
    }else if(!strcmp(trozos[1], "-addr")){
        printf("Environ: %p (stored at %p)\n", __environ, &__environ);
        printf("Main argv3: %p (stored at %p)\n", argv3, &argv3);
    }else{
        printf("Use: environ [-environ | -addr]\n");
    }
}

void comand_FORK (tListPro pro){
    pid_t pid;

    if ((pid=fork())==0){
        deleteListPro(&pro);
        printf ("Executing process %d\n", getpid());
    }else if (pid!=-1){
        waitpid (pid,NULL,0);
        printf("Child process %d has finished.\n", pid);
    }
}

void comand_EXEC (char *trozos[], bool isExecute, tListPro pro){

    char *prio = checkPrio(trozos);
    bool background = checkBackground(trozos);
    int pid_son;

    if(isExecute){
        execute(trozos);
        return;
    }

    if((pid_son = fork()) == 0){
        execute(trozos);
    }else{
        if(background){
            insertElementPro(&pro, pid_son, trozos);
        }else{
            int st;
            waitpid(pid_son, &st, 0);
        }

        if(prio != NULL){
            setpriority(PRIO_PROCESS, pid_son, (int) strtoul(prio, NULL, 10));
        }
    }
}

void comand_DELJOBS (char *trozos[], tListPro pro){
    if(!strcmp(trozos[1], "-term")){
        clearListTerminatedSignaledPro (&pro, false);
    }else if(!strcmp(trozos[1], "-sig")){
        clearListTerminatedSignaledPro (&pro, true);
    }
}

void comand_JOB (char *trozos[], tListPro pro){

    if(trozos[1] == NULL){
        printListPro(pro);
    }else{
        int pid, st;

        if(strcmp(trozos[1], "-fg") == 0 && trozos[2] != NULL){

            pid = (int)strtol(trozos[2], NULL, 10);

            if(processExist(pid, pro)){

                waitpid(pid, &st, 0);
                int valor = WEXITSTATUS(st);

                if (WIFEXITED(st)) {
                    printf ("Process %d finished succesfully. Returned value %d.\n", pid, valor);
                } else if (WIFSIGNALED(st)){
                    printf ("Process %d finished by signal %s.\n", pid, nombreSenal(st));
                }else{
                    printf("Process finished for unknown reasons.\n");
                }

                removeAtPidPro(pid, &pro);
            }else{
                printf("ERROR: The process doesn't exist\n");
            }

        }else if(strcmp(trozos[1], "-fg") != 0){
            pid = (int)strtol(trozos[1], NULL, 10);
            printInfoByPid(pid, pro);
        }
    }
}




int delete (char * trozos){

    int estado;
    FILE *archivo;

    estado = emptyDir(trozos);
    archivo = fopen(trozos, "r");

    if (estado == 1 && rmdir(trozos) == 0){
        return 1;
    }else if (archivo != NULL){
        fseek(archivo, 0, SEEK_END);

        if (ftell(archivo) == 0 && remove(trozos) == 0) {
            fclose (archivo);
            return 3;
        } else {
            fclose (archivo);
            return 4;
        }

    }else if (estado == 0){
        return 2;
    }

    return 0;
}

int emptyDir(const char *path) {

    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        return -1;
    }

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            closedir(dir);
            return 0;
        }
    }

    closedir(dir);
    return 1;
}

char LetraTF (mode_t m){
    switch (m&S_IFMT) {
        case S_IFSOCK: return 's';
        case S_IFLNK: return 'l';
        case S_IFREG: return '-';
        case S_IFBLK: return 'b';
        case S_IFDIR: return 'd';
        case S_IFCHR: return 'c';
        case S_IFIFO: return 'p';
        default: return '?';
    }
}

void print_permissions(mode_t mode){
    printf("%c", LetraTF(mode));
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void printDir (char *path, bool extra, bool acc, bool link){

    bool exist;

    if(extra){

        struct stat fileStat;

        if (lstat(path, &fileStat) != -1) {

            struct passwd *ownerInfo = getpwuid(fileStat.st_uid);
            struct group *groupInfo = getgrgid(fileStat.st_gid);
            char timestamp[20];

            if(acc){
                time_t ultimoAcceso = fileStat.st_atime;
                strftime(timestamp, sizeof(timestamp), "%Y/%m/%d-%H:%M", localtime(&ultimoAcceso));
            }else{
                strftime(timestamp, sizeof(timestamp), "%Y/%m/%d-%H:%M", localtime(&fileStat.st_mtime));
            }

            const char *nombre = basename(path);

            printf("%s  %lu  (%ld)  %s  %s  ", timestamp, (unsigned long)fileStat.st_nlink,
                   (long)fileStat.st_ino, ownerInfo->pw_name, groupInfo->gr_name);
            print_permissions(fileStat.st_mode);
            printf("  %d  %s\t", (int)fileStat.st_size, nombre);
            exist = true;

        }else{
            perror ("Error getting information from the file or directory");
            exist = false;
        }

        if (link && exist){

            char simLink[MAX];
            ssize_t len = readlink(path, simLink, MAX - 1);

            if (len != -1) {
                printf("-> %s", simLink);
            }
        }

    }else{

        struct stat fileStat;

        if (lstat(path, &fileStat) != -1) {
            const char *nombre = basename(path);
            printf("%d %s\t", (int)fileStat.st_size, nombre);
            exist = true;
        }else{
            perror("Error getting information from the file or directory");
            exist = false;
        }
    }

    if (exist){
        printf("\n");
    }
}

void printLevelDirectories (const char *path, bool extra, bool acc, bool link, bool hid){

    struct dirent *entry;
    DIR *dir;
    char entryPath[1024];

    if (!(dir = opendir(path))) {
        perror("Error opening the directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);
        if (!hid){
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printDir(entryPath, extra, acc, link);
            }
        }else{
            printDir(entryPath, extra, acc, link);
        }

    }

    closedir(dir);
}

void printDirectories (char *path, bool extra, bool acc, bool link, int rec, bool hid){

    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char entryPath[1024];

    if (stat(path, &sb)==0){

        if (S_ISDIR(sb.st_mode)){

            dir = opendir(path);

            if (rec != 2){
                printf(" ### %s ###\n", path);
                if (rec == 1){
                    printLevelDirectories(path,extra,acc,link,hid);
                    printf ("\n");
                }
            }

            while ((entry = readdir(dir)) != NULL) {

                snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

                if (!hid){

                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {

                        if (rec == 0) {
                            printDir(entryPath, extra, acc, link);
                        }else{
                            if (entry->d_type == DT_DIR) {
                                printDirectories(entryPath,extra,acc,link,rec,hid);
                            }
                        }

                    }

                }else {

                    if (rec == 0) {
                        printDir(entryPath, extra, acc, link);
                    }else{
                        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && entry->d_type == DT_DIR) {
                            printDirectories(entryPath,extra,acc,link,rec,hid);
                        }
                    }

                }
            }

            if (rec == 2){
                printf(" ### %s ###\n", path);
                printLevelDirectories(path,extra,acc,link,hid);
                printf ("\n");
            }

            closedir(dir);

        }else if (S_ISREG(sb.st_mode)){
            printDir (path, extra, acc, link);
        }else{
            perror("Error opening the file or directory");
        }
    }
}

void allocateMemory (void *p, size_t cont, unsigned char byte){
    unsigned char *arr = (unsigned char *) p;
    size_t i;

    for (i = 0; i < cont; i++)
        arr[i] = byte;
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam, tListMem *LM) {
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    if (tam)
        flags = flags | IPC_CREAT | IPC_EXCL;
    if (clave == IPC_PRIVATE){
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);
    if ((p = shmat(id, NULL, 0)) == (void *) -1) {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s);
    tItemMem item;
    item.address = p;
    item.size = s.shm_segsz;
    strcpy(item.allocation_type, "shared");
    item.key = clave;
    strcpy(item.file_name, " ");
    item.file_descriptor = 0;
    item.shmid = 0;
    insertItemMem(item, LM);
    return (p);
}

ssize_t readFile(char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;
    if (cont == -1)
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

ssize_t writeFile (char *f, void *p, size_t cont, int overwrite){
    ssize_t n;
    int df, aux, flags = O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags = O_CREAT | O_WRONLY | O_TRUNC;

    if ((df = open(f, flags, 0777)) == -1)
        return -1;

    if ((n = write(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

void *mapFile (char *file, int protection, tListMem mem) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR;
    if (stat(file, &s) == -1 || (df = open(file, modo)) == -1)
        return NULL;
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED){
        close (df);
        return NULL;
    }

    tItemMem item;
    item.address = p;
    item.size = s.st_size;
    strcpy(item.allocation_type, "mmap");
    item.key = 0;
    strcpy(item.file_name, file);
    item.file_descriptor = df;
    item.shmid = 0;
    insertItemMem(item, &mem);

    return p;
}

void doPmap (void){
    pid_t pid;
    char elpid[32];
    char *argv[4] = {"pmap", elpid, NULL};

    sprintf(elpid, "%d", (int) getpid());
    if ((pid = fork()) == -1) {
        perror("Imposible crear proceso");
        return;
    }
    if (pid == 0) {
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0] = "procstat";
        argv[1] = "vm";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute procstat (FreeBSD)");

        argv[0] = "procmap", argv[1] = elpid;
        argv[2] = NULL;
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute procmap (OpenBSD)");

        argv[0] = "vmmap";
        argv[1] = "-interleave";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

void recursive (int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);

    if (n > 0){
        recursive(n - 1);
    }
}

int findVar (char * var, char *e[]){
    int pos=0;
    char aux[MAX];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL){
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    }

    errno=ENOENT;
    return(-1);
}

void onlyTheName (char *str) {
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == '=') {
            str[i] = '\0';
        }
        i++;
    }
}

int changeVar (char * var, char * valor, char *e[], int pos){
    char *aux;

    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

char * executable (char *s){
    char path[MAX];
    static char aux2[MAX];
    struct stat st;
    char *p;

    if (s==NULL || (p=getenv("PATH"))==NULL)
        return s;
    if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;

    strncpy (path, p, MAX);

    for (p=strtok(path,":"); p!=NULL; p=strtok(NULL,":")){
        sprintf (aux2,"%s/%s",p,s);
        if (lstat(aux2,&st)!=-1)
            return aux2;
    }

    return s;
}

int OurExecvpe(char *file, char *const argv[], char *const envp[]){
    return (execve(executable(file),argv, envp));
}

void execute(char *trozos[]){
    char *envp[MAX/2], *argv[MAX/2];
    int i = 0, executablePos, ret, pos;

    while((pos = findVar(trozos[i], __environ)) != -1){
        envp[i] = __environ[pos];
        i++;
    }
    envp[i] = NULL;

    executablePos = i;

    while(trozos[i] != NULL){
        argv[i-executablePos] = trozos[i];
        if(strcmp("&", trozos[i]) == 0 || trozos[i][0] == '@'){
            break;
        }
        i++;
    }
    argv[i-executablePos] = NULL;

    if(envp[0] == NULL){
        ret = OurExecvpe(trozos[executablePos], argv, __environ);
    }else{
        ret = OurExecvpe(trozos[executablePos], argv, envp);
    }

    if(ret == -1){
        perror("ERROR");
        return;
    }
}

char * checkPrio(char *trozos[]){

    for(int i = 0; trozos[i] != NULL; i++){
        if(trozos[i][0] == '@'){
            return trozos[i] + 1;
        }
    }

    return NULL;
}

bool checkBackground(char *trozos[]){

    for(int i = 0; trozos[i] != NULL; i++){
        if(!strcmp(trozos[i], "&")){
            return true;
        }
    }

    return false;
}
