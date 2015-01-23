#version 410 core

in vec3 position; // Position of the vertex
in vec3 normal;   // Normal of the vertex
in vec2 texCoord; // UV texture coordinates
in float arrowOffset; // Sideways offset for billboarded normal arrows

out vec3 clr; // Computed color for this vertex
out vec2 texc;

// Transformation matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float repeatU = 1.0;
uniform float repeatV = 1.0;

void main(){
    texc = vec2(texCoord.x * repeatU, texCoord.y * repeatV);
//    texc = vec2(texCoord.x, texCoord.y);

    vec4 position_cameraSpace = view * model * vec4(position, 1.0);
//    vec4 normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal), 0);

    gl_Position = projection * position_cameraSpace;

    clr = vec3(1.0);
}
