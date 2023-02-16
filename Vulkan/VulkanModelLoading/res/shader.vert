#version 450

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iColor;

layout (location = 0) out vec3 oColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 proj;
    mat4 view;
} perFrameData;

layout (push_constant) uniform Push {
    mat4 model;
} perObjectData;

void main() {
    gl_Position = perFrameData.proj * perFrameData.view * perObjectData.model * vec4(iPos, 1.0);
    //gl_Position = vec4(iPos, 1.0);
    oColor = iColor;
}