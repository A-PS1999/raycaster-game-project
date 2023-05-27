#version 460 core

in vec2 texCoords;
out vec4 colour;

uniform sampler2D spriteImage;
uniform vec3 spriteColour;

void main()
{
	colour = vec4(spriteColour, 1.0) * texture(spriteImage, texCoords);
}