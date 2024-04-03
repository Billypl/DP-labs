#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#pragma argsused

struct dane_dla_watku
{
    char nazwa[50];
    int parametr;
} dane[3] = {{"[1]", 5}, {"[2]", 8}, {"[3]", 12}};

COORD coord = {0, 0};
void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int priorytety[3] = {THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_ABOVE_NORMAL};

HANDLE watki[3];
DWORD WINAPI funkcja_watku(void *argumenty);

int main(int argc, char **argv)
{
    int i;
    DWORD id;
    system("cls");
    printf(" Uruchomienie programu \n ");

    // tworzenie watkow
    for (i = 0; i < 3; i++)
    {
        watki[i] = CreateThread(
            NULL,             // atrybuty bezpieczenstwa
            0,                // inicjalna wielkosc stosu
            funkcja_watku,    // funkcja watku
            (void *)&dane[i], // dane dla funkcji watku
            0,                // flagi utworzenia
            &id);
        if (watki[i] != INVALID_HANDLE_VALUE)
        {
            printf(" Utworzylem watek %s o id %x \n ", dane[i].nazwa, id);
            SetThreadPriority(watki[i], priorytety[i]);
        }
    }
    Sleep(20000);
    return 0;
}

DWORD WINAPI funkcja_watku(void *argumenty)
{
    unsigned int licznik = 0;
    struct dane_dla_watku *moje_dane = (struct dane_dla_watku *)argumenty;
    // wyswietlenie informacji o uruchomieniu
    gotoxy(1, moje_dane->parametr);
    printf("% s ", moje_dane->nazwa);
    Sleep(1000);
    // praca , watki sa terminowane przez zakonczenie programu funkcji main
    while (1)
    {
        gotoxy(licznik++ / 5000 + 5, moje_dane->parametr + 1);
        printf(".");
    }
    return 0;
}