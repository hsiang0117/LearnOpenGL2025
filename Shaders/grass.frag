#version 430 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D grass_texture;

void main()
{   
    vec4 texColor = texture(grass_texture, texCoords);
    if(texColor.a < 0.1)
        discard;
    fragColor = texColor;
}