#version 410 core

in vec2 texc;

out vec4 fragColor;

uniform sampler2D tex;
uniform int useTexture = 0;
uniform float transparency = 1.0;

void main(){
    vec3 texColor = texture(tex, texc).rgb;
    texColor = clamp(texColor + vec3(1-useTexture), vec3(0.0), vec3(1.0));
    fragColor = vec4(texColor, transparency);
//    fragColor = vec4(1, 0, 0, 1);
}
