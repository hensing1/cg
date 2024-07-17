#include "hermite.hpp"

#include <vector>
#include <utility>

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <string>
#include <glm/gtx/string_cast.hpp>


Hermite::Hermite() {
    points = std::vector<hermite_point>({});
    HERMITE_MATRIX = transpose(mat4(
         1.0f,  0.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,  0.0f,
        -3.0f, -2.0f,  3.0f, -1.0f,
         2.0f,  1.0f, -2.0f,  1.0f
    ));
}

Hermite::Hermite(std::vector<hermite_point> *input) {
    points = *input;
    HERMITE_MATRIX = transpose(mat4(
         1.0f,  0.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,  0.0f,
        -3.0f, -2.0f,  3.0f, -1.0f,
         2.0f,  1.0f, -2.0f,  1.0f
    ));
}

Hermite& Hermite::operator=(const Hermite& other) {
    points = other.points;
    return *this;
}

void Hermite::set_path(std::vector<hermite_point> new_path) {
    this -> points = new_path;
}
std::vector<hermite_point> Hermite::get_path() {
    return points;
}

vec3 Hermite::evaluateSplineAllowLoop(float t) {

    if (t < 0) return points[0].pos; // t ist negativ -> Kann nicht evaluiert werden

    int segment = int(floor(t)) % points.size();
    float diff = t - floor(t);

    hermite_point *point1 = &points[segment];
    hermite_point *point2;
    if (segment < points.size()-1) point2 = &points[segment+1];
    else point2 = &points[0];

    vec3 res = vec3(0.0f);
    vec4 t_vector = vec4(1.0f, diff, diff*diff, diff*diff*diff) * HERMITE_MATRIX;
    res.x = dot(t_vector, vec4(point1 -> pos.x, point1 -> vel.x, point2 -> pos.x, point2 -> vel.x));
    res.y = dot(t_vector, vec4(point1 -> pos.y, point1 -> vel.y, point2 -> pos.y, point2 -> vel.y));
    res.z = dot(t_vector, vec4(point1 -> pos.z, point1 -> vel.z, point2 -> pos.z, point2 -> vel.z));

    return res;

}

vec3 Hermite::evaluateSpline(float t) {

    if (t < 0) return points[0].pos; // t ist negativ -> Kann nicht evaluiert werden
    if (t >= points.size()-1) return points[points.size()-1].pos;

    int segment = int(floor(t)) % points.size();
    float diff = t - floor(t);

    hermite_point *point1 = &points[segment];
    hermite_point *point2 = &points[segment+1];

    vec3 res = vec3(0.0f);
    vec4 t_vector = vec4(1.0f, diff, diff*diff, diff*diff*diff) * HERMITE_MATRIX;
    res.x = dot(t_vector, vec4(point1 -> pos.x, point1 -> vel.x, point2 -> pos.x, point2 -> vel.x));
    res.y = dot(t_vector, vec4(point1 -> pos.y, point1 -> vel.y, point2 -> pos.y, point2 -> vel.y));
    res.z = dot(t_vector, vec4(point1 -> pos.z, point1 -> vel.z, point2 -> pos.z, point2 -> vel.z));

    return res;

}




/** 
 *   NOTE: Quintic Hermite Splines ab hier
 */

QuinticHermite::QuinticHermite() {
    points = std::vector<quintic_hermite_point>({});
    HERMITE_MATRIX_LU = mat3(0.0f);
    HERMITE_MATRIX_RU = mat3(0.0f);
    HERMITE_MATRIX_LL = mat3(0.0f);
    HERMITE_MATRIX_RL = mat3(0.0f);
}

QuinticHermite::QuinticHermite(quintic_hermite_point first_point, std::vector<vec3> points)
{
    std::vector<quintic_hermite_point> result({first_point});
    init_matrices();
    
    for (int i = 0; i < points.size(); i++) {
        result.push_back(calculateVelAndAcc(result[i], points[i]));
    }

    this -> points = result;

}

QuinticHermite::QuinticHermite(std::vector<quintic_hermite_point> *input) {
    points = *input;
    init_matrices();
}

void QuinticHermite::init_matrices() {
    // Mehrere Matrizen notwendig, weil 6x6-Matrix nicht von GLSM zur Verfügung gestellt.
    HERMITE_MATRIX_LU = transpose(mat3(
         1.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  0.0f,  0.5f
    ));
    HERMITE_MATRIX_RU = transpose(mat3(
         0.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  0.0f
    ));
    HERMITE_MATRIX_LL = transpose(mat3(
       -10.0f, -6.0f, -1.5f,
        15.0f,  8.0f,  1.5f,
        -6.0f, -3.0f, -0.5f
    ));
    HERMITE_MATRIX_RL = transpose(mat3(
        10.0f, -4.0f,  0.5f,
       -15.0f,  7.0f, -1.0f,
         6.0f, -3.0f,  0.5f
    ));
}


