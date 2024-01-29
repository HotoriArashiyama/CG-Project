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
}

#endif // !__TXT_H__
