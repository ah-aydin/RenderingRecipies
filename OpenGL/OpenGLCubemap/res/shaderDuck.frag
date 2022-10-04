#version 460

layout (location = 0) out vec4 outColor;

layout (std140, binding = 0) uniform PerFrameData {
    uniform mat4 model;
    uniform mat4 MVP;
    uniform vec4 cameraPos;
};

struct PerVertexData {
    vec2 uv;
    vec3 normal;
    vec3 worldPos;
};

layout (location = 0) in PerVertexData vtx;

layout (binding = 0) uniform sampler2D texture0;

void main() {
    vec4 color = texture(texture0, vtx.uv);

    outColor = color;
}
