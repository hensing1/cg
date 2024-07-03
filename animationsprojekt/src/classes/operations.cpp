#include "operations.hpp"


// Implementation von get_rotation_matrix.
// Es wird je eine Matrix für die Rotation in x-, y- und z-Richtung erstellt und diese dann multipliziert.
mat3 Operations::get_rotation_matrix(vec3 angles) {
    mat3 rotation_x(
         1.0,      0.0,             0.0,
         0.0, cos(angles[0]), -sin(angles[0]),
         0.0, sin(angles[0]),  cos(angles[0])
    );
    mat3 rotation_y(
         cos(angles[1]), 0.0, sin(angles[1]),
              0.0,       1.0,      0.0,
        -sin(angles[1]), 0.0, cos(angles[1])
    );
    mat3 rotation_z(
        cos(angles[2]), -sin(angles[2]), 0.0,
        sin(angles[2]),  cos(angles[2]), 0.0,
              0.0,           0.0,        1.0
    );
    // OpenGL legt Matrizen falsch herum an -> muss einmal transponiert werden
    return transpose(rotation_x * rotation_y * rotation_z); 
}
