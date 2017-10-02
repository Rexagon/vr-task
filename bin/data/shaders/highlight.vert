#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTextureCoords;
layout(location = 2) in vec3 vNormal;

uniform mat4 transformation;
uniform mat4 cameraProjection;
uniform mat4 cameraTransformation;

out vec2 fTextureCoords;
out vec3 fNormal;

void main() {
	gl_Position = cameraProjection * cameraTransformation * transformation * vec4(vPosition * 1.08f, 1);

	fTextureCoords = vTextureCoords;
	fNormal = (transpose(inverse(transformation)) * vec4(vNormal, 1.0)).xyz;
}