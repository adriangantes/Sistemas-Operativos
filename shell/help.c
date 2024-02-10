/*
 * TITLE: SO LABS
 * AUTHOR 1: Adrián Edreira Gantes LOGIN 1: adrian.gantes@udc.es
 * GROUP: 1.2
 */

#include "help.h"

void printHelp (){
    printf ("Comandos disponibles:\n\n");
    printf ("authors   pid   chdir   date   time   hist   comand\n");
    printf ("open   close   dup   listopen   infosys   help\n");
    printf ("quit   exit   bye   list   create   stat   delete\n");
    printf ("deltree   malloc   shared   mmap   read   write\n");
    printf ("memdump   memfill   mem   recurse   uid   showvar\n");
    printf ("changevar   subsvar   showenv   fork   exec   jobs\n");
    printf ("deljobs   job\n");
}

void printComandHelp (char *trozos[]){
    if (strcmp(trozos[1], "quit") == 0 || strcmp(trozos[1], "exit") == 0 || strcmp(trozos[1], "bye") == 0 ){
        printf("*  quit/exit/bye - Finaliza la ejecución de la shell.\n");
    }

    else if (strcmp(trozos[1], "hist") == 0){
        printf("*  hist - Imprime todos los comandos que se han introducido con su número de orden.\n");
        printf("\t -c - Borra (vacía) la lista de comandos históricos.\n");
        printf("\t -n - Imprime los primeros n comandos históricos (Espera un entero positivo).\n");
    }

    else if (strcmp(trozos[1], "authors") == 0){
        printf("*  authors - Muestra los nombres y logins de los creadores de esta shell.\n");
        printf("\t -n - Muestra solo los nombres de los creadores.\n");
        printf("\t -l - Muestra solo los logins de los creadores.\n");
    }

    else if (strcmp(trozos[1], "pid") == 0){
        printf("*  pid - Imprime el PID del proceso que ejecuta la shell.\n");
        printf("\t -p - Imprime el PID del proceso padre de la shell.\n");
    }

    else if (strcmp(trozos[1], "date") == 0){
        printf("*  date - Imprime la fecha actual en formato DD/MM/YYYY.\n");
    }

    else if (strcmp(trozos[1], "time") == 0){
        printf("*  time - Imprime la hora actual en formato hh:mm:ss.\n");
    }

    else if (strcmp(trozos[1], "comand") == 0){
        printf("*  comand [N] - Repite el comando número N de la lista histórica (espera un entero).\n");
    }

    else if (strcmp(trozos[1], "infosys") == 0){
        printf("*  infosys - Imprime información sobre la máquina que ejecuta la shell.\n");
    }

    else if (strcmp(trozos[1], "help") == 0){
        printf("*  help - Muestra una lista de comandos disponibles.\n");
        printf("\t [cmd] - Muestra información sobre el comando \"cmd\".\n");
    }

    else if (strcmp(trozos[1], "chdir") == 0){
        printf("*  chdir - Imprime el directorio de trabajo actual.\n");
        printf("\t [dir] - Cambia el directorio de trabajo actual a 'dir'.\n");
    }

    else if (strcmp(trozos[1], "open") == 0){
        printf("* open [file] [mode] - Abre el archivo file y lo añade (junto con el descriptor de archivo) a la lista de archivos abiertos de la shell.\n");
        printf("\t [mode] - Puede ser: cr para O_CREAT, ap para O_APPEND, ex para O_EXCL, ro para O_RDONLY, rw para O_RDWR, wo para O_WRONLY y tr para O_TRUNC.\n");
    }

    else if (strcmp(trozos[1], "close") == 0) {
        printf("*  close [df] - Cierra el descriptor de archivo df y elimina el elemento correspondiente de la lista.\n");
    }

    else if (strcmp(trozos[1], "dup") == 0) {
        printf("*  dup [df] - Duplica el descriptor del archivo df.\n");
    }

    else if (strcmp(trozos[1], "listopen") == 0) {
        printf("*  listopen - Lista los archivos abiertos de la shell con su descripción, nombre y modo de apertura.\n");
    }

    else if(strcmp(trozos[1], "list") == 0){
        printf("*  list - Lista los contenidos del directorio.\n");
        printf("\t -hid - Incluye los ficheros ocultos.\n");
        printf("\t -recb - Recursivo (antes).\n");
        printf("\t -reca - Recursivo (despues).\n");
        printf("\t -long | -link | -acc - Mismo funcionamiento que en stat.\n");
    }

    else if(strcmp(trozos[1], "create") == 0){
        printf("*  create - Crea archivos o directorios.\n");
        printf("\t -f - Crea un archivo.\n");
    }

    else if(strcmp(trozos[1], "stat") == 0){
        printf("*  stat - Da información de los directorios y los archivos.\n");
        printf("\t -long - Listado largo.\n");
        printf("\t -acc - Accestime.\n");
        printf("\t -link - Si es enlace simbolico devuelve el path contenido.\n");
    }

    else if(strcmp(trozos[1], "delete") == 0){
        printf("*  delete - Borra archivos o directorios vacíos.\n");
    }

    else if(strcmp(trozos[1], "deltree") == 0){
        printf("*  deltree - Borra archivos o directorios no vacíos de manera recursiva.\n");
    }

    else if(strcmp(trozos[1], "malloc") == 0){
        printf("*  malloc [tam] - Asigna un bloque de memoria de tamaño [tam] (espera un entero positivo).\n");
        printf("\t -free - Desasigna un bloque de memoria de tamaño tam.\n");
    }

    else if(strcmp(trozos[1], "shared") == 0){
        printf("*  shared [cl] [tam] - Asigna memoria compartida con la clave cl en el programa.\n");
        printf("\t -free - Desmapea el bloque de memoria compartida de la clave.\n");
        printf("\t -create - Asigna el bloque de memoria compartida de clave cl y tamaño tam.\n");
        printf("\t -delkey - Elimina del sistema la clave de memoria cl.\n");
    }

    else if(strcmp(trozos[1], "mmap") == 0){
        printf("*  mmap [f] [prm] - Mapea el fichero f con permisos prm.\n");
        printf("\t -free - Desmapea el fichero f.\n");
    }

    else if(strcmp(trozos[1], "read") == 0){
        printf("*  read [f] [dir] [n] - Lee n bytes desde f a la direccion dir.\n");
    }

    else if(strcmp(trozos[1], "write") == 0){
        printf("*  write [f] [dir] [n] - Escribe n bytes desde la direccion dir a f.\n");
        printf("\t -o - Sobreescribe.\n");
    }

    else if(strcmp(trozos[1], "memdump") == 0){
        printf("*  memdump [dir] [n] - Vuelca en pantallas los contenidos (n bytes) de la posicion de memoria dir.\n");
    }

    else if(strcmp(trozos[1], "memfill") == 0){
        printf("*  memfill [dir] [n] [byte] - Llena la memoria a partir de dir con n bytes (byte).\n");
    }

    else if(strcmp(trozos[1], "mem") == 0){
        printf("*  mem - Muestra detalles de la memoria del proceso.\n");
        printf("\t -blocks - Muestra los bloques de memoria asignados.\n");
        printf("\t -funcs - Muestra las direcciones de las funciones.\n");
        printf("\t -vars - Muestra las direcciones de las variables.\n");
        printf("\t -all - Todas las opciones.\n");
        printf("\t -pmap - Muestra la salida del comando pmap.\n");
    }

    else if(strcmp(trozos[1], "recurse") == 0){
        printf("*  recurse [n] - Invoca a la función recursiva n veces (espera un entero positivo).\n");
    }

    else if(strcmp(trozos[1], "uid") == 0){
        printf("*  uid - Accede a las credenciales del proceso que ejecuta el shell.\n");
        printf("\t -get - Muestra las credenciales.\n");
        printf("\t -set [id] - Establece la credencial al valor numerico id.\n");
        printf("\t -set -l [id] - Establece la credencial a login id.\n");
    }

    else if(strcmp(trozos[1], "showvar") == 0){
        printf("*  showvar [var] - Muestra el valor y las direcciones de la variable de entorno var.\n");
    }

    else if(strcmp(trozos[1], "changevar") == 0){
        printf("*  changevar [var] [valor] - Cambia el valor de una variable de entorno.\n");
        printf("\t -a - Accede por el tercer argumento de main.\n");
        printf("\t -e - Accede mediante environ.\n");
        printf("\t -p - Accede mediante putenv.\n");
    }

    else if(strcmp(trozos[1], "subsvar") == 0){
        printf("*  subsvar [var1] [var2] [valor] - Sustituye la variable de entorno var1 con var2 = valor.\n");
        printf("\t -a - Accede por el tercer argumento de main.\n");
        printf("\t -e - Accede mediante environ.\n");
    }

    else if(strcmp(trozos[1], "showenv") == 0){
        printf("*  showenv - Muestra el entorno del proceso.\n");
        printf("\t -environ - Accede usando environ (en lugar del tercer argumento de main).\n");
        printf("\t -addr - Muestra el valor y donde se almacenan environ y el tercer argumento de main.\n");
    }

    else if(strcmp(trozos[1], "fork") == 0){
        printf("*  fork - El shell hace fork y queda en espera a que su hijo termine.\n");
    }

    else if(strcmp(trozos[1], "exec") == 0){
        printf("*  exec [var1] [var2] ... prog args ... [@pri] - Ejecuta, sin crear proceso, prog con argumentos en un\n"
               "\t\t\tentorno que contiene solo las variables var1, var2...\n");
    }

    else if(strcmp(trozos[1], "jobs") == 0){
        printf("*  jobs - Lista los procesos en segundo plano.\n");
    }

    else if(strcmp(trozos[1], "deljobs") == 0){
        printf("*  deljobs - Elimina los procesos de la lista procesos en segundo plano.\n");
        printf("\t -term - Los terminados.\n");
        printf("\t -sig - Los terminados por signal.\n");
    }

    else if(strcmp(trozos[1], "job") == 0){
        printf("*  job [pid] - Muestra informacion del proceso pid.\n");
        printf("\t -fg - Lo pasa a primer plano.\n");
    }

    else{
        perror("Command not found in help list\n");
    }
}