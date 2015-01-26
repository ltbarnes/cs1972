#version 410 core

in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 texc;

void main () {
	texc = position;
	
	mat3 v = mat3(view);

	gl_Position = projection * mat4(v) * vec4(position, 1.0);
	//gl_Position = projection * view * vec4(position, 1.0);
}
