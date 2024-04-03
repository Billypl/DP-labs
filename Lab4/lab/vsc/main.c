#include <Windows.h>
#include "includes.h"

struct ProcessesHandler
{
    int currProcNum;
    PROCESS_INFORMATION processes[MAX_PROCESSES_NUMBER];
    bool isEnded[MAX_PROCESSES_NUMBER];
} processesHandler = {0};

int startNewProcess(DWORD priority);

int fullProcessClose(int processNumber);
int terminateProcess(int processNumber);
void closeProcessHandles(int processNumber);

void handleNewProcessMenu()
{
    indent();
    int num;
    DWORD priority = 0;

    while (true)
    {
        LOGGER(printf("Choose priority: \n"));
        LOGGER(printf("1. REALTIME_PRIORITY_CLASS\n"));
        LOGGER(printf("2. HIGH_PRIORITY_CLASS\n"));
        LOGGER(printf("3. NORMAL_PRIORITY_CLASS\n"));
        LOGGER(printf("4. IDLE_PRIORITY_CLASS\n"));
        scanf("%d", &num);

        switch (num)
        {
        case 1:
            priority = REALTIME_PRIORITY_CLASS;
            break;
        case 2:
            priority = HIGH_PRIORITY_CLASS;
            break;
        case 3:
            priority = NORMAL_PRIORITY_CLASS;
            break;
        case 4:
            priority = IDLE_PRIORITY_CLASS;
            break;
        default:
            break;
        }
        if (priority != 0)
        {
            startNewProcess(priority);
            system("cls");
            LOGGER(printColored(printf("Successfully created process with index: %d \n", processesHandler.currProcNum), GREEN));
            deindent();
            return;
        }
    }
}

void handleProcessStopping()
{
    indent();
    int processNumber;
    while (true)
    {
        LOGGER(printf("Which process you want to close?\n"));
        scanf("%d", &processNumber);
        if (terminateProcess(processNumber) == 0)
        {
            system("cls");
            LOGGER(printColored(printf("Successfully closed process with index: %d \n", processNumber), GREEN));
            deindent();
            return;
        }
    }
}

void handleProcessesShowing()
{
    indent();
    if (processesHandler.currProcNum == 0)
    {
        LOGGER(printf("List is empty\n"));
    }
    for (int i = 0; i < processesHandler.currProcNum; i++)
    {
        PROCESS_INFORMATION *procInfo = &processesHandler.processes[i];
        FILETIME createTime, exitTime, kernelTime, userTime;
        SYSTEMTIME sysCreateTime, sysExitTime;

        if (GetProcessTimes(procInfo->hProcess, &createTime, &exitTime, &kernelTime, &userTime))
        {
            LOGGER(printf("Process number: %d\n", i));
            FileTimeToSystemTime(&createTime, &sysCreateTime);
            LOGGER(printColored(printf("Process creation time: %02d:%02d:%02d\n",
                                       sysCreateTime.wHour, sysCreateTime.wMinute, sysCreateTime.wSecond),
                                GREEN));
            FileTimeToSystemTime(&exitTime, &sysExitTime);
            if (exitTime.dwLowDateTime == 0 && exitTime.dwHighDateTime == 0)
            {
                LOGGER(printColored(printf("Process destroy time: "), RED));
                printf("Process is still running\n");
            }
            else
            {
                LOGGER(printColored(printf("Process destroy time: %02d:%02d:%02d\n",
                                           sysExitTime.wHour, sysExitTime.wMinute, sysExitTime.wSecond),
                                    RED));
            }
        }
        else
        {
            LOGGER(printColored(printf("Failed to get process times. Error code: %d\n", GetLastError()), RED));
        }
    }
    LOGGER(printf("Press any key to go back\n"));
    getchar();
    getchar();
    deindent();
    system("cls");
}

void handleProcessesMenu()
{
    indent();
    int num;
    while (true)
    {
        LOGGER(printf("1. Start new process (with configurable priority)\n"));
        LOGGER(printf("2. Stop a process\n"));
        LOGGER(printf("3. Show processes list\n"));
        LOGGER(printf("4. Close the program\n"));
        scanf("%d", &num);

        switch (num)
        {
        case 1:
            handleNewProcessMenu();
            break;
        case 2:
            handleProcessStopping();
            break;
        case 3:
            handleProcessesShowing();
            break;
        case 4:
            return;
        default:
            break;
        }
    }
    deindent();
}

int main()
{
    printColored(printf("Start\n"), GREEN);
    handleProcessesMenu();
    for (int i = 0; i < processesHandler.currProcNum; i++)
    {
        if (processesHandler.isEnded[i] == false)
        {
            terminateProcess(i);
        }
        closeProcessHandles(i);
    }
    printColored(printf("Finish\n"), GREEN);
    return 0;
}

/*------------------------------------------------------------------------------------------------------------*/
int startNewProcess(DWORD priority)
{
    char exe_path[] = "sub.exe";
    STARTUPINFO structStartupInfo; /* struktura startowa */
    PROCESS_INFORMATION subProcInfo;
    // wypełniamy strukturę startową danymi bieżącego procesu
    GetStartupInfo(&structStartupInfo);

    if (!CreateProcess(NULL,               /* lpApplicationName - nazwa modułu, zostawiamy puste */
                       exe_path,           /* lpCommandLine - lista parametrów, jeśli lpApplicationName == NULL to pierwszy parametr tu jest ścieżką do pliku EXE który będzie procesem podrzędnym */
                       NULL,               /* lpProcessAttributes - wskaźnik do struktury SECURITY_ATTRIBUTES, która określa, czy zwrócone dojście do nowego obiektu procesu może być dziedziczone przez procesy potomne */
                       NULL,               /* lpThreadAttributes */
                       FALSE,              /* bInheritHandles */
                       CREATE_NEW_CONSOLE, /* dwCreationFlags - ustawiając flagę CREATE_NEW_CONSOLE umożliwiamy otwarcie nowego okna konsoli dla procesu podrzędnego */
                       0,                  /* lpEnvironment */
                       0,                  /* lpCurrentDirectory */
                       &structStartupInfo, /* lpStartupInfo - wskaźnik na strukturę startową */
                       &subProcInfo        /* lpProcessInformation - wskaźnik na strukturę z danymi procesu podrzędnego */
                       ))
    {
        LOGGER(printColored(printf("CreateProcess failed (%d).\n", GetLastError()), RED));
        return 1;
    }

    processesHandler.processes[processesHandler.currProcNum] = subProcInfo;
    processesHandler.currProcNum++;

    return 0;
}

int terminateProcess(int processNumber)
{
    // próbujemy zakończyć proces podrzędny (bo sam się nie skończy wraz z zakończeniem procesu nadrzędnego!)
    // niebezpieczna metoda
    if (!TerminateProcess(processesHandler.processes[processNumber].hProcess, 0))
    {
        // LOGGER(printColored(printf("TerminateProcess failed (%d).\n", GetLastError()), RED));
        return 2;
    }
    processesHandler.isEnded[processNumber] = true;
    return 0;
}

void closeProcessHandles(int processNumber)
{
    // zamykamy stosowne uchwyty
    CloseHandle(processesHandler.processes[processNumber].hProcess);
    CloseHandle(processesHandler.processes[processNumber].hThread);
}

int fullProcessClose(int processNumber)
{
    int ret = terminateProcess(processNumber);
    closeProcessHandles(processNumber);
    return ret;
}