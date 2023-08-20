#version 330 core

in vec3 texture_coordinates;
in vec4 texture_color;

out vec4 fragment_color;

uniform samplerCube skybox_texture;

void main()
{    
    vec3 corrected_coordinates = vec3(texture_coordinates.xy, -texture_coordinates.z);
    fragment_color = texture(skybox_texture, corrected_coordinates) * texture_color;
}