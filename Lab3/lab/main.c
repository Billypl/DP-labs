#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    printf("Hello, World!\n");
    char *commands[] = {"sleep 10", "sleep 3", "sleep 5"};
    const int elementsCount = sizeof(commands) / sizeof(commands[0]);
    int pid[elementsCount];

    for (int i = 0; i < elementsCount; i++)
    {
        int p = fork();
        if (p == 0)
        {
            printf("proces o pid %d zaczyna się ", getpid());
            printf("(komenda: %s) \n", commands[i]);
            system(commands[i]);
            printf(" proces o pid %d konczy dzialanie ", getpid());
            printf("(komenda: %s) \n", commands[i]);
            return 0;
        }
        else
        {
            pid[i] = p;
        }
    }
    for (int j = 0; j < elementsCount; j++)
    {
        waitpid(pid[j], NULL, 0);
    }
    printf("Wszysto sie skonczylo \n");

    return 0;
}
// gcc main.c -o main