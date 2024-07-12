# GUI

**Die GUI kann durch die Taste `KOMMA` gezeigt und versteckt werden!**

<br>

Oben in der GUI werden einige wichtige Debug-Informationen angezeigt.

Dazu zählt insbesondere die **Zeit innerhalb der Szene**, welche mit dem "_Reset Time_"-Knopf zurückgesetzt werden kann. Die anderen Zeiten sind die **globale Zeit** seit Start des Prozesses sowie die **Startzeit der Szene**.

Desweiteren wird dort angezeigt, im wievielten **Frame** und an welcher **(kartesischen) Position** in der Animation man sich befindet.

<br>

Darunter gibt es einige interaktive Elemente.
Aus einer Auswahl kann eine beliebige **Szene ausgewählt** und dorthin **gesprungen** werden.

Es kann außerdem der **DEBUG-Modus** aktiviert und die **Animation** pausiert werden.

<br><br>


# DEBUG-Modus

Wenn der DEBUG-Modus aktiv ist, folgt die Kamera nicht mehr ihrem eigentlichen Pfad. Stattdessen kann sie [**vom Nutzer selbst bewegt werden**](#tastenbelegung).

Im DEBUG-Modus werden einige relevante Debug-Objekte gerendert, die sonst nicht sichtbar sind.
Diese sind:
- Eine kleine türkise Kugel, welche dem eigentlichen Kamerapfad folgt, wenn die Animation läuft.
- Größere magentafarbene Kugeln für alle Punkte der `QuinticHermite`-Spline, welcher die Kamera folgt.
- Vier Kugeln bei $(0,0,0)$, welche die Richtungen der Koordinatenachsen zeigen.

<br><br>


# Tastenbelegung
<center>

**Taste** | Nur in Debug verfügbar | Funktion
:---: | :---: | --- 
W | **JA** | Bewegt die Kamera **vorwärts**
A | **JA** | Bewegt die Kamera **nach links**
S | **JA** | Bewegt die Kamera **rückwärts**
D | **JA** | Bewegt die Kamera **nach rechts**
SHIFT | **JA** | Bewegt die Kamera **ach unten**
SPACE | **JA** | Bewegt die Kamera **nach oben**
BACKSPACE | - | Setzt die Zeit innerhalb der Szene zurück
KOMMA | - | Öffnet / Schließt die GUI
ESC | - | Beendet das Programm

</center>
