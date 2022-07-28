#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
flat in float v_TexID;

uniform sampler2D u_Textures[32];

void main() {
    vec4 texColor = vec4(v_TexCoord, 0, 1) * v_Color;
    if(texColor.a < 0.1)
        discard;
    color = texColor;
}