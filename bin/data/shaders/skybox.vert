#version 330

layout(location = 0) in vec3 vPosition;

uniform mat4 cameraRotation;
uniform mat4 cameraProjection;

out vec3 fTextureCoords;

void main() {
	vec4 pos = cameraProjection * cameraRotation * vec4(20 * vPosition, 1);
	gl_Position = pos.xyww;

	fTextureCoords = vPosition;
}