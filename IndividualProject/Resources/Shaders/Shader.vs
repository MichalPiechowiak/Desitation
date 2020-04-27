#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 Normal; 
out vec3 Vertex; 

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

void main()
{
	mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
	BoneTransform += gBones[BoneIDs[1]] * Weights[1];
	BoneTransform += gBones[BoneIDs[2]] * Weights[2];
	BoneTransform += gBones[BoneIDs[3]] * Weights[3];

	vec4 PosL = BoneTransform * vec4(vertexPos, 1.0);

	TexCoords = texCoords;
	Normal = vec3(BoneTransform * vec4(vertexPos, 1.0));
	Vertex = vec3(model * vec4(vertexPos, 1.0));

	gl_Position = projection * view * model * PosL;
	
}