quintic_hermite_point QuinticHermite::calculateVelAndAcc(quintic_hermite_point last_point, vec3 current_position) {
    //float diff_squared = diff * diff;
    //float diff_cubed = diff_squared * diff;

    quintic_hermite_point res;
    res.pos = current_position;

    vec3 x_vector = vec3(0.0f, 1.0f, 2.0f);
    vec3 y_vector = vec3(3.0f, 4.0f, 5.0f);

    // Äquivalent zur Multiplikation eines 6x1-Vektors mit einer 6x6-Matrix.
    vec3 t_vector_U = x_vector * HERMITE_MATRIX_LU + y_vector * HERMITE_MATRIX_LL;
    vec3 t_vector_L = x_vector * HERMITE_MATRIX_RU + y_vector * HERMITE_MATRIX_RL;

    res.vel.x = dot(t_vector_U, vec3(last_point.pos.x, last_point.vel.x, last_point.acc.x)) +
            dot(t_vector_L, vec3(current_position.x,  0.0f, 0.0f));
    res.vel.y = dot(t_vector_U, vec3(last_point.pos.y, last_point.vel.y, last_point.acc.y)) +
            dot(t_vector_L, vec3(current_position.y,  0.0f, 0.0f));
    res.vel.z = dot(t_vector_U, vec3(last_point.pos.z, last_point.vel.z, last_point.acc.z)) +
            dot(t_vector_L, vec3(current_position.z,  0.0f, 0.0f));

    x_vector = vec3(0.0f, 0.0f, 2.0f);
    y_vector = vec3(6.0f, 12.0f, 20.0f);

    // Äquivalent zur Multiplikation eines 6x1-Vektors mit einer 6x6-Matrix.
    t_vector_U = x_vector * HERMITE_MATRIX_LU + y_vector * HERMITE_MATRIX_LL;
    t_vector_L = x_vector * HERMITE_MATRIX_RU + y_vector * HERMITE_MATRIX_RL;

    res.acc.x = dot(t_vector_U, vec3(last_point.pos.x, last_point.vel.x, last_point.acc.x)) +
            dot(t_vector_L, vec3(current_position.x,  0.0f, 0.0f));
    res.acc.y = dot(t_vector_U, vec3(last_point.pos.y, last_point.vel.y, last_point.acc.y)) +
            dot(t_vector_L, vec3(current_position.y,  0.0f, 0.0f));
    res.acc.z = dot(t_vector_U, vec3(last_point.pos.z, last_point.vel.z, last_point.acc.z)) +
            dot(t_vector_L, vec3(current_position.z,  0.0f, 0.0f));

    return res;
}


vec3 QuinticHermite::evaluateSplineAllowLoop(float t) {

    if (t < 0) return points[0].pos; // t ist negativ -> Kann nicht evaluiert werden

    int segment = int(floor(t)) % points.size();
    float diff = t - floor(t);

    quintic_hermite_point *point1 = &points[segment];
    quintic_hermite_point *point2;
    if (segment < points.size()-1) point2 = &points[segment+1];
    else point2 = &points[0];

    vec3 res = vec3(0.0f);

    float diff_squared = diff * diff;
    float diff_cubed = diff_squared * diff;

    vec3 x_vector = vec3(1.0f, diff, diff_squared);
    vec3 y_vector = vec3(diff_cubed, diff_squared * diff_squared, diff_squared * diff_cubed);

    // Äquivalent zur Multiplikation eines 6x1-Vektors mit einer 6x6-Matrix.
    vec3 t_vector_U = x_vector * HERMITE_MATRIX_LU + y_vector * HERMITE_MATRIX_LL;
    vec3 t_vector_L = x_vector * HERMITE_MATRIX_RU + y_vector * HERMITE_MATRIX_RL;

    res.x = dot(t_vector_U, vec3(point1 -> pos.x, point1 -> vel.x, point1 -> acc.x)) +
            dot(t_vector_L, vec3(point2 -> pos.x, point2 -> vel.x, point2 -> acc.x));
    res.y = dot(t_vector_U, vec3(point1 -> pos.y, point1 -> vel.y, point1 -> acc.y)) +
            dot(t_vector_L, vec3(point2 -> pos.y, point2 -> vel.y, point2 -> acc.y));
    res.z = dot(t_vector_U, vec3(point1 -> pos.z, point1 -> vel.z, point1 -> acc.z)) +
            dot(t_vector_L, vec3(point2 -> pos.z, point2 -> vel.z, point2 -> acc.z));
    
    return (res);

}


vec3 QuinticHermite::evaluateSpline(float t) {

    if (t < 0) return points[0].pos; // t ist negativ -> Kann nicht evaluiert werden
    if (t >= points.size()-1) return points[points.size()-1].pos;

    int segment = int(floor(t)) % points.size();
    float diff = t - floor(t);

    quintic_hermite_point *point1 = &points[segment];
    quintic_hermite_point *point2 = &points[segment+1];

    vec3 res = vec3(0.0f);

    float diff_squared = diff * diff;
    float diff_cubed = diff_squared * diff;

    vec3 x_vector = vec3(1.0f, diff, diff_squared);
    vec3 y_vector = vec3(diff_cubed, diff_squared * diff_squared, diff_squared * diff_cubed);

    // Äquivalent zur Multiplikation eines 6x1-Vektors mit einer 6x6-Matrix.
    vec3 t_vector_U = x_vector * HERMITE_MATRIX_LU + y_vector * HERMITE_MATRIX_LL;
    vec3 t_vector_L = x_vector * HERMITE_MATRIX_RU + y_vector * HERMITE_MATRIX_RL;

    res.x = dot(t_vector_U, vec3(point1 -> pos.x, point1 -> vel.x, point1 -> acc.x)) +
            dot(t_vector_L, vec3(point2 -> pos.x, point2 -> vel.x, point2 -> acc.x));
    res.y = dot(t_vector_U, vec3(point1 -> pos.y, point1 -> vel.y, point1 -> acc.y)) +
            dot(t_vector_L, vec3(point2 -> pos.y, point2 -> vel.y, point2 -> acc.y));
    res.z = dot(t_vector_U, vec3(point1 -> pos.z, point1 -> vel.z, point1 -> acc.z)) +
            dot(t_vector_L, vec3(point2 -> pos.z, point2 -> vel.z, point2 -> acc.z));
    
    return (res);
}
