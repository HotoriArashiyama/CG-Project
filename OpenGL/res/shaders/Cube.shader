#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform vec4 u_Color;
void main()
{
	gl_Position = u_MVP*vec4(position, 1.0);
	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform vec3 lightColor;
uniform sampler2D u_Texture;
uniform sampler2D u_Texture2;

void main()
{
	float refRatio = 1.0;
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor * vec4(lightColor * refRatio, 1.0);
};