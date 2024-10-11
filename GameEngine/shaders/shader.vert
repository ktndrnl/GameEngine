#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

out vec4 vCol;
out vec2 texCoord;
out vec3 normalFrag;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos, 0.0, 1.0), 1.0);
    texCoord = tex;
    normalFrag = mat3(transpose(inverse(model))) * normal;

    fragPos = (model * vec4(pos, 1.0)).xyz;
}