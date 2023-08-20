#version 330 core

in vec2 uv;

uniform sampler2D sun_texture;

out vec4 FragColor;

void main()
{
    FragColor = texture(sun_texture, uv);
}