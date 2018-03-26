#version 430 core

in vec3 Color;
in vec2 texCoord;

out vec4 fColor;

uniform sampler2D texture0;

void main()
{
	//fColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	fColor = texture(texture0, texCoord);
}