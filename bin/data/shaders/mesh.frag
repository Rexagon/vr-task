#version 330

in vec2 fTextureCoords;
in vec3 fNormal;

uniform sampler2D diffuseTexture;

void main() {
	vec4 color = texture(diffuseTexture, fTextureCoords);

	float light = clamp(dot(normalize(fNormal), normalize(vec3(-2, 1, -1))), 0, 2);
	vec4 ambient = vec4(0.4f, 0.4f, 0.45f, 1.0f);

	gl_FragColor = clamp(ambient * color + light * color, 0, 1);
}