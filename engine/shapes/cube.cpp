#include "cube.h"

#include <iostream>
using namespace std;

Cube::Cube()
{
}


Cube::~Cube()
{
}


void Cube::calcVerts()
{
    m_numVerts = 14;
    int size = m_numVerts * 8;
    m_vertexData = new GLfloat[size];

    int index = 0;
    glm::vec3 norm = glm::vec3(0, 0, 1);

    addVertex(&index, glm::vec3(-0.5,  0.5, -0.5), norm); // 4
    addVertex(&index, glm::vec3( 0.5,  0.5, -0.5), norm); // 3
    addVertex(&index, glm::vec3(-0.5,  0.5,  0.5), norm); // 7
    addVertex(&index, glm::vec3( 0.5,  0.5,  0.5), norm); // 8
    addVertex(&index, glm::vec3( 0.5, -0.5,  0.5), norm); // 5

    addVertex(&index, glm::vec3( 0.5,  0.5, -0.5), norm); // 3
    addVertex(&index, glm::vec3( 0.5, -0.5, -0.5), norm); // 1
    addVertex(&index, glm::vec3(-0.5,  0.5, -0.5), norm); // 4
    addVertex(&index, glm::vec3(-0.5, -0.5, -0.5), norm); // 2
    addVertex(&index, glm::vec3(-0.5,  0.5,  0.5), norm); // 7

    addVertex(&index, glm::vec3(-0.5, -0.5,  0.5), norm); // 6
    addVertex(&index, glm::vec3( 0.5, -0.5,  0.5), norm); // 5
    addVertex(&index, glm::vec3(-0.5, -0.5, -0.5), norm); // 2
    addVertex(&index, glm::vec3( 0.5, -0.5, -0.5), norm); // 1
}


