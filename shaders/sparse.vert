#version 410 core

in vec3 position; // Position of the vertex
in vec2 texCoord; // UV texture coordinates

out vec2 texc;

// Transformation matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec2 subImages = vec2(1.);
uniform vec2 subPos = vec2(0.);
uniform vec2 repeatUV = vec2(1.0);

void main() {

    texc = (subPos / subImages) + (texCoord / subImages); // for atlas
//        texc = vec2(texCoord.x * repeatU, texCoord.y * repeatV);

    gl_Position = projection * view * model * vec4(position, 1.0);
}
