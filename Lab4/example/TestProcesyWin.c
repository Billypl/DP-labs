#include <stdio.h>
#include "Windows.h"

int new_process_start()
{
    char exe_path[] = "cmd.exe";        /* wiersz poleceń (command line) Windows jako proces podrzędny */
    STARTUPINFO structStartupInfo;      /* struktura startowa */
    PROCESS_INFORMATION structProcInfo; /* struktura z danymi procesu podrzędnego */

    // wypełniamy strukturę startową danymi bieżącego procesu
    GetStartupInfo(&structStartupInfo);

    // tworzymy nowy proces podrzędny
    if (!CreateProcess(NULL,               /* lpApplicationName - nazwa modułu, zostawiamy puste */
                       exe_path,           /* lpCommandLine - lista parametrów, jeśli lpApplicationName == NULL to pierwszy parametr tu jest ścieżką do pliku EXE który będzie procesem podrzędnym */
                       NULL,               /* lpProcessAttributes - wskaźnik do struktury SECURITY_ATTRIBUTES, która określa, czy zwrócone dojście do nowego obiektu procesu może być dziedziczone przez procesy potomne */
                       NULL,               /* lpThreadAttributes */
                       FALSE,              /* bInheritHandles */
                       CREATE_NEW_CONSOLE, /* dwCreationFlags - ustawiając flagę CREATE_NEW_CONSOLE umożliwiamy otwarcie nowego okna konsoli dla procesu podrzędnego */
                       0,                  /* lpEnvironment */
                       0,                  /* lpCurrentDirectory */
                       &structStartupInfo, /* lpStartupInfo - wskaźnik na strukturę startową */
                       &structProcInfo     /* lpProcessInformation - wskaźnik na strukturę z danymi procesu podrzędnego */
                       ))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    // chwilę czekamy (5 sek.) :)
    Sleep(5000);

    // próbujemy zakończyć proces podrzędny (bo sam się nie skończy wraz z zakończeniem procesu nadrzędnego!)
    if (!TerminateProcess(structProcInfo.hProcess, 0))
    {
        printf("TerminateProcess failed (%d).\n", GetLastError());
        return 2;
    }

    // zamykamy stosowne uchwyty
    CloseHandle(structProcInfo.hProcess);
    CloseHandle(structProcInfo.hThread);
    return 0;
}

int main()
{
    printf("Start\n");
    int res = new_process_start();
    printf("Finish\n");
    return res;
}
