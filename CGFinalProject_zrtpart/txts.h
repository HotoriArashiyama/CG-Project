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
}

#endif // !__TXT_H__
