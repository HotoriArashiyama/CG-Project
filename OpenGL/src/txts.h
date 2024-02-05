#pragma once

#ifndef __TXT_H__
#define __TXT_H__

namespace txts_h {
	/*glew Shaders*/

	/*shaders of skybox (not used eventually)*/
	const char* vtxShaderSkyBox =
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"out vec3 TexCoord;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"void main(){\n"
		"    TexCoord = pos;\n"
		"    gl_Position = projection * view * vec4(pos, 1.0);\n"
		"}\n";
	const char* fragShaderSkyBox =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 TexCoord;\n"
		"uniform samplerCube skybox;\n"
		"void main(){\n"
		"    FragColor = texture(skybox, TexCoord);\n"
		"}\n";
	/*shaders of normal obj*/
	const char* vtxShaderNrmlObj =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoords;\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main(){\n"
		"	TexCoords = aTexCoords;\n"
		"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"}\n";
	const char* fragShaderNrmlObj =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoords;\n"
		"uniform sampler2D texture_diffuse1;\n"
		"void main(){\n"
		"	FragColor = texture(texture_diffuse1, TexCoords);\n"
		"}\n";
	/*shaders of obj in hands*/
	const char* vtxShaderCarryonObj =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoords;\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main(){\n"
		"	TexCoords = aTexCoords;\n"
		"	gl_Position = projection * model * vec4(aPos, 1.0);\n"
		"}\n";
	const char* fragShaderCarryonObj =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoords;\n"
		"uniform sampler2D texture_diffuse1;\n"
		"void main(){\n"
		"	FragColor = texture(texture_diffuse1, TexCoords);\n"
		"}\n";
	/*shaders of lights, from jkx's part*/
	const char* vtxShaderDualLight =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoords;\n"

		"out vec3 FragPos; // 片段在世界空间的位置向量\n"
		"out vec3 Normal; // 片段在世界空间的法向量\n"
		"out vec2 TexCoords; // 片段的纹理坐标\n"

		"uniform mat4 model; // 模型矩阵\n"
		"uniform mat4 view; // 观察矩阵\n"
		"uniform mat4 projection; // 投影矩阵\n"

		"void main() {\n"
		"	FragPos = vec3(model * vec4(aPos, 1.0)); // 将顶点位置从局部空间变换到世界空间\n"
		"	Normal = mat3(transpose(inverse(model))) * aNormal;  // 法线变换到世界空间\n"
		"	TexCoords = aTexCoords; // 纹理坐标不需要变换\n"

		"	gl_Position = projection * view * vec4(FragPos, 1.0); // 将顶点位置从世界空间变换到裁剪空间\n"
		"}\n";
	const char* fragShaderDualLight =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"\n"
		"struct Material {\n"
		"    sampler2D diffuse;\n"
		"    sampler2D specular;\n"
		"    float shininess;\n"
		"}; \n"
		"\n"
		"struct DirLight {\n"
		"    vec3 direction;\n"
		"	\n"
		"    vec3 ambient;\n"
		"    vec3 diffuse;\n"
		"    vec3 specular;\n"
		"};\n"
		"\n"
		"struct PointLight {\n"
		"    vec3 position;\n"
		"    \n"
		"    float constant;\n"
		"    float linear;\n"
		"    float quadratic;\n"
		"	\n"
		"    vec3 ambient;\n"
		"    vec3 diffuse;\n"
		"    vec3 specular;\n"
		"};\n"
		"\n"
		"struct SpotLight {\n"
		"    vec3 position;\n"
		"    vec3 direction;\n"
		"    float cutOff;\n"
		"    float outerCutOff;\n"
		"  \n"
		"    float constant;\n"
		"    float linear;\n"
		"    float quadratic;\n"
		"  \n"
		"    vec3 ambient;\n"
		"    vec3 diffuse;\n"
		"    vec3 specular;       \n"
		"};\n"
		"\n"
		"#define NR_POINT_LIGHTS 4\n"
		"\n"
		"in vec3 FragPos;\n"
		"in vec3 Normal;\n"
		"in vec2 TexCoords;\n"
		"\n"
		"uniform vec3 viewPos;\n"
		"uniform DirLight dirLight;\n"
		"uniform PointLight pointLights[NR_POINT_LIGHTS];\n"
		"uniform SpotLight spotLight;\n"
		"uniform Material material;\n"
		"\n"
		"// function prototypes\n"
		"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
		"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
		"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
		"\n"
		"void main()\n"
		"{    \n"
		"    // properties\n"
		"    vec3 norm = normalize(Normal);\n"
		"    vec3 viewDir = normalize(viewPos - FragPos);\n"
		"    \n"
		"    // == =====================================================\n"
		"    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight\n"
		"    // For each phase, a calculate function is defined that calculates the corresponding color\n"
		"    // per lamp. In the main() function we take all the calculated colors and sum them up for\n"
		"    // this fragment's final color.\n"
		"    // == =====================================================\n"
		"    // phase 1: directional lighting\n"
		"    vec3 result = CalcDirLight(dirLight, norm, viewDir)* 3;\n"
		"    // phase 2: point lights\n"
		"    for(int i = 0; i < NR_POINT_LIGHTS; i++)\n"
		"        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    \n"
		"    // phase 3: spot light\n"
		"    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    \n"
		"    \n"
		"    FragColor = vec4(result, 1.0);\n"
		"}\n"
		"\n"
		"// calculates the color when using a directional light.\n"
		"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n"
		"{\n"
		"    vec3 lightDir = normalize(-light.direction);\n"
		"    // diffuse shading\n"
		"    float diff = max(dot(normal, lightDir), 0.0);\n"
		"    // specular shading\n"
		"    vec3 reflectDir = reflect(-lightDir, normal);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
		"    // combine results\n"
		"    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n"
		"    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n"
		"    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n"
		"    return (ambient + diffuse + specular);\n"
		"}\n"
		"\n"
		"// calculates the color when using a point light.\n"
		"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
		"{\n"
		"    vec3 lightDir = normalize(light.position - fragPos);\n"
		"    // diffuse shading\n"
		"    float diff = max(dot(normal, lightDir), 0.0);\n"
		"    // specular shading\n"
		"    vec3 reflectDir = reflect(-lightDir, normal);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
		"    // attenuation\n"
		"    float distance = length(light.position - fragPos);\n"
		"    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n"
		"    // combine results\n"
		"    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n"
		"    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n"
		"    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n"
		"    ambient *= attenuation;\n"
		"    diffuse *= attenuation;\n"
		"    specular *= attenuation;\n"
		"    return (ambient + diffuse + specular);\n"
		"}\n"
		"\n"
		"// calculates the color when using a spot light.\n"
		"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
		"{\n"
		"    vec3 lightDir = normalize(light.position - fragPos);\n"
		"    // diffuse shading\n"
		"    float diff = max(dot(normal, lightDir), 0.0);\n"
		"    // specular shading\n"
		"    vec3 reflectDir = reflect(-lightDir, normal);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
		"    // attenuation\n"
		"    float distance = length(light.position - fragPos);\n"
		"    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n"
		"    // spotlight intensity\n"
		"    float theta = dot(lightDir, normalize(-light.direction)); \n"
		"    float epsilon = light.cutOff - light.outerCutOff;\n"
		"    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);\n"
		"    // combine results\n"
		"    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n"
		"    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n"
		"    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n"
		"    ambient *= attenuation * intensity;\n"
		"    diffuse *= attenuation * intensity;\n"
		"    specular *= attenuation * intensity;\n"
		"    return (ambient + diffuse + specular);\n"
		"}\n";
	const char* vtxShaderLightCube =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"}\n";
	const char* fragShaderLightCube =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 lightColor; // 添加一个uniform变量来设置颜色\n"
		"void main() {\n"
		"	FragColor = lightColor; // 使用自定义的颜色\n"
		"}\n";
}

#endif // !__TXT_H__
