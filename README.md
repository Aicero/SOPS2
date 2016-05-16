# [SOPS2] Projekt 2: Problem śpiącego fryzjera
Salon fryzjerski składa się z gabinetu z jednym fotelem oraz z poczekalni zawierającej n krzeseł. W danym momencie w gabinecie może być strzyżony tylko jeden klient, reszta czeka na wolnych krzesłach w poczekalni. Fryzjer po skończeniu strzyżenia prosi do gabinetu kolejnego klienta, lub ucina sobie drzemkę, jeśli poczekalnia jest pusta. Nowy klient budzi fryzjera jeśli ten śpi, lub siada na wolne miejsce w poczekalni jeśli fryzjer jest zajęty. Jeśli poczekalnia jest pełna, to klient nie wchodzi do niej i rezygnuje z wizyty.

Napisz program koordynujący pracę gabinetu. Zsynchronizuj wątki klientów i fryzjera:
 - bez wykorzystania zmiennych warunkowych (tylko mutexy/semafory) **[17 p]**
 - wykorzystując zmienne warunkowe (condition variables) **[17 p]**


Aby móc obserwować działanie programu, każdemu klientowi przydziel numer. Program powinien wypisywać komunikaty według poniższego przykładu:

        Res:2 WRomm: 5/10 [in: 4]

Oznacza to, że dwóch klientów zrezygnowało z powodu braku miejsc (*Res*), w poczekalni (*WRoom*) zajętych jest 5 z 10 krzeseł, a w gabinecie obsługiwany jest klient o numerze 4. Po uruchomieniu programu z parametrem *-debug* należy wypisywać cała kolejka klientów czekających, a także lista klientów, którzy nie dostali się do gabinetu. Komunikat należy wypisywać w momencie zmiany którejkolwiek z tych wartości.


### Uwagi dot. sprawozdania:

 - Rozwiązania wykorzystujące zmienne warunkowe muszą posiadać kolejkę FIFO dla czekających procesów.
 - **Proszę koniecznie zaznaczyć wybraną wersję projektu (tym samym oczekiwaną ilość punktów).**
 - Zamieścić w sprawozdaniu tą część kod programu, wyróżniając (np. pogrubioną czcionką) fragmenty korzystające z mechanizmów synchronizacji.
 - Opisać konkretne przeznaczenie i sposób wykorzystania każdego mechanizmu synchronizacji (semafora, mutexu, zmiennej warunkowej). Można to zrobić w formie komentarzy do kodu umieszczonych w miejscach, gdzie używany jest któryś z tych mechanizmów).
 - Za każdy przypadek potencjalnego wyścigu **-3p**.


### Zarys działania programu
##### *1. Parametry wejściowe:*

        liczba krzeseł w poczekalni (*)
        -debug (opcjonalny) - wypisywana jest cała kolejka klientów czekających oraz tych, którzy zrezygnowali z usług (komunikat wypisywany w momencie zmiany którejkolwiek z tych wartości)
