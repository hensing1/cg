#include "hermite.hpp"

#include <vector>


#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <string>
#include <glm/gtx/string_cast.hpp>



Hermite::Hermite(std::vector<hermite_point> *input) {
    points = *input;
    HERMITE_MATRIX = transpose(mat4(
         1.0f,  0.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,  0.0f,
        -3.0f, -2.0f,  3.0f, -1.0f,
         2.0f,  1.0f, -2.0f,  1.0f
    ));
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
    //std::cout << "T-VEKTOR:  " << to_string(t_vector) << std::endl;
    res.x = dot(t_vector, vec4(point1 -> pos.x, point1 -> vel.x, point2 -> pos.x, point2 -> vel.x));
    res.y = dot(t_vector, vec4(point1 -> pos.y, point1 -> vel.y, point2 -> pos.y, point2 -> vel.y));
    res.z = dot(t_vector, vec4(point1 -> pos.z, point1 -> vel.z, point2 -> pos.z, point2 -> vel.z));

    //std::cout << "VEKTOR (X):  " << to_string(vec4(points[segment].pos.x, points[segment].vel.x, points[segment+1].pos.x, points[segment+1].vel.x)) << std::endl;
    //std::cout << "VEKTOR (Y):  " << to_string(vec4(points[segment].pos.y, points[segment].vel.y, points[segment+1].pos.y, points[segment+1].vel.y)) << std::endl;
    //std::cout << "SEGMENT:  " << std::to_string(segment) << "  POSITION:  " << to_string(res) << std::endl << std::endl;
    return (res);

}

vec3 Hermite::evaluateSpline(float t) {
    if (t > float(points.size()-1) || t < 0) return points[points.size()-1].pos; // t ist zu groß oder negativ -> Kann nicht evaluiert werden

    int segment = int(floor(t));
    float diff = t - floor(t);


    vec3 res = vec3(0.0f);
    vec4 t_vector = vec4(1.0f, diff, diff*diff, diff*diff*diff) * HERMITE_MATRIX;
    //std::cout << "T-VEKTOR:  " << to_string(t_vector) << std::endl;
    res.x = dot(t_vector, vec4(points[segment].pos.x, points[segment].vel.x, points[segment+1].pos.x, points[segment+1].vel.x));
    res.y = dot(t_vector, vec4(points[segment].pos.y, points[segment].vel.y, points[segment+1].pos.y, points[segment+1].vel.y));
    res.z = dot(t_vector, vec4(points[segment].pos.z, points[segment].vel.z, points[segment+1].pos.z, points[segment+1].vel.z));

    //std::cout << "VEKTOR (X):  " << to_string(vec4(points[segment].pos.x, points[segment].vel.x, points[segment+1].pos.x, points[segment+1].vel.x)) << std::endl;
    //std::cout << "VEKTOR (Y):  " << to_string(vec4(points[segment].pos.y, points[segment].vel.y, points[segment+1].pos.y, points[segment+1].vel.y)) << std::endl;
    //std::cout << "SEGMENT:  " << std::to_string(segment) << "  POSITION:  " << to_string(res) << std::endl << std::endl;
    return (res);

}
