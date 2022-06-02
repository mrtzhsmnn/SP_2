**Sichere Programmierung**  
**Projekt 2**  
Nico Haisch  
(84360)  
Moritz Hausmann  
(84367)  
Friedrich Röntsch  
(68522)  

# Einleitung

In Praktikum 2 der Vorlesung "Sichere Programmierung" war es Aufgabe
bereits erlangte Kenntnisse im Debugging von C Programmen mit dem GNU
Debugger (GDB) weiter zu vertiefen. GDB bietet Möglichkeiten zur
Ablaufverfolgung eines Programms wie Breakpoints oder die Ausgabe des
Stacktrace sowie ein Eingreifen in die Ausführung von Programmen. GDB
ermöglicht es Benutzern beispielsweise, die Variablen des Programms zu
manipulieren oder Funktionen unabhängig vom normalen Programmablauf
aufzurufen. Ab Version 7.0 ist die Ablaufverfolgung nicht nur vorwärts,
sondern auch rückwärts möglich (Reverse Debugging). Außerdem kann GDB
mit Python und GNU Guile automatisiert werden.  
  
Um diese Funktionen des GDB zu testen wurden vier C-Programme
vorgegeben, welche zunächst analysiert und anschließend mithilfe des GDB
getestet und weiter untersucht werden sollten. Hierbei wurde der Code im
GDB dissasembliert und mithilfe von Breakpoints der Programmablauf
nachvollzogen. Hierbei galt es einige Auffälligkeiten des jeweiligen
Codes näher zu betrachten und zu analysieren. Auch konkrete Fehler im
Code wurden so ersichtlich, welche dann teilweise korrigiert werden
sollten. Dies war durch die übersichtliche Darstellung im GDB mit seinen
verschiedenen Möglichkeiten der Nachverfolgung mit Breakpoints und des
Stacktrace schnell ersichtlich und umsetzbar.  

# Aufgabe 1

## Teilaufgabe a)

Die erste Aufgabe beginnt einführend mit einem noch relativ
unkomplizierten C-Programm. Welches aus den zehn Zeilen Code

    #include <stdio.h>
    
    int main() {
      unsigned int i;
    
      for (i=0; i<20; i++) {
        printf("i: %2d\n", i); 
      }
      return 0;
    }

besteht.  
  
Der Code beginnt mit einem anfänglichen `#include`-statment in Zeile
eins,

    #include <stdio.h>

mit welchem, die Standard-Bibliothek `stdio.h` für Ein- und Ausgabe
importiert wird. Diese wird benötigt um den in Zeile sieben aufgerufenen
`printf`-Befehl ausführen zu können.  
  
