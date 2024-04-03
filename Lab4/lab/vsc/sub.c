#include <Windows.h>
#include "includes.h"

const char animation[] = {'|', '/', '-', '\\'};
const int animationLength = sizeof(animation) / sizeof(animation[0]);
// | / - \\
// ⠁⠂⠄⡀⢀⠠⠐⠈
// ▖ ▘ ▝ ▗
// ┤ ┘ ┴ └ ├ ┌ ┬ ┐

struct threadData
{
    DWORD id;
    int threadNumber;
    int counter;
    int maxCounter;
    int animationState;
    bool isDeleted;
    bool isCompleted;
};

COORD coord = {0, 0};
void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
HANDLE mutex;
int globalCounter;
int threadsCount = 0;
HANDLE threads[MAX_THREADS_NUMBER];
struct threadData threadsData[MAX_THREADS_NUMBER];

DWORD WINAPI worker(void *args);

void clearLogsAndGainMutex();

void createNewThread(int THREAD_PRIORITY, int argument);
void deleteThread(int index);

void handleThreadsMenu();
void handleNewThreadCreation();
void handleThreadDeletion();
void handleThreadPriorityChange();
void changeThreadPriority(int index, int NEW_PRIORITY);
int handlePriorityChoosing();
int handleThreadSpeedChoosing();

int main(int argc, char **argv)
{
    mutex = CreateMutex(NULL, FALSE, NULL);
    int i;
    system("cls");
    printf("Uruchomienie programu \n");
    handleThreadsMenu();
    printf("FINISED - wait for 1s\n");
    return 0;
}

void printLoadingBar(struct threadData *argumentData)
{
    double progress = (double)++argumentData->counter / argumentData->maxCounter;
    argumentData->animationState = (argumentData->animationState + 1) % 4;

    WaitForSingleObject(mutex, INFINITE);

    gotoxy(17, LOADING_BARS_OFFSET + argumentData->threadNumber);
    printf("%c", animation[argumentData->animationState]);
    for (int i = 0; i < (int)(progress * LOADING_BAR_LENGTH); ++i)
    {
        printf(LOADING_BAR_CHAR);
    }
    gotoxy(0, LOADING_BARS_OFFSET + argumentData->threadNumber);
    printf("T%d: % 5x [% 4d%%]\n", argumentData->threadNumber, argumentData->id, (int)(progress * 100));
    gotoxy(0, 0);
    ReleaseMutex(mutex);
}

DWORD WINAPI worker(void *args)
{
    struct threadData *argumentData = (struct threadData *)args;
    int workerDuration = 60000;
    int sleepTime = argumentData->maxCounter;

    gotoxy(1, argumentData->maxCounter);
    while (argumentData->counter < argumentData->maxCounter && !argumentData->isDeleted)
    {
        Sleep(sleepTime);
        printLoadingBar(argumentData);
    }
    if (argumentData->isDeleted)
    {
        deleteThread(argumentData->threadNumber);
    }
    argumentData->isCompleted = true;
    return 0;
}

void clearLogsAndGainMutex()
{
    WaitForSingleObject(mutex, INFINITE);
    gotoxy(0, 0);
    for (int i = 0; i < LOADING_BARS_OFFSET; i++)
    {
        printf("                                                                 \n");
    }
    gotoxy(0, 5);
}

void createNewThread(int THREAD_PRIORITY, int argument)
{
    threadsData[threadsCount].counter = 0;
    threadsData[threadsCount].maxCounter = argument;
    threadsData[threadsCount].threadNumber = threadsCount;
    threadsData[threadsCount].animationState = 0;
    threadsData[threadsCount].isDeleted = false;
    threadsData[threadsCount].isCompleted = false;

    threads[threadsCount] = CreateThread(
        NULL,                               // atrybuty bezpieczenstwa
        0,                                  // inicjalna wielkosc stosu
        worker,                             // funkcja watku
        (void *)&threadsData[threadsCount], // dane dla funkcji watku
        0,                                  // flagi utworzenia
        (&threadsData[threadsCount].id));
    SetThreadPriority(threads[threadsCount], THREAD_PRIORITY);
    threadsCount++;
}

