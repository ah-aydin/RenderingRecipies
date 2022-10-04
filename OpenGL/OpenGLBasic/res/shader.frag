#version 460

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

void main() {
	outColor = vec4(aColor, 1.0);
}