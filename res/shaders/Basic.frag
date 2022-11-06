#version 410 core

//layout(location = 0) out vec4 color;
layout(location = 0) out vec4 pos;
layout(location = 1) out vec4 nrm;
layout(location = 2) out vec4 colSp;

in vec2 v_TexCoord;
in vec4 v_Color;
flat in vec3 v_Mat;

in vec3 v_FragPos;
in vec3 v_Normal;
flat in vec2 v_Size;

uniform sampler2D u_Textures[32];


void main() {
    int index = int(v_Mat.x);
    vec2 coords = vec2(mod(v_TexCoord.x*v_Size.x, v_Mat.y), v_TexCoord.y*v_Size.y);
    vec4 texColor = texture(u_Textures[index], coords) * v_Color;
    //if(texColor.a < 0.1)
    //    discard;

    float specStr = texture(u_Textures[index], (coords + vec2(v_Mat.y, 0))).r;
    //vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 norm = normalize(v_Normal);

    //vec3 result = vec3(0.0);

    //result += CalcDirLight(u_DirLight, norm, viewDir, texColor.rgb, specStr);
    //for(int i = 0; i < LightNum; i++)
    //    result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir, texColor.rgb, specStr);
    //result *= texColor.rgb;

    //color = vec4(result, texColor.w);
    
    pos = vec4(v_FragPos, v_Mat.z);
    nrm = vec4(norm, specStr);
    colSp = vec4(texColor.rgb, 1.0);
}