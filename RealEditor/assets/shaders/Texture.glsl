//#type vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoordinates;
layout(location = 2) in vec4 inColor;
layout(location = 3) in float textureIndex;
layout(location = 4) in float tilingFactor;
layout(location = 5) in int entityId;


uniform mat4 viewProjection;

out vec2 outTexCoords;
out vec4 color;
out float texIndex;
out float ioTilingFactor;
flat out int ioEntityId;
		
void main(){
	color = inColor;
	outTexCoords = textCoordinates;
	texIndex = textureIndex;
	ioTilingFactor = tilingFactor;
	ioEntityId = entityId;
	gl_Position = viewProjection * vec4(position,1.0);
}

//#type pixel
#version 330 core

layout(location = 0) out vec4 outColor;
layout(location = 1) out int outEntityId;
		
in vec2 outTexCoords;
in vec4 color;
in float texIndex;
in float ioTilingFactor;
flat in int ioEntityId;

uniform sampler2D uTextures[32]; // how many textures inside 
		
void main(){
	//TODO: tilingFactor
	outEntityId = ioEntityId;
	outColor = texture(uTextures[int(texIndex)], outTexCoords * ioTilingFactor) * color;
}
