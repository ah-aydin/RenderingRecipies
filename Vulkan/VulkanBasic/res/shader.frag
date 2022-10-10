#version 450

layout (location = 0) out vec4 oColor;

layout (location = 0) in vec3 iColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} perFrameData;

layout (push_constant) uniform Push {
    mat4 model;
} perObjectData;


void main() {
    oColor = vec4(iColor, 1.0);
}