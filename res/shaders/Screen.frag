#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

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

uniform vec3 u_viewPos;
uniform DirLight u_DirLight;
uniform PointLight u_PointLights[16];

uniform int LightNum;

float ambStr = 0.2;

vec3 CalcDirLight(DirLight Light, vec3 normal , vec3 viewDir, vec3 color, float specStr, float v_Mat) {
    vec3 lightDir = normalize(-Light.dir);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), v_Mat);

    vec3 ambiant = Light.color * ambStr * color;
    vec3 diffuse = diff * Light.color * color;
    vec3 specular = specStr * spec * Light.color;
    return (ambiant + diffuse + specular);
}

vec3 CalcPointLight(PointLight Light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color, float specStr, float v_Mat) {
    float Distance = length(Light.pos - fragPos);
    //float Str = 1 - 0.1*Distance;
    //if(Str < 0.0)
    //    return vec3(0.0);
    //Str = clamp(Str, 0.0, 1.0);
    //float Str = 1.0;
    float attenuation = 1.0 / (Light.con + Light.lin * Distance + 
                        Light.quad * (Distance * Distance)); 
    

    vec3 lightDir = normalize(Light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), v_Mat);

    vec3 ambiant = Light.color * ambStr * color;
    vec3 diffuse = diff * Light.color * color;
    vec3 specular = specStr * spec * Light.color;
    return (((ambiant + diffuse + specular) * attenuation));
    return vec3(ambiant);
}

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    float MatZ = texture(gPosition, TexCoords).a;
    vec3 norm = texture(gNormal, TexCoords).rgb;
    float specStr = texture(gNormal, TexCoords).a;
    vec3 texColor = texture(gAlbedoSpec, TexCoords).rgb;


    vec3 viewDir = normalize(u_viewPos - FragPos);



    vec3 result = vec3(0.0);
    //result += CalcDirLight(u_DirLight, norm, viewDir, texColor.rgb, specStr, MatZ);
    //result += CalcPointLight(u_PointLights[0], norm, FragPos, viewDir, texColor.rgb, specStr, MatZ);
    //for(int i = 0; i < 0; i++)
    //    result += CalcPointLight(u_PointLights[i], norm, FragPos, viewDir, texColor.rgb, specStr, MatZ);
    
    
    // then calculate lighting as usual
    vec3 lighting  = texColor * 0.1; // hard-coded ambient component
    viewDir  = normalize(u_viewPos - FragPos);
    vec3 specular;
    for(int i = 0; i < 12; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(u_PointLights[i].pos - FragPos);
        vec3 diffuse = max(dot(norm, lightDir), 0.0) * texColor * u_PointLights[i].color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
        specular = u_PointLights[i].color *  spec * specStr;
        // attenuation
        float distance = length(u_PointLights[i].pos - FragPos);
        float attenuation = 1.0 ;/// (1.0 + u_PointLights[i].lin * distance + u_PointLights[i].quad * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;        
    }


    FragColor = vec4(lighting, 1.0);
} 