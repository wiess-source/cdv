#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#define MSGSIZE 255
#define SIZE 10

int main()
{
    pid_t child_pid, wpid;
    int status = 0;
    int n = 10;
    int p[2];
    int buf[MSGSIZE];
    char string1[100];
    char string2[100] = ".txt";
    //Father code (before child processes start)

    printf("Podaj nazwe pliku: \n");
    gets(string1);
    strcat(string1, string2);

    if (pipe(p) < 0)
        exit(1);

    FILE* plik;
    plik = fopen(string1, "w");


    int tab[SIZE] = { 0 };
    for (int id = 0; id < n; id++) {
        if ((child_pid = fork()) == 0) {

            pid_t y = getpid(); /* Process ID - PID */
            fprintf(plik, "Wywolanie nr %i. Numer procesu: %i \n", id + 1, y);

            tab[id] = y;
            write(p[1], &tab[id], sizeof(tab[id]));

            exit(0);
        }
    }
    close(p[1]);
    fclose(plik);

    while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes

    for (int id = 0; id < n; id++) {
        read(p[0], &buf[id], sizeof(buf[id]));
        printf("Wywolanie nr %i. Numer procesu: %i \n", id + 1, buf[id]);
    }

    close(p[0]);
    printf("\nKoniec programu!\n");
    return 0;
    //Father code (After all child processes end)
}
