# Übung 7: Funktionen und parallele Programmierung II

In den nachfolgenden Aufgaben wird der zweite Teil der letzten Übung fortgesetzt.
Es soll die näherungsweise Lösung des linearen Gleichungssystems

Ax=b, A=a<sub>i,j</sub> &isin; &real;<sup>n x n</sup> und b,x &isin; &real;
	
also

&sum; a<sub>i,j</sub> x<sub>j</sub> = b<sub>i</sub>, i &isin; {1,...,n}
	
ermittelt. Dabei soll das Jacobi-Verfahren verwendet werden, welches die *i*-te Gleichung nach x<sub>i</sub> auflöst. Hieraus ergibt sich folgende Iterationsvorschrift für den *m+1*-ten Iterati-onsschritt:

x<sub>i</sub><sup>m+1</sup> = 1 / a<sub>i,i</sub> (b<sub>i</sub> - &sum; a<sub>i,j</sub><sup>m</sup>)

Um zu erkennen, ob das Iterationsverfahren die Lösung gefunden hat, soll der Euklidische Abstand zwischen x<sup>m</sup> und x<sup>m+1</sup> verwendet werden. Ist dieser sehr klein (bei uns kleiner als &radic; (n 􏰄􏰂￼0.0000001 · n)), wurde der Lösungsvektor gefunden und das Iterationsverfahren kann abgebrochen werden. Der Abstand wird wie folgt bestimmt: &radic; (&sum; (x<sub>i</sub><sup>m</sup> - x<sub>i</sub><sup>m+1</sup>)<sup>2</sup>)

**Hinweis:** Im L2P finden Sie eine äquivalentes PDF-Dokument, welches die Formeln *schöner* und eventuell *lesbarer* darstellt. 

1. Parallelisieren Sie das Jacobi-Verfahren (und nicht die Bestimmung des Euklidischen Abstands) durch die Verwendung von POSIX-Threads. Gehen Sie dabei zur Vereinfachung davon aus, dass `n` durch die Anzahl der Threads ohne Rest teilbar ist.
2. Verbessern Sie Ihre Lösung, indem Sie SSE-Instruktionen zur Bestimmung des Euklidischen Abstands verwenden. Gehen Sie zur Vereinfachung davon aus, dass `n` eine gerade Zahl ist.
3. Verbessern Sie Ihre zuvor entwickelte Lösung des Jacobi-Verfahrens, in dem Sie neben den POSIX-Threads nun auch SSE-Instruktionen verwenden. Nutzen Sie dabei die beiden in den vorherigen Aufgabenteilen genannten Vereinfachungen.
4. Welchen Geschwindigkeitszuwachs erzielen Sie gegenüber der ursprünglichen, nicht parallelisierten Version aus der vorherigen Übung?