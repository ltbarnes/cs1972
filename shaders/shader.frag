#version 410 core

in vec3 clr;
in vec2 texc;

out vec4 fragColor;

uniform vec4 color;

uniform sampler2D tex;
uniform int useTexture = 0;

void main(){
    vec3 texColor = texture(tex, texc).rgb;
    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
    fragColor = color * (1-useTexture) + vec4(texColor, 1) * useTexture;
}
