#version 330 core
layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 projection_view;
uniform mat4 model;

out vec2 uv;
out vec3 normal;
out vec3 fragment_position;

void main()
{
    uv = uvIn;
    normal = mat3(transpose(inverse(model))) * normalIn;
    fragment_position = vec3(model * vec4(positionIn, 1.0));

    gl_Position = projection_view * model * vec4(positionIn, 1.0);
}