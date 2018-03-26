#version 430 core
layout(location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vertexTexture;

uniform mat4 uMVP;

out vec3 Color;

out vec2 texCoord;

void main()
{
	texCoord = vertexTexture;
	Color = vColor;
	gl_Position = uMVP * vec4(vPosition, 1.0f);
}
