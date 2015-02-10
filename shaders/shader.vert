#version 410 core

in vec3 position; // Position of the vertex
in vec3 normal;   // Normal of the vertex
in vec2 texCoord; // UV texture coordinates

out vec3 color; // Computed color for this vertex
out vec2 texc;

// Transformation matrices
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Light data
const int MAX_LIGHTS = 10;
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

uniform vec3 diffuse_color;
uniform vec3 world_color = vec3(0.2);
uniform vec3 specular_color = vec3(1.0);
uniform float shininess = 1.0;
uniform float transparency = 1.0;

uniform vec2 subImages = vec2(1.);
uniform vec2 subPos = vec2(0.);
uniform vec2 repeatUV = vec2(1.0);

uniform vec3 allBlack = vec3(1.0);

void main(){

    texc = (subPos / subImages) + (texCoord / subImages); // for atlas
//        texc = vec2(texCoord.x * repeatU, texCoord.y * repeatV);

    vec4 position_cameraSpace = view * model * vec4(position, 1.0);
    vec4 normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(view * model))) * normal), 0.0);

    vec4 position_worldSpace = model * vec4(position, 1.0);
    vec4 normal_worldSpace = vec4(normalize(mat3(transpose(inverse(model))) * normal), 0.0);

    gl_Position = projection * position_cameraSpace;

    color = world_color * diffuse_color; // Add ambient component

    for (int i = 0; i < MAX_LIGHTS; i++) {
        // Point Light
        vec4 vertexToLight = normalize(view * vec4(-lightPositions[i], 0));// - position_cameraSpace);

        if (transparency < 1.0)
            color = diffuse_color;
        else
        {
            // Add diffuse component
            float diffuseIntensity = max(0.0, dot(vertexToLight, normal_cameraSpace));
            color += max(vec3(0), lightColors[i] * diffuse_color * diffuseIntensity);

            // Add specular component
            if (abs(shininess) > 0.001)
            {
                vec4 lightReflection = normalize(-reflect(vertexToLight, normal_cameraSpace));
                vec4 eyeDirection = normalize(vec4(0,0,0,1) - position_cameraSpace);
                float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), shininess);
                color += max (vec3(0), lightColors[i] * specular_color * specIntensity);
            }
        }
    }
    color = clamp(color, 0.0, 1.0) * allBlack;
}
