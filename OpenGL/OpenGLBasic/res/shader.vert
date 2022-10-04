#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec2 outTexCoord;

void main() {
	gl_Position = vec4(aPos, 1.0);
    outColor = aColor;
    outTexCoord = aTexCoord; 
}