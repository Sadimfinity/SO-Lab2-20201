#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char *argv[]) {
    int pid;
    char * comando = (char *) malloc(1 + strlen("/bin/")+ strlen(argv[1]) );
    strcpy(comando, "/bin/");
    strcat(comando, argv[1]);

    printf("el comando es %s\n", comando);
    printf("PADRE: Soy el proceso padre y mi pid es: %d\n", getpid());
    struct timeval start, end;
    gettimeofday(&start, NULL);


    pid = fork();

    // En cuanto llamamos a fork se crea un nuevo proceso. En el proceso
    // padre 'pid' contendrá el pid del proceso hijo. En el proceso hijo
    // 'pid' valdrá 0. Eso es lo que usamos para distinguir si el código
    // que se está ejecutando pertenece al padre o al hijo.

    if (pid) // Este es el proceso padre
    {
        printf("PADRE: Soy el proceso padre y mi pid sigue siendo: %d\n", getpid());
        wait(NULL);
        printf("PADRE: Proceso terminado\n");
        gettimeofday(&end, NULL);
        printf("El tiempo que se demoro el comando fue %ld micro segundos\n",
        ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
        
    }
    else // Proceso hijo
    {   
        printf("HIJO: Soy el proceso hijo y mi pid es: %d\n", getpid());
        execl(comando, comando, NULL);
        
    }
}
