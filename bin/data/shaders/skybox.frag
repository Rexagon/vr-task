#version 330

in vec3 fTextureCoords;

uniform samplerCube diffuseTexture;

void main() {
	gl_FragColor = texture(diffuseTexture, fTextureCoords);
}