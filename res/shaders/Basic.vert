#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 color;
layout(location = 4) in vec3 Mat;

uniform mat4 u_PV;
uniform mat4 u_M;

out vec2 v_TexCoord; 
out vec4 v_Color;
flat out vec3 v_Mat;

out vec4 v_FragPos;
out vec3 v_Normal;
flat out vec2 v_Size;

void main() {
    v_FragPos = u_M * vec4(position, 1.0);
    v_Normal = normal;

    v_Color = color;
    v_TexCoord = texCoord.xy;
    v_Mat = Mat;
    v_Size = texCoord.zw;
    gl_Position =  u_PV * vec4(position, 1.0);
}