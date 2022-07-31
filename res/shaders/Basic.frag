#version 410 core

layout(location = 0) out vec4 color;

struct DirLight {
    vec3 dir;
    vec3 color;
};

struct PointLight {
    vec3 color;

    float con;
    float lin;
    float quad; 
    vec3 pos;
};

in vec2 v_TexCoord;
in vec4 v_Color;
flat in vec3 v_Mat;

in vec3 v_FragPos;
in vec3 v_Normal;
flat in vec2 v_Size;

uniform sampler2D u_Textures[32];

uniform vec3 u_viewPos;
uniform DirLight u_DirLight;
uniform PointLight u_PointLights[16];

uniform int LightNum;

float ambStr = 0.2;

vec3 CalcDirLight(DirLight Light, vec3 normal , vec3 viewDir, vec3 color, float specStr) {
    vec3 lightDir = normalize(-Light.dir);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), v_Mat.z);

    vec3 ambiant = Light.color * ambStr * color;
    vec3 diffuse = diff * Light.color * color;
    vec3 specular = specStr * spec * Light.color;
    return (ambiant + diffuse + specular);
}

vec3 CalcPointLight(PointLight Light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color, float specStr) {
    float Distance = length(Light.pos - fragPos);
    float Str = 1 - 0.1*Distance;
    if(Str < 0.0)
        return vec3(0.0, 0.0, 0.0);
    //Str = clamp(Str, 0.0, 1.0);
    //float Str = 1.0;
    float attenuation = 1.0 / (Light.con + Light.lin * Distance + 
                        Light.quad * (Distance * Distance)); 
    

    vec3 lightDir = normalize(Light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), v_Mat.z);

    vec3 ambiant = Light.color * ambStr * color;
    vec3 diffuse = diff * Light.color * color;
    vec3 specular = specStr * spec * Light.color;
    return (((ambiant + diffuse + specular) * attenuation) * Str);
    //return vec3(Str);
}


void main() {
    int index = int(v_Mat.x);
    vec2 coords = vec2(mod(v_TexCoord.x*v_Size.x, v_Mat.y), v_TexCoord.y*v_Size.y);
    vec4 texColor = texture(u_Textures[index], coords) * v_Color;
    if(texColor.a < 0.1)
        discard;

    float specStr = texture(u_Textures[index], (coords + vec2(v_Mat.y, 0))).r;
    vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 norm = normalize(v_Normal);

    vec3 result;

    //result += CalcDirLight(u_DirLight, norm, viewDir, texColor.rgb, specStr);
    for(int i = 0; i < LightNum; i++)
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir, texColor.rgb, specStr);
    //result *= texColor.rgb;

    
    color = vec4(result, texColor.w);
    //color = texColor;
}