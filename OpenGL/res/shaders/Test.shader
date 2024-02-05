#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 u_MVP;


void main()
{
	gl_Position = u_MVP * vec4(position, 1.0);
	Normal = aNormal;
	FragPos = vec3(model * vec4(position, 1.0));
	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

//in vec3 Normal;
//in vec3 FragPos;
//in vec2 v_TexCoord;
//out vec4 FragColor;
////out vec4 color;
//
//uniform vec4 objColor;
////uniform vec4 objectColor;
//uniform vec4 lightColor;
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//uniform sampler2D u_Texture;
//
//void main()
//{
//	float ambientStrength = 0.3;
//	vec4 ambient = ambientStrength * lightColor;
//
//	vec3 norm = normalize(Normal);
//	vec3 lightDir = normalize(lightPos - FragPos);
//
//	float diff = max(dot(norm, lightDir), 0.0);
//	vec4 diffuse = diff * lightColor;
//
//	float specularStrength = 0.5;
//	vec3 viewDir = normalize(viewPos - FragPos);
//	vec3 reflectDir = reflect(-lightDir, norm);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//	vec4 specular = specularStrength * spec * lightColor;
//
//
//	//vec4 result = (ambient + diffuse) * objColor;
//	//vec4 result = (ambient + diffuse) * lightColor;
//	vec4 result = (ambient + diffuse + specular) * objColor;
//	FragColor = vec4(result.r, result.g, result.b, 1)* texture(u_Texture, v_TexCoord);
//	
//	//vec4 texColor = texture(u_Texture, v_TexCoord);
//	//color = texColor;
//	//color = u_Color;
//	//FragColor = vec4(lightColor * u_Color);
//
//};
#define NR_POINT_LIGHTS 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3
    vec3 result=CalcDirLight(dirLight, norm, viewDir);
}


// calculates the color when using a directional light.

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}