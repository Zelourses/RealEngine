//#type vertex
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoordinates;
layout(location = 2) in vec4 color;
layout(location = 3) in float textureIndex;
layout(location = 4) in float tilingFactor;
layout(location = 5) in int entityId;

layout(std140, binding = 0) uniform Camera {
	mat4 viewProjection;
};
struct VertexOutput {
	vec2 texCoords;
	vec4 color;
	float texIndex;
	float tilingFactor;
};

layout(location = 0) out VertexOutput vertexOut;
// I am not sure that this is right location
layout(location = 4) out flat int ioEntityId;
		
void main(){
	vertexOut.color = color;
	vertexOut.texCoords = textCoordinates;
	vertexOut.texIndex = textureIndex;
	vertexOut.tilingFactor = tilingFactor;
	ioEntityId = entityId;
	gl_Position = viewProjection * vec4(position,1.0);
}

//#type fragment
#version 450 core

layout(location = 0) out vec4 outColor;
layout(location = 1) out int outEntityId;
		
struct VertexOutput {
	vec2 texCoords;
	vec4 color;
	float texIndex;
	float tilingFactor;
};

layout(location = 0) in VertexOutput vertexInput;
layout(location = 4) in flat int ioEntityId;

layout(binding = 0) uniform sampler2D uTextures[32]; // how many textures inside 
		
void main(){
	outEntityId = ioEntityId;
	outColor = texture(uTextures[int(vertexInput.texIndex)], vertexInput.texCoords * vertexInput.tilingFactor) * vertexInput.color;
}
