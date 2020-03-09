#version 420 core

out vec4 FragColor;

struct Material{
    //sampler2D ambient;// ambient equals diffuse.
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    float shininess;// For specular. Bigger for smaller specular boundary.
};

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

    LightColor color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;

    float bound;

    LightColor color;
};
layout (std140, binding = 1) uniform VP{vec3 viewPos;};

uniform int bUseNormalMap;

uniform DirLight dirLight;//One direction light.

#define MAX_POINT_LIGHTS 16
uniform int pointLightNum;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;

in VS_OUT
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Color;
    vec3 Normal;
	vec4 FragPosLightSpace;//For directional light.

    mat3 TBN;
};

uniform sampler2D shadowMap;
uniform samplerCube depthCubemap;

uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 lightDir, vec3 normal, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 tangentLightPos, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
float ShadowCalculation(vec4 fragPosLS, vec3 n, vec3 lightDir);
float OMinShadowCalculation(vec3 fragPos, vec3 n, vec3 lightPos, float far_plane);

void main(){
	
    //vec3 norm = normalize(Normal);
    vec3 tanNorm = texture(material.normalMap, TexCoord).rgb;
    tanNorm = normalize(tanNorm * 2.0 - 1.0);  
    //vec3 norm = normalize(Normal);
    vec3 norm;
    if(bUseNormalMap == 1)
        norm = normalize(TBN*tanNorm);
    else
        norm = normalize(Normal);
        norm = normalize(Normal);
    
    vec3 viewDir = normalize(viewPos-FragPos);
	
    //Direction lighting.
    float dirShadow = ShadowCalculation(FragPosLightSpace, norm, normalize(dirLight.direction)); 
    vec3 lightingResult = CalcDirLight(dirLight, dirLight.direction, norm, viewDir, dirShadow);
    
    //Point Lighting.
    for(int i=0;i<pointLightNum;i++)
    {
        float pointShadow = OMinShadowCalculation(FragPos, norm, pointLights[i].position, pointLights[i].bound);
        lightingResult += CalcPointLight(pointLights[i], pointLights[i].position, norm, FragPos, viewDir, pointShadow);
	}

    //Spot lighting.
    //lightingResult += CalcSpotLight(spotLight, norm, FragPos, viewDir, shadow);
    //lightingResult = TangentPointLightPos[0];
    //float gamma = 2.2;
    //lightingResult = pow(lightingResult, vec3(1.0/gamma));
    //lightingResult = TangentViewPos;
    FragColor = vec4(lightingResult, 1.f);
    //FragColor = vec4(vec3(norm), 1.f);
}

vec3 CalcDirLight(DirLight light, vec3 tangentLightDir, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(tangentLightDir);
    vec3 reflectDir = reflect(lightDir, normal);
    vec3 halfwayDir = normalize(-lightDir + viewDir);

    float diff = max(dot(normal, -lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//Blinn-Phong.
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.color.ambient * vec3(texture(material.diffuseMap, TexCoord));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuseMap, TexCoord));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specularMap, TexCoord));
    
    return (ambient+(diffuse+specular)*(1.0-shadow));
}

vec3 CalcPointLight(PointLight light, vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(fragPos - lightPos);
    vec3 reflectDir = reflect(lightDir, normal);
    vec3 halfwayDir = normalize(-lightDir + viewDir);

    float diff = max(dot(normal, -lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//Blinn-Phong.
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //Decays in distance(nolinear).
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic * (distance * distance));
    
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuseMap, TexCoord));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuseMap, TexCoord));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specularMap, TexCoord));
    
    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    return (ambient+(diffuse+specular)*(1.0-shadow));
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{

    vec3 lightDir = normalize(fragPos - light.position);
    vec3 reflectDir = reflect(lightDir, normal);
    vec3 halfwayDir = normalize(-lightDir + viewDir);

    float diff = max(dot(normal, -lightDir), 0.0);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);//Blinn-Phong.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic * (distance * distance));

    //Cosine Angle of lightDir and spotDir.
    float theta = dot(-lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);//Smooth boundary(in [0,1]~[outCutoff, cutOff]).
    
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuseMap, TexCoord));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuseMap, TexCoord));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specularMap, TexCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient+(diffuse+specular)*(1.0-shadow))*intensity;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;//To [0,1].
    //if(projCoords.x<0.f || projCoords.x > 1.f || projCoords.y < 0.f || projCoords.y > 1.f)
      //  return 0.0;//Out of light sapace, cast no shadow.
    if(projCoords.z > 1.f) return 0;//Out of far plane.
    
    ///////////////////////
    // PCF shadowing.
    float currentDepth = projCoords.z;
    float bias = max(0.001 * (1.0 - dot(norm, lightDir)), 0.0005);

    float shadow = 0.f;
    vec2 p_size = 1.f/textureSize(shadowMap, 0);//Get a pixel size in shadow map.
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy - p_size*vec2(i, j)).r;   
            shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0);
	    }
	}

    return shadow/9.f;// 1 in shadow, 0 not.
}

float OMinShadowCalculation(vec3 fragPos, vec3 norm, vec3 lightPos, float far_plane)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;//A sample radius.

    if(currentDepth >= far_plane) return 0;//Sample out of range.
    float shadow = 0.f;
    vec3 sampleOffsetDirections[20] = vec3[]
    (
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    float bias = max(0.015 * (1.0 - dot(norm, fragToLight)), 0.005);

    for(int i = 0; i < 20; ++i)
    {
        vec3 samplePoint = fragToLight + sampleOffsetDirections[i] * diskRadius;
        float closestDepth = texture(depthCubemap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(20);


    return shadow;
}