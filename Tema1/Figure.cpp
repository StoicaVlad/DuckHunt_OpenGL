#include "Figure.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* geometry::CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* geometry::CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
    glm::vec3 vertex = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(vertex, color),
        VertexFormat(vertex + glm::vec3(length, 0, 0), color),
        VertexFormat(vertex + glm::vec3(length / 2, length, 0), color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2};

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* geometry::CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(height, 0, 0), color),
        VertexFormat(corner + glm::vec3(height, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }
    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* geometry::CreateCircle(const std::string& name, float radius, float x, float y, glm::vec3 color, bool fill) {

    const float PI = 3.1415926f;
    int i = 0;

    unsigned int fragments = 30;

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;
    std::vector<VertexFormat> vertices;
    float increment = 2.0f * PI / fragments;

    circle->SetDrawMode(GL_TRIANGLE_FAN);

    for (float currAngle = 0.0f; currAngle <= 2.0f * PI; currAngle += increment)
    {
        VertexFormat v = VertexFormat(glm::vec3(radius * cos(currAngle) + x, radius * sin(currAngle) + y, 0), color);
        vertices.push_back(v);
        indices.push_back(i);
        i++;
    }
    circle->InitFromData(vertices, indices);
    return circle;

}