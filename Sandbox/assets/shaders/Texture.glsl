#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoordinates;
layout(location = 2) in vec4 inColor;
layout(location = 3) in float textureIndex;
layout(location = 4) in float tilingFactor;
		
uniform mat4 viewProjection;

out vec2 outTexCoords;
out vec4 color;
out float texIndex;
out float ioTilingFactor;
		
void main(){
	color = inColor;
	outTexCoords = textCoordinates;
	texIndex = textureIndex;
	ioTilingFactor = tilingFactor;
	gl_Position = viewProjection * vec4(position,1.0);
}

#type pixel
#version 330 core

layout(location = 0) out vec4 outColor;
		
in vec2 outTexCoords;
in vec4 color;
in float texIndex;
in float ioTilingFactor;

uniform sampler2D uTextures[32]; // how many textures inside 
		
void main(){
//TODO: tilingFactor
	outColor = texture(uTextures[int(texIndex)], outTexCoords * ioTilingFactor) * color;
}
