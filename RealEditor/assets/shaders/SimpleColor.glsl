#type vertex
#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 viewProjection;
uniform mat4 transform;


void main() {
	gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#type pixel
#version 330 core


layout (location = 0) out vec4 color;

uniform vec4 uColor;

void main(){
	color = uColor;
}
