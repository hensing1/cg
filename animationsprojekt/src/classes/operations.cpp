#include "operations.hpp"


mat3 Operations::get_rotation_matrix(vec3 angles) {
    mat3 first_rotation(
         1.0,      0.0,             0.0,
         0.0, cos(angles[0]), -sin(angles[0]),
         0.0, sin(angles[0]),  cos(angles[0])
    );
    mat3 second_rotation(
         cos(angles[1]), 0.0, sin(angles[1]),
              0.0,       1.0,      0.0,
        -sin(angles[1]), 0.0, cos(angles[1])
    );
    mat3 third_rotation(
        cos(angles[2]), -sin(angles[2]), 0.0,
        sin(angles[2]),  cos(angles[2]), 0.0,
              0.0,           0.0,        1.0
    );
    return first_rotation * second_rotation * third_rotation;
}
