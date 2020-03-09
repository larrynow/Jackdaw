#version 420 core

layout(location = 0)out vec4 FragColor;
layout(location = 1)out vec4 BrightColor;

#define MAX_LIGHTS 16

in VS_OUT
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Color;
    vec3 Normal;
	vec4 FragPosLightSpace;//For directional light.

    vec3 TangentLightDir;
	vec3 TangentLightPos[MAX_LIGHTS];

	vec3 TangentViewPos;
	vec3 TangentFragPos;
};

struct Material{
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    sampler2D heightMap;

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

uniform int bUseNormalMap;
uniform int bUseHeightMap;

uniform Material material;

uniform bool lighting;
uniform bool shinning;

uniform DirLight dirLight;//One direction light.

uniform int pointLightNum;
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLight;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

vec3 CalcPointLight(PointLight light, vec3 lightPos, 
    vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texC, float shadow);
vec3 CalcDirLight(DirLight light, vec3 lightDir, vec3 normal, 
    vec3 viewDir, vec2 texC, float shadow);

void main()
{
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec2 texCoords;
    if(bUseHeightMap == 1)
        texCoords = ParallaxMapping(TexCoord, viewDir);
    else
        texCoords = TexCoord;

    vec3 normal = texture(material.normalMap, texCoords).rgb;
    if(bUseNormalMap == 1)
    {
        normal = normalize(normal * 2.0 - 1.0);
	}
    else
        normal = vec3(0.f, 0.f, 1.f);
    
    if(lighting)
    {
        vec3 lightingResult;
        for(int i = 0; i< pointLightNum; i++)
        {
            lightingResult += CalcPointLight(pointLights[0], TangentLightPos[0], 
                normal, TangentFragPos, viewDir, texCoords, 0.f);
		}

        lightingResult += CalcDirLight(dirLight, TangentLightDir, normal, viewDir, texCoords, 0.f);

        FragColor =  vec4(lightingResult, 1.0f);
	}
    else
    {
        FragColor = vec4(Color, 1.0);
	}

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(shinning && (brightness > 1.0))//Out bright color in another color buffer.
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, 
        abs(dot(vec3(0.f, 0.f, 1.f), viewDir)));

    float layerDepth = 1.0/numLayers;
    float currentDepth = 0.f;
	float depth = 1.f - texture(material.heightMap, texCoords).r;

	float scale = 0.01f;
    vec2 p = viewDir.xy * scale;
    vec2 deltaTexCoords = p/numLayers;

    vec2 currentTexCoords = texCoords;
    while(currentDepth < depth)
    {
        currentTexCoords -= deltaTexCoords;
        depth = 1.f -texture(material.heightMap, currentTexCoords).r;//Update.
        currentDepth += layerDepth;
	}

    //Interpolation.
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;//Prev texcorrd.

    float depthAfter = currentDepth - depth;
    float depthPrev = 1.f -texture(material.heightMap, prevTexCoords).r
        - currentDepth + layerDepth;

    float r = depthAfter/(depthAfter + depthPrev);//Bigger r for prev.

	return r * prevTexCoords + (1-r)*currentTexCoords;
}

vec3 CalcPointLight(PointLight light, vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewDir,vec2 texCoords, float shadow)
{
    vec3 lightDir = normalize(fragPos - lightPos);
    vec3 reflectDir = reflect(lightDir, normal);
    vec3 halfwayDir = normalize(-lightDir + viewDir);

    float diff = max(dot(normal, -lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 256);//Blinn-Phong.
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //Decays in distance(nolinear).
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic * (distance * distance));
    
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuseMap, texCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuseMap, texCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specularMap, texCoords));
    
    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    return (ambient+(diffuse+specular)*(1.0-shadow));
}

vec3 CalcDirLight(DirLight light, vec3 tangentLightDir, vec3 normal, vec3 viewDir, vec2 texCoords, float shadow)
{
    vec3 lightDir = normalize(tangentLightDir);
    vec3 reflectDir = reflect(lightDir, normal);
    vec3 halfwayDir = normalize(-lightDir + viewDir);

    float diff = max(dot(normal, -lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//Blinn-Phong.
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.color.ambient * vec3(texture(material.diffuseMap, texCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuseMap, texCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specularMap, texCoords));
    
    return (ambient+(diffuse+specular)*(1.0-shadow));
}