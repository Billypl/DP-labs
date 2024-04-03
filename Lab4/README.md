Lab 4 - wieloprocesowość i wielowątkowość w systemie Windows

Przykładowy plik (C, Windows) pokazujący tworzenie nowego procesu: example/TestProcesyWin.c

Zadanie zaliczeniowe (C, Windows):
Uwaga: za pozostawione w kodzie C nieobsłużone (a obsługiwalne) ostrzeżenia (warnings) punktacja za rozwiązane zadanie będzie zmniejszana o 10%-20% (w zależności od ilości i znaczenia ostrzeżeń).

Należy napisać program konsolowy
(**ProgramGlowny.exe**) w którym można (za pomocą komend "menu" - opisane!):

1. stworzyć nowy proces (koniecznie uruchamiany w nowym oknie konsoli) o konfigurowalnym priorytecie uruchamiający ProgramPodrzedny.exe;
2. zatrzymać wybrany proces;
3. wyświetlić listę procesów uruchomionych z aplikacji;
4. zamknąć program (+zatrzymanie wszystkich procesów).

**ProgramPodrzedny.exe** należy również napisać jako program konsolowy udostępniający (swoje własne!) komendy menu (opisane):

1. utwórz nowy wątek (uruchamiający jakąś dowolną procedurę obliczeniową, najlepiej o konfigurowalnych parametrach, różniących się przy kolejnych wywołaniach tworzenia nowego wątku, która trwa ok. 1 min.) o konfigurowalnym priorytecie;
2. usuń wybrany wątek;
3. zmień priorytet wybranego wątku;
4. wyświetl listę wątków (uruchomionych w ramach tego procesu!);
5. zakończ dany proces (+zatrzymanie wszystkich wątków uruchomionych w tym procesie).
