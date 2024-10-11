#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <vector>

namespace mathUtils
{
    inline float calculateVerticalFov(const float horizontalFov, const float aspectRatio)
    {
        const auto horizontalFovRadians = glm::radians(horizontalFov);
        const auto verticalFovRadians = 2.0f * std::atan(std::tan(horizontalFovRadians / 2.0f) / aspectRatio);
        return glm::degrees(verticalFovRadians);
    }

    inline void calcAverageNormals(const std::vector<unsigned int>& indices, std::vector<GLfloat>& vertices,
                                   unsigned int vLength, unsigned int normalOffset)
    {
        assert(vertices.size() % vLength == 0 && "Vertex array size should be divisible by vLength");

        // Calculate face normals and accumulate them for each vertex
        for (size_t i = 0; i < indices.size(); i += 3)
        {
            unsigned int index0 = indices[i] * vLength;
            unsigned int index1 = indices[i + 1] * vLength;
            unsigned int index2 = indices[i + 2] * vLength;

            glm::vec3 v0(vertices[index0], vertices[index0 + 1], vertices[index0 + 2]);
            glm::vec3 v1(vertices[index1], vertices[index1 + 1], vertices[index1 + 2]);
            glm::vec3 v2(vertices[index2], vertices[index2 + 1], vertices[index2 + 2]);

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;

            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            vertices[index0 + normalOffset] += normal.x;
            vertices[index0 + normalOffset + 1] += normal.y;
            vertices[index0 + normalOffset + 2] += normal.z;

            vertices[index1 + normalOffset] += normal.x;
            vertices[index1 + normalOffset + 1] += normal.y;
            vertices[index1 + normalOffset + 2] += normal.z;

            vertices[index2 + normalOffset] += normal.x;
            vertices[index2 + normalOffset + 1] += normal.y;
            vertices[index2 + normalOffset + 2] += normal.z;
        }

        // Normalize the accumulated normals
        for (size_t i = 0; i < vertices.size() / vLength; ++i)
        {
            unsigned int nOffset = i * vLength + normalOffset;
            glm::vec3 normal(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
            normal = glm::normalize(normal);

            vertices[nOffset] = normal.x;
            vertices[nOffset + 1] = normal.y;
            vertices[nOffset + 2] = normal.z;
        }
    }
}
