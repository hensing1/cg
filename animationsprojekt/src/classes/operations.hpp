#pragma once

#include <glm/glm.hpp>
using namespace glm;


/**
 *  NOTE: Diese Klasse ist Hilfsklasse für unterschiedliche sinnvolle Operationen für die Animation von Objekten
*/

class Operations {

public:

   //  NOTE: Erstellt eine Matrix, mit dem ein Objekt beliebig gedreht werden kann.
   // Input ist ein Vector mit den drei Eulerwinkeln (Yaw, Pitch und Roll).
   // Diese Winkel sind im Bogenmaß anzugeben -> 2*PI ist eine volle Rotation
   mat3 get_rotation_matrix(vec3 angles);

};
