#version 330 core
layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec3 normalIn;

uniform mat4 projection_view;

out vec2 uv;

void main()
{
    gl_Position = projection_view * vec4(positionIn, 1.0);
    uv = uvIn;
}