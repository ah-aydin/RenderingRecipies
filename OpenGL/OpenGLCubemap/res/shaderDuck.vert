#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

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

layout (location = 0) out PerVertexData vtx;

void main() {
    //gl_Position = proj * view * model * vec4(aPos, 1.0);
    gl_Position = MVP * vec4(aPos, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(model)));

    vtx.uv = aTexCoord;
    vtx.normal = aNormal * normalMatrix;
    vtx.worldPos = (model * vec4(aPos, 1.0)).xyz;
}