# Zmienione pliki
- `usr/src/fs/table.c` - dodano puste handlery nowych wywołań
- `usr/include/minix/com.h` - dodano numery wywołań
- `usr/include/minix/callnr.h` - dodano numery wywołań
- `usr/src/mm/proto.h` - dodano prototypy wywołań
- `usr/src/mm/getset.c` - dodano prototypy wywołań
- `/usr/src/kernel/clock.c` - zmieniono częstotliwość wywołania sched()
- `/usr/src/kernel/proc.c` - zmienono algorytm szeregowania w funkcji sched()
- `/usr/src/mm/table.c` - dodano funkcji obsługujące wywołania
- `/usr/include/getset_group.h` - warppery wywołań
- `/usr/src/mm/getset.h` - obsługa wywołań
- `/usr/src/kernel/proc.h` - dodanie grupy ptocesu
- `/usr/src/kernel/system.c` - ustawianie domyślnie grupy A
- `/usr/src/kernel/main.c` - ustawianie domyślnie grupy A


# Algorytm szeregowania - koncepcja
Każdy proces posiada pole p_group, które informuje do jakiej grupy nalezy proces. W pliku `proc.c` w funkcji `sched()` zaostał zaimplementowany algorytm szeregowania. Algorytm szuka po kolei procesów z grupy A, B, C i wstawie odpowiedni na początek listy. Brana pod uwagę jest sytuacja gdy ostatni element jest przesuwany na początek. Ogon listy `rdy_tail[USER_Q]` jest wtedy przesuwany adekwatnie.

Odpowiedni kwant czasu jest zagwarantowany poprzez modyfikacje funkcji `do_clocktick()`. Zmiennej `sched_tick` jest przypisywany odpowiedni kwant czasu dla procesu z danej grupy.

# Testy

Przykładowo: nadanie A 2 kawnty, B 4 kwanty, C 6 kwantów. Stworznie procesów i nadanie im odpowiednich grup. Proporcja czasów powinna wyjść 1:2:3.