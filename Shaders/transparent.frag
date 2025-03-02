#version 430 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D transparent_texture;

void main()
{   
    vec4 texColor = texture(transparent_texture, texCoords);
    fragColor = texColor;
}