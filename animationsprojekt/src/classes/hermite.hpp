
#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <vector>



/**
 *  NOTE:  Die Hermite-Klasse dient der Erzeugung von Hermite-Splines. 
 *         Bei Erstellung eines Objekts der Klasse muss ein Vektor an 'hermite_points' übergeben werden.
 *         Diese bestehen aus einer Position im Raum und einer Geschwindigkeit.
 *         Ein Objekt wird nacheinander alle Positionen mit entsprechender Geschwindigkeit passieren.
 *         Dazu muss die Funktion evaluateSpline(float t) aufgerufen werden.
 *         Es existiert auch eine Variante der Funktion, welche die Wiederholung des Ablaufes für zu große t erlaubt.
 *         Hierbei wird nach dem letzten Punkt wieder der erste Punkt passiert.
 */

struct hermite_point {
   vec3 pos;
   vec3 vel;
};

struct quintic_hermite_point {
   vec3 pos;
   vec3 vel;
   vec3 acc;
};


class Hermite {

public:
   Hermite();
   Hermite(std::vector<hermite_point> *input);

   /** 
    * Gibt zu einem t die passende Position im Raum gemäß der Spline zurück.
    * Ist das t zu groß, so wird die Position des letzten Punktes gewählt.
   */
   vec3 evaluateSpline(float t);

   /**
    *  Funktioniert genau wie evaluateSpline(), jedoch wiederholt sich der Ablauf der Spline für zu große t.
   */
   vec3 evaluateSplineAllowLoop(float t);

private:
   std::vector<hermite_point> points;
   mat4 HERMITE_MATRIX;


};


class QuinticHermite {

public:
   QuinticHermite();
   QuinticHermite(std::vector<quintic_hermite_point> *input);

   /** 
    * Gibt zu einem t die passende Position im Raum gemäß der Spline zurück.
    * Ist das t zu groß, so wird die Position des letzten Punktes gewählt.
   */
   vec3 evaluateSpline(float t);

   /**
    *  Funktioniert genau wie evaluateSpline(), jedoch wiederholt sich der Ablauf der Spline für zu große t.
   */
   vec3 evaluateSplineAllowLoop(float t);

private:
   std::vector<quintic_hermite_point> points;
   mat3 HERMITE_MATRIX_LU, HERMITE_MATRIX_RU, HERMITE_MATRIX_LL, HERMITE_MATRIX_RL;


};
