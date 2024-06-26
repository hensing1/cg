# Konzept
*Einführung in die Computergrafik und Visualisierung, SoSe 2024*

[[Infos zum Animationsprojekt.pdf]]

## Das Team

Das Team besteht aus Florian Barth (s58fbarth), John Derksen (s6joderk) und Henning Lehmann (s88jlehm).

## Die Komponenten

Die folgenden Techniken sollen im Rahmen dieses Animationsprojekts umgesetzt werden:

| Kategorie | Komponente                      | Beschreibung                                                                                                     | Verantwortlicher |
| --------- | ------------------------------- | ---------------------------------------------------------------------------------------------------------------- | ---------------- |
| 0         | Prozedurale Geometrie           | Simulation von Wolken mit Voronoi-Rauschen                                                                       | Henning          |
|           | Subdivision Surfaces            | Darstellung einer Erdkugel mit Loop Subdivision - evtl. mit Displacement Mapping für Hervorhebung der Kontinente | Henning          |
| 1         | Shadow Mapping                  | Einfacher Schattenwurf                                                                                           | Florian          |
|           | Fokusunschärfe                  | Abwechselndes Fokussieren der Kamera auf Objekte im Vorder- und Hintergrund                                      | Florian          |
| 2         | Datengetriebene Materialmodelle | Darstellung einer Holzoberfläche mit BTF                                                                         | John             |
| 3         | Umgebungsverdeckung             | Abdunklung unterhalb/hinter Möbeln in einer Szene                                                                | John             |
| 4         | Cinematic Engine                | Kamerafahrt und Keyframe-Interpolation mit Splines, nahtloser Übergang zwischen verschiedenen Szenen             | Florian          |
|           | Sound                           | Hinterlegung des Animationsfilms mit Soundeffekten                                                               | Henning          |
<div style="page-break-after: always;"></div>

## Die Story

### Beschreibung
Der Film startet im Weltraum mit der Ansicht der Erdkugel. Es wird hereingezoomt, die Kamera fährt durch eine Wolkendecke und der Informatik-Campus erscheint. Die Kamera fährt in das Hörsaalzentrum und nimmt Platz in Hörsaal 3, in welchem ein stilisierter Prof. Hullin die Komponenten erklärt, die im vorliegenden Animationsfilm verwendet werden.
In der Ego-Perspektive angekommen, wird ein Laptop aufgeklappt, auf welchem nach und nach die eingangs gezeigte Szene modelliert wird. Die Kamera wechselt währenddessen abwechselnd den Fokus zwischen Laptop und Hullin.
Ist die Szene fertig, wird langsam auf den Laptopbildschirm hereingezoomt, bis dieser das gesamte Fenster einnimmt. Der Film beginnt von neuem.

### Storyboard
![[CG_Storyboard.pdf]]

### Gestaltung und technische Umsetzung

- ***Szene 1: Erdkugel***
	- Erstellung der Kugel: Loop Subdivision
	- Stil: Umrisse der Kontinente handgezeichnet, Textur wird verwendet um Kontinente hervorzuheben und einzufärben
	- $\Rightarrow$ Erstellung eines Skripts zur Generierung des Meshes der Erdkugel
	- Wolken:
		- Dienen als Übergang zwischen Szene 1 und 2
		- Umsetzung mit mehrfach überlagertem Voronoi-Rauschen
			- Menge des Layerings ggf. abhängig von der Distanz der Wolken zur Kamera
		- Annäherung der Lichtbrechung durch Raytracing
		- Notlösung: kleinere "verschmolzene" Kugeln mit Raymarching
- ***Szene 2: Campus***
	- Nachstellung der Gebäude mit Blender
	- Einfacher Schattenwurf der Gebäude durch Modellierung der Sonne als Punktlichtquelle
- ***Szene 3: Hörsaal***
	- Nachstellung des Mobiliars in Blender
	- Texturierung der Sitze, des Bodens und der Wände mit BTFs
	- Schattenwurf mit Shadow Mapping und Umgebungsverdeckung
	- Darstellung von Prof. Hullin, Händen, und anderen Studierenden mit einfachen geometrischen Formen
	- Animation von Prof. Hullin mit Splines
	- Abwechselnde Tiefenunschärfe von Laptop und Dozent
	 
<div style="page-break-after: always;"></div>

## Erwartete Herausforderungen

Die genaue Darstellung der Wolken ist zu diesem Zeitpunkt noch nicht vollständig ausgearbeitet. Eventuell muss hier auf Notlösungen wie Raymarching oder schlichtes Überblenden auf Weiß als Übergang zwischen den beiden Szenen zurückgegriffen werden.

Ebenso bleibt der Aufwand für die Erstellung der verschiedenen Geometrieobjekte (niemand von uns kann Blender)im Unklaren.

Ungelöste Fragen:
- Wie soll die Beamer-Projektion im Hörsaal dargestellt werden?
- Wie gestalten wir auf realistische Weise die Beleuchtung im Hörsaal?
- Existiert für jedes $\epsilon>0$ eine Konstante $K_\epsilon$, so dass für alle Tripel teilerfremder positiver ganzer Zahlen $a,\,b,\,c$ mit $a+b=c$ die Ungleichung $c< K_{\epsilon}(\text{rad}(abc))^{1+\epsilon}$ gilt?
- Wo ist Buddy Hollys Brille jetzt?

Die letzten beiden Fragen werden im Rahmen des vorliegenden Projekts vermutlich nicht beantwortet. Aber man weiß ja nie!

<!--
- Geometrie
	- Erde
		- Subdivision Surfaces (40 P)
	- Campus
	- Evtl. Hörsaalzentrum-Eingangsbereich
	- Hörsaal
		- Pult
		- Bänke
		- Tür
	- Laptop
	- Hullin
	- Hände
- Kamera
	- Fokusunschärfe (20 P)
	- Kamerafahrt (Keyframe-Interpolation mit Splines) (50 P)
- Wolken????
	- Volumetrische Einfachstreuung? (50 P)
- Beamer
	- Hullin wirft Schatten?
		- Shadow Mapping (30 P)
- Beleuchtung im Hörsaal (Umgebungsverdeckung, 30/80 P)
- Cartoon Voice (20/40 P)

Kategorie 0: Subdivision Surfaces, Blender-Assets -> 40 + x
Kategorie 1: Fokusunschärfe -> 20
Kategorie 2: Shadow Mapping, Materialmodell für Holz? -> 30 + 40
Kategorie 3: (Umgebungsverdeckung), Volumetrische Einfachstreuung (30) + 
Kategorie 4: Cinematic Engine, Sound -> 50 + 20


| Szene    | Techniken            | Verantwortlich | Punkte |
| -------- | -------------------- | -------------- | ------ |
| Weltraum | Subdivision Surfaces |                |        |
| Wolke    | Volumetriegedöns     |                |        |
| Campus   | Blender              | Henning        |        |
|          | Kamerafahrt          | Florian        |        |
|          | Sonne                |                |        |
| Hörsaal  | Blender              | Henning        |        |
|          | Beleuchtung          | John           |        |
|          | Materialmodell       | John           | 40     |
|          | Sound                |                | 20     |

-->