Dem dann folgend, wird mit Hilfe von

    int main() {

die Funktion `main` mit dem Rückgabewert `int` definiert. Die Funktion
erwartet keine Übergabeparameter.  
  
Der initialen Definition der Funktion folgend, kann dann in der vierten
Zeile mittels

    unsigned int i;

die erste und einzige Variable in dem Code definiert werden. Bei dem
Datentyp von `i` handelt es sich um ein sogenannten `unsigned int` also
um einen vorzeichenlosen Integer. Dieser wird zwar deklariert allerdings
nicht direkt initialisiert.  
  
Über die Zeilen sechs bis sieben erstreckt sich die folgende
`for`-Schleife,

    for (i=0; i<20; i++) {
        printf("i: %2d\n", i); 
      }

diese beginnt mit der Zuweisung des Wertes `0` auf den vorzeichenlosen
Integer `i`. Dann wird die Abbruchbedingung der kopfgesteuerten Schleife
definiert, diese soll den Schleifenkörper nämlich nicht mehr
wiederholen, wenn `i` größer als 19 ist. Das `printf`-statment im Körper
der Schleife führt dann zu der Ausgabe `i:` gefolgt von dem Wert der
Variable `i`. In der Zeile acht, wird dann mit der geschlossenen
geschweiften Klammer die Beendigung der `for`-Schleife indiziert.  
  
In der neunten Zeile

    return 0;

wird dann mit dem `return 0` das Programm mit dem Rückgabewert `0` vom
Typ `int` beendet.

## Teilaufgabe b)

Um das Programm ausführen zu können, muss man den Code mit

    gcc gdb-uebung-1.c -g -o gdb-ueb1

also der GNU Compiler Collection, kompilieren. Die flag `-o` zeigt dabei
auf das gewünschte output file welches mit `gdb-ueb1` benannt wurde. Um
debug-Informationen beim kompilieren zu erzeugen setzt man zusätzlich
die flag `-g`.  
  
Führt man das Programm nun mit

    ./gdb-ueb1

aus, erhält man die Ausgabe,

    i:  0
    i:  1
    i:  2
    [...]
    i: 19

welche zeigt, dass der Schleifenkörper wie zu erwarten war `20` mal
ausgeführt wurde. Da die Schleifenvariable `i` mit dem Wert `0`
initialisiert wurde, wird bei der Ausgabe von `i` mit `0` begonnen und
bereits mit `19` beendet.

## Teilaufgabe c)

Um den vorgegebenen C-Code zu disassemblieren muss man das Programm mit
der debug flag `-g` kompiliert haben. Dann wurde der kompilierte code
mit

    objdump -d -M "intel" -S ueb1

aufgerufen. Wobei `-d` das disassemblieren, `-M "intel"` die Darstellung
in Intel-Notation und `-S` die vermischte Ansicht von source code und
assemblercode auslöst. `ueb1` ist das mit debug-option kompilierte
Programm.  
  
Mit der oben beschriebenen und erklärten Eingabe erhält man die stark
gekürzte Ausgabe,

``` 
    [...]
    for (i=0; i<20; i++) {
1155:   c7 45 fc 00 00 00 00    mov    DWORD PTR [rbp-0x4],0x0
115c:   eb 1a                   jmp    1178 <main+0x2f>
    printf("i: %2d\n", i); 
115e:   8b 45 fc                mov    eax,DWORD PTR [rbp-0x4]
1161:   89 c6                   mov    esi,eax
1163:   48 8d 3d 9a 0e 00 00    lea    rdi,[rip+0xe9a]        
116a:   b8 00 00 00 00          mov    eax,0x0
116f:   e8 dc fe ff ff          call   1050 <printf@plt>
    for (i=0; i<20; i++) {
1174:   83 45 fc 01             add    DWORD PTR [rbp-0x4],0x1
1178:   83 7d fc 13             cmp    DWORD PTR [rbp-0x4],0x13
117c:   76 e0                   jbe    115e <main+0x15>
    }
    [...]
```

bei welcher der assembler code und der source code der Aufgabenstellung
vermischt angezeigt werden. Jede Zeile assembler code beginnt mit einer
Adresse im Speicher auf welche auch bei Sprungbefehlen referenziert
wird.  
  
Zunächst wird mit dem assembler-Befehl `mov` in

    1155:   c7 45 fc 00 00 00 00    mov    DWORD PTR [rbp-0x4],0x0

die Variable `i` auf `0` gesetzt. Hierbei ist `rbp-0x4` die
Registeradresse von `i` und `DWORD PTR` referenziert auf ein 32-Bit Wort
an der angegebenen Adresse. `0x0` ist der Hexadezimal-Wert für 0.  
  
Dem folgend wird dann mit dem `jmp`-Befehl in

    115c:   eb 1a                   jmp    1178 <main+0x2f>

auf die assembler-Zeile 1178 gesprungen (hier Zeile 12).  
  
In dieser Zeile wird dann die die `cmp`-Funktion aufgerufen,

    1178:   83 7d fc 13             cmp    DWORD PTR [rbp-0x4],0x13

mit dieser wird `i`, hier referenziert mit `DWORD PTR [rbp-0x4]`, mit
dem hexadezimal-Wert `13` (in dezimal also 19) verglichen. Die Funktion
ist sehr ähnlich mit der `sub`-Funktion nur, dass hier die übergebenen
Elemente nicht verändert, also das Ergebnis der Subtraktion nicht
gespeichert wird. Allerdings wird die equals-flag gesetzt, falls die
Subtraktion 0 ergibt, und die below-flag gesetzt, wenn
\(\texttt{i} < 19\) ist.  
  
Um dieses Ergebnis der `cmp`-Funktion zu nutzen wird mit

    117c:   76 e0                   jbe    115e <main+0x15>

ein konditioneller Sprungbefehl anhand der flags ausgeführt. `jbe` steht
hier für das Akronym "jump below equal". Dieses bedeutet, dass sofern
bei der vorhergegangenen Vergleichsoperation der zu vergleichende Wert
kleiner oder gleich war, gesprungen wird. Logisch wird hier also
\(\texttt{i}\,\leq\,19\) und somit der Vergleichsteil der `for`-Schleife
ausgeführt, welcher zwar als \(\texttt{i}\,<\,20\) definiert war aber
äquivalent ist. Der Sprung weißt auf den Anteil des assembler codes,
der den Aufruf von `printf` vorbereitet und schlussendlich durchführt.  
  
Relevant für die `for`-Schleife wird dann wieder die Zeile 11 (Adresse
`1174`). Diese wird nach dem Schleifenkörper also dem Aufruf von
`printf` ausgeführt. In dieser Zeile wird mit

    1174:   83 45 fc 01             add    DWORD PTR [rbp-0x4],0x1

der Wert von `i` um `1` erhöht. Hierbei handelt es sich also um den Teil
der `for`-Schleife, in welchem die Schleifenvariable erhöht wird.

## Teilaufgabe d)

Zunächst öffnet man den GDB-Debugger mit

    gdb ueb1

wobei `ueb1` das mit debug-flag kompilierte Programm ist. Im Anschluss
wurde mit

    (gdb) break 6

ein Breakpoint in Zeile 6 also bei der Definition der `for`-Schleife
gesetzt. Mit diesem konnte der Zustand vor dem ersten Schleifendurchlauf
beobachtet werden. Sichtbar wird hier besonders wie sich die
Zählervariable `i` verändert. Diese kann hier mit dem Befehl `p i` (`p`
ist hier kurz für print) ausgelesen werden. Mit dem Befehl

    (gdb) run

zum Starten des Programms erhält man dann dem Folgend die Ausgabe

    Breakpoint 1, main () at gdb-uebung-1.c:6
    6       for (i=0; i<20; i++) {
    (gdb) p i
    $1 = 0

welche zeigt, dass der Brakpoint getroffen und die Ausführung pausiert
wurde.  
  
Anschließend wurde mit `p i` der Wert der Variable `i` ausgegeben. Diese
Ausgabe zeigt, wie `i` auch schon vor der ersten Wertzuweisung den Wert
`0` trägt.  
  
Außer dass `i` kontinuierlich hochgezählt wird, und die Ausgabe daher
immer weiter hochgezählte `i`’s ausgibt, ist der Mittelteil der
Ausführung von dem Programm in GDB nicht relevant. Interessant hingegen
wird dann noch einmal der Schluss, wenn der Code sich der
Abbruchbedingung der `for`-Schleife annähert. Hierzu musste also
wiederholt der `next`-Befehl ausgeführt werden. So springt man an die
Stelle von der Ausgabe von `i: 18`. Nach Erreichung des Selbigen kann
mit dem `next`-Befehl Zeilenweise der Code ausgeführt werden.

    [...]
    (gdb) next
    i: 18
    6       for (i=0; i<20; i++) {
    (gdb) next
    7            printf("i: %2d\n", i); 
    (gdb) next
    i: 19
    6       for (i=0; i<20; i++) {

Hier sieht man, wie mit \(\texttt{i}\, = \, 19\) das letzte mal die
`for`-Schleife aufgerufen wird, danach wird allerdings `i` auf 20
erhöht, und die Abbruchbedingung erkannt weswegen dann der in
[2.3](#1c) angesprochene `jbe`-Befehl nicht mehr ausgeführt wird,

    (gdb) next
    9       return 0;
    (gdb) p i
    $2 = 20

sodass dann bereits das `return`-statment ausgeführt wurde. Wenn man
dann ein letztes mal `i` ausgeben lässt sieht man das es nun den Wert
`20` besitzt.

# Aufgabe 2

## Teilaufgabe a)

Der Code für Aufgabe zwei ist, wie in

    #include <stdio.h>
    
    int f(int a, int b) {
      return 3*a + 7*b;
    }
    
    int g(int a, int b) {
      return 10*a*a - 3*b;
    }
    
    int h(int a, int b) {
      return a + b + 300;
    }
    
    int main() {
      int a = 5, b=9, c=0;
    
      c = f(g(a,h(a,b)),h(b,a));
    
      printf("a = %d, b = %d, c = %d\n", a, b, c);
    }

zu sehen ist, in mehrere Funktionen unterteilt.  
  
Zum einen die `main`-Funktion, zum anderen die Funktionen `f`, `g` und
`h` welche alle jeweils zwei integer als Variablen bei der Übergabe
erwarten und den Rückgabewert `int` haben. In der ersten Zeile wird dann
noch die Standardbibliothek für Ein- und Ausgabe eingebunden.  
  
Logisch und strukturell sinnvoll mit der

    int main() {

Funktion beginnend wird nun der Code aufgearbeitet.  
In der Zeile 16 werden die drei Integer-Variablen `a`, `b` und `c`
definiert,

    int a = 5, b=9, c=0;

wobei `a` mit dem Wert `5` und `b` mit `9` die letzte der drei Variablen
wird dann mit `0` initialisiert.  
  
Nun wird mit

    c = f(g(a,h(a,b)),h(b,a));

zunächst wird die Funktion `h` mit `a` und `b` aufgerufen. Der
Rückgabewert dieser Funktion wird dann als zweites Attribut von dem
nächsten Aufruf, nämlich der Funktion `g` verwendet. Als erstes
Attribut wird hierbei dann `a` verwendet. Der Rückgabewert dieses
Aufrufs ist dann das Attribut des Aufrufs der `f`-Funktion. Diese hat
als zweites Attribut den Rückgabewert einer mit `b` und `a` aufgerufenen
`h`-Funktion. Wie Unten zu sehen ist, ist bei der `h`-Funktion die
Reihenfolge von `a` und `b` beim Aufruf irrelevant. Das Ergebnis der
`f`-Funktion wird dann in der Variable `c` gespeichert.  
  
Schlussendlich wird mit

    printf("a = %d, b = %d, c = %d\n", a, b, c);

noch ein formatiertes `printf`-statment ausgeführt, bei welchem die
Werte aller Variablen beschriftet ausgegeben werden.  
  
Die alphabetisch sortiert zuerst definierte Funktion ist die

    int f(int a, int b) {

Funktion, welche wie oben beschrieben bei Aufruf zwei Integer übergeben
bekommen soll. Die Rückgabe

    return 3*a + 7*b;
    }

beinhaltet eine Rechnung und stellt damit die Einzige Funktion der
Funktion dar. Die Rückgabe wird mit
\(3\, \cdot \, \texttt{a} \ +\ 7\, \cdot\, \texttt{b}\) in der selben
Zeile der Rückgabe und innerhalb des `return`-statments berechnet.  
  
Die mit

    int g(int a, int b) {

deklarierte Funktion `g` gibt genau wie sein alphabetischer Vorgänger
eine Berechnung aus den übergebenen Werten `a` und `b` zurück. Hierzu
wird mit

    return 10*a*a - 3*b;
    }

zunächst die Rechnung
\(10\, \cdot \, \texttt{a}^2 \: - \: 3\, \cdot \, \texttt{b}\)
ausgeführt und das Ergebnis zurückgegeben.  
  
Den beiden Vorgängern `f` und `g` folgend, wird mit

    int h(int a, int b) {

die letzte Funktion definiert. Diese arbeitet analog zu den
vorhergegangenen, mit der Ausnahme, dass die Rechnung bei

    return a + b + 300;
    }

abgeändert ist. Sodass hier \(\texttt{a}\,+\,\texttt{b}\,+\,300\)
zurückgegeben wird.

## Teilaufgabe b)

Um das Programm auszuführen, muss man zunächst den obigen source code
kompilieren. Dies erreicht man mit dem Befehl:

    gcc gdb-uebung-2.c -g -o gdb-ueb2

Hier ist der Befehl analog verwendet zu der in [2.2](#1b) gezeigten
Anwendung. Hat man das erledigt so sieht man nach dem Ausführen des
Programmes mit

    ./gdb-ueb2

die Ausgabe

    a = 5, b = 9, c = 122

Schlicht gesagt bedeutet das, dass für \(\texttt{a}\, =\, 5\) und
\(\texttt{b}\, =\, 9\) die kombinierten Formeln aus dem Aufruf
`f(g(a,h(a,b)),h(b,a))` `c = 122` ergeben. Als mathematischer Ausdruck
sind die wiederholten Aufrufe als \[3(10a^2-3(a+b+300))+7(a+b+300)\]
darstellbar. Dieser liefert, genau wie unsere Ausgabe suggeriert, mit
den Werten `5` für `a` und `9` für `b` das Ergebnis 122.

## Teilaufgabe c)

Um zu zeigen in welcher Reihenfolge die Funktionen aufgerufen werden,
wurde mit

    gdb gdb-ueb2

der Gnu Debugger aufgerufen, und mit

    (gdb) break 4
    (gdb) break 8
    (gdb) break 12
    (gdb) break 16
    (gdb) break 18

in den Zeilen 4, 8, 12, 16 und 18 Breakpoints gesetzt. Führt man nun das
Programm mit

    (gdb) run

aus und springt mehrmals mit `c` bzw. `continue` jeweils zum nächsten
Breakpoint, so wird anhand der Ausgabe

    Breakpoint 4, h (a=9, b=5) at gdb-uebung-2.c:12
    12      return a + b + 300;
    (gdb) c
    Continuing.
    
    Breakpoint 4, h (a=5, b=9) at gdb-uebung-2.c:12
    12      return a + b + 300;
    (gdb) c
    Continuing.
    
    Breakpoint 3, g (a=5, b=314) at gdb-uebung-2.c:8
    8       return 10*a*a - 3*b;
    (gdb) c
    Continuing.
    
    Breakpoint 2, f (a=-692, b=314) at gdb-uebung-2.c:4
    4       return 3*a + 7*b;

klar, dass zuerst die Funktion `h`, dann erneut `h`, danach dann `g` und
Schlussendlich die Funktion `f` ausgeführt wurde. Mit den Rückgabewerten
wurden dann die entsprechenden folgenden Funktionen aufgerufen, sodass
schlussendlich `f` mit `a=-692` und `b=314` aufgerufen wurde und als
Rückgabewert den Wert 122 ergab.  
  
Um aufzuzeigen, welche Stackframes erzeugt wurden, und außerdem, was der
Inhalt dieser Stackframes ist, sind die gleichen Breakpoints wie bei dem
vorhergegangenen `gdb`-Aufruf ausreichend. Mit dem Befehl `bt full`
werden sowohl alle Stackframes als auch deren Inhalt ausgegeben. In
Zeile 18 ergibt sich dann, nach einem `run` und einem `continue` mit dem
Ausgabebefehl

    (gdb) bt full

erstmals folgendes Bild

    #0  main () at gdb-uebung-2.c:18
            a = 5
            b = 9
            c = 0

bei diesem ist der initiale Stack Frame von `main` mit den in Zeile 16
zugewiesenen Variablen als Inhalt sichtbar. Beim ersten Aufruf von `h`
hingegen wird dann mit

    (gdb) bt full
    #0  h (a=9, b=5) at gdb-uebung-2.c:12
    No locals.
    #1  0x00005555555551ee in main () at gdb-uebung-2.c:18
            a = 5
            b = 9
            c = 0

deutlich, dass für den Aufruf der Funktion ein weiterer Stack Frame
erzeugt wurde. Analog sieht man bei jedem weiteren Aufruf einen neuen
erzeugten Stack Frame und dass der Stackframe von der zuvor aufgerufenen
Funktion terminiert wurde. Beispielhaft ist dies an

    (gdb) bt full
    #0  f (a=-692, b=314) at gdb-uebung-2.c:4
    No locals.
    #1  0x0000555555555216 in main () at gdb-uebung-2.c:18
            a = 5
            b = 9
            c = 0

sichtbar, da ja `f` die als letztes aufgerufene Funktion ist. Und
keinerlei Stackframes der vorhergegangenen Funktionen mehr vorhanden
sind. Besonders deutlich ist das wenn man

    (gdb) bt full

in der Zeile 20 also bei dem `printf` ausführt, denn dann erkennt man
an,

    #0  main () at gdb-uebung-2.c:20
            a = 5
            b = 9
            c = 122

dass lediglich der initiale Stackframe besteht.  
  
Abgesehen von den bereits gezeigten Stackframes wurden außerdem die
Stackframes für den zweiten Aufruf von `h`, an der entsprechenden Stelle
ausgegeben mit

    (gdb) bt full

und sich damit darstellend als

    #0  h (a=5, b=9) at gdb-uebung-2.c:12
    No locals.
    #1  0x00005555555551ff in main () at gdb-uebung-2.c:18
            a = 5
            b = 9
            c = 0

Und des Weiteren der Stackframe für den Aufruf von `g` anzuzeigen mit:

    (gdb) bt full

damit dann dargestellt als:

    #0  g (a=5, b=314) at gdb-uebung-2.c:8
    No locals.
    #1  0x000055555555520d in main () at gdb-uebung-2.c:18
            a = 5
            b = 9
            c = 0

Um also die Frage zu beantworten wie viele Stackframes erzeugt wurden,
kann man sagen dass inklusive dem von `main` erzeugten, fünf Stackframes
erzeugt worden sind. Der Inhalt der Frames waren immer die Variablen mit
den sich änderten Werten, welche in Zeile 16 definiert wurden.

# Aufgabe 3: Rekursion

## Teilaufgabe a)

Der in Aufgabe 3 enthaltende Code zum Thema Rekursion, implementiert
eine Funktion `f`, welche sich in ihrem Rumpf innerhalb einer
Verzweigung rekursiv aufruft und dabei eine Multiplikation mit dem
Ergebnis des rekursiven Aufrufs durchführt. In der `main`-Funktion wird
eine Variable `i` als Übergabeparameter für `f` initialisiert. In einer
Variablen `r` wird dann das Ergebnis des Aufrufs der Funktion `f` mit
`i` gespeichert und zuletzt die Werte, die in `i` und `r` gespeichert
wurden, auf der Konsole ausgegeben.
<span id="rekursioncode" label="rekursioncode">\[rekursioncode\]</span>

    #include <stdio.h>
    
    unsigned int f(unsigned int i) {
        if (i>1) {
            return i * f(i-1);
        } else {
            return 1;
        }
    }
    
    int main() {
        unsigned int i=5, r=0;
    
        r = f(i);
    
        printf("i = %d, f(i) = %d\n", i, r);
    }

Zunächst wird wieder die Standard-Bibliothek `stdio.h` importiert.

    #include <stdio.h>

Anschließend folgt die Deklaration der Funktion `f`,

    unsigned int f(unsigned int i) {

welche eine vorzeichenlose Ganzzahl zurückgibt sowie eine vorzeichenlose
Ganzzahl in der Variable `i`, als Parameter übergeben bekommt.  
  
Im Rumpf der Funktion befindet sich eine Verzweigung.

    if (i>1) {
            return i * f(i-1);

Hier wird zunächst die Bedingung geprüft, ob in der Variable `i` ein
größerer Wert als \(1\) gespeichert ist. Ist dies der Fall so wird in
Zeile fünf `i` mit einem rekursiven Aufruf von `f` multipliziert, wobei
`f` als Parameter `i-1` erhält. Es ist bereits ersichtlich, dass dieser
Teil der Funktion mehrmals aufgerufen wird, sofern für `i` ein
hinreichend großer Wert übergeben wurde und die Anzahl der Aufrufe durch
die Rechnung \(\texttt{i} - 1\) beim rekursiven Aufruf begrenzt wird.  
  
Im `else`-Fall,

    } else {
            return 1;
        }

also sofern \(\texttt{i} < 2\), wird der Wert `1` mit einem `return`
übergeben.  
  
An dieser Stelle ist die Funktion beendet und es geht in Zeile elf

    int main() {

mit der `main`-Funktion weiter. Es folgt eine Deklarierung der Variablen
mit `i` und `r`,

    unsigned int i=5, r=0;

welche sofort mit den Werten `i=5` und `r=0` als positive Ganzzahlen
initialisiert werden.  
  
Nun wird in `r` das Ergebnis des Aufrufs der Funktion `f` mit Parameter
`i` gespeichert.

    r = f(i);

Zuletzt werden die in `i` und `r` gespeicherten Werte mit einem
`printf`-statment

    printf("i = %d, f(i) = %d\n", i, r);

auf der Konsole ausgegeben.

## Teilaufgabe b)

Das vorliegende Programm soll nun kompiliert und ausgeführt werden.
Hierfür wurde wieder `gcc`

    gcc gdb-uebung-3.c -g -o gdb-ueb3

mit den entsprechenden flags genutzt. Anschließend wurde das Programm
mit

    ./gdb-ueb3

ausgeführt.  
  
Es folgte die Ausgabe:

    i = 5, f(i) = 120

Die erfolgreiche Ausgabe der Ergebnisse der rekursiven Berechnung in `f`
lässt darauf schließen, dass das Programm ohne Fehler übersetzt und
ausgeführt werden konnte. Nach der Ausgabe wurde das Programm wie
erwartet mit `return 0` beendet.

## Teilaufgabe c)

Zur Untersuchung der Berechnung der Variablen `r` in Zeile 14 in
[\[rekursioncode\]](#rekursioncode) und deren erzeugte Stack Frames muss
zunächst das Programm mit dem Befehl

    gdb ./gdb-ueb3

im Debugger ausgeführt werden. Anschließend wird mit dem Befehl

    (gdb) break f
    Haltepunkt 1 at 0x1149: file gdb-uebung-3.c, line 3.

der Breakpoint auf die Funktion `f` gesetzt, damit im einzelnen durch
die Funktion gesprungen werden kann, ohne das Programm bis zum Ende
auszuführen. Der Befehl

    (gdb) run
    Starting program: /home/tux/Schreibtisch/SP_2-main/Code/gdb-ueb3
    
    Breakpoint 1, f (i=21845) at gdb-uebung-3.c:3
    3   unsigned int f(unsigned int i) {

führt nun das Programm bis zum gewählten Breakpoint aus.  
  
Um zu überprüfen ob bei der Ausführung des Programms bis zum Breakpoint
bereits ein Stack Frame erzeugt wurde, wird der Befehl `backtrace` oder
kurz `bt` genutzt.

    (gdb) bt
    #0  f (i=21845) at gdb-uebung-3.c:3
    #1  0x000055555555519c in main () at gdb-uebung-3.c:14

Es wird ersichtlich, dass im ersten Schritt der initiale Stack Frame von
`main` und anschließend darüber ein Stack Frame für den ersten Aufruf
von `f` erzeugt wurde.  
Mit dem Befehl

    (gdb) c
    Continuing.
    
    Breakpoint 1, f (i=32767) at gdb-uebung-3.c:3
    3   unsigned int f(unsigned int i) {

kann das Programm durchlaufen werden, bis der Breakpoint erneut erreicht
wird.

Dieser Befehl wird analaog noch drei mal ausgeführt. Anschließend werden
mit `bt` wieder die aktuellen Stack Frames ausgegeben.

    (gdb) bt
    #0  0x0000555555555155 in f (i=0) at gdb-uebung-3.c:3
    #1  0x000055555555516b in f (i=2) at gdb-uebung-3.c:5
    #2  0x000055555555516b in f (i=3) at gdb-uebung-3.c:5
    #3  0x000055555555516b in f (i=4) at gdb-uebung-3.c:5
    #4  0x000055555555516b in f (i=5) at gdb-uebung-3.c:5
    #5  0x000055555555519c in main () at gdb-uebung-3.c:14

Es wurden mit jedem Aufruf von `f` neue Stack Frames erzeugt. Somit
wurden vier neue Stack Frames sowie der initiale Stack Frame für `main`
und der Stack Frame für den ersten Aufruf von `f` erzeugt. Der aus den
einzelnen Stack Frames resultierende Call Stack besteht daher aus sechs
Stack Frames.  
  
Würde man nun im Programm mit `c` weiter springen, wird der `else`-Block
ausgeführt und `main` erhält `return 1;`. Dies hat die anschließende
Ausgabe des Ergebnisses zur Folge und die Stack Frames würden
entsprechend wieder frei gegeben. Davor soll noch der Inhalt der Stack
Frames ausgegeben werden. Dies wird durch Nutzen des Befehls `info frame
<nr>` erreicht, hier am Beispiel des Stack Frames mit der Nummer zwei.

    (gdb) info frame 2
    Stack frame at 0x7fffffffdf40:
     rip = 0x55555555516b in f (gdb-uebung-3.c:5); 
     saved rip = 0x55555555516b
     called by frame at 0x7fffffffdf60, 
     caller of frame at 0x7fffffffdf20
     source language c.
     Arglist at 0x7fffffffdf18, args: i=3
     Locals at 0x7fffffffdf18, Previous frame's sp is 0x7fffffffdf40
     Saved registers:
     rbp at 0x7fffffffdf30, rip at 0x7fffffffdf38

Diese umfangreiche Ausgabe für jeden Stack Frame, zeigt die Parameter
(`i=3`) sowie deren Startadresse, lokale Variablen mit ihrer Adresse,
die Sprache in der der source vorliegt, die Adresse des aktuellen Stack
Frames, die Adresse des darüber liegenden Stack Frames (caller), des
darunter liegenden Stack Frames (called), die Adresse auf die der Stack
Pointer des caller Stack Frames gezeigt hat als Start der
Speicheradresse des aktuellen Stack Frames und zuletzt die Register,
welche im Stack Frame gespeichert wurden (hier rpb und rip).  
  
Nicht immer werden derart umfangreiche Informationen benötigt, daher
können auch einzelne Informationen ausgegeben werden, indem der
gewünschte Stack Frame mit `frame <nr>` angezeigt wird

    (gdb) frame 1
    #1  0x000055555555516b in f (i=2) at gdb-uebung-3.c:5
    5       return i * f(i-1);

und anschließend z. B. mit `info args` nur die Parameter des gewählten
Stack Frames ausgegeben werden.

    (gdb) info args
    i = 2

Aus der nacheinander Erzeugung der Stack Frames mit ihren Parametern,
kann nun folgender Aufrufbaum (Call Graph) der rekursiven Funktion `f`
abgeleitet werden, deren letzte Rückgabe in der Variablen `r` in Zeile
14 aus [\[rekursioncode\]](#rekursioncode) gespeichert wird.

![Call Graph](Aufgabe-3-Rekursion-Call-Graph)

Um die Frage zu klären wie die Parameterübergabe in assembler erfolgt,
muss das Programm zunächst wie in Aufgabe 1 disassembliert werden. Dazu
wird nun folgender Befehl ausgeführt.

    objdump -d -M "intel" -S gdb-ueb3

Es folgt eine gekürzte Ausgabe der `.text` Sektion des assembler codes.

``` 
    [...]
    114d:   55                      push   rbp
    114e:   48 89 e5                mov    rbp,rsp
    1151:   48 83 ec 10             sub    rsp,0x10
    1155:   89 7d fc                mov    DWORD PTR [rbp-0x4],edi
  if (i>1) {
    1158:   83 7d fc 01             cmp    DWORD PTR [rbp-0x4],0x1
    115c:   76 13                   jbe    1171 <f+0x28>
    return i * f(i-1);
    115e:   8b 45 fc                mov    eax,DWORD PTR [rbp-0x4]
    1161:   83 e8 01                sub    eax,0x1
    1164:   89 c7                   mov    edi,eax
    1166:   e8 de ff ff ff          call   1149 <f>
    116b:   0f af 45 fc             imul   eax,DWORD PTR [rbp-0x4]
    116f:   eb 05                   jmp    1176 <f+0x2d>
  } else {
    return 1;
    1171:   b8 01 00 00 00          mov    eax,0x1
  }
}
    1176:   c9                      leave  
    1177:   c3                      ret
    [...]
```

Um nun die Übergabe der Parameter Zeile für Zeile analysieren zu können,
wird das Text User Interface (TUI) des GDB genutzt, mit dem sich sowohl
source code als auch dessen disassemblierter Code nebeneinander
darstellen lassen.  
Zunächst wird mit dem Befehl `tui enable` die TUI Umgebung aktiviert.

    (gdb) tui enable

Anschließend wird mit dem Befehl `layout split` sowohl der source code
als auch assembler gleichzeitig dargestellt. Mit `focus asm` kann das
assembler Fenster per Scrolling oder Pfeiltasten navigiert werden.

    (gdb) layout split
    (gdb) focus asm

Mit dem Befehl `ni` kann nun, analog wie beim Befehl `next` durch die
Zeilen des assembler codes gesprungen werden.

    (gdb) ni

Der entsprechende Breakpoint sollte noch aus den vorherigen Schritten
gesetzt sein, mit `break f` und anschließender Eingabe `run` könnte das
Programm bei Bedarf auch erneut gestartet werden.  
Es wird nun Zeile für Zeile mit `ni` durch den assembler code navigiert
und dabei analysiert wie die einzelnen assembler Zeilen im Verhältnis
zum source code ausgeführt werden. Als erstes fällt auf wie der neue
Stack Frame aufgebaut wird. Zunächst wird beim Aufruf von `f` der Base
Pointer (`rbp`-Register) mit `push` im Stack gesichert, welcher auf das
Ende des Stack Frames zeigt.

``` 
    [...]
    114d:   55                      push   rbp
```

Als nächstes wird der Stack Pointer (`rsp`-Register) im Base Pointer
(`rbp`-Register) gespeichert und in der nächsten Zeile der Stack Pointer
um \(16\) Byte verringert, um Platz im Stack für lokale Variablen
geschaffen.

``` 
    114e:   48 89 e5                mov    rbp,rsp
    1151:   48 83 ec 10             sub    rsp,0x10
    [...]
```

Jetzt kann der Parameter `i` auf dem Stack vom `edi`-Register mit einer
\(32\) Bit Deklaration gespeichert werden.

``` 
    [...]
    1155:   89 7d fc                mov    DWORD PTR [rbp-0x4],edi
    [...]
```

Somit wurde der erste Parameter über die Register initialisiert. Im
Anschluss wird die Bedingung in der Verzweigung geprüft und bei `i > 1`
in die Rekursion gesprungen. Wobei weitere Parameter über den Stack
übergeben und das Ergebnis des Aufrufs von `i` über das `eax`-Register
an das `edi`-Register zurückgegeben werden. Der Befehl `call` sichert
den Instruction Pointer im `epi`-Register auf dem Stack und ruft
anschließend wieder `f` auf, wobei ein neuer Stack Frame entsteht.

``` 
    [...]
    115e:   8b 45 fc                mov    eax,DWORD PTR [rbp-0x4]
    1161:   83 e8 01                sub    eax,0x1
    1164:   89 c7                   mov    edi,eax
    1166:   e8 de ff ff ff          call   1149 <f>
    [...]
```

Sobald nach einigen Aufrufen die Bedingung für den Aufruf vom
`else`-Block erfüllt, wird nach dessen Ausführung, die eigentliche
Multiplikation durchgeführt und danach `jmp` durchgeführt.

``` 
    116b:   0f af 45 fc   imul   eax,DWORD PTR [rbp-0x4]
    116f:   eb 05         jmp    1176 <f+0x2d>
```

es wird zum `leave` Befehl gesprungen, welcher den Rückgabewert des
Aufrufs im Stack speichert, den Stack Pointer zurücksetzt und den Base
Pointer frei gibt, also den Stack Frame wieder vom Stapel nimmt und
zuletzt mit `ret` die Kontrolle an den vorherigen Aufruf (caller) durch
Manipulation des Instruction Pointer im `epi`-Register übergeben.

``` 
    1176:   c9                      leave  
    1177:   c3                      ret
```

# Aufgabe 4: Berechnung einer Summe

## Ursprünglicher Code

    #include <stdio.h>
    
    int main() {
        int anzahl;
        float summe;
        float i;
    
        summe = 0, anzahl = 0;
        for (i = 1000; i <= 1000.03; i += .01) {
            summe += i;
            anzahl++;
        }
        printf("Summe: %f, Anzahl: %d\n", summe, anzahl);
    
        return 0;
    }

## Funktion

Der in Aufgabe 4 enthaltende Code hat die Aufgabe eine Summe zu
berechnen. Innerhalb einer `main`-Funktion werden drei Variablen
deklariert und weiter unten initialisiert. In einer `for`-Schleife wird
eine Summe mit der Zählvariablen `i`, über die iteriert wird in der
Variablen `summe` gespeichert und `anzahl` um \(1\) erhöht, mit deren
Hilfe die Anzahl der Iterationen verfolgt werden können. Zuletzt erfolgt
eine Ausgabe der in den Variablen gespeicherten Werte, auf der Konsole.

## Teilaufgabe a)

Zuerst wird mit

    #include <stdio.h>

die Standard-Bibliothek für Ein- und Ausgaben importiert.  
  
Anschließend wird die `main`-Funktion deklariert

    int main() {

Es erfolgt die Deklaration dreier Variablen,

    int anzahl;
        float summe;
        float i;

wobei `anzahl` der Datentyp `int` und `summe` sowie `i` der Datentyp
`float` zugewiesen wird.  
  
Anschließend werden `summe` und `anzahl` jeweils mit den Werten \(0\)
initialisiert.

    summe = 0, anzahl = 0;

Nun folgt eine `for`-Schleife,

    for (i = 1000; i <= 1000.03; i += .01) {

in welcher zunächst die Variable `i` mit dem Wert \(1000\) initialisiert
wird, dann die Abbruchbedingung \(\texttt{i} <= \texttt{1000.03}\)
festgelegt wird und nach jeder Iteration `i` mit dem Wert \(0.01\)
summiert wird.  
  
Innerhalb der `for`-Schleife,

    summe += i;
            anzahl++;
        }

wird in Zeile zehn, in der Variablen `summe` das Ergebnis der Berechnung
\(\texttt{summe}+\texttt{i}\) gespeichert. Anschließend wird der in
`anzahl` gespeicherte Wert um \(1\) erhöht, um so die erfolgten
Schleifendurchläufe zählen zu können.  
  
In Zeile 13,

    printf("Summe: %f, Anzahl: %d\n", summe, anzahl);

werden die in den Variablen `summe` und `anzahl` gespeicherten Werte auf
der Konsole ausgegeben, um ablesen zu können, wie oft die Schleife
durchlaufen und welches Ergebnis für `summe` innerhalb der Schleife
berechnet wurde.  
  
Zuletzt wird die Funktion mit einem `return` in Zeile 15, mit
Rückgabewert \(0\) beendet.

    return 0;
    }

## Teilaufgabe b)

Das vorliegende Programm soll nun kompiliert und ausgeführt werden.
Hierfür wurde wieder `gcc` mit den entsprechenden flags genutzt: `gcc
gdb-uebung-4.c -g -o gdb-ueb4`. Anschließend wurde das Programm mit
`./gdb-ueb4` ausgeführt.  
  
Es folgte die Ausgabe

    Summe: 3000.030029, Anzahl: 3

Hierbei fällt auf, dass mit der berechneten Summe \(3000.030029\) etwas
nicht stimmen kann. Schließlich wird bei der Berechnung der Summe in der
`for`-Schleife die Variable `i` bei jeder Iteration auf `i += .01`
gesetzt. Dies sollte in der Berechnung innerhalb der Schleife keine
Ergebnisse mit mehr als zwei Nachkommastellen ergeben.

## Teilaufgabe c)

Zur weiteren Analyse des Programms wird dieses mit dem Befehl `gdb
./gdb-ueb4` im Debugger ausgeführt. Wie bisher auch, wird der Breakpoint
mit `break main` gesetzt, diesmal auf die `main`-Funktion und das
Programm mit `run` bis zum Breakpoint ausgeführt.

    (gdb) run
    Breakpoint 1, main () at gdb-uebung-4.c:3
    3   int main() {

Mit `next` oder kürzer `n` wird die nächste Zeile ausgeführt.

    (gdb) n
    8       summe = 0, anzahl = 0;

`summe` und `anzahl` werden jeweils mit den Werten `0` initialisiert.
Mit einer weiteren Ausführung des Befehls `n` lässt sich zum
Schleifenkopf der `for`-Schleife springen.

    (gdb) n
    9       for (i = 1000; i <= 1000.03; i += .01) {

Anschließend wird der in `i` gespeicherte Wert angezeigt, indem der
Befehl `p i` genutzt wird, wobei `p` für `print` steht.

    (gdb) p i
    $5 = 1000

Es ist ersichtlich, dass `i` wie erwartet mit dem Wert `1000`
initialisiert wurde. Mit weiteren Ausführungen des Befehls `n` wird die
Schleife durchlaufen bis diese das erste mal wiederholt wird, da `i
<= 1000.03` noch `false` liefert. An dieser Stelle wird `i += 0.01`
gerechnet. Wir lassen uns den neuen Inhalt von `i` ausgeben.

    (gdb) p i
    $6 = 1000.01001

Es fällt auf, dass der Wert für `i` nicht dem zu erwartenden Wert von
\(1000.01\) entspricht, der aus der Rechnung \(1000 + 0,01\) hervorgehen
müsste. Stattdessen wird zusätzlich \(0,00001\) addiert. Wir führen die
Schleife insgesamt drei mal aus bis `i` folgenden Wert annimmt:

    $7 = 1000.03003

Somit setzt sich die zusätzliche Addition mit \(0,00001\) bei jeder
Iteration fort. Es wird nun einmal der Wert von `anzahl` ausgeben.

    (gdb) p anzahl
    $8 = 3

Außerdem wird der Wert von `summe` ausgegeben.

    (gdb) p summe
    $9 = 3000.03003

Dieser Wert ist aufgrund der fehlerhaften Berechnung von `i` ebenfalls
falsch. Führt man die Schleife nun noch einmal aus, ergibt sich für `i
<= 1000.03` der Wert `true` und die Schleife wird nicht noch einmal
wiederholt, sondern ein falsches Ergebnis für `summe` ausgegeben.  
  
Der Grund hierfür ist ein Problem bei Berechnungen mit
Gleitkommaarithmetik (englisch: floating point) des Datentyps `float`.
Es können bei diesem Datentyp lediglich \(32\) Bit bzw. \(4\) Bytes
gespeichert werden, davon \(1\) Bit für das Vorzeichen, \(8\) für den
Exponenten und \(23\) Bits für die Mantisse, was einer Reichweite von
ca. sechs bis neun Dezimalstellen entspricht. Dies ist für unser Code
Beispiel nicht ausreichend. Deshalb wird für den Wert von `i` sowohl bei
der Rechnung `i += 0.01` als auch für die Berechnung `summe` ein
falsches Ergebnis berechnet. Die Folge ist, dass die `for`-Schleife
einmal zu wenig durchlaufen wird, denn mit der fehlerhaften Berechnung
von `i` wird bereits eine Iteration zu früh die Abbruchbedingung `i
<= 1000.03` erfüllt. Folglich ergibt sich sowohl ein Rundungsfehler in
`i`, als auch ein falsches Ergebnis in `summe` aufgrund des verfrühten
Abbruchs der `for`-Schleife.

## Teilaufgabe d)

Der vorgegebene Code wurde folgendermaßen modifiziert.

    #include <stdio.h>
    
    int main () {
            int anzahl;
            double summe;
            double i;
    
            summe = 0, anzahl = 0;
            for (i = 1000; i <= 1000.03; i += 0.01) {
                summe += i;
                anzahl++;
            }
            printf("Summe: %f, Anzahl: %d\n", summe, anzahl);
    
            return 0;
    }

Um die oben genannten Probleme bei der Nutzung von `float`-Datentypen zu
vermeiden, wurden diese in Zeile vier und fünf durch den Datentyp
`double` ausgetauscht.

    double summe;
        double i;

Im Gegensatz zum `float`-Datentypen können in einem `double` insgesamt
\(64\) Bit bzw. \(8\) Bytes gespeichert werden, davon \(1\) Bit für das
Vorzeichen, \(11\) für den Exponenten und \(52\) Bits für Mantisse.
Daraus ergibt sich eine entsprechend höhere Reichweite, welche das für
unser Code Beispiel relevante Problem löst und nach Ausführung das
korrekte Ergebnis berechnet.

    Summe: 4000.060000, Anzahl: 4

Für noch mehr Genauigkeit könnte die Zählvariable `i` auch in einem
Datentyp `long double` gespeichert werden, dies war in unserem Beispiel
jedoch nicht nötig. Grundsätzlich sollte, sofern möglich, die Verwendung
von Gleitkommazahlen als Zählvariable für Schleifen vermieden werden.
Eine einfache Umwandlung des Datentyps in `double` hat für unser
Beispiel bereits ausgereicht, sollte jedoch eine Summe mit Zahlen
berechnet werden, die auch nicht mehr in die \(64\) Bit von `double`
oder `long double` passen, so müsste der Datentyp `int` für die
Zählvariable verwendet werden sowie innerhalb der Schleife alle Werte
mit \(100\) multipliziert werden, um das Gleitkomma zu entfernen. In
einem weiteren Schritt könnte z. B. bei der Ausgabe der berechnete Wert
mit \(100\) dividiert und in einer Variable des Datentyps `double`
gespeichert werden.

# Aufgabe 5: Binäre Suche

## Ursprünglicher Code

    #include <stdio.h>
    #include <stdlib.h>
    
    #define MAX 8 
    int array[MAX] = {1,4,12,18,26,31,40,42};
    int rekursionstiefe = 0;
    
    int binarysearch(int zahl, int links, int rechts) {
        rekursionstiefe++;
        int mitte = (links + rechts) / 2;
        printf("\nRekursionstiefe: %d", rekursionstiefe);
        if (array[mitte] == zahl)
            return mitte; 
        if (links == rechts)
            return -1; 
        if (array[mitte] > zahl)
            return binarysearch(zahl, links, mitte); 
        else
            return binarysearch(zahl, mitte, rechts);
    }
    
    int main(int argc, char *argv[]) {
      int zahl, position, i;
        if(argc < 2) {
            printf("Benutzung: %s <zu suchende Zahl>\n", argv[0]);
            return 1;
        }
        zahl = atoi(argv[1]);
    
        for (i = 0; i < MAX; i++) {
            printf("%4d", array[i]);
        }
        position = binarysearch(zahl, 0, 7); 
        if (position >= 0) {
            printf("\nGesuchte Zahl %d an Arrayposition %d\n", zahl, 
            position);
        }
        else {
            printf("\nZahl %d nicht gefunden\n", zahl);
        }
        return 0;
    }

## Funktion

In Aufgabe 5 liegt ein C Programm vor, welches über die Kommandozeile
aufgerufen werden soll. Das Programm soll in einem Array von Zahlen eine
übergebene Zahl suchen. Falls sie gefunden wird soll die Stelle
zurückgegeben werden, an dem die Zahl im Array gespeichert ist.

## Teilaufgabe a)

Um in C ein funktionierendes Programm zu bauen, benötigt man C
Bibliotheken. Diese sind in C Header Dateien, die dann in ein Programm
eingebunden und verwendet werden können. In der Uebung 5 sind das die
`stdio.h` und die `stdlib.h`. Diese zwei Bibliotheken beinhalten
Standard Methoden wie zum Beispiel `printf`. Beide Bibliotheken werden
eingebunden, indem man

    #include <stdio.h>
    #include <stdlib.h>

am Anfang des Programms nutzt. In Zeile 4 wird mit einem `define` eine
Konstante für das gesamte Programm definiert. Das Maximum wird mit dem
Befehl,

    #define MAX 8 

auf `8` gesetzt.

Anschließend wird eine globale Liste: `array` mit der Größe `MAX`, die
vorher als `8` definiert wurde deklariert. Außerdem wird in Zeile 6 die
globale Variable

    int array[MAX] = {1,4,12,18,26,31,40,42};
    int rekursionstiefe = 0;

`rekursionstiefe` definiert und auf 0 gesetzt.

Anschließend wird die eigentliche Funktion deklariert. Die Funktion muss
mit 3 ganzen Zahlen aufgerufen werden. Die erste Zahl ist die Zahl, die
man in der Liste suchen will, die zweite Zahl ist die linke Begrenzung,
des Abschnitts im Array den man durchsucht. Die dritte Zahl ist
logischerweise die rechte Begrenzung. Die Funktion wird per

    int binarysearch(int zahl, int links, int rechts) {

deklariert und so Ihren Rückgabewert, und die Übergabeparameter
bestimmt.

Jedes mal wenn diese Funktion aufgerufen wird, wird die
`rekursionstiefe`

    rekursionstiefe++;

um eins erhöht.

Dann wird die Mitte des (Teil-) Arrays mit

    int mitte = (links + rechts) / 2;

berechnet, und in die Variable `mitte` geschrieben.

Streng genommen wird so aber gar nicht die genaue Mitte berechnet, da
diese bei der ersten Berechnung zum Beispiel bei 3,5 liegt. Die ,5 wird
allerdings abgeschnitten da es sich bei der Variable `mitte` um einen
Integer, also eine ganze Zahl handelt.

Anschließend wird die Rekursionstiefe, also wie oft die Funktion
aufgerufen wurde, mit

    printf("\nRekursionstiefe: %d", rekursionstiefe);

ausgegeben.

Im Anschluss soll überprüft werden, ob das Element an der berechneten
Stelle `mitte` gefunden wurde. Tritt der Fall

    if (array[mitte] == zahl)
            return mitte; 

ein, so wird der Hauptfunktion der Index des gefundenen Elements
zurückgegeben.

Nach einem `return` wird immer in die Hauptfunktion gesprungen, diese
wird später genauer behandelt. Es muss nur hier gesagt werden, wenn
diese `if`-Bedingung eintritt, so wird die Funktion an dieser Stelle
verlassen. Dies gilt auch für die folgenden `return`-statements.

Als nächstes wird überprüft, ob die Liste schon komplett durchlaufen
wurde, und somit das gesuchte Element nicht gefunden wurde. Dies wird
überprüft, indem man die Variable `links` mit der Variable `rechts`
vergleicht. Diese beiden geben die Grenzen des zu durchsuchenden Arrays
an. Ist also

    if (links == rechts)
            return -1;

so wird der Hauptfunktion `-1` zurückgegeben. Später wird genauer
erläutert, was die Hauptfunktion dann mit den zurückgegebenen Werten
macht.

Wenn keiner dieser beiden Fälle eintritt, und man somit nicht in die
Hauptfunktion springt, läuft die Funktion noch weiter, und ruft sich in
einer der beiden folgenden Abzweigungen selbst auf. Es entsteht also
eine Rekursion.

Zuerst wird überprüft, ob die Zahl, die man sucht, kleiner als die Zahl
in der Mitte des Arrays ist. Ist sie kleiner, so wird die Funktion
erneut mit

    if (array[mitte] > zahl)
            return binarysearch(zahl, links, mitte);

in Zeile 17 aufgerufen.

Der Funktion wird also als `rechts` die berechnete `mitte` übergeben und
der Parameter `links` bleibt gleich.

Ist die gesuchte Zahl größer als die `mitte`, so wird die Funktion
erneut mit

    else
            return binarysearch(zahl, mitte, rechts);

aufgerufen.

Die Funktion ist an dieser Stelle zu Ende. Damit diese aber auch benutzt
und aufgerufen wird, braucht man noch eine Hauptfunktion. Diese wird mit

    int main(int argc, char *argv[]) {

in Zeile 22 deklariert.

Dabei steht der erste Parameter `argc` für die Anzahl der übergebenen
Argumente, wenn das Programm aufgerufen wird. Der zweite Parameter `char
*argv[]` ist ein Vektor, in welchem die einzelnen Argumente gespeichert
sind. Dabei ist das nullte Element in dem Array immer der Programmname
selbst. Das erste Element ist also der erste eigentlich übergebene
Parameter.  
Als erstes werden in der Hauptfunktion die drei ganzzahligen Variablen

    int zahl, position, i;

deklariert. Anschließend wird überprüft, ob eine zu suchende Zahl
übergeben wurde. Dies passiert einfach indem abgeprüft wird,

    if(argc < 2) {

ist. Tritt dieser Fall ein, so wird in Zeile 26

    printf("Benutzung: %s <zu suchende Zahl>\n", argv[0]);
            return 1;

ausgegeben und in Zeile 27 mit `return 1` das Programm beendet.

Wurde eine zu suchende Zahl angegeben, so muss diese noch einer Variable
zugewiesen werden. Der übergebene Parameter kann nicht einfach
zugewiesen werden, da dieser als String übergeben wird, also wird zuerst
die übergebene Zeichenkette zu einem Integer konvertiert. Dies passiert
durch ein Aufruf der Funktion `atoi()`. Der errechnete Integer Wert wird
mit

    zahl = atoi(argv[1]);

in `zahl` gespeichert.

Nun wird mit

    for (i = 0; i < MAX; i++) {
            printf("%4d", array[i]);
        }

durch das Array iteriert und einmal jedes Element in Zeile 31
ausgegeben.

Anschließend wird das erste Mal unsere Funktion `binarysearch`
aufgerufen. Sie wird natürlich mit der gespeicherten, zu suchenden Zahl,
und den Grenzen des ursprünglichen Arrays aufgerufen. Das Ergebnis wird
in unsere Variable

    position = binarysearch(zahl, 0, 7); 

gespeichert.

Nun muss noch überprüft werden, ob die Funktion einen sinnvollen Wert
zurückgegeben hat. Dazu prüft man, ob die gespeicherte Variable
`position` eine positive Zahl ist. Denn wenn wir uns zurückerinnern an
die Funktion, so stellt man fest, dass wenn die Zahl nicht gefunden
wurde, wird von der Funktion -1 zurückgegeben und erfüllt somit die
Bedingung

    if (position >= 0) {

nicht.

Wird die Bedingung erfüllt, also die Zahl wurde im Array gefunden, so
bekommen wir von der Funktion eine positive ganze Zahl, welche dann mit
einem Hinweis ausgegeben wird. Diese Zahl beschreibt den Index der
gesuchten Zahl im Array:

    printf("\nGesuchte Zahl %d an Arrayposition %d\n", zahl, 
            position);

Wurde die gesuchte Zahl nicht gefunden so wird der Hinweis

    else {
            printf("\nZahl %d nicht gefunden\n", zahl);
        }

in Zeile 38 ausgegeben.

Anschließend wird das Programm per

    return 0;

beendet.

Das Programm ist nun fertig und funktionsfähig. Ob das Programm
funktioniert, wird im nächsten Schritt erörtert.

## Teilaufgabe b)

Um das Programm ausführen zu können, muss es wie in vorherigen Aufgaben
wie folgt kompiliert werden:

    gcc gdb-uebung-5.c -g -o gdb-ueb5

Anschließend kann das Programm ausgeführt werden. Aufrufen des Programms
mit

    ./gdb-uebung-5 1

ergibt die Ausgabe,

``` 
   1   4  12  18  26  31  40  42
Rekursionstiefe: 1
Rekursionstiefe: 2
Rekursionstiefe: 3
Gesuchte Zahl 1 an Arrayposition 0
```

welche genau so zu erwarten war.

Aufrufen des Programms mit

    ./gdb-uebung-5 4

ergibt die Ausgabe

``` 
   1   4  12  18  26  31  40  42
Rekursionstiefe: 1
Rekursionstiefe: 2
Gesuchte Zahl 4 an Arrayposition 1
```

Aufrufen des Programms mit

    ./gdb-uebung-5 26

ergibt die Ausgabe

``` 
   1   4  12  18  26  31  40  42
Rekursionstiefe: 1
Rekursionstiefe: 2
Rekursionstiefe: 3
Gesuchte Zahl 26 an Arrayposition 4
```

Rufen wir das Programm nun mit einer Zahl auf, die nicht in unserer
Liste ist, um diesen Fehlerfall zu testen. Führen wir es also mit

    ./gdb-uebung-5 27

aus , sollte die Ausgabe

``` 
   1   4  12  18  26  31  40  42
Rekursionstiefe: 1
Rekursionstiefe: 2
Rekursionstiefe: 3
Rekursionstiefe: 4
Zahl 27 nicht gefunden
```

diese sein. Unsere Ausgabe ist jedoch

    Rekursionstiefe: 174513
    Rekursionstiefe: 174514
    Rekursionstiefe: 174515
    Rekursionstiefe: 174516
            ...
    Speicherzugriffsfehler (Speicherabzug geschrieben)

Dieser Fehler wird im späteren Verlauf genauer erläutert, zuerst testen
wir noch eine letzte Ausgabe. Aufrufen des Programms mit der Zahl `42`

    ./gdb-uebung-5 42

sollte die Ausgabe

``` 
   1   4  12  18  26  31  40  42
Rekursionstiefe: 1
Rekursionstiefe: 2
Rekursionstiefe: 3
Gesuchte Zahl 42 an Arrayposition 7
```

ergeben.

Die Ausgabe dabei ist,

    Rekursionstiefe: 174477
    Rekursionstiefe: 174478
    Rekursionstiefe: 174479
    Rekursionstiefe: 174480
            ...
    Speicherzugriffsfehler (Speicherabzug geschrieben)

welche falsch ist. Auch hier ist wieder der selbe Fehler aufgetreten,
obwohl diese Zahl im Array gefunden werden sollte. Im Folgenden
Abschnitt wird der Prozess des Debuggings anhand dieser Fehler genauer
erläutert.

## Teilaufgabe c)

Um ein Programm mit dem GNU-Debugger zu Debuggen, muss die Datei, wie
auch vor dem Ausführen zuerst kompiliert werden. Zuerst kompiliert man
also das Programm mit

    gcc -o ueb5 gdb-uebung-5.c -g

Dies passiert analog zu Aufgabe [2.2](#1b). Nun kann mit dem
eigentlichen Debuggen begonnen werden. Zuerst startet man den Debugger
mit dem Befehl

    gdb ueb5

Dann gibt es mehrere Möglichkeiten fortzufahren. Man könnte das Programm
zuerst mit einem Funktionierenden Aufrufparameter zeilenweise
analysieren. Jedoch ergibt es mehr Sinn, mit einem Parameter zu
debuggen, welcher bei den Tests zu einem Fehler geführt hat. In unserem
Fall war das die Zahl `27`. Bei diesem Aufruf hätte das Programm die
Meldung ausgeben sollen, dass der Parameter nicht im Array gefunden
wurde. Warum dies nicht der Fall ist soll nun analysiert werden.  
Als erstes wird das Programm per

    (gdb) start 27

gestartet.

In der Ausgabe

    Temporärer Haltepunkt 1 at 0x122e: file gdb-uebung-5.c, line 22.
    Starting program: /home/tux/Dokumente/Praktikum2/ueb5 27
    
    Temporary breakpoint 1, main (argc=21845, argv=0x0) at 
    gdb-uebung-5.c:22
    22  int main(int argc, char *argv[]) {

sieht man, dass automatisch ein Breakpoint gesetzt wurde. Nun kann das
Programm Zeilenweise analysiert werden, um den Fehler zu eliminieren.  
Mit `next` kann in die nächste Zeile des Programms gesprungen werden.
Begonnen wird natürlich, wie oben zu sehen, in der Hauptfunktion in
Zeile \(22\). Springen wir einmal in die nächste Zeile so wird unser
Programm aufgerufen und man kann testweise die ersten beiden Parameter
überprüfen. Wurden zwei Parameter übergeben und was für eine Zahl wurde
übergeben? Mit dem Befehl

    (gdb) print argc
    $1 = 2

gibt man testweise `argc` aus.

Zeilen mit einem Dollarzeichen davor sind immer die Ergebnisse unserer
Eingaben. In unserem Fall ist `argc` also `2`, genau wie erwartet.
Einmal der Programmname selbst und hoffentlich die `27` als zweiter
Parameter. Führt man den Befehl

    (gdb) print argv[1]
    $2 = 0x7fffffffe394 "27"

aus, so ist die Ausgabe wie erwartet `27` und bis zu diesem Punkt läuft
alles wie erwartet ab.

Springen wir mit einem weiteren

    (gdb) next

nun eine Zeile weiter, so überprüft das Programm genau das:

    24      if(argc < 2) {

Der genaue Ablauf wurde oben bereits erläutert, wir springen nun an den
Punkt, wo das erste Mal unsere `binarysearch` Funktion aufgerufen wird.
Dazu setzen wir einen Breakpoint in Zeile 33 und führen das Programm mit
`continue` bis zu Zeile 33 aus:

    (gdb) break 33
    Haltepunkt 2 at 0x5555555552bf: file gdb-uebung-5.c, line 33.
    (gdb) continue
    Continuing.
    
    Breakpoint 2, main (argc=2, argv=0x7fffffffe068) 
    at gdb-uebung-5.c:33

Wir befinden uns nun also an dieser Stelle:

    33      position = binarysearch(zahl, 0, 7); 

Mit

    (gdb) step
    binarysearch (zahl=0, links=0, rechts=32767) at gdb-uebung-5.c:8
    8   int binarysearch(int zahl, int links, int rechts) {

gelangen wir nun in unsere aufgerufene Funktion. Bereits hier sehen wir,
dass für den rechten Wert nicht die \(7\) übergeben wurde, sondern ihre
Speicheradresse. Genau an dieser Stelle setzen wir wieder mit

    (gdb) break 8 
    Haltepunkt 3 at 0x555555555169: file gdb-uebung-5.c, line 8.

einen Breakpoint um immer nach einem Aufruf der Funktion anzuhalten.

Setzen wir mit

    (gdb) break 10 

in Zeile `10` einen Breakpoint und springen mit

    (gdb) continue

bis zu diesem Punkt so berechnen wir die

    10      int mitte = (links + rechts) / 2;

Um unsere berechnete `mitte` zu untersuchen, führen wir noch einmal
`next` aus, sodass die Zeile ausgeführt wurde, und geben anschließend
mit

    (gdb) next
    11      printf("\nRekursionstiefe: %d", rekursionstiefe);
    (gdb) print mitte
    $5 = 3

in Zeile Drei die `mitte` aus. Hier sehen wir schon, dass die `mitte
= 3` ist. Die eigentliche Mitte jedoch ist \(3,5\). Da an dieser Stelle
allerdings kein Element ist, da ein Array ganzzahlig indiziert ist, muss
man entweder die \(3\) oder \(4\) nehmen. In unserem Fall wählen wir
immer das kleinere, da wir die Nachkommazahl abschneiden. Dies ist an
dieser Stelle kein Problem, wird uns später allerdings zum Verhängnis.
Anschließend passieren die Überprüfungen, die oben genauer beschrieben
sind. In unserem Fall ist die `zahl` = \(27\) und somit kleiner als
unser Array an der Stelle `mitte`. Dies kann mit

    (gdb) print mitte
    $1 = 3
    (gdb) print array[3]
    $2 = 18
    (gdb) print zahl
    $3 = 27

überprüft werden. Wir befinden uns aktuell in Zeile \(16\), nachdem die
anderen Überprüfungen wie erwartet falsch waren. Führt man in Zeile
\(16\) ein

    (gdb) next
    19          return binarysearch(zahl, mitte, rechts);

aus, so springt das Programm in Zeile \(19\), da unsere `if` Anweisung
in Zeile \(16\) falsch war, und somit der `else` Block ausgeführt wird.

Mit

    (gdb) step
    
    Breakpoint 3, binarysearch (zahl=-1, links=-1, rechts=0) at 
    gdb-uebung-5.c:8
    8   int binarysearch(int zahl, int links, int rechts) {

springen wir wieder in den nächsten Aufruf der Funktion.

Der nächste Aufruf passiert Analog dazu, und unsere Funktion wird
anschließend mit den Parametern

    (gdb) print links
    $5 = 3
    (gdb) print rechts
    $6 = 5

aufgerufen. Die `mitte` befindet sich nun bei

    (gdb) print mitte
    $9 = 4

Unser Array an der Stelle

    (gdb) print array[4]
    $10 = 26

hat den Wert 26.

Dieser ist immer noch kleiner als unsere gesuchte Zahl und somit wird
wieder der `else` Block ausgeführt und die Funktion wird erneut mit den
Werten

    (gdb) print links
    $11 = 4
    (gdb) print rechts
    $12 = 5

erneut ausgeführt. Daraus errechnet sich wieder

    (gdb) print mitte
    $13 = 4

Da sich die `mitte` also nicht geändert hat, ist unsere Zahl immer noch
größer als der Array an der vierten Stelle. Und die Funktion wird nach
einem `next` wieder mit

    (gdb) next
    19          return binarysearch(zahl, mitte, rechts);

aufgerufen. Ab diesem Punkt kann sich die Berechnung der `mitte` nie
ändern, da wir immer die Nachkommazahl abschneiden. Somit wird nie eine
Abbruchbedingung erfüllt, und die Funktion ruft sich in Zeile \(19\)
immer wieder selber mit den selben Werten auf. Unsere Funktion sollte
eigentlich in Zeile \(14\) ab einem gewissen Punkt abbrechen, diese
Bedingung wird allerdings durch die Fehlerhafte Formel niemals erfüllt\!
Das Problem lässt sich mit dem Befehl

    (gdb) backtrace

gut aufzeigen.

Die Ausgabe

    [...]
    #174639 0x000055555555522c in binarysearch (zahl=27, links=4, 
    rechts=5) at gdb-uebung-5.c:19
    #174640 0x000055555555522c in binarysearch (zahl=27, links=4, 
    rechts=5) at gdb-uebung-5.c:19
    #174641 0x000055555555522c in binarysearch (zahl=27, links=3, 
    rechts=5) at gdb-uebung-5.c:19
    #174642 0x0000555555555218 in binarysearch (zahl=27, links=3, 
    rechts=7) at gdb-uebung-5.c:17
    #174643 0x000055555555522c in binarysearch (zahl=27, links=0, 
    rechts=7) at gdb-uebung-5.c:19
    #174644 0x00005555555552d3 in main (argc=2, argv=0x7fffffffe058) at
    gdb-uebung-5.c:33

zeigt nun alle Aufrufe von Funktionen. Hier wird klar, dass sich die
Werte von `links` und `rechts` ab einem bestimmtem Punkt nicht mehr
ändern, und somit die Abbruchbedingung in Zeile \(14\) niemals erfüllt
wird. Somit kann auch der Hinweis "Zahl nicht gefunden" niemals
ausgegeben werden, da nicht mehr in die Hauptfunktion gesprungen werden
kann.

Analog dazu, lässt sich das Problem mit der \(42\) erklären. Hierbei
sollte das Programm jedoch nicht abbrechen, sondern unsere Zahl finden\!
Jedoch passiert auch hier das selbe. Unser Array wird von der `mitte`
nach `rechts` durchsucht und die `mitte` wandert immer weiter nach
`rechts`, bis zum Index \(6\). Im Anschluss sollte die `mitte`
allerdings \(7\) sein. Jedoch reicht es hier aus logisch über das
Problem nachzudenken. Ist man am Punkt angekommen, an dem die Funktion
mit \(6\) und \(7\) aufgerufen wird, so kann die Berechnung der `mitte`
niemals \(7\) ergeben\! Die Berechnung ergibt immer \(6,5\) und unsere
Nachkommazahl wird fälschlicherweise abgeschnitten und das Programm ruft
sich, wie vorher bereits erläutert, immer wieder selbst auf, ohne dass
sich die Werte ändern\! Dies lässt sich auch mit dem `backtrace` Befehl
gut zeigen. Die Ausgabe

    [...]
    #174639 0x000055555555522c in binarysearch (zahl=42, links=6,
    rechts=7) at gdb-uebung-5.c:19
    #174640 0x000055555555522c in binarysearch (zahl=42, links=6,
    rechts=7) at gdb-uebung-5.c:19
    #174641 0x000055555555522c in binarysearch (zahl=42, links=5,
    rechts=7) at gdb-uebung-5.c:19
    #174642 0x000055555555522c in binarysearch (zahl=42, links=3,
    rechts=7) at gdb-uebung-5.c:19
    #174643 0x000055555555522c in binarysearch (zahl=42, links=0,
    rechts=7) at gdb-uebung-5.c:19
    #174644 0x00005555555552d3 in main (argc=2, argv=0x7fffffffe068) at
    gdb-uebung-5.c:33

ist hierbei \(174644\) Zeilen lang, und scrollt man ganz ans Ende, so
sieht man die Stacks am Anfang unseres Programmes und außerdem sieht
man, dass sich eben die Aufrufwerte nach dem vierten Aufruf unserer
Funktion nicht mehr ändern.

Hier sieht man, dass sich der Wert von `links` nicht mehr ändert, und es
wird nur die Stelle überprüft, die in der Berechnung von `mitte`
berechnet wurde. Da `mitte` jedoch nie den Wert \(7\) annimmt, kann
weder der Wert gefunden werden, noch kann das Programm beendet werden,
da wieder keine Abbruchbedingung erfüllt wird.

Das Problem lässt sich lösen, indem man die Formel so ändert, das jeder
Index im Array erreicht werden kann und somit auch Abbruchbedingungen
erfüllt werden können. Die Lösung des Problems wird im nächsten Punkt
genauer erklärt. Es wurde nun bereits erörtert, an welchem Punkt eine
fehlerhafte Programmierung vorliegt, doch weshalb bricht unser Programm
ab?

### Teilaufgabe c : Aus welchem Grund bricht das Programm ab?

Führt man das Programm einmal komplett

    (gdb) run 27

im Debugger aus, so erhalten wir einen

    Program received signal SIGSEGV, Segmentation fault.

Fehler. Eine mögliche Ursache für diesen Fehler könnte ein
Stack-Overflow sein. Wir können wie folgt untersuchen, ob es sich also
um einen solchen handelt. Zuerst kompilieren wir das Programm nochmal,
aber mit dem Befehl,

    gcc -Wall -Wpedantic -fsanitize=address -g gdb-ueb-5.c

und anderen Flags. Führen wir nun das Programm wieder mit \(27\) aus:

    ./a.out 27

So erhalten wir wie gewohnt unsere Ausgabe wie oben auch, und zusätzlich
erhalten wir noch Informationen zu den Stacks. Dabei zeigt uns die
Ausgabe,

    ==2716==ERROR: AddressSanitizer: stack-overflow on address 0x7ffdc4
    ad6ff8 (pc 0x7f2105d2bfe6 bp 0x7ffdc4ad7092 sp 0x7ffdc4ad7000 T0)

genau das an was wir erwartet haben.

Das bedeutet wir erhalten auf einem Stack mit obiger Adresse einen
Stack-Overflow. Dieser entsteht durch oben beschriebene unendliche
Rekursion. Nach Eintreten des Stack-Overflows wird das Programm
automatisch terminiert und somit die unendliche Rekursion abgebrochen.  
Wir müssen nun also unsere Formel so verändern, dass wir jeden Index im
Array mit dieser Formel berechnen können. Sonst können wir überhaupt
nicht richtig überprüfen, ob unser gesuchtes Element in der Liste ist.
Außerdem ist es dann auch möglich, dass eine Abbruchbedingung erreicht
wird, da dann `links` auch den selben Wert annehmen kann wie `rechts`
und somit entsteht keine unendliche Rekursion mehr, und das Problem des
Stack-Overflows ist damit gelöst.

## Erklärung der Modifikationen

Wie bereits oben erläutert, musste die Formel abgeändert werden, damit
jeder Index im Array berechnet und überprüft werden kann. Hierbei war
das Problem, dass man bei einer rechtsseitigen Suche nicht jeden Index
berechnen konnte, da zum Beispiel das Randelement also der Index 7 nicht
erreicht wurde.  
Also wurden folgende Modifikationen an der Formel durchgeführt. Aus
unserer ursprünglichen Formel haben sich die Formeln \[\begin{aligned}
    \mathrm{\texttt{mitteright}}\, &=\, ((\mathrm{\texttt{links}}\, +\, \mathrm{\texttt{rechts}})\, /\, 2)\, +\, 1;\\
    \mathrm{\texttt{mitteleft}}\, &=\, (\mathrm{\texttt{links}}\, +\, \mathrm{\texttt{rechts}})\, /\, 2;\end{aligned}\]
ergeben.

Die zweite Formel ist dabei direkt die ursprüngliche Formel, da diese
für den linken Teil des Arrays korrekt war. Also können wir diese
weiterhin verwenden, jedoch nur noch für den linken Teil eines Arrays.
Was damit genau gemeint ist, wird später erläutert.  
Die erste Formel ist für den rechten Teil des Arrays zuständig. Durch
die `+1` am Ende wird jedes ursprünglich berechnete Element ein Index
größer, was bedeutet, dass nun auch der letzte Index erreicht werden
kann.  
Da wir nun aber zwei Formeln haben, müssen auch immer beide überprüft
werden. Dies führt zu den folgenden Änderungen.

Mit

    if (array[mitteleft] == zahl ){
            return mitteleft; }
    if (array[mitteright] == zahl){
            return mitteright;}

in Zeile 13 werden beide errechneten Indizes überprüft.

Anschließend wird in Zeile 19 überprüft, ob

    if (array[mitteleft] > zahl){
            return binarysearch(zahl, links, mitteleft);}

ist. Es wird nun der Array an der Stelle `mitteleft` mit `zahl`
verglichen. Falls diese Bedingung erfüllt ist, wird `binarysearch` mit
`mitteleft` als rechte Grenze aufgerufen. Dies entspricht noch dem
ursprünglichen, außer dass `mitte` in `mitteleft` umbenannt wurde.

Eine größere Änderung ist die folgende. Damit wir alle Elemente im
rechten Teil des Arrays erreichen können, muss die andere Formel
verwendet werden, um den Rundungsfehler zu eliminieren. Außerdem muss
natürlich der Array an der Stelle `mitteright` mit der gesuchten `zahl`
verglichen werden und anschließend muss auch der nächste Aufruf

    if (array[mitteright] < zahl){
            return binarysearch(zahl, mitteright, rechts);}

mit `mitteright` und `rechts` durchgeführt werden. Dies führt dazu, dass
eben auch `mitteright` den letzten Index, also \(7\) als Wert annehmen
kann, und somit keine Endlosschleife entsteht. Die restlichen Sachen
bleiben gleich aber Abbruchbedingungen können durch die neuen Formeln
erfüllt werden.

## Teilaufgabe d)

    #include <stdio.h>
    #include <stdlib.h>
    
    #define MAX 8 
    int array[MAX] = {1,4,12,18,26,31,40,42};
    int rekursionstiefe = 0;
    
    int binarysearch(int zahl, int links, int rechts) {
        rekursionstiefe++;
        int mitteright = ((links + rechts) / 2)+1;
        int mitteleft = (links + rechts) / 2;
        printf("\nRekursionstiefe: %d", rekursionstiefe);
        if (array[mitteleft] == zahl ){
            return mitteleft; }
        if (array[mitteright] == zahl){
            return mitteright;}
        if (links == rechts){
            return -1; }
        if (array[mitteleft] > zahl){
            return binarysearch(zahl, links, mitteleft);}
        if (array[mitteright] < zahl){
            return binarysearch(zahl, mitteright, rechts);}
        return -1;
    }
    
    int main(int argc, char *argv[]) {
      int zahl, position, i;
        if(argc < 2) {
            printf("Benutzung: %s <zu suchende Zahl>\n", argv[0]);
            return 1;
        }
        zahl = atoi(argv[1]);
    
        for (i = 0; i < MAX; i++) {
            printf("%4d", array[i]);
        }
        position = binarysearch(zahl, 0, 7);
        if (position >= 0) {
            printf("\nGesuchte Zahl %d an Arrayposition %d\n", zahl, 
            position);
        }
        else {
            printf("\nZahl %d nicht gefunden\n", zahl);
        }
        return 0;
    }