void deleteThread(int index)
{
    threadsData[index].isDeleted = true;
    WaitForSingleObject(mutex, INFINITE);
    gotoxy(17, LOADING_BARS_OFFSET + threadsData[index].threadNumber);
    printf("[DELETED]                                             \n");
    ReleaseMutex(mutex);
}

void handleThreadDeletion()
{

    int num;
    clearLogsAndGainMutex();

    gotoxy(0, 10);
    printf(threadsCount == 0 ? "No threads aviable!\n" : "Provide index of a thread to delete:\n");

    ReleaseMutex(mutex);

    if (threadsCount == 0)
    {
        return;
    }

    while (true)
    {
        scanf("%d", &num);
        if (num >= 0 && num < threadsCount)
        {
            deleteThread(num);
            return;
        }
    }
}

void changeThreadPriority(int index, int NEW_PRIORITY)
{
    SetThreadPriority(threads[index], NEW_PRIORITY);
}

int handlePriorityChoosing()
{
    int num;
    int priority = -2137;
    clearLogsAndGainMutex();
    printf("Choose priority: \n");
    printf("1. THREAD_PRIORITY_ABOVE_NORMAL\n");
    printf("2. THREAD_PRIORITY_BELOW_NORMAL\n");
    printf("3. THREAD_PRIORITY_HIGHEST\n");
    printf("4. THREAD_PRIORITY_IDLE\n");
    printf("5. THREAD_PRIORITY_LOWEST\n");
    printf("6. THREAD_PRIORITY_NORMAL\n");
    printf("7. THREAD_PRIORITY_TIME_CRITICAL\n");
    gotoxy(0, 0);
    ReleaseMutex(mutex);

    while (true)
    {
        scanf("%d", &num);
        switch (num)
        {
        case 1:
            priority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case 2:
            priority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case 3:
            priority = THREAD_PRIORITY_HIGHEST;
            break;
        case 4:
            priority = THREAD_PRIORITY_IDLE;
            break;
        case 5:
            priority = THREAD_PRIORITY_LOWEST;
            break;
        case 6:
            priority = THREAD_PRIORITY_NORMAL;
            break;
        case 7:
            priority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
        default:
            break;
        }

        if (priority != -2137)
        {
            return priority;
        }
    }
}
void handleThreadPriorityChange()
{
    int priority = handlePriorityChoosing();
    int num;

    clearLogsAndGainMutex();

    gotoxy(0, 10);
    printf("Provide index of a thread to change priority:\n");

    ReleaseMutex(mutex);

    while (true)
    {
        scanf("%d", &num);
        if (num >= 0 && num < threadsCount)
        {
            changeThreadPriority(num, priority);
            return;
        }
    }
}
void handleNewThreadCreation()
{
    int priority = handlePriorityChoosing();
    int speed = handleThreadSpeedChoosing();
    if (priority != -2137)
    {
        createNewThread(priority, speed);
        clearLogsAndGainMutex();
        gotoxy(0, 0);
        printColored(printf("Successfully created thread with index: %d \n", threadsCount), GREEN);
        gotoxy(0, 5);
        ReleaseMutex(mutex);
        return;
    }
}

int handleThreadSpeedChoosing()
{
    int num;
    clearLogsAndGainMutex();

    printf("Provide workload (the greater the longer work time (about 50-500))): \n");
    gotoxy(0, 0);

    ReleaseMutex(mutex);
    scanf("%d", &num);
    return num;
}
void handleThreadsMenu()
{
    int num;
    gotoxy(0, 10);
    while (true)
    {
        WaitForSingleObject(mutex, INFINITE);
        gotoxy(0, 10);
        printf("1. Create new thread (with argument and configurable priority)\n");
        printf("2. Delete a thread\n");
        printf("3. Change thread priority\n");
        printf("4. Finish process\n");
        ReleaseMutex(mutex);
        scanf("%d", &num);
        switch (num)
        {
        case 1:
            handleNewThreadCreation();
            break;
        case 2:
            handleThreadDeletion();
            break;
        case 3:
            handleThreadPriorityChange();
            break;
        case 4:
            return;
        default:
            break;
        }
    }
}
