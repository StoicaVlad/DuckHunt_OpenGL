#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace geometry
{

    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill = false);

    Mesh* CreateCircle(const std::string& name, float radius, float x, float y, glm::vec3 color, bool fill);

}
