#version 420 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct LightColor{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct DirLight {
    vec3 direction;
    LightColor color;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
	
    float bound;

    vec3 color;
};
layout (std140, binding = 1) uniform VP{vec3 viewPos;};

const int NR_LIGHTS = 32;
uniform int pointLightNum;
uniform PointLight pointLights[NR_LIGHTS];

uniform DirLight dirLight;//One direction light.

vec3 CalcDirLight(DirLight light, vec3 lightDir, vec3 normal, 
    vec3 viewDir, vec2 texC, float shadow);

void main()
{             
    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // Then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.5; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < pointLightNum; ++i)
    {
        // Diffuse
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse*2 * pointLights[i].color;
        // Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
        vec3 specular = pointLights[i].color * spec * Specular;
        // Attenuation
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }    
    lighting += CalcDirLight(dirLight, normalize(dirLight.direction), Normal, viewDir, TexCoords, 0.f);

    FragColor = vec4(lighting, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 tangentLightDir, vec3 normal, vec3 viewDir, vec2 texCoords, float shadow)
{
    vec3 lightDir = normalize(tangentLightDir);
    vec3 reflectDir = reflect(lightDir, normal);
    vec3 halfwayDir = normalize(-lightDir + viewDir);

    float diff = max(dot(normal, -lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);//Blinn-Phong.
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 diffuse = light.color.diffuse * diff * texture(gAlbedoSpec, texCoords).rgb;
    vec3 specular = light.color.specular * spec * texture(gAlbedoSpec, texCoords).a;
    
    return (diffuse+specular);
}