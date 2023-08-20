#version 330 core
layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec2 uvIn;

uniform mat4 model;
uniform mat4 projection;

out vec2 uv;

void main()
{
    gl_Position = projection * model * vec4(positionIn, 1.0);

    uv = uvIn;
}