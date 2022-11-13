#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace geometry
{
    // draw a square on screen starting from left bottom corner
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    // draw a triangle on screen starting from left bottom corner
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    // draw a rectangle on screen starting from left bottom corner
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill = false);

    // draw a circle on screen starting from the central point
    Mesh* CreateCircle(const std::string& name, float radius, float x, float y, glm::vec3 color, bool fill);

}
