#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragment_position;
in vec4 color;

uniform sampler2D object_texture;

out vec4 FragColor;

void main()
{
    FragColor = texture(object_texture, uv) * color;
